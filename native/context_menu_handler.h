// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef JCEF_NATIVE_CONTEXT_MENU_HANDLER_H_
#define JCEF_NATIVE_CONTEXT_MENU_HANDLER_H_
#pragma once

#include <jni.h>
#include "include/cef_context_menu_handler.h"

#include "jni_scoped_helpers.h"

// ContextMenuHandler implementation.
class ContextMenuHandler : public CefContextMenuHandler {
 public:
  ContextMenuHandler(JNIEnv* env, jobject handler);

  // CefContextMenuHandler methods
  void OnBeforeContextMenu(CefRefPtr<CefBrowser> browser,
                           CefRefPtr<CefFrame> frame,
                           CefRefPtr<CefContextMenuParams> params,
                           CefRefPtr<CefMenuModel> model) override;
  bool OnContextMenuCommand(CefRefPtr<CefBrowser> browser,
                            CefRefPtr<CefFrame> frame,
                            CefRefPtr<CefContextMenuParams> params,
                            int command_id,
                            EventFlags event_flags) override;
  void OnContextMenuDismissed(CefRefPtr<CefBrowser> browser,
                              CefRefPtr<CefFrame> frame) override;

 protected:
  ScopedJNIObjectGlobal handle_;

  // Include the default reference counting implementation.
  IMPLEMENT_REFCOUNTING(ContextMenuHandler);
};

#endif  // JCEF_NATIVE_CONTEXT_MENU_HANDLER_H_
