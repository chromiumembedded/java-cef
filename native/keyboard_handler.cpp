// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "keyboard_handler.h"

#include "client_handler.h"
#include "jni_util.h"
#include "util.h"

KeyboardHandler::KeyboardHandler(JNIEnv* env, jobject handler) {
  jhandler_ = env->NewGlobalRef(handler);
}

KeyboardHandler::~KeyboardHandler() {
  JNIEnv* env = GetJNIEnv();
  env->DeleteGlobalRef(jhandler_);
}

jobject getjEvent(JNIEnv* env, const CefKeyEvent& event) {
  jobject jkeyEventType = NULL;
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

  jobject jevent = NewJNIObject(
      env, "org/cef/handler/CefKeyboardHandler$CefKeyEvent",
      "(Lorg/cef/handler/CefKeyboardHandler$CefKeyEvent$EventType;IIIZCCZ)V",
      jkeyEventType, event.modifiers, event.windows_key_code,
      event.native_key_code, (event.is_system_key != 0 ? JNI_TRUE : JNI_FALSE),
      event.character, event.unmodified_character,
      (event.focus_on_editable_field != 0 ? JNI_TRUE : JNI_FALSE));
  return jevent;
}

bool KeyboardHandler::OnPreKeyEvent(CefRefPtr<CefBrowser> browser,
                                    const CefKeyEvent& event,
                                    CefEventHandle os_event,
                                    bool* is_keyboard_shortcut) {
  JNIEnv* env = GetJNIEnv();
  if (!env)
    return false;

  jobject jevent = getjEvent(env, event);
  if (!jevent)
    return false;

  jobject jboolRef = NewJNIBoolRef(env, *is_keyboard_shortcut);
  if (!jboolRef) {
    env->DeleteLocalRef(jevent);
    return false;
  }

  jboolean jresult = JNI_FALSE;
  JNI_CALL_METHOD(env, jhandler_, "onPreKeyEvent",
                  "(Lorg/cef/browser/CefBrowser;Lorg/cef/handler/"
                  "CefKeyboardHandler$CefKeyEvent;Lorg/cef/misc/BoolRef;)Z",
                  Boolean, jresult, GetJNIBrowser(browser), jevent, jboolRef);

  *is_keyboard_shortcut = GetJNIBoolRef(env, jboolRef);
  env->DeleteLocalRef(jevent);
  env->DeleteLocalRef(jboolRef);
  return (jresult != JNI_FALSE);
}

bool KeyboardHandler::OnKeyEvent(CefRefPtr<CefBrowser> browser,
                                 const CefKeyEvent& event,
                                 CefEventHandle os_event) {
  JNIEnv* env = GetJNIEnv();
  if (!env)
    return false;

  jobject jevent = getjEvent(env, event);
  if (!jevent)
    return false;

  jboolean jresult = JNI_FALSE;
  JNI_CALL_METHOD(env, jhandler_, "onKeyEvent",
                  "(Lorg/cef/browser/CefBrowser;Lorg/cef/handler/"
                  "CefKeyboardHandler$CefKeyEvent;)Z",
                  Boolean, jresult, GetJNIBrowser(browser), jevent);

  env->DeleteLocalRef(jevent);
  return (jresult != JNI_FALSE);
}
