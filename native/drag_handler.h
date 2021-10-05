// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef JCEF_NATIVE_DRAG_HANDLER_H_
#define JCEF_NATIVE_DRAG_HANDLER_H_
#pragma once

#include <jni.h>

#include "include/cef_drag_handler.h"

#include "jni_scoped_helpers.h"

// DragHandler implementation.
class DragHandler : public CefDragHandler {
 public:
  DragHandler(JNIEnv* env, jobject handler);

  // CefDragHandler methods
  bool OnDragEnter(CefRefPtr<CefBrowser> browser,
                   CefRefPtr<CefDragData> dragData,
                   CefDragHandler::DragOperationsMask mask) override;

 protected:
  ScopedJNIObjectGlobal handle_;

  // Include the default reference counting implementation.
  IMPLEMENT_REFCOUNTING(DragHandler);
};

#endif  // JCEF_NATIVE_DRAG_HANDLER_H_
