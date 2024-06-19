// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "dialog_handler.h"

#include "client_handler.h"
#include "jni_util.h"
#include "util.h"

namespace {

// JNI CefFileDialogCallback object.
class ScopedJNIFileDialogCallback
    : public ScopedJNIObject<CefFileDialogCallback> {
 public:
  ScopedJNIFileDialogCallback(JNIEnv* env, CefRefPtr<CefFileDialogCallback> obj)
      : ScopedJNIObject<CefFileDialogCallback>(
            env,
            obj,
            "org/cef/callback/CefFileDialogCallback_N",
            "CefFileDialogCallback") {}
};

}  // namespace

DialogHandler::DialogHandler(JNIEnv* env, jobject handler)
    : handle_(env, handler) {}

bool DialogHandler::OnFileDialog(
    CefRefPtr<CefBrowser> browser,
    FileDialogMode mode,
    const CefString& title,
    const CefString& default_file_path,
    const std::vector<CefString>& accept_filters,
    const std::vector<CefString>& accept_extensions,
    const std::vector<CefString>& accept_descriptions,
    CefRefPtr<CefFileDialogCallback> callback) {
  ScopedJNIEnv env;
  if (!env)
    return false;

  ScopedJNIBrowser jbrowser(env, browser);
  ScopedJNIString jtitle(env, title);
  ScopedJNIString jdefaultFilePath(env, default_file_path);
  ScopedJNIObjectLocal jacceptFilters(env,
                                      NewJNIStringVector(env, accept_filters));
  ScopedJNIObjectLocal jacceptExtensions(
      env, NewJNIStringVector(env, accept_extensions));
  ScopedJNIObjectLocal jacceptDescriptions(
      env, NewJNIStringVector(env, accept_descriptions));
  ScopedJNIFileDialogCallback jcallback(env, callback);

  ScopedJNIObjectResult jmode(env);
  switch (mode) {
    default:
      JNI_CASE(env, "org/cef/handler/CefDialogHandler$FileDialogMode",
               FILE_DIALOG_OPEN, jmode);
      JNI_CASE(env, "org/cef/handler/CefDialogHandler$FileDialogMode",
               FILE_DIALOG_OPEN_MULTIPLE, jmode);
      JNI_CASE(env, "org/cef/handler/CefDialogHandler$FileDialogMode",
               FILE_DIALOG_SAVE, jmode);
  }

  jboolean jreturn = JNI_FALSE;

  JNI_CALL_METHOD(
      env, handle_, "onFileDialog",
      "(Lorg/cef/browser/CefBrowser;Lorg/cef/handler/"
      "CefDialogHandler$FileDialogMode;Ljava/lang/String;Ljava/lang/"
      "String;Ljava/util/Vector;Ljava/util/Vector;Ljava/util/Vector;Lorg/cef/"
      "callback/CefFileDialogCallback;)Z",
      Boolean, jreturn, jbrowser.get(), jmode.get(), jtitle.get(),
      jdefaultFilePath.get(), jacceptFilters.get(), jacceptExtensions.get(),
      jacceptDescriptions.get(), jcallback.get());

  if (jreturn == JNI_FALSE) {
    // If the Java method returns "false" the callback won't be used and
    // the reference can therefore be removed.
    jcallback.SetTemporary();
  }

  return (jreturn != JNI_FALSE);
}
