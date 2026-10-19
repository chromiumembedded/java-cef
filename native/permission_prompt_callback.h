#ifndef JCEF_NATIVE_PERMISSION_PROMPT_CALLBACK_H_
#define JCEF_NATIVE_PERMISSION_PROMPT_CALLBACK_H_
#pragma once

#include <jni.h>
#include "include/cef_permission_handler.h"
#include "include/cef_base.h"

class PermissionPromptCallback : public CefBaseRefCounted {
 public:
  explicit PermissionPromptCallback(CefRefPtr<CefPermissionPromptCallback> callback);

  void Continue(cef_permission_request_result_t result);

 private:
  CefRefPtr<CefPermissionPromptCallback> callback_;
  IMPLEMENT_REFCOUNTING(PermissionPromptCallback);
};

jobject NewJNIPermissionPromptCallback(JNIEnv* env, CefRefPtr<CefPermissionPromptCallback> callback);

#endif
