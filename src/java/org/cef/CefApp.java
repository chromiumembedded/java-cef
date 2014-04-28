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

import org.cef.handler.CefAppHandler;
import org.cef.handler.CefAppHandlerAdapter;

/**
 * Exposes static methods for managing the global CEF context.
 */
public class CefApp extends CefAppHandlerAdapter {
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

  /**
   * To get an instance of this class, use the method
   * getInstance() instead of this CTOR.
   * 
   * The CTOR is called by getInstance() as needed and
   * loads all required JCEF libraries.
   * 
   * @throws UnsatisfiedLinkError
   */
  private CefApp(String [] args) throws UnsatisfiedLinkError {
    super(args);
    if (OS.isWindows()) {
      System.loadLibrary("icudt");
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
     return getInstance(null);
  }

  public static synchronized CefApp getInstance(String [] args) throws UnsatisfiedLinkError {
    if (self == null) {
      self = new CefApp(args);
    }
    return self;
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

  private final Thread context = new Thread() {
    @Override
    public void start() {
      if (!isAlive() && super.getState() == State.NEW) {
        lock.lock();
        try {
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
        initialize();
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
          c.destroyAllBrowser();
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
      SwingUtilities.invokeAndWait(new Runnable() {
        @Override
        public void run() {
          String library_path = getJcefLibPath();
          System.out.println("initialize on " + Thread.currentThread() +
                             " with library path " + library_path);
          isInitialized_ = N_Initialize(library_path, appHandler_);
        }
      });
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

  private final native boolean N_Initialize(String pathToJavaDLL, CefAppHandler appHandler);
  private final native void N_Shutdown();
  private final native void N_DoMessageLoopWork();
}
