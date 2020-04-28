// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "jsdialog_handler.h"

#include "client_handler.h"
#include "jni_util.h"
#include "util.h"

namespace {

// JNI CefJSDialogCallback object.
class ScopedJNIJSDialogCallback : public ScopedJNIObject<CefJSDialogCallback> {
 public:
  ScopedJNIJSDialogCallback(JNIEnv* env, CefRefPtr<CefJSDialogCallback> obj)
      : ScopedJNIObject<CefJSDialogCallback>(
            env,
            obj,
            "org/cef/callback/CefJSDialogCallback_N",
            "CefJSDialogCallback") {}
};

}  // namespace

JSDialogHandler::JSDialogHandler(JNIEnv* env, jobject handler)
    : handle_(env, handler) {}

bool JSDialogHandler::OnJSDialog(CefRefPtr<CefBrowser> browser,
                                 const CefString& origin_url,
                                 CefJSDialogHandler::JSDialogType dialog_type,
                                 const CefString& message_text,
                                 const CefString& default_prompt_text,
                                 CefRefPtr<CefJSDialogCallback> callback,
                                 bool& suppress_message) {
  ScopedJNIEnv env;
  if (!env)
    return false;

  ScopedJNIBrowser jbrowser(env, browser);
  ScopedJNIString joriginUrl(env, origin_url);
  ScopedJNIString jmessageText(env, message_text);
  ScopedJNIString jdefaultPromptText(env, default_prompt_text);
  ScopedJNIJSDialogCallback jcallback(env, callback);
  ScopedJNIBoolRef jsuppressMessage(env, suppress_message);

  ScopedJNIObjectResult jdialogType(env);
  switch (dialog_type) {
    default:
      JNI_CASE(env, "org/cef/handler/CefJSDialogHandler$JSDialogType",
               JSDIALOGTYPE_ALERT, jdialogType);
      JNI_CASE(env, "org/cef/handler/CefJSDialogHandler$JSDialogType",
               JSDIALOGTYPE_CONFIRM, jdialogType);
      JNI_CASE(env, "org/cef/handler/CefJSDialogHandler$JSDialogType",
               JSDIALOGTYPE_PROMPT, jdialogType);
  }

  jboolean jresult = JNI_FALSE;

  JNI_CALL_METHOD(
      env, handle_, "onJSDialog",
      "(Lorg/cef/browser/CefBrowser;Ljava/lang/String;"
      "Lorg/cef/handler/CefJSDialogHandler$JSDialogType;Ljava/lang/String;"
      "Ljava/lang/String;Lorg/cef/callback/CefJSDialogCallback;Lorg/cef/misc/"
      "BoolRef;)Z",
      Boolean, jresult, jbrowser.get(), joriginUrl.get(), jdialogType.get(),
      jmessageText.get(), jdefaultPromptText.get(), jcallback.get(),
      jsuppressMessage.get());

  suppress_message = jsuppressMessage;

  if (jresult == JNI_FALSE) {
    // If the Java method returns "false" the callback won't be used and
    // the reference can therefore be removed.
    jcallback.SetTemporary();
  }

  return (jresult != JNI_FALSE);
}

bool JSDialogHandler::OnBeforeUnloadDialog(
    CefRefPtr<CefBrowser> browser,
    const CefString& message_text,
    bool is_reload,
    CefRefPtr<CefJSDialogCallback> callback) {
  ScopedJNIEnv env;
  if (!env)
    return false;

  ScopedJNIBrowser jbrowser(env, browser);
  ScopedJNIString jmessageText(env, message_text);
  ScopedJNIJSDialogCallback jcallback(env, callback);

  jboolean jresult = JNI_FALSE;

  JNI_CALL_METHOD(env, handle_, "onBeforeUnloadDialog",
                  "(Lorg/cef/browser/CefBrowser;Ljava/lang/String;ZLorg/cef/"
                  "callback/CefJSDialogCallback;)Z",
                  Boolean, jresult, jbrowser.get(), jmessageText.get(),
                  (is_reload ? JNI_TRUE : JNI_FALSE), jcallback.get());

  if (jresult == JNI_FALSE) {
    // If the Java method returns "false" the callback won't be used and
    // the reference can therefore be removed.
    jcallback.SetTemporary();
  }

  return (jresult != JNI_FALSE);
}

void JSDialogHandler::OnResetDialogState(CefRefPtr<CefBrowser> browser) {
  ScopedJNIEnv env;
  if (!env)
    return;

  ScopedJNIBrowser jbrowser(env, browser);

  JNI_CALL_VOID_METHOD(env, handle_, "onResetDialogState",
                       "(Lorg/cef/browser/CefBrowser;)V", jbrowser.get());
}

void JSDialogHandler::OnDialogClosed(CefRefPtr<CefBrowser> browser) {
  ScopedJNIEnv env;
  if (!env)
    return;

  ScopedJNIBrowser jbrowser(env, browser);

  JNI_CALL_VOID_METHOD(env, handle_, "onDialogClosed",
                       "(Lorg/cef/browser/CefBrowser;)V", jbrowser.get());
}
