// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "display_handler.h"

#include "jni_util.h"

namespace {

int GetCursorId(cef_cursor_type_t type) {
  ScopedJNIEnv env;
  if (!env)
    return 0;

  const char* cursorClassName = "java/awt/Cursor";
  JNI_STATIC_DEFINE_INT_RV(env, ScopedJNIClass(env, cursorClassName),
                           CROSSHAIR_CURSOR, 0);
  JNI_STATIC_DEFINE_INT_RV(env, ScopedJNIClass(env, cursorClassName),
                           DEFAULT_CURSOR, 0);
  JNI_STATIC_DEFINE_INT_RV(env, ScopedJNIClass(env, cursorClassName),
                           E_RESIZE_CURSOR, 0);
  JNI_STATIC_DEFINE_INT_RV(env, ScopedJNIClass(env, cursorClassName),
                           HAND_CURSOR, 0);
  JNI_STATIC_DEFINE_INT_RV(env, ScopedJNIClass(env, cursorClassName),
                           MOVE_CURSOR, 0);
  JNI_STATIC_DEFINE_INT_RV(env, ScopedJNIClass(env, cursorClassName),
                           N_RESIZE_CURSOR, 0);
  JNI_STATIC_DEFINE_INT_RV(env, ScopedJNIClass(env, cursorClassName),
                           NE_RESIZE_CURSOR, 0);
  JNI_STATIC_DEFINE_INT_RV(env, ScopedJNIClass(env, cursorClassName),
                           NW_RESIZE_CURSOR, 0);
  JNI_STATIC_DEFINE_INT_RV(env, ScopedJNIClass(env, cursorClassName),
                           S_RESIZE_CURSOR, 0);
  JNI_STATIC_DEFINE_INT_RV(env, ScopedJNIClass(env, cursorClassName),
                           SE_RESIZE_CURSOR, 0);
  JNI_STATIC_DEFINE_INT_RV(env, ScopedJNIClass(env, cursorClassName),
                           SW_RESIZE_CURSOR, 0);
  JNI_STATIC_DEFINE_INT_RV(env, ScopedJNIClass(env, cursorClassName),
                           TEXT_CURSOR, 0);
  JNI_STATIC_DEFINE_INT_RV(env, ScopedJNIClass(env, cursorClassName),
                           W_RESIZE_CURSOR, 0);
  JNI_STATIC_DEFINE_INT_RV(env, ScopedJNIClass(env, cursorClassName),
                           WAIT_CURSOR, 0);

  switch (type) {
    case CT_CROSS:
      return JNI_STATIC(CROSSHAIR_CURSOR);
    case CT_HAND:
      return JNI_STATIC(HAND_CURSOR);
    case CT_IBEAM:
      return JNI_STATIC(TEXT_CURSOR);
    case CT_WAIT:
      return JNI_STATIC(WAIT_CURSOR);
    case CT_EASTRESIZE:
      return JNI_STATIC(E_RESIZE_CURSOR);
    case CT_NORTHRESIZE:
      return JNI_STATIC(N_RESIZE_CURSOR);
    case CT_NORTHEASTRESIZE:
      return JNI_STATIC(NE_RESIZE_CURSOR);
    case CT_NORTHWESTRESIZE:
      return JNI_STATIC(NW_RESIZE_CURSOR);
    case CT_SOUTHRESIZE:
      return JNI_STATIC(S_RESIZE_CURSOR);
    case CT_SOUTHEASTRESIZE:
      return JNI_STATIC(SE_RESIZE_CURSOR);
    case CT_SOUTHWESTRESIZE:
      return JNI_STATIC(SW_RESIZE_CURSOR);
    case CT_WESTRESIZE:
      return JNI_STATIC(W_RESIZE_CURSOR);
    case CT_MOVE:
      return JNI_STATIC(MOVE_CURSOR);
    default:
      return JNI_STATIC(DEFAULT_CURSOR);
  }
}

}  // namespace

DisplayHandler::DisplayHandler(JNIEnv* env, jobject handler)
    : handle_(env, handler) {}

void DisplayHandler::OnAddressChange(CefRefPtr<CefBrowser> browser,
                                     CefRefPtr<CefFrame> frame,
                                     const CefString& url) {
  ScopedJNIEnv env;
  if (!env)
    return;

  ScopedJNIBrowser jbrowser(env, browser);
  ScopedJNIFrame jframe(env, frame);
  jframe.SetTemporary();
  ScopedJNIString jurl(env, url);

  JNI_CALL_VOID_METHOD(env, handle_, "onAddressChange",
                       "(Lorg/cef/browser/CefBrowser;Lorg/cef/browser/"
                       "CefFrame;Ljava/lang/String;)V",
                       jbrowser.get(), jframe.get(), jurl.get());
}

void DisplayHandler::OnTitleChange(CefRefPtr<CefBrowser> browser,
                                   const CefString& title) {
  ScopedJNIEnv env;
  if (!env)
    return;

  ScopedJNIBrowser jbrowser(env, browser);
  ScopedJNIString jtitle(env, title);

  JNI_CALL_VOID_METHOD(env, handle_, "onTitleChange",
                       "(Lorg/cef/browser/CefBrowser;Ljava/lang/String;)V",
                       jbrowser.get(), jtitle.get());
}

bool DisplayHandler::OnTooltip(CefRefPtr<CefBrowser> browser, CefString& text) {
  ScopedJNIEnv env;
  if (!env)
    return false;

  ScopedJNIBrowser jbrowser(env, browser);
  ScopedJNIString jtext(env, text);
  jboolean jreturn = JNI_FALSE;

  JNI_CALL_METHOD(env, handle_, "onTooltip",
                  "(Lorg/cef/browser/CefBrowser;Ljava/lang/String;)Z", Boolean,
                  jreturn, jbrowser.get(), jtext.get());

  return (jreturn != JNI_FALSE);
}

void DisplayHandler::OnStatusMessage(CefRefPtr<CefBrowser> browser,
                                     const CefString& value) {
  ScopedJNIEnv env;
  if (!env)
    return;

  ScopedJNIBrowser jbrowser(env, browser);
  ScopedJNIString jvalue(env, value);

  JNI_CALL_VOID_METHOD(env, handle_, "onStatusMessage",
                       "(Lorg/cef/browser/CefBrowser;Ljava/lang/String;)V",
                       jbrowser.get(), jvalue.get());
}

bool DisplayHandler::OnConsoleMessage(CefRefPtr<CefBrowser> browser,
                                      cef_log_severity_t level,
                                      const CefString& message,
                                      const CefString& source,
                                      int line) {
  ScopedJNIEnv env;
  if (!env)
    return false;

  jobject jlevel = nullptr;
  switch (level) {
    JNI_CASE(env, "org/cef/CefSettings$LogSeverity", LOGSEVERITY_VERBOSE,
             jlevel);
    JNI_CASE(env, "org/cef/CefSettings$LogSeverity", LOGSEVERITY_INFO, jlevel);
    JNI_CASE(env, "org/cef/CefSettings$LogSeverity", LOGSEVERITY_WARNING,
             jlevel);
    JNI_CASE(env, "org/cef/CefSettings$LogSeverity", LOGSEVERITY_ERROR, jlevel);
    JNI_CASE(env, "org/cef/CefSettings$LogSeverity", LOGSEVERITY_FATAL, jlevel);
    JNI_CASE(env, "org/cef/CefSettings$LogSeverity", LOGSEVERITY_DISABLE,
             jlevel);
    case LOGSEVERITY_DEFAULT:
      break;
  }

  ScopedJNIBrowser jbrowser(env, browser);
  ScopedJNIString jmessage(env, message);
  ScopedJNIString jsource(env, source);
  jboolean jreturn = JNI_FALSE;

  JNI_CALL_METHOD(
      env, handle_, "onConsoleMessage",
      "(Lorg/cef/browser/CefBrowser;Lorg/cef/CefSettings$LogSeverity;"
      "Ljava/lang/String;Ljava/lang/String;I)Z",
      Boolean, jreturn, jbrowser.get(), jlevel, jmessage.get(), jsource.get(),
      line);

  return (jreturn != JNI_FALSE);
}

// TODO(JCEF): Expose all parameters.
bool DisplayHandler::OnCursorChange(CefRefPtr<CefBrowser> browser,
                                    CefCursorHandle cursor,
                                    cef_cursor_type_t type,
                                    const CefCursorInfo& custom_cursor_info) {
  ScopedJNIEnv env;
  if (!env)
    return false;

  ScopedJNIBrowser jbrowser(env, browser);
  const int cursorId = GetCursorId(type);
  jboolean jreturn = JNI_FALSE;

  JNI_CALL_METHOD(env, handle_, "onCursorChange",
                  "(Lorg/cef/browser/CefBrowser;I)Z", Boolean, jreturn,
                  jbrowser.get(), cursorId);

  return (jreturn != JNI_FALSE);
}
