// Copyright (c) 2015 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "util.h"

#include <X11/Xlib.h>
#undef Success

#include "include/base/cef_callback.h"

#include "jni_util.h"
#include "temp_window.h"

namespace util {

namespace {

void X_XMoveResizeWindow(unsigned long browserHandle,
                         int x,
                         int y,
                         unsigned int width,
                         unsigned int height) {
  ::Display* xdisplay = (::Display*)TempWindow::GetDisplay();
  XMoveResizeWindow(xdisplay, browserHandle, 0, 0, width, height);
  XFlush(xdisplay);
}

void X_XReparentWindow(unsigned long browserHandle,
                       unsigned long parentDrawable) {
  ::Display* xdisplay = (::Display*)TempWindow::GetDisplay();
  XReparentWindow(xdisplay, browserHandle, parentDrawable, 0, 0);
  XFlush(xdisplay);
}

void X_XSync(bool discard) {
  ::Display* xdisplay = (::Display*)TempWindow::GetDisplay();
  XSync(xdisplay, discard);
}

}  // namespace

// This function is called by LifeSpanHandler::OnAfterCreated().
void AddCefBrowser(CefRefPtr<CefBrowser> browser) {
  // TODO(jcef): Implement this function stub to do some platform dependent
  // tasks for the browser reference like registering mouse events.

  UNUSED(browser);
}

// This function is called by LifeSpanHandler::DoClose().
void DestroyCefBrowser(CefRefPtr<CefBrowser> browser) {
  browser->GetHost()->CloseBrowser(true);
}

CefWindowHandle GetWindowHandle(JNIEnv* env, jobject canvas) {
  return GetDrawableOfCanvas(canvas, env);
}

void SetParent(CefWindowHandle browserHandle,
               CefWindowHandle parentHandle,
               base::OnceClosure callback) {
  SetParentSync(browserHandle, parentHandle, nullptr, std::move(callback));
}

void SetParentSync(CefWindowHandle browserHandle,
                   CefWindowHandle parentHandle,
                   CriticalWait* waitCond,
                   base::OnceClosure callback) {
  if (waitCond) {
    waitCond->lock()->Lock();
  }
  if (parentHandle == kNullWindowHandle)
    parentHandle = TempWindow::GetWindowHandle();
  if (parentHandle != kNullWindowHandle && browserHandle != kNullWindowHandle)
    X_XReparentWindow(browserHandle, parentHandle);

  if (waitCond) {
    X_XSync(false);
    waitCond->WakeUp();
    waitCond->lock()->Unlock();
  }
  std::move(callback).Run();
}

void SetWindowBounds(CefWindowHandle browserHandle,
                     const CefRect& contentRect) {
  X_XMoveResizeWindow(browserHandle, contentRect.x, contentRect.y,
                      contentRect.width, contentRect.height);
}

void SetWindowSize(CefWindowHandle browserHandle, int width, int height) {
  X_XMoveResizeWindow(browserHandle, 0, 0, width, height);
}

}  // namespace util
