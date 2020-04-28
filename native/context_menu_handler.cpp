// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "context_menu_handler.h"

#include "jni_util.h"

namespace {

// JNI CefContextMenuParams object.
class ScopedJNIContextMenuParams
    : public ScopedJNIObject<CefContextMenuParams> {
 public:
  ScopedJNIContextMenuParams(JNIEnv* env, CefRefPtr<CefContextMenuParams> obj)
      : ScopedJNIObject<CefContextMenuParams>(
            env,
            obj,
            "org/cef/callback/CefContextMenuParams_N",
            "CefContextMenuParams") {}
};

}  // namespace

ContextMenuHandler::ContextMenuHandler(JNIEnv* env, jobject handler)
    : handle_(env, handler) {}

void ContextMenuHandler::OnBeforeContextMenu(
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    CefRefPtr<CefContextMenuParams> params,
    CefRefPtr<CefMenuModel> model) {
  ScopedJNIEnv env;
  if (!env)
    return;

  ScopedJNIBrowser jbrowser(env, browser);
  ScopedJNIFrame jframe(env, frame);
  jframe.SetTemporary();
  ScopedJNIContextMenuParams jparams(env, params);
  jparams.SetTemporary();
  ScopedJNIMenuModel jmodel(env, model);
  jmodel.SetTemporary();

  JNI_CALL_VOID_METHOD(env, handle_, "onBeforeContextMenu",
                       "(Lorg/cef/browser/CefBrowser;Lorg/cef/browser/"
                       "CefFrame;Lorg/cef/callback/CefContextMenuParams;"
                       "Lorg/cef/callback/CefMenuModel;)V",
                       jbrowser.get(), jframe.get(), jparams.get(),
                       jmodel.get());
}

bool ContextMenuHandler::OnContextMenuCommand(
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    CefRefPtr<CefContextMenuParams> params,
    int command_id,
    EventFlags event_flags) {
  ScopedJNIEnv env;
  if (!env)
    return false;

  ScopedJNIBrowser jbrowser(env, browser);
  ScopedJNIFrame jframe(env, frame);
  jframe.SetTemporary();
  ScopedJNIContextMenuParams jparams(env, params);
  jparams.SetTemporary();
  jboolean result = JNI_FALSE;

  JNI_CALL_METHOD(env, handle_, "onContextMenuCommand",
                  "(Lorg/cef/browser/CefBrowser;Lorg/cef/browser/CefFrame;Lorg/"
                  "cef/callback/"
                  "CefContextMenuParams;II)Z",
                  Boolean, result, jbrowser.get(), jframe.get(), jparams.get(),
                  (jint)command_id, (jint)event_flags);

  return (result != JNI_FALSE);
}

void ContextMenuHandler::OnContextMenuDismissed(CefRefPtr<CefBrowser> browser,
                                                CefRefPtr<CefFrame> frame) {
  ScopedJNIEnv env;
  if (!env)
    return;

  ScopedJNIBrowser jbrowser(env, browser);
  ScopedJNIFrame jframe(env, frame);
  jframe.SetTemporary();

  JNI_CALL_VOID_METHOD(
      env, handle_, "onContextMenuDismissed",
      "(Lorg/cef/browser/CefBrowser;Lorg/cef/browser/CefFrame;)V",
      jbrowser.get(), jframe.get());
}
