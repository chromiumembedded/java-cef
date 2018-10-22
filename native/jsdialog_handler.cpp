// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "jsdialog_handler.h"

#include "client_handler.h"
#include "jni_util.h"
#include "util.h"

JSDialogHandler::JSDialogHandler(JNIEnv* env, jobject handler) {
  jhandler_ = env->NewGlobalRef(handler);
}

JSDialogHandler::~JSDialogHandler() {
  JNIEnv* env = GetJNIEnv();
  env->DeleteGlobalRef(jhandler_);
}

bool JSDialogHandler::OnJSDialog(CefRefPtr<CefBrowser> browser,
                                 const CefString& origin_url,
                                 CefJSDialogHandler::JSDialogType dialog_type,
                                 const CefString& message_text,
                                 const CefString& default_prompt_text,
                                 CefRefPtr<CefJSDialogCallback> callback,
                                 bool& suppress_message) {
  JNIEnv* env = GetJNIEnv();
  if (!env)
    return false;

  jobject jdialogType = NULL;
  switch (dialog_type) {
    default:
      JNI_CASE(env, "org/cef/handler/CefJSDialogHandler$JSDialogType",
               JSDIALOGTYPE_ALERT, jdialogType);
      JNI_CASE(env, "org/cef/handler/CefJSDialogHandler$JSDialogType",
               JSDIALOGTYPE_CONFIRM, jdialogType);
      JNI_CASE(env, "org/cef/handler/CefJSDialogHandler$JSDialogType",
               JSDIALOGTYPE_PROMPT, jdialogType);
  }

  jobject jboolRef = NewJNIBoolRef(env, suppress_message);
  if (!jboolRef)
    return false;

  jobject jcallback =
      NewJNIObject(env, "org/cef/callback/CefJSDialogCallback_N");
  if (!jcallback)
    return false;
  SetCefForJNIObject(env, jcallback, callback.get(), "CefJSDialogCallback");

  jboolean jresult = JNI_FALSE;
  jstring jorigin_url = NewJNIString(env, origin_url);
  jstring jmessage_text = NewJNIString(env, message_text);
  jstring jdefault_prompt_text = NewJNIString(env, default_prompt_text);
  JNI_CALL_METHOD(
      env, jhandler_, "onJSDialog",
      "(Lorg/cef/browser/CefBrowser;Ljava/lang/String;"
      "Lorg/cef/handler/CefJSDialogHandler$JSDialogType;Ljava/lang/String;"
      "Ljava/lang/String;Lorg/cef/callback/CefJSDialogCallback;Lorg/cef/misc/"
      "BoolRef;)Z",
      Boolean, jresult, GetJNIBrowser(browser), jorigin_url, jdialogType,
      jmessage_text, jdefault_prompt_text, jcallback, jboolRef);

  suppress_message = GetJNIBoolRef(env, jboolRef);

  if (jresult == JNI_FALSE) {
    // If the java method returns "false", the callback won't be used and
    // therefore the reference can be removed.
    SetCefForJNIObject<CefJSDialogCallback>(env, jcallback, NULL,
                                            "CefJSDialogCallback");
  }
  env->DeleteLocalRef(jcallback);
  env->DeleteLocalRef(jorigin_url);
  env->DeleteLocalRef(jmessage_text);
  env->DeleteLocalRef(jdefault_prompt_text);
  env->DeleteLocalRef(jboolRef);
  return (jresult != JNI_FALSE);
}

bool JSDialogHandler::OnBeforeUnloadDialog(
    CefRefPtr<CefBrowser> browser,
    const CefString& message_text,
    bool is_reload,
    CefRefPtr<CefJSDialogCallback> callback) {
  JNIEnv* env = GetJNIEnv();
  if (!env)
    return false;

  jobject jcallback =
      NewJNIObject(env, "org/cef/callback/CefJSDialogCallback_N");
  if (!jcallback)
    return false;
  SetCefForJNIObject(env, jcallback, callback.get(), "CefJSDialogCallback");

  jboolean jresult = JNI_FALSE;
  jstring jmessage_text = NewJNIString(env, message_text);
  JNI_CALL_METHOD(env, jhandler_, "onBeforeUnloadDialog",
                  "(Lorg/cef/browser/CefBrowser;Ljava/lang/String;ZLorg/cef/"
                  "callback/CefJSDialogCallback;)Z",
                  Boolean, jresult, GetJNIBrowser(browser), jmessage_text,
                  (is_reload ? JNI_TRUE : JNI_FALSE), jcallback);

  if (jresult == JNI_FALSE) {
    // If the java method returns "false", the callback won't be used and
    // therefore the reference can be removed.
    SetCefForJNIObject<CefJSDialogCallback>(env, jcallback, NULL,
                                            "CefJSDialogCallback");
  }
  env->DeleteLocalRef(jcallback);
  env->DeleteLocalRef(jmessage_text);
  return (jresult != JNI_FALSE);
}

void JSDialogHandler::OnResetDialogState(CefRefPtr<CefBrowser> browser) {
  JNIEnv* env = GetJNIEnv();
  if (!env)
    return;
  JNI_CALL_VOID_METHOD(env, jhandler_, "onResetDialogState",
                       "(Lorg/cef/browser/CefBrowser;)V",
                       GetJNIBrowser(browser));
}

void JSDialogHandler::OnDialogClosed(CefRefPtr<CefBrowser> browser) {
  JNIEnv* env = GetJNIEnv();
  if (!env)
    return;
  JNI_CALL_VOID_METHOD(env, jhandler_, "onDialogClosed",
                       "(Lorg/cef/browser/CefBrowser;)V",
                       GetJNIBrowser(browser));
}
