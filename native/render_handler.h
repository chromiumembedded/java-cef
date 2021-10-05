// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef JCEF_NATIVE_RENDER_HANDLER_H_
#define JCEF_NATIVE_RENDER_HANDLER_H_
#pragma once

#include <jni.h>

#include "include/cef_render_handler.h"

#include "jni_scoped_helpers.h"

// RenderHandler implementation.
class RenderHandler : public CefRenderHandler {
 public:
  RenderHandler(JNIEnv* env, jobject handler);

  // CefRenderHandler methods
  virtual bool GetRootScreenRect(CefRefPtr<CefBrowser> browser,
                                 CefRect& rect) override;
  virtual void GetViewRect(CefRefPtr<CefBrowser> browser,
                           CefRect& rect) override;

  virtual bool GetScreenInfo(CefRefPtr<CefBrowser> browser,
                             CefScreenInfo& screen_info) override;

  virtual bool GetScreenPoint(CefRefPtr<CefBrowser> browser,
                              int viewX,
                              int viewY,
                              int& screenX,
                              int& screenY) override;
  virtual void OnPopupShow(CefRefPtr<CefBrowser> browser, bool show) override;
  virtual void OnPopupSize(CefRefPtr<CefBrowser> browser,
                           const CefRect& rect) override;
  virtual void OnPaint(CefRefPtr<CefBrowser> browser,
                       PaintElementType type,
                       const RectList& dirtyRects,
                       const void* buffer,
                       int width,
                       int height) override;
  virtual bool StartDragging(CefRefPtr<CefBrowser> browser,
                             CefRefPtr<CefDragData> drag_data,
                             DragOperationsMask allowed_ops,
                             int x,
                             int y) override;
  virtual void UpdateDragCursor(CefRefPtr<CefBrowser> browser,
                                DragOperation operation) override;

  bool GetViewRect(jobject browser, CefRect& rect);
  bool GetScreenPoint(jobject browser,
                      int viewX,
                      int viewY,
                      int& screenX,
                      int& screenY);

 protected:
  ScopedJNIObjectGlobal handle_;

  // Include the default reference counting implementation.
  IMPLEMENT_REFCOUNTING(RenderHandler);
};

#endif  // JCEF_NATIVE_RENDER_HANDLER_H_
