// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef JCEF_NATIVE_KEYBOARD_HANDLER_H_
#define JCEF_NATIVE_KEYBOARD_HANDLER_H_
#pragma once

#include <jni.h>
#include "include/cef_keyboard_handler.h"

// KeyboardHandler implementation.
class KeyboardHandler : public CefKeyboardHandler {
 public:
  KeyboardHandler(JNIEnv* env, jobject handler);
  virtual ~KeyboardHandler();

  // CefKeyboardHandler methods
  virtual bool OnPreKeyEvent(CefRefPtr<CefBrowser> browser,
                             const CefKeyEvent& event,
                             CefEventHandle os_event,
                             bool* is_keyboard_shortcut) OVERRIDE;

  virtual bool OnKeyEvent(CefRefPtr<CefBrowser> browser,
                          const CefKeyEvent& event,
                          CefEventHandle os_event) OVERRIDE;

 protected:
  jobject jhandler_;

  // Include the default reference counting implementation.
  IMPLEMENT_REFCOUNTING(KeyboardHandler);
};

#endif  // JCEF_NATIVE_KEYBOARD_HANDLER_H_
