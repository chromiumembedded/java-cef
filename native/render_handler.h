// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef JCEF_NATIVE_RENDER_HANDLER_H_
#define JCEF_NATIVE_RENDER_HANDLER_H_
#pragma once

#include <jni.h>
#include "include/cef_render_handler.h"

// RenderHandler implementation.
class RenderHandler : public CefRenderHandler {
 public:
  RenderHandler(JNIEnv* env, jobject handler);
  virtual ~RenderHandler();

  // CefRenderHandler methods
  virtual bool GetRootScreenRect(CefRefPtr<CefBrowser> browser,
                                 CefRect& rect) OVERRIDE;
  virtual bool GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect) OVERRIDE;
  virtual bool GetScreenPoint(CefRefPtr<CefBrowser> browser,
                              int viewX,
                              int viewY,
                              int& screenX,
                              int& screenY) OVERRIDE;
  virtual void OnPopupShow(CefRefPtr<CefBrowser> browser,
                           bool show) OVERRIDE;
  virtual void OnPopupSize(CefRefPtr<CefBrowser> browser,
                           const CefRect& rect) OVERRIDE;
  virtual void OnPaint(CefRefPtr<CefBrowser> browser,
                       PaintElementType type,
                       const RectList& dirtyRects,
                       const void* buffer,
                       int width, int height) OVERRIDE;
  virtual void OnCursorChange(CefRefPtr<CefBrowser> browser,
                              CefCursorHandle cursor,
                              CursorType type,
                              const CefCursorInfo& custom_cursor_info) OVERRIDE;
  virtual bool StartDragging(CefRefPtr<CefBrowser> browser,
                             CefRefPtr<CefDragData> drag_data,
                             DragOperationsMask allowed_ops,
                             int x, int y) OVERRIDE;
  virtual void UpdateDragCursor(CefRefPtr<CefBrowser> browser,
                                DragOperation operation) OVERRIDE;

  bool GetViewRect(jobject browser, CefRect& rect);
  bool GetScreenPoint(jobject browser,
                      int viewX,
                      int viewY,
                      int& screenX,
                      int& screenY);

 protected:
  jobject jhandler_;

  // Include the default reference counting implementation.
  IMPLEMENT_REFCOUNTING(RenderHandler);
};

#endif  // JCEF_NATIVE_RENDER_HANDLER_H_
