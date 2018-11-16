// Copyright (c) 2016 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "context.h"

#include "include/cef_app.h"

#include "client_app.h"
#include "jni_util.h"

#if defined(OS_MACOSX)
#include "util_mac.h"
#endif

#if defined(OS_POSIX)
#include "signal_restore_posix.h"
#endif

namespace {

Context* g_context = NULL;

}  // namespace

// static
void Context::Create() {
  new Context();
}

// static
void Context::Destroy() {
  DCHECK(g_context);
  if (g_context)
    delete g_context;
}

// static
Context* Context::GetInstance() {
  return g_context;
}

bool Context::PreInitialize(JNIEnv* env, jobject c) {
  DCHECK(thread_checker_.CalledOnValidThread());

  JavaVM* jvm;
  jint rs = env->GetJavaVM(&jvm);
  DCHECK_EQ(rs, JNI_OK);
  if (rs != JNI_OK)
    return JNI_FALSE;
  SetJVM(jvm);

  jobject javaClass = env->GetObjectClass(c);
  jobject javaClassLoader = NULL;
  JNI_CALL_METHOD(env, javaClass, "getClassLoader", "()Ljava/lang/ClassLoader;",
                  Object, javaClassLoader);
  env->DeleteLocalRef(javaClass);
  ASSERT(javaClassLoader);
  if (!javaClassLoader)
    return false;
  SetJavaClassLoader(env, javaClassLoader);

  return true;
}

bool Context::Initialize(JNIEnv* env,
                         jobject c,
                         jstring argPathToJavaDLL,
                         jobject appHandler,
                         jobject jsettings) {
  DCHECK(thread_checker_.CalledOnValidThread());

#if defined(OS_WIN)
  CefMainArgs main_args(::GetModuleHandle(NULL));
#else
  CefMainArgs main_args(0, NULL);
#endif

  const std::string& module_dir = GetJNIString(env, argPathToJavaDLL);

  CefSettings settings = GetJNISettings(env, jsettings);

  // Sandbox is not supported because:
  // - Use of a separate sub-process executable on Windows.
  // - Use of a temporary file to communicate custom schemes to the
  //   renderer process.
  settings.no_sandbox = true;

#if defined(OS_WIN) || defined(OS_LINUX)
  // Use external message pump with OSR.
  external_message_pump_ = !!settings.windowless_rendering_enabled;
  if (!external_message_pump_) {
    // Windowed rendering on Windows requires multi-threaded message loop,
    // otherwise something eats the messages required by Java and the Java
    // window becomes unresponsive.
    //
    // Actually the same appears to be true for Linux, which is why we also
    // need multithreaded message loops there. Note that however, on Linux
    // it is more difficult to get this to work: it is necessary for the first
    // call to Xlib to be a call to XInitThreads! Since Java itself calls
    // Xlib when it initializes the first window, an application must make
    // sure to invoke this method before any other Xlib functions are called
    // - including by the Java runtime itself, which makes this feat a little
    // tricky. The CefApp class exposes a static method for this purpose,
    // initXlibForMultithreading(), but the host application must load the
    // jcef native lib by itself in order to use it, and it must invoke it
    // VERY early, ideally at the beginning of the main method.
    // Another neat trick to get this done is to create a special native lib
    // just for this purpose like described in this StackOverflow thread:
    // https://stackoverflow.com/questions/24559368
    settings.multi_threaded_message_loop = true;
  }
#endif

  // Use CefAppHandler.onScheduleMessagePumpWork to schedule calls to
  // DoMessageLoopWork.
  settings.external_message_pump = external_message_pump_;

  CefRefPtr<ClientApp> client_app(
      new ClientApp(module_dir, CefString(&settings.cache_path), appHandler));
  bool res = false;

#if defined(OS_POSIX)
  // CefInitialize will reset signal handlers. Backup/restore the original
  // signal handlers to avoid crashes in the JVM (see issue #41).
  BackupSignalHandlers();
#endif

#if defined(OS_MACOSX)
  res = util_mac::CefInitializeOnMainThread(main_args, settings,
                                            client_app.get());
#else
  res = CefInitialize(main_args, settings, client_app.get(), NULL);
#endif

#if defined(OS_POSIX)
  RestoreSignalHandlers();
#endif

  return res;
}

void Context::OnContextInitialized() {
  REQUIRE_UI_THREAD();
  temp_window_.reset(new TempWindow());
}

void Context::DoMessageLoopWork() {
  DCHECK(thread_checker_.CalledOnValidThread());

#if defined(OS_MACOSX)
  util_mac::CefDoMessageLoopWorkOnMainThread();
#else
  CefDoMessageLoopWork();
#endif
}

void Context::Shutdown() {
  DCHECK(thread_checker_.CalledOnValidThread());

  // Clear scheme handler factories on shutdown to avoid refcount DCHECK.
  CefClearSchemeHandlerFactories();

  ClientApp::eraseTempFiles();
#if defined(OS_MACOSX)
  util_mac::CefShutdownOnMainThread();
#else
  // Pump CefDoMessageLoopWork a few times before shutting down.
  if (external_message_pump_) {
    for (int i = 0; i < 10; ++i)
      CefDoMessageLoopWork();
  }

  temp_window_.reset(nullptr);

  CefShutdown();
#endif
}

Context::Context() : external_message_pump_(true) {
  DCHECK(!g_context);
  g_context = this;

#if defined(OS_MACOSX)
  // On macOS we create this object very early to allow LibraryLoader
  // assignment. However, we still want the PreInitialize() call to determine
  // thread ownership.
  thread_checker_.DetachFromThread();
#endif
}

Context::~Context() {
  DCHECK(thread_checker_.CalledOnValidThread());
  g_context = NULL;
}
