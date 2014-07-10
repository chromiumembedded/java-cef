// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef CEF_TESTS_CEFCLIENT_DRAG_HANDLER_H_
#define CEF_TESTS_CEFCLIENT_DRAG_HANDLER_H_
#pragma once

#include <jni.h>
#include "include/cef_drag_handler.h"

// DragHandler implementation.
class DragHandler : public CefDragHandler {
 public:
  DragHandler(JNIEnv* env, jobject handler);
  virtual ~DragHandler();

  // CefDragHandler methods
  virtual bool OnDragEnter(CefRefPtr<CefBrowser> browser,
                           CefRefPtr<CefDragData> dragData,
                           CefDragHandler::DragOperationsMask mask) OVERRIDE;

 protected:
  jobject jhandler_;

  // Include the default reference counting implementation.
  IMPLEMENT_REFCOUNTING(DragHandler);
  // Include the default locking implementation.
  IMPLEMENT_LOCKING(DragHandler);
};

#endif  // CEF_TESTS_CEFCLIENT_DRAG_HANDLER_H_
