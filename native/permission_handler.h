#ifndef JCEF_NATIVE_PERMISSION_HANDLER_H_
#define JCEF_NATIVE_PERMISSION_HANDLER_H_
#pragma once

#include <jni.h>
#include "include/cef_permission_handler.h"
#include "jni_scoped_helpers.h"

class PermissionHandler : public CefPermissionHandler {
 public:
  PermissionHandler(JNIEnv* env, jobject handler);

  bool OnRequestMediaAccessPermission(
      CefRefPtr<CefBrowser> browser,
      CefRefPtr<CefFrame> frame,
      const CefString& requesting_origin,
      uint32_t requested_permissions,
      CefRefPtr<CefMediaAccessCallback> callback) override;

  bool OnShowPermissionPrompt(
      CefRefPtr<CefBrowser> browser,
      uint64_t prompt_id,
      const CefString& requesting_origin,
      uint32_t requested_permissions,
      CefRefPtr<CefPermissionPromptCallback> callback) override;

  void OnDismissPermissionPrompt(
      CefRefPtr<CefBrowser> browser,
      uint64_t prompt_id,
      cef_permission_request_result_t result) override;

 protected:
  ScopedJNIObjectGlobal handle_;
  IMPLEMENT_REFCOUNTING(PermissionHandler);
};

#endif
