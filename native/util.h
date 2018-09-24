// Copyright (c) 2011 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef JCEF_NATIVE_UTIL_H_
#define JCEF_NATIVE_UTIL_H_
#pragma once

#include <string>

#ifdef USING_JAVA

#include <jni.h>

#include "include/cef_browser.h"
#include "include/cef_task.h"

#endif  // USING_JAVA

#if defined(OS_WIN)

#include <windows.h>  // NOLINT(build/include_order)

#ifndef NDEBUG
#define ASSERT(condition) \
  if (!(condition)) {     \
    DebugBreak();         \
  }
#else
#define ASSERT(condition) ((void)0)
#endif

#else  // !OS_WIN

#include <assert.h>  // NOLINT(build/include_order)

#ifndef NDEBUG
#define ASSERT(condition) \
  if (!(condition)) {     \
    assert(false);        \
  }
#else
#define ASSERT(condition) ((void)0)
#endif

#endif  // !OS_WIN

#if defined(OS_MACOSX)

// due a missing export definition within Java 7u40 (and prior)
// for Mac OS X, we have to redefine JNIEXPORT.
// Otherwise the JNI function names wouldn't be exported to libjcef
#ifdef JNIEXPORT
#undef JNIEXPORT
#define JNIEXPORT __attribute__((visibility("default")))
#endif  // JNIEXPORT

#endif  // OS_MACOSX

#define REQUIRE_UI_THREAD() ASSERT(CefCurrentlyOn(TID_UI));
#define REQUIRE_IO_THREAD() ASSERT(CefCurrentlyOn(TID_IO));
#define REQUIRE_FILE_THREAD() ASSERT(CefCurrentlyOn(TID_FILE));

// Used to silence warnings about unused variables.
#if !defined(UNUSED)
#define UNUSED(x) ((void)(x))
#endif

namespace util {

// Get the process-id of the current process.
int GetPid();

// Get the process-id of the parent process (that process that launched the
// current process).
int GetParentPid();

// Returns the absolute path for a temporary file whose name is unique to the
// process. |identifer| is attached to the filename.
std::string GetTempFileName(const std::string& identifer, bool useParentId);

#ifdef USING_JAVA

// Called by LifeSpanHandler::OnAfterCreated() to do some platform dependent
// tasks for the browser reference like registering mouse events.
void AddCefBrowser(CefRefPtr<CefBrowser> browser);

// Called by CefBrowser.close(true) to destroy the native browser window.
void DestroyCefBrowser(CefRefPtr<CefBrowser> browser);

#if defined(OS_MACOSX)

// Set the parent of |browserHandle|. If the parent is NULL the browser will be
// parented to the TempWindow.
void SetParent(CefWindowHandle browserHandle, jlong parentHandle);

#else  // !defined(OS_MACOSX)

// Return the window handle for the specified canvas.
CefWindowHandle GetWindowHandle(JNIEnv* env, jobject canvas);

// Set the parent of |browserHandle|. If the parent is NULL the browser will be
// parented to the TempWindow.
void SetParent(CefWindowHandle browserHandle, CefWindowHandle parentHandle);

// Set the window bounds for |browserHandle|.
void SetWindowBounds(CefWindowHandle browserHandle, const CefRect& contentRect);

// Set the window size for |browserHandle|.
void SetWindowSize(CefWindowHandle browserHandle, int width, int height);

// Set focus to the parent of |browserHandle|.
void FocusParent(CefWindowHandle browserHandle);

#endif  // !defined(OS_MACOSX)

#endif  // USING_JAVA

}  // namespace util

#endif  // JCEF_NATIVE_UTIL_H_
