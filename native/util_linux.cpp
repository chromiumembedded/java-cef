// Copyright (c) 2015 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "util.h"

#include <X11/Xlib.h>
#undef Success

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
}

void X_XReparentWindow(unsigned long browserHandle,
                       unsigned long parentDrawable) {
  ::Display* xdisplay = (::Display*)TempWindow::GetDisplay();
  XReparentWindow(xdisplay, browserHandle, parentDrawable, 0, 0);
}

void X_XSetInputFocusParent(unsigned long browserHandle) {
  ::Display* xdisplay = (::Display*)TempWindow::GetDisplay();

  Window root_win;
  Window parent_win;
  Window* child_windows;
  unsigned int num_child_windows;
  XQueryTree(xdisplay, browserHandle, &root_win, &parent_win, &child_windows,
             &num_child_windows);
  XFree(child_windows);

  XSetInputFocus(xdisplay, parent_win, RevertToParent, CurrentTime);
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
  // TODO(jcef): Implement this function stub to do some platform dependent
  // cleanup tasks for the browser reference.

  UNUSED(browser);
}

CefWindowHandle GetWindowHandle(JNIEnv* env, jobject canvas) {
  return GetDrawableOfCanvas(canvas, env);
}

void SetParent(CefWindowHandle browserHandle, CefWindowHandle parentHandle) {
  if (parentHandle == kNullWindowHandle)
    parentHandle = TempWindow::GetWindowHandle();
  if (parentHandle != kNullWindowHandle && browserHandle != kNullWindowHandle)
    X_XReparentWindow(browserHandle, parentHandle);
}

void SetWindowBounds(CefWindowHandle browserHandle,
                     const CefRect& contentRect) {
  X_XMoveResizeWindow(browserHandle, contentRect.x, contentRect.y,
                      contentRect.width, contentRect.height);
}

void SetWindowSize(CefWindowHandle browserHandle, int width, int height) {
  X_XMoveResizeWindow(browserHandle, 0, 0, width, height);
}

void FocusParent(CefWindowHandle browserHandle) {
  X_XSetInputFocusParent(browserHandle);
}

}  // namespace util
