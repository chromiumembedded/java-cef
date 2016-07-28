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

bool Context::PreInitialize(JNIEnv *env, jobject c) {
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

bool Context::Initialize(JNIEnv *env, jobject c,
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

#if defined(OS_WIN)
  // Use external message pump with OSR.
  external_message_pump_ = !!settings.windowless_rendering_enabled;
  if (!external_message_pump_) {
    // Windowed rendering on Windows requires multi-threaded message loop,
    // otherwise something eats the messages required by Java and the Java
    // window becomes unresponsive.
    settings.multi_threaded_message_loop = true;
  }
#endif

  // Use CefAppHandler.onScheduleMessagePumpWork to schedule calls to
  // DoMessageLoopWork.
  settings.external_message_pump = external_message_pump_;

  CefRefPtr<ClientApp> client_app(new ClientApp(module_dir, appHandler));
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

  CefShutdown();
#endif
}

void Context::DoMessageLoopWork() {
  DCHECK(thread_checker_.CalledOnValidThread());

#if defined(OS_MACOSX)
  util_mac::CefDoMessageLoopWorkOnMainThread();
#else
  CefDoMessageLoopWork();
#endif
}

Context::Context()
  : external_message_pump_(true) {
  DCHECK(!g_context);
  g_context = this;
}

Context::~Context() {
  DCHECK(thread_checker_.CalledOnValidThread());
  g_context = NULL;
}
