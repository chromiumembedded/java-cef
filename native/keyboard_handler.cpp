// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "keyboard_handler.h"

#include "client_handler.h"
#include "jni_util.h"
#include "util.h"

namespace {

jobject NewJNIKeyEvent(JNIEnv* env, const CefKeyEvent& event) {
  ScopedJNIObjectResult jkeyEventType(env);
  switch (event.type) {
    default:
      JNI_CASE(env, "org/cef/handler/CefKeyboardHandler$CefKeyEvent$EventType",
               KEYEVENT_RAWKEYDOWN, jkeyEventType);
      JNI_CASE(env, "org/cef/handler/CefKeyboardHandler$CefKeyEvent$EventType",
               KEYEVENT_KEYDOWN, jkeyEventType);
      JNI_CASE(env, "org/cef/handler/CefKeyboardHandler$CefKeyEvent$EventType",
               KEYEVENT_KEYUP, jkeyEventType);
      JNI_CASE(env, "org/cef/handler/CefKeyboardHandler$CefKeyEvent$EventType",
               KEYEVENT_CHAR, jkeyEventType);
  }

  return NewJNIObject(
      env, "org/cef/handler/CefKeyboardHandler$CefKeyEvent",
      "(Lorg/cef/handler/CefKeyboardHandler$CefKeyEvent$EventType;IIIZCCZ)V",
      jkeyEventType.get(), event.modifiers, event.windows_key_code,
      event.native_key_code, (event.is_system_key != 0 ? JNI_TRUE : JNI_FALSE),
      event.character, event.unmodified_character,
      (event.focus_on_editable_field != 0 ? JNI_TRUE : JNI_FALSE));
}

}  // namespace

KeyboardHandler::KeyboardHandler(JNIEnv* env, jobject handler)
    : handle_(env, handler) {}

bool KeyboardHandler::OnPreKeyEvent(CefRefPtr<CefBrowser> browser,
                                    const CefKeyEvent& event,
                                    CefEventHandle os_event,
                                    bool* is_keyboard_shortcut) {
  ScopedJNIEnv env;
  if (!env)
    return false;

  ScopedJNIObjectLocal jevent(env, NewJNIKeyEvent(env, event));
  if (!jevent)
    return false;

  ScopedJNIBoolRef jboolRef(env, *is_keyboard_shortcut);
  if (!jboolRef)
    return false;

  jboolean jresult = JNI_FALSE;
  ScopedJNIBrowser jbrowser(env, browser);
  JNI_CALL_METHOD(env, handle_, "onPreKeyEvent",
                  "(Lorg/cef/browser/CefBrowser;Lorg/cef/handler/"
                  "CefKeyboardHandler$CefKeyEvent;Lorg/cef/misc/BoolRef;)Z",
                  Boolean, jresult, jbrowser.get(), jevent.get(),
                  jboolRef.get());

  *is_keyboard_shortcut = jboolRef;
  return (jresult != JNI_FALSE);
}

bool KeyboardHandler::OnKeyEvent(CefRefPtr<CefBrowser> browser,
                                 const CefKeyEvent& event,
                                 CefEventHandle os_event) {
  ScopedJNIEnv env;
  if (!env)
    return false;

  ScopedJNIObjectLocal jevent(env, NewJNIKeyEvent(env, event));
  if (!jevent)
    return false;

  jboolean jresult = JNI_FALSE;
  ScopedJNIBrowser jbrowser(env, browser);
  JNI_CALL_METHOD(env, handle_, "onKeyEvent",
                  "(Lorg/cef/browser/CefBrowser;Lorg/cef/handler/"
                  "CefKeyboardHandler$CefKeyEvent;)Z",
                  Boolean, jresult, jbrowser.get(), jevent.get());

  return (jresult != JNI_FALSE);
}
