// Copyright (c) 2016 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef JCEF_NATIVE_CONTEXT_H_
#define JCEF_NATIVE_CONTEXT_H_
#pragma once

#include <jni.h>

#include "include/base/cef_macros.h"
#include "include/base/cef_scoped_ptr.h"
#include "include/base/cef_thread_checker.h"

#include "temp_window.h"

#if defined(OS_MACOSX)
#include "include/wrapper/cef_library_loader.h"
#endif

class Context {
 public:
  static void Create();
  static void Destroy();

#if defined(OS_MACOSX)
  void set_library_loader(CefScopedLibraryLoader* library_loader) {
    DCHECK(!library_loader_);
    library_loader_.reset(library_loader);
  }
#endif

  // Returns the singleton instance of this object.
  static Context* GetInstance();

  bool PreInitialize(JNIEnv* env, jobject c);
  bool Initialize(JNIEnv* env,
                  jobject c,
                  jstring argPathToJavaDLL,
                  jobject appHandler,
                  jobject jsettings);
  void OnContextInitialized();
  void DoMessageLoopWork();
  void Shutdown();

 private:
  Context();
  ~Context();

  bool external_message_pump_;
  base::ThreadChecker thread_checker_;

  scoped_ptr<TempWindow> temp_window_;

#if defined(OS_MACOSX)
  scoped_ptr<CefScopedLibraryLoader> library_loader_;
#endif

  DISALLOW_COPY_AND_ASSIGN(Context);
};

#endif  // JCEF_NATIVE_CONTEXT_H_
