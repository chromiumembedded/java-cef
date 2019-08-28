// Copyright (c) 2019 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "jni_scoped_helpers.h"

ScopedJNIObjectGlobal::ScopedJNIObjectGlobal(JNIEnv* env, jobject handle)
    : jhandle_(NULL) {
  if (handle) {
    jhandle_ = env->NewGlobalRef(handle);
    DCHECK(jhandle_);
  }
}

ScopedJNIObjectGlobal::~ScopedJNIObjectGlobal() {
  if (jhandle_) {
    BEGIN_ENV(env);
    env->DeleteGlobalRef(jhandle_);
    END_ENV(env);
  }
}

jobject ScopedJNIObjectGlobal::get() const {
  return jhandle_;
}

ScopedJNIObjectGlobal::operator jobject() const {
  return jhandle_;
}

ScopedJNIObjectLocal::ScopedJNIObjectLocal(JNIEnv* env, jobject handle)
    : ScopedJNIBase<jobject>(env) {
  jhandle_ = handle;
}

ScopedJNIObjectResult::ScopedJNIObjectResult(JNIEnv* env)
    : ScopedJNIBase<jobject>(env) {}

ScopedJNIString::ScopedJNIString(JNIEnv* env, const CefString& str)
    : ScopedJNIBase<jstring>(env) {
  jhandle_ = NewJNIString(env, str);
  DCHECK(jhandle_);
}

ScopedJNIBrowser::ScopedJNIBrowser(JNIEnv* env, CefRefPtr<CefBrowser> obj)
    : ScopedJNIBase<jobject>(env) {
  if (obj) {
    // Will return NULL for browsers that represent native popup windows.
    jhandle_ = GetJNIBrowser(env_, obj);
  } else {
    jhandle_ = NULL;
  }
}

void ScopedJNIBrowser::SetHandle(jobject handle, bool should_delete) {
  DCHECK(!jhandle_);
  DCHECK(handle);
  jhandle_ = handle;
  delete_ref_ = should_delete;
}

CefRefPtr<CefBrowser> ScopedJNIBrowser::GetCefObject() const {
  if (!jhandle_)
    return NULL;
  return GetCefBrowser(env_, jhandle_);
}

ScopedJNIAuthCallback::ScopedJNIAuthCallback(JNIEnv* env,
                                             CefRefPtr<CefAuthCallback> obj)
    : ScopedJNIObject<CefAuthCallback>(env,
                                       obj,
                                       "org/cef/callback/CefAuthCallback_N",
                                       "CefAuthCallback") {}

ScopedJNIDragData::ScopedJNIDragData(JNIEnv* env, CefRefPtr<CefDragData> obj)
    : ScopedJNIObject<CefDragData>(env,
                                   obj,
                                   "org/cef/callback/CefDragData_N",
                                   "CefDragData") {}

ScopedJNIFrame::ScopedJNIFrame(JNIEnv* env, CefRefPtr<CefFrame> obj)
    : ScopedJNIObject<CefFrame>(env,
                                obj,
                                "org/cef/browser/CefFrame_N",
                                "CefFrame") {}

ScopedJNIMenuModel::ScopedJNIMenuModel(JNIEnv* env, CefRefPtr<CefMenuModel> obj)
    : ScopedJNIObject<CefMenuModel>(env,
                                    obj,
                                    "org/cef/callback/CefMenuModel_N",
                                    "CefMenuModel") {}

ScopedJNIMessageRouter::ScopedJNIMessageRouter(JNIEnv* env,
                                               CefRefPtr<CefMessageRouter> obj)
    : ScopedJNIObject<CefMessageRouter>(env,
                                        obj,
                                        "org/cef/browser/CefMessageRouter_N",
                                        "CefMessageRouter") {}

ScopedJNIPostData::ScopedJNIPostData(JNIEnv* env, CefRefPtr<CefPostData> obj)
    : ScopedJNIObject<CefPostData>(env,
                                   obj,
                                   "org/cef/network/CefPostData_N",
                                   "CefPostData") {}

ScopedJNIPostDataElement::ScopedJNIPostDataElement(
    JNIEnv* env,
    CefRefPtr<CefPostDataElement> obj)
    : ScopedJNIObject<CefPostDataElement>(
          env,
          obj,
          "org/cef/network/CefPostDataElement_N",
          "CefPostDataElement") {}

ScopedJNIPrintSettings::ScopedJNIPrintSettings(JNIEnv* env,
                                               CefRefPtr<CefPrintSettings> obj)
    : ScopedJNIObject<CefPrintSettings>(env,
                                        obj,
                                        "org/cef/misc/CefPrintSettings_N",
                                        "CefPrintSettings") {}

ScopedJNIRequest::ScopedJNIRequest(JNIEnv* env, CefRefPtr<CefRequest> obj)
    : ScopedJNIObject<CefRequest>(env,
                                  obj,
                                  "org/cef/network/CefRequest_N",
                                  "CefRequest") {}

ScopedJNIResponse::ScopedJNIResponse(JNIEnv* env, CefRefPtr<CefResponse> obj)
    : ScopedJNIObject<CefResponse>(env,
                                   obj,
                                   "org/cef/network/CefResponse_N",
                                   "CefResponse") {}

ScopedJNIBoolRef::ScopedJNIBoolRef(JNIEnv* env, bool value)
    : ScopedJNIBase<jobject>(env) {
  jhandle_ = NewJNIBoolRef(env, value);
  DCHECK(jhandle_);
}

ScopedJNIBoolRef::operator bool() const {
  return GetJNIBoolRef(env_, jhandle_);
}

ScopedJNIIntRef::ScopedJNIIntRef(JNIEnv* env, int value)
    : ScopedJNIBase<jobject>(env) {
  jhandle_ = NewJNIIntRef(env, value);
  DCHECK(jhandle_);
}

ScopedJNIIntRef::operator int() const {
  return GetJNIIntRef(env_, jhandle_);
}

ScopedJNIStringRef::ScopedJNIStringRef(JNIEnv* env, const CefString& value)
    : ScopedJNIBase<jobject>(env) {
  jhandle_ = NewJNIStringRef(env, value);
  DCHECK(jhandle_);
}

ScopedJNIStringRef::operator CefString() const {
  return GetJNIStringRef(env_, jhandle_);
}
