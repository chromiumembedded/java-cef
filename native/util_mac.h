// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef JCEF_NATIVE_UTIL_MAC_H_
#define JCEF_NATIVE_UTIL_MAC_H_

#include <jni.h>
#include <string>

#include "include/cef_app.h"
#include "include/cef_base.h"

#include "client_app.h"

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

CefWindowHandle CreateBrowserContentView(NSWindow* window, CefRect& orig);
void TranslateRect(CefWindowHandle view, CefRect& orig);

bool CefInitializeOnMainThread(const CefMainArgs& args,
                               const CefSettings& settings,
                               CefRefPtr<ClientApp> application);

void CefShutdownOnMainThread();

void CefDoMessageLoopWorkOnMainThread();

void SetVisibility(CefWindowHandle handle, bool isVisible);

void UpdateView(CefWindowHandle handle,
                CefRect contentRect,
                CefRect browserRect);

}  // namespace util_mac

#endif  // JCEF_NATIVE_UTIL_MAC_H_
