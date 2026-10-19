#ifndef JCEF_NATIVE_MEDIA_ACCESS_CALLBACK_H_
#define JCEF_NATIVE_MEDIA_ACCESS_CALLBACK_H_
#pragma once

#include <jni.h>
#include "include/cef_permission_handler.h"
#include "include/cef_base.h"

class MediaAccessCallback : public CefBaseRefCounted {
 public:
  explicit MediaAccessCallback(CefRefPtr<CefMediaAccessCallback> callback);

  void Continue(uint32_t allowed_permissions);
  void Cancel();

 private:
  CefRefPtr<CefMediaAccessCallback> callback_;
  IMPLEMENT_REFCOUNTING(MediaAccessCallback);
};

jobject NewJNIMediaAccessCallback(JNIEnv* env, CefRefPtr<CefMediaAccessCallback> callback);

#endif
