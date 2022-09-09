// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "CefCookieManager_N.h"

#include "include/base/cef_callback.h"
#include "include/cef_cookie.h"
#include "include/cef_task.h"
#include "include/wrapper/cef_closure_task.h"

#include "completion_callback.h"
#include "cookie_visitor.h"
#include "jni_scoped_helpers.h"
#include "jni_util.h"

namespace {

const char kCefClassName[] = "CefCookieManager";

CefRefPtr<CefCookieManager> GetSelf(jlong self) {
  return reinterpret_cast<CefCookieManager*>(self);
}

CefCookie GetJNICookie(JNIEnv* env, jobject jcookie) {
  CefCookie cookie;

  ScopedJNIClass cls(env, "org/cef/network/CefCookie");
  if (!cls)
    return cookie;

  CefString name(&cookie.name);
  CefString value(&cookie.value);
  CefString domain(&cookie.domain);
  CefString path(&cookie.path);
  CefBaseTime creation, lastAccess, expires;

  GetJNIFieldString(env, cls, jcookie, "name", &name);
  GetJNIFieldString(env, cls, jcookie, "value", &value);
  GetJNIFieldString(env, cls, jcookie, "domain", &domain);
  GetJNIFieldString(env, cls, jcookie, "path", &path);
  GetJNIFieldBoolean(env, cls, jcookie, "secure", &cookie.secure);
  GetJNIFieldBoolean(env, cls, jcookie, "httponly", &cookie.httponly);
  GetJNIFieldDate(env, cls, jcookie, "creation", &creation);
  cookie.creation = creation;
  GetJNIFieldDate(env, cls, jcookie, "lastAccess", &lastAccess);
  cookie.last_access = lastAccess;
  GetJNIFieldBoolean(env, cls, jcookie, "hasExpires", &cookie.has_expires);
  if (cookie.has_expires) {
    GetJNIFieldDate(env, cls, jcookie, "expires", &expires);
    cookie.expires = expires;
  }

  return cookie;
}

}  // namespace

JNIEXPORT jobject JNICALL
Java_org_cef_network_CefCookieManager_1N_N_1GetGlobalManager(JNIEnv* env,
                                                             jclass cls) {
  // TODO(JCEF): Expose the callback object.
  CefRefPtr<CefCookieManager> manager =
      CefCookieManager::GetGlobalManager(nullptr);
  if (!manager)
    return nullptr;

  ScopedJNIObjectLocal jManager(env, NewJNIObject(env, cls));
  if (!jManager)
    return nullptr;

  SetCefForJNIObject(env, jManager, manager.get(), kCefClassName);
  return jManager.Release();
}

JNIEXPORT void JNICALL
Java_org_cef_network_CefCookieManager_1N_N_1Dispose(JNIEnv* env,
                                                    jobject obj,
                                                    jlong self) {
  SetCefForJNIObject<CefCookieManager>(env, obj, nullptr, kCefClassName);
}

JNIEXPORT jboolean JNICALL
Java_org_cef_network_CefCookieManager_1N_N_1VisitAllCookies(JNIEnv* env,
                                                            jobject obj,
                                                            jlong self,
                                                            jobject jvisitor) {
  CefRefPtr<CefCookieManager> manager = GetSelf(self);
  if (!manager || !jvisitor)
    return JNI_FALSE;

  CefRefPtr<CefCookieVisitor> visitor = new CookieVisitor(env, jvisitor);
  return manager->VisitAllCookies(visitor) ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL
Java_org_cef_network_CefCookieManager_1N_N_1VisitUrlCookies(
    JNIEnv* env,
    jobject obj,
    jlong self,
    jstring jurl,
    jboolean jincludeHttpOnly,
    jobject jvisitor) {
  CefRefPtr<CefCookieManager> manager = GetSelf(self);
  if (!manager || !jvisitor)
    return JNI_FALSE;

  CefRefPtr<CefCookieVisitor> visitor = new CookieVisitor(env, jvisitor);
  bool result = manager->VisitUrlCookies(
      GetJNIString(env, jurl), (jincludeHttpOnly != JNI_FALSE), visitor);
  return result ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL
Java_org_cef_network_CefCookieManager_1N_N_1SetCookie(JNIEnv* env,
                                                      jobject obj,
                                                      jlong self,
                                                      jstring jurl,
                                                      jobject jcookie) {
  CefRefPtr<CefCookieManager> manager = GetSelf(self);
  if (!manager)
    return JNI_FALSE;

  // The method CefCookieManager::SetCookie must be called on the IO thread.
  // We ignore its return value and return the result of the PostTask event to
  // java instead.
  // TODO(JCEF): Expose the callback object.
  bool result = CefPostTask(
      TID_IO, base::BindOnce(base::IgnoreResult(&CefCookieManager::SetCookie),
                             manager.get(), GetJNIString(env, jurl),
                             GetJNICookie(env, jcookie),
                             CefRefPtr<CefSetCookieCallback>()));
  return result ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL
Java_org_cef_network_CefCookieManager_1N_N_1DeleteCookies(JNIEnv* env,
                                                          jobject obj,
                                                          jlong self,
                                                          jstring jurl,
                                                          jstring jcookieName) {
  CefRefPtr<CefCookieManager> manager = GetSelf(self);
  if (!manager)
    return JNI_FALSE;

  // The method CefCookieManager::DeleteCookies must be called on the IO thread.
  // We ignore its return value and return the result of the PostTask event to
  // java instead.
  // TODO(JCEF): Expose the callback object.
  bool result = CefPostTask(
      TID_IO,
      base::BindOnce(base::IgnoreResult(&CefCookieManager::DeleteCookies),
                     manager.get(), GetJNIString(env, jurl),
                     GetJNIString(env, jcookieName),
                     CefRefPtr<CefDeleteCookiesCallback>()));
  return result ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL
Java_org_cef_network_CefCookieManager_1N_N_1FlushStore(JNIEnv* env,
                                                       jobject obj,
                                                       jlong self,
                                                       jobject jhandler) {
  CefRefPtr<CefCookieManager> manager = GetSelf(self);
  if (!manager)
    return JNI_FALSE;
  CefRefPtr<CefCompletionCallback> callback =
      new CompletionCallback(env, jhandler);
  bool result = manager->FlushStore(callback);
  return result ? JNI_TRUE : JNI_FALSE;
}
