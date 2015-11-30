// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef;

import java.io.File;
import java.io.FilenameFilter;
import java.nio.file.Path;
import java.nio.file.Paths;
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
    public final int JCEF_COMMIT_NUMBER;

    public final int CEF_VERSION_MAJOR;
    public final int CEF_COMMIT_NUMBER;

    public final int CHROME_VERSION_MAJOR;
    public final int CHROME_VERSION_MINOR;
    public final int CHROME_VERSION_BUILD;
    public final int CHROME_VERSION_PATCH;

    private CefVersion(int jcefCommitNo, int cefMajor, int cefCommitNo,
                       int chrMajor, int chrMin, int chrBuild, int chrPatch) {
      JCEF_COMMIT_NUMBER = jcefCommitNo;

      CEF_VERSION_MAJOR = cefMajor;
      CEF_COMMIT_NUMBER = cefCommitNo;

      CHROME_VERSION_MAJOR = chrMajor;
      CHROME_VERSION_MINOR = chrMin;
      CHROME_VERSION_BUILD = chrBuild;
      CHROME_VERSION_PATCH = chrPatch;
    }

    public String getJcefVersion() {
      return CEF_VERSION_MAJOR
          + "." + CHROME_VERSION_BUILD
          + "." + JCEF_COMMIT_NUMBER;
    }

    public String getCefVersion() {
      return CEF_VERSION_MAJOR
          + "." + CHROME_VERSION_BUILD
          + "." + CEF_COMMIT_NUMBER;
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
   * The CefAppState gives you a hint if the CefApp is already usable or not usable
   * any more. See values for details.
   */
  public enum CefAppState {
    /**
     * No CefApp instance was created yet. Call getInstance() to create a new
     * one.
     */
    NONE,

    /**
     * CefApp is new created but not initialized yet. No CefClient and no
     * CefBrowser was created until now.
     */
    NEW,

    /**
     * CefApp is in its initializing process. Please wait until initializing is
     * finished.
     */
    INITIALIZING,

    /**
     * CefApp is up and running. At least one CefClient was created and the
     * message loop is running. You can use all classes and methods of JCEF now.
     */
    INITIALIZED,

    /**
     * CefApp is in its shutdown process. All CefClients and CefBrowser 
     * instances will be disposed. No new CefClient or CefBrowser is allowed to
     * be created. The message loop will be performed until all CefClients and
     * all CefBrowsers are disposed completely.
     */
    SHUTTING_DOWN,

    /**
     * CefApp is terminated and can't be used any more. You can shutdown the
     * application safely now.
     */
    TERMINATED
  }

  /**
   * According the singleton pattern, this attribute keeps
   * one single object of this class.
   */
  private static CefApp self = null;
  private static CefAppHandler appHandler_ = null;
  private static CefAppState state_ = CefAppState.NONE;
  private HashSet<CefClient> clients_ = new HashSet<CefClient>();
  private final Lock lock = new ReentrantLock();
  private final Condition cefInitialized = lock.newCondition();
  private final Condition cefShutdown = lock.newCondition();
  private CefSettings settings_ = null;
  private boolean executeDefaultShutdown_ = false;

  /**
   * To get an instance of this class, use the method
   * getInstance() instead of this CTOR.
   * 
   * The CTOR is called by getInstance() as needed and
   * loads all required JCEF libraries.
   * 
   * @throws UnsatisfiedLinkError
   */
  private CefApp(String [] args, CefSettings settings)
      throws UnsatisfiedLinkError {
    super(args);
    if (settings != null)
      settings_ = settings.clone();
    if (OS.isWindows()) {
      System.loadLibrary("jawt");
      System.loadLibrary("libcef");
    } else if (OS.isLinux()) {
      System.loadLibrary("cef");
    }
    System.loadLibrary("jcef");
    if (appHandler_ == null) {
      appHandler_ = this;
    }

    // Perform native pre-initialization.
    if (!N_PreInitialize())
      throw new IllegalStateException("Failed to pre-initialize native code");

    // On Mac we're registering a shutdown hook to shutdown the native CEF
    // part. This is useful if it is missed to call CefApp.disopse() before
    // System.exit(0). Unfortunately this approach works only for Mac because
    // on Windows this will cause a "performed on wrong thread" exception in
    // the native CEF part. And calling SwingUtilities.invokeAndWait doesn't
    // work because the dispatcher thread starts this shutdown hook (thread) and
    // calls "join" on it. Therefore invokeAndWait will end up in a deadlock.
    if (OS.isMacintosh()) {
      Runtime.getRuntime().addShutdownHook(new Thread("JCEF Shutdown Hook") {
        @Override
        public void run() {
          try {
            if (executeDefaultShutdown_) {
              return;
            }

            for (CefClient c : clients_) {
              c.dispose();
            }

            Thread.sleep(150);
            lock.lock();
            cefShutdown.signal();
            lock.unlock();
            N_Shutdown();

            // Avoid a deadlock. Give the native code at least 150 milliseconds
            // to terminate.
            Thread.sleep(150);
          } catch (Exception e) { }
        }
      });
    }
  }

  /**
   * Assign an AppHandler to CefApp. The AppHandler can be used to evaluate
   * application arguments, to register your own schemes and to hook into the
   * shutdown sequence. See CefAppHandler for more details.
   *
   * This method must be called before CefApp is initialized. CefApp will be
   * initialized automatically if you call createClient() the first time.
   * @param appHandler An instance of CefAppHandler.
   *
   * @throws IllegalStateException in case of CefApp is already initialized
   */
  public static void addAppHandler(CefAppHandler appHandler)
      throws IllegalStateException {
    if (getState().compareTo(CefAppState.NEW) > 0)
      throw new IllegalStateException("Must be called before CefApp is initialized");
    appHandler_ = appHandler;
  }

  /**
   * Get an instance of this class.
   * @return an instance of this class
   * @throws UnsatisfiedLinkError
   */
  public static synchronized CefApp getInstance() throws UnsatisfiedLinkError {
     return getInstance(null, null);
  }

  public static synchronized CefApp getInstance(String [] args)
      throws UnsatisfiedLinkError {
    return getInstance(args, null);
  }

  public static synchronized CefApp getInstance(CefSettings settings)
      throws UnsatisfiedLinkError {
    return getInstance(null, settings);
  }

  public static synchronized CefApp getInstance(String [] args,
      CefSettings settings) throws UnsatisfiedLinkError {
    if (settings != null) {
      if (getState() != CefAppState.NONE && getState() != CefAppState.NEW)
        throw new IllegalStateException("Settings can only be passed to CEF" +
            " before createClient is called the first time.");
    }
    if (self == null) {
      if (getState() == CefAppState.TERMINATED)
        throw new IllegalStateException("CefApp was terminated");
      self = new CefApp(args, settings);
      setState(CefAppState.NEW);
    }
    return self;
  }

  public final void setSettings(CefSettings settings)
      throws IllegalStateException {
    if (getState() != CefAppState.NONE && getState() != CefAppState.NEW)
      throw new IllegalStateException("Settings can only be passed to CEF" +
          " before createClient is called the first time.");
    settings_ = settings.clone();
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
   * Returns the current state of CefApp.
   * @return current state.
   */
  public final static CefAppState getState() {
    synchronized (state_) {
      return state_;
    }
  }

  private static final void setState(final CefAppState state) {
    synchronized (state_) {
      state_ = state;
    }
    SwingUtilities.invokeLater(new Runnable() {
      @Override
      public void run() {
        if (appHandler_ != null)
          appHandler_.stateHasChanged(state);
      }
    });
  }

  /**
   * To shutdown the system, it's important to call the dispose
   * method. Calling this method closes all client instances with
   * and all browser instances each client owns. After that the
   * message loop is terminated and CEF is shutdown.
   */
  public synchronized final void dispose() {
    switch (getState()) {
      case NEW:
        // Nothing to do inspite of invalidating the state
        setState(CefAppState.TERMINATED);
        break;

      case INITIALIZING:
      case INITIALIZED:
        // (3) Shutdown sequence. Close all clients and continue.
        setState(CefAppState.SHUTTING_DOWN);
        if (clients_.isEmpty()) {
          shutdown();
        } else {
          for (CefClient c : clients_) {
            c.dispose();
          }
        }
        break;

      case NONE:
      case SHUTTING_DOWN:
      case TERMINATED:
        // Ignore shutdown, CefApp is already terminated, in shutdown progress
        // or was never created (shouldn't be possible)
        break;
    }
  }

  /**
   * Creates a new client instance and returns it to the caller.
   * One client instance is responsible for one to many browser
   * instances
   * @return a new client instance
   */
  public synchronized CefClient createClient() {
    switch (getState()) {
      case NEW:
        setState(CefAppState.INITIALIZING);
        context.setDaemon(true);
        context.start();
        // FALL THRU

      case INITIALIZING:
      case INITIALIZED:
        CefClient client = new CefClient();
        clients_.add(client);
        return client;

      default:
        throw new IllegalStateException("Can't crate client in state " + state_);
    }

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

  private final Thread context = new Thread("JCEF-MessageQueue") {
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
        if (CefApp.getState().compareTo(CefAppState.INITIALIZED) < 0) {
          initialize();
        }
        cefInitialized.signal();

        // (2) Handle message loop.
        if (OS.isMacintosh() || OS.isWindows()) {
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
      } catch (Throwable e) {
        e.printStackTrace();
      } finally {
        lock.unlock();
      }
    }
  };

  /**
   * This method is called by a CefClient if it was disposed. This causes
   * CefApp to clean up its list of available client instances. If all clients
   * are disposed, CefApp will be shutdown.
   * @param client the disposed client.
   */
  final protected void clientWasDisposed(CefClient client) {
    clients_.remove(client);
    if (clients_.isEmpty() && getState().compareTo(CefAppState.SHUTTING_DOWN) >= 0) {
      // Shutdown native system. This will shutdown the message loop as well
      shutdown();
    }
  }

  /**
   * This method is invoked by the native code (currently on Mac only) in case
   * of a termination event (e.g. someone pressed CMD+Q). The native
   * termination process is interrupted until CefApp calls continueTerminate().
   */
  final protected void handleBeforeTerminate() {
    SwingUtilities.invokeLater(new Runnable() {
      @Override
      public void run() {
        CefAppHandler handler =
            (CefAppHandler) ((appHandler_ == null) ? this : appHandler_);
        if (!handler.onBeforeTerminate()) {
          executeDefaultShutdown_ = true;
          dispose();
        }
      }
    });
  }

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

          CefSettings settings = settings_ != null ? settings_ :
              new CefSettings();

          // Avoid to override user values by testing on NULL
          if (OS.isMacintosh()) {
            if (settings.browser_subprocess_path == null) {
              Path path = Paths.get(library_path,
                  "../Frameworks/jcef Helper.app/Contents/MacOS/jcef Helper");
              settings.browser_subprocess_path =
                  path.normalize().toAbsolutePath().toString();
            }
          } else if (OS.isWindows()) {
            if (settings.browser_subprocess_path == null) {
              settings.browser_subprocess_path = library_path +
                  "\\jcef_helper.exe";
            }
          } else if (OS.isLinux()) {
            if (settings.browser_subprocess_path == null)
              settings.browser_subprocess_path = library_path + "/jcef_helper";
            if (settings.resources_dir_path == null)
              settings.resources_dir_path = library_path;
            if (settings.locales_dir_path == null)
              settings.locales_dir_path = library_path + "/locales";
          }

          if (N_Initialize(library_path, appHandler_, settings))
            setState(CefAppState.INITIALIZED);
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
      SwingUtilities.invokeLater(new Runnable() {
        @Override
        public void run() {
          System.out.println("  shutdown on " + Thread.currentThread());

          // Shutdown message loop
          try {
            lock.lock();
            cefShutdown.signal();
          } finally {
            lock.unlock();
          }

          // Wait for termination of message loop
          try {
            context.join();
          } catch (InterruptedException e) { }

          // Shutdown native CEF
          N_Shutdown();
          System.out.println("shutdown complete");

          if (executeDefaultShutdown_) {
            continueTerminate();
          }
          setState(CefAppState.TERMINATED);
          CefApp.self = null;
        }
      });
    } catch (Exception e) {
      e.printStackTrace();
    }
  }

  private final void continueTerminate() {
    new Thread("JCEF Terminate") {
      @Override
      public void run() {
        N_ContinueDefaultTerminate();
      }
    }.start();
  }

  /**
   * Perform a single message loop iteration.
   */
  private final void doMessageLoopWork() {
    try {
      SwingUtilities.invokeLater(new Runnable() {
        @Override
        public void run() {
          if (getState() != CefAppState.TERMINATED)
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

  private final native boolean N_PreInitialize();
  private final native boolean N_Initialize(String pathToJavaDLL,
      CefAppHandler appHandler, CefSettings settings);
  private final native void N_Shutdown();
  private final native void N_DoMessageLoopWork();
  private final native CefVersion N_GetVersion();
  private final native boolean N_RegisterSchemeHandlerFactory(String schemeName,
      String domainName, CefSchemeHandlerFactory factory);
  private final native boolean N_ClearSchemeHandlerFactories();
  private final native void N_ContinueDefaultTerminate();
}
