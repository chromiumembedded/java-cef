// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef;

import java.io.File;
import java.io.FilenameFilter;
import java.util.HashSet;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;
import javax.swing.SwingUtilities;

import org.cef.callback.CefSchemeHandlerFactory;
import org.cef.handler.CefAppHandler;
import org.cef.handler.CefAppHandlerAdapter;

/**
 * Exposes static methods for managing the global CEF context.
 */
public class CefApp extends CefAppHandlerAdapter {

  public final class CefVersion {
    public final int JCEF_REVISION;

    public final int CEF_VERSION_MAJOR;
    public final int CEF_REVISION;

    public final int CHROME_VERSION_MAJOR;
    public final int CHROME_VERSION_MINOR;
    public final int CHROME_VERSION_BUILD;
    public final int CHROME_VERSION_PATCH;

    private CefVersion(int jcefRev, int cefMajor, int cefRev,
                       int chrMajor, int chrMin, int chrBuild, int chrPatch) {
      JCEF_REVISION = jcefRev;

      CEF_VERSION_MAJOR = cefMajor;
      CEF_REVISION = cefRev;

      CHROME_VERSION_MAJOR = chrMajor;
      CHROME_VERSION_MINOR = chrMin;
      CHROME_VERSION_BUILD = chrBuild;
      CHROME_VERSION_PATCH = chrPatch;
    }

    public String getJcefVersion() {
      return CEF_VERSION_MAJOR
          + "." + CHROME_VERSION_BUILD
          + "." + CEF_REVISION
          + "." + JCEF_REVISION;
    }

    public String getCefVersion() {
      return CEF_VERSION_MAJOR
          + "." + CHROME_VERSION_BUILD
          + "." + CEF_REVISION;
    }

    public String getChromeVersion() {
      return CHROME_VERSION_MAJOR
          + "." + CHROME_VERSION_MINOR
          + "." + CHROME_VERSION_BUILD
          + "." + CHROME_VERSION_PATCH;
    }

    @Override
    public String toString() {
      return "JCEF Version = " + getJcefVersion() + "\n"
          + "CEF Version = " + getCefVersion() + "\n"
          + "Chromium Version = " + getChromeVersion();
    }
  }

  /**
   * According the singleton pattern, this attribute keeps
   * one single object of this class.
   */
  private static CefApp self = null;
  private static CefAppHandler appHandler_ = null;
  private HashSet<CefClient> clients_ = new HashSet<CefClient>();
  private final Lock lock = new ReentrantLock();
  private final Condition cefInitialized = lock.newCondition();
  private final Condition cefShutdown = lock.newCondition();
  private final Condition cefShutdownFinished = lock.newCondition();
  private boolean isInitialized_ = false;
  private final boolean osrSupportEnabled_;

  /**
   * To get an instance of this class, use the method
   * getInstance() instead of this CTOR.
   * 
   * The CTOR is called by getInstance() as needed and
   * loads all required JCEF libraries.
   * 
   * @throws UnsatisfiedLinkError
   */
  private CefApp(String [] args, boolean enableOsr) throws UnsatisfiedLinkError {
    super(args);
    osrSupportEnabled_ = enableOsr;
    if (OS.isWindows()) {
      System.loadLibrary("libcef");
    } else if (OS.isLinux()) {
      System.loadLibrary("cef");
    }
    System.loadLibrary("jcef");
    if (appHandler_ == null) {
      appHandler_ = this;
    }
  }

  public static void addAppHandler(CefAppHandler appHandler) {
    appHandler_ = appHandler;
  }

  /**
   * Get an instance of this class.
   * @return an instance of this class
   * @throws UnsatisfiedLinkError
   */
  public static synchronized CefApp getInstance() throws UnsatisfiedLinkError {
     return getInstance(null, false);
  }

  public static synchronized CefApp getInstance(String [] args) throws UnsatisfiedLinkError {
    return getInstance(args, false);
  }

  public static synchronized CefApp getInstance(boolean enableOsr) throws UnsatisfiedLinkError {
    return getInstance(null, enableOsr);
  }

  public static synchronized CefApp getInstance(String [] args, boolean enableOsr)
                                                    throws UnsatisfiedLinkError {
    if (self == null) {
      self = new CefApp(args, enableOsr);
    }
    return self;
  }

  public final CefVersion getVersion() {
    try {
      return N_GetVersion();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return null;
  }

  /**
   * To shutdown the system, it's important to call the dispose
   * method. Calling this method closes all client instances with
   * and all browser instances each client owns. After that the
   * message loop is terminated and CEF is shutdown.
   */
  public synchronized final void dispose() {
    Thread t = new Thread() {
      @Override
      public void run() {
        lock.lock();
        try {
          // Initiate shutdown sequence and wait for its
          // finalization.
          cefShutdown.signal();
          cefShutdownFinished.awaitUninterruptibly();
        } catch (Throwable err) {
          err.printStackTrace();
        } finally {
          System.out.println("shutdown complete");
          self = null;
          lock.unlock();
        }
      }
    };

    // If dispose() is called on the dispatcher thread,
    // the shutdown sequence MUST be performed within a
    // dedicated thread. Otherwise we'll run in a deadlock.
    // In all other cases, it's enough to call the run() method
    // within the same process as dispose() is called.
    if (SwingUtilities.isEventDispatchThread())
      t.start();
    else
      t.run();
  }

  /**
   * Creates a new client instance and returns it to the caller.
   * One client instance is responsible for one to many browser
   * instances
   * @return a new client instance
   */
  public CefClient createClient() {
    if (!isInitialized_) {
      context.start();
    }
    CefClient client = new CefClient();
    clients_.add(client);
    return client;
  }

  /**
   * Register a scheme handler factory for the specified |scheme_name| and
   * optional |domain_name|. An empty |domain_name| value for a standard scheme
   * will cause the factory to match all domain names. The |domain_name| value
   * will be ignored for non-standard schemes. If |scheme_name| is a built-in
   * scheme and no handler is returned by |factory| then the built-in scheme
   * handler factory will be called. If |scheme_name| is a custom scheme then
   * also implement the CefApp::OnRegisterCustomSchemes() method in all processes.
   * This function may be called multiple times to change or remove the factory
   * that matches the specified |scheme_name| and optional |domain_name|.
   * Returns false if an error occurs. This function may be called on any thread
   * in the browser process.
   */
  public boolean registerSchemeHandlerFactory(String schemeName,
                                              String domainName,
                                              CefSchemeHandlerFactory factory) {
    try {
      return N_RegisterSchemeHandlerFactory(schemeName, domainName, factory);
    } catch (Exception err) {
      err.printStackTrace();
    }
    return false;
  }

  /**
   * Clear all registered scheme handler factories. Returns false on error. This
   * function may be called on any thread in the browser process.
   */
  public boolean clearSchemeHandlerFactories() {
    try {
      return N_ClearSchemeHandlerFactories();
    } catch (Exception err) {
      err.printStackTrace();
    }
    return false;
  }

  private final Thread context = new Thread() {
    @Override
    public void start() {
      if (!isAlive() && super.getState() == State.NEW) {
        lock.lock();
        try {
          // Avoid a deadlock if called on the dispatcher thread.
          if (SwingUtilities.isEventDispatchThread()) {
            initialize();
          }
          super.start();
          // start thread and wait until CEF is up and running
          cefInitialized.awaitUninterruptibly();
        } finally {
          lock.unlock();
        }
      }
    }

    @Override
    public void run() {
      // synchronize startup with starting process
      lock.lock();
      try {
        // (1) Initialize native system.
        if (!isInitialized_) {
          initialize();
        }
        cefInitialized.signal();

        // (2) Handle message loop.
        if (OS.isMacintosh()) {
          cefShutdown.awaitUninterruptibly();
        } else {
          boolean doLoop = true;
          while (doLoop) {
            doMessageLoopWork();
            try {
              doLoop = !cefShutdown.await(33, TimeUnit.MILLISECONDS);
            } catch (Exception e) {
              // ignore exception
            }
          }
        }

        // (3) Shutdown sequence. Close all clients first.
        for (CefClient c : clients_) {
          c.dispose();
        }
        clients_.clear();

        // (4) Perform one last message loop (tidy up).
        doMessageLoopWork();

        // (5) Shutdown native system.
        shutdown();
        cefShutdownFinished.signal();

      } catch (Throwable e) {
        e.printStackTrace();
      } finally {
        lock.unlock();
      }
    }
  };

  /**
   * Initialize the context.
   * @return true on success
   */
  private final void initialize() {
    try {
      Runnable r = new Runnable() {
        @Override
        public void run() {
          String library_path = getJcefLibPath();
          System.out.println("initialize on " + Thread.currentThread() +
                             " with library path " + library_path);
          isInitialized_ =
              N_Initialize(library_path, appHandler_, osrSupportEnabled_);
        }
      };
      if (SwingUtilities.isEventDispatchThread())
        r.run();
      else
        SwingUtilities.invokeAndWait(r);
    } catch (Exception e) {
      e.printStackTrace();
    }
  }

  /**
   * Shut down the context.
   */
  private final void shutdown() {
    try {
      SwingUtilities.invokeAndWait(new Runnable() {
        @Override
        public void run() {
          System.out.println("  shutdown on " + Thread.currentThread());
          N_Shutdown();
        }
      });
    } catch (Exception e) {
      e.printStackTrace();
    }
  }

  /**
   * Perform a single message loop iteration.
   */
  private final void doMessageLoopWork() {
    try {
      SwingUtilities.invokeLater(new Runnable() {
        @Override
        public void run() {
          N_DoMessageLoopWork();
        }
      });
    } catch (Exception err) {
      err.printStackTrace();
    }
  }

  /**
   * Get the path which contains the jcef library
   * @return The path to the jcef library
   */
  private final String getJcefLibPath() {
    String library_path = System.getProperty("java.library.path");
    String[] paths = library_path.split(System.getProperty("path.separator"));
    for (String path : paths) {
      File dir = new File(path);
      String[] found = dir.list(new FilenameFilter() {
        @Override
        public boolean accept(File dir, String name) {
          return (name.equalsIgnoreCase("libjcef.dylib") ||
                  name.equalsIgnoreCase("libjcef.so") ||
                  name.equalsIgnoreCase("jcef.dll"));
        }
      });
      if (found != null && found.length != 0)
        return path;
    }
    return library_path;
  }

  private final native boolean N_Initialize(String pathToJavaDLL,
                                            CefAppHandler appHandler,
                                            boolean enableOsr);
  private final native void N_Shutdown();
  private final native void N_DoMessageLoopWork();
  private final native CefVersion N_GetVersion();
  private final native boolean N_RegisterSchemeHandlerFactory(String schemeName,
                                                              String domainName,
                                                              CefSchemeHandlerFactory factory);
  private final native boolean N_ClearSchemeHandlerFactories();
}
