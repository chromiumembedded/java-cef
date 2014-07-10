// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef CEF_TESTS_CEFCLIENT_UTIL_MAC_H_
#define CEF_TESTS_CEFCLIENT_UTIL_MAC_H_

#include <string>
#include <jni.h>
#include "include/cef_base.h"
#include "include/cef_app.h"

#ifdef __OBJC__
@class NSView;
@class NSWindow;
#else
struct NSView;
class NSWindow;
#endif  // __OBJC__

namespace util_mac {

// Converts a relative path to an absolute path.
std::string GetAbsPath(const std::string& path);

// Returns true if |ptr| is an NSView instance.
bool IsNSView(void* ptr);

CefWindowHandle GetParentView(CefWindowHandle childView);
void TranslateRect(CefWindowHandle view, CefRect& orig);

bool CefInitializeOnMainThread(const CefMainArgs& args,
                               const CefSettings& settings,
                               CefRefPtr<CefApp> application);

void CefQuitMessageLoopOnMainThread();

void SetVisibility(CefWindowHandle handle, bool isVisible);

}  // namespace util_mac

#endif  // CEF_TESTS_CEFCLIENT_UTIL_MAC_H_
