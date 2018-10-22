// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "context_menu_handler.h"

#include "client_handler.h"
#include "jni_util.h"
#include "util.h"

ContextMenuHandler::ContextMenuHandler(JNIEnv* env, jobject handler) {
  jhandler_ = env->NewGlobalRef(handler);
}

ContextMenuHandler::~ContextMenuHandler() {
  JNIEnv* env = GetJNIEnv();
  env->DeleteGlobalRef(jhandler_);
}

void ContextMenuHandler::OnBeforeContextMenu(
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    CefRefPtr<CefContextMenuParams> params,
    CefRefPtr<CefMenuModel> model) {
  JNIEnv* env = GetJNIEnv();
  if (!env)
    return;

  jobject jparams =
      NewJNIObject(env, "org/cef/callback/CefContextMenuParams_N");
  if (!jparams)
    return;
  jobject jmodel = NewJNIObject(env, "org/cef/callback/CefMenuModel_N");
  if (!jmodel) {
    env->DeleteLocalRef(jparams);
    return;
  }

  SetCefForJNIObject(env, jparams, params.get(), "CefContextMenuParams");
  SetCefForJNIObject(env, jmodel, model.get(), "CefMenuModel");

  jobject jframe = GetJNIFrame(env, frame);

  JNI_CALL_VOID_METHOD(env, jhandler_, "onBeforeContextMenu",
                       "(Lorg/cef/browser/CefBrowser;Lorg/cef/browser/"
                       "CefFrame;Lorg/cef/callback/CefContextMenuParams;"
                       "Lorg/cef/callback/CefMenuModel;)V",
                       GetJNIBrowser(browser), jframe, jparams, jmodel);

  // Do not keep references to |params| or |model| outside of this callback.
  SetCefForJNIObject<CefContextMenuParams>(env, jparams, NULL,
                                           "CefContextMenuParams");
  SetCefForJNIObject<CefMenuModel>(env, jmodel, NULL, "CefMenuModel");

  env->DeleteLocalRef(jparams);
  env->DeleteLocalRef(jmodel);
  env->DeleteLocalRef(jframe);
}

bool ContextMenuHandler::OnContextMenuCommand(
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    CefRefPtr<CefContextMenuParams> params,
    int command_id,
    EventFlags event_flags) {
  JNIEnv* env = GetJNIEnv();
  if (!env)
    return false;

  jobject jparams =
      NewJNIObject(env, "org/cef/callback/CefContextMenuParams_N");
  if (!jparams)
    return false;
  SetCefForJNIObject(env, jparams, params.get(), "CefContextMenuParams");

  jboolean result = JNI_FALSE;
  jobject jframe = GetJNIFrame(env, frame);
  JNI_CALL_METHOD(env, jhandler_, "onContextMenuCommand",
                  "(Lorg/cef/browser/CefBrowser;Lorg/cef/browser/CefFrame;Lorg/"
                  "cef/callback/"
                  "CefContextMenuParams;II)Z",
                  Boolean, result, GetJNIBrowser(browser), jframe, jparams,
                  (jint)command_id, (jint)event_flags);

  // Do not keep references to |params| or |model| outside of this callback.
  SetCefForJNIObject<CefContextMenuParams>(env, jparams, NULL,
                                           "CefContextMenuParams");
  env->DeleteLocalRef(jparams);
  env->DeleteLocalRef(jframe);
  return (result != JNI_FALSE);
}

void ContextMenuHandler::OnContextMenuDismissed(CefRefPtr<CefBrowser> browser,
                                                CefRefPtr<CefFrame> frame) {
  JNIEnv* env = GetJNIEnv();
  if (!env)
    return;

  jobject jframe = GetJNIFrame(env, frame);
  JNI_CALL_VOID_METHOD(
      env, jhandler_, "onContextMenuDismissed",
      "(Lorg/cef/browser/CefBrowser;Lorg/cef/browser/CefFrame;)V",
      GetJNIBrowser(browser), jframe);
  env->DeleteLocalRef(jframe);
}
