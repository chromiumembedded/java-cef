// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "CefCookieManager_N.h"
#include "include/cef_cookie.h"
#include "include/cef_task.h"
#include "include/cef_runnable.h"

#include "completion_callback.h"
#include "cookie_visitor.h"
#include "jni_util.h"

JNIEXPORT jobject JNICALL Java_org_cef_network_CefCookieManager_1N_N_1GetGlobalManager
  (JNIEnv *env, jclass cls) {
  CefRefPtr<CefCookieManager> manager = CefCookieManager::GetGlobalManager();
  if (!manager.get())
    return NULL;

  jobject jManager = NewJNIObject(env, cls);
  if (!jManager)
    return NULL;

  SetCefForJNIObject(env, jManager, manager.get(), "CefCookieManager");
  return jManager;
}

JNIEXPORT jobject JNICALL Java_org_cef_network_CefCookieManager_1N_N_1CreateManager
  (JNIEnv *env, jclass cls, jstring jpath, jboolean jpersistSessionCookies) {
  CefRefPtr<CefCookieManager> manager =
      CefCookieManager::CreateManager(GetJNIString(env, jpath),
                                      (jpersistSessionCookies != JNI_FALSE));
  if (!manager.get())
    return NULL;

  jobject jManager = NewJNIObject(env, cls);
  if (!jManager)
    return NULL;

  SetCefForJNIObject(env, jManager, manager.get(), "CefCookieManager");
  return jManager;
}

JNIEXPORT void JNICALL Java_org_cef_network_CefCookieManager_1N_N_1SetSupportedSchemes
  (JNIEnv *env, jobject obj, jobject jschemes) {
  CefRefPtr<CefCookieManager> manager =
      GetCefFromJNIObject<CefCookieManager>(env, obj, "CefCookieManager");
  if (!manager.get())
    return;

  std::vector<CefString> schemes;
  GetJNIStringVector(env, jschemes, schemes);
  manager->SetSupportedSchemes(schemes);
}

JNIEXPORT jboolean JNICALL Java_org_cef_network_CefCookieManager_1N_N_1VisitAllCookies
  (JNIEnv *env, jobject obj, jobject jvisitor) {
  CefRefPtr<CefCookieManager> manager =
      GetCefFromJNIObject<CefCookieManager>(env, obj, "CefCookieManager");
  if (!manager.get() || !jvisitor)
    return JNI_FALSE;

  CefRefPtr<CefCookieVisitor> visitor = new CookieVisitor(env, jvisitor);
  if (!visitor.get())
    return JNI_FALSE;

  return manager->VisitAllCookies(visitor) ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL Java_org_cef_network_CefCookieManager_1N_N_1VisitUrlCookies
  (JNIEnv *env, jobject obj, jstring jurl, jboolean jincludeHttpOnly, jobject jvisitor) {
  CefRefPtr<CefCookieManager> manager =
      GetCefFromJNIObject<CefCookieManager>(env, obj, "CefCookieManager");
  if (!manager.get() || !jvisitor)
    return JNI_FALSE;

  CefRefPtr<CefCookieVisitor> visitor = new CookieVisitor(env, jvisitor);
  if (!visitor.get())
    return JNI_FALSE;

  bool result = manager->VisitUrlCookies(GetJNIString(env, jurl),
                                         (jincludeHttpOnly != JNI_FALSE),
                                         visitor);
  return result ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL Java_org_cef_network_CefCookieManager_1N_N_1SetCookie
  (JNIEnv *env, jobject obj, jstring jurl, jobject jcookie) {
  CefRefPtr<CefCookieManager> manager =
      GetCefFromJNIObject<CefCookieManager>(env, obj, "CefCookieManager");
  if (!manager.get())
    return JNI_FALSE;

  // The method CefCookieManager::SetCookie must be called on the IO thread.
  // We ignore its return value and return the result of the PostTask event to
  // java instead.
  bool result = CefPostTask(TID_IO, NewCefRunnableMethod(manager.get(),
                                                         &CefCookieManager::SetCookie,
                                                         GetJNIString(env, jurl),
                                                         GetJNICookie(env, jcookie))); 
  return result ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL Java_org_cef_network_CefCookieManager_1N_N_1DeleteCookies
  (JNIEnv *env, jobject obj, jstring jurl, jstring jcookieName) {
  CefRefPtr<CefCookieManager> manager =
      GetCefFromJNIObject<CefCookieManager>(env, obj, "CefCookieManager");
  if (!manager.get())
    return JNI_FALSE;

  // The method CefCookieManager::DeleteCookies must be called on the IO thread.
  // We ignore its return value and return the result of the PostTask event to
  // java instead.
  bool result = CefPostTask(TID_IO, NewCefRunnableMethod(manager.get(),
                                                         &CefCookieManager::DeleteCookies,
                                                         GetJNIString(env, jurl),
                                                         GetJNIString(env, jcookieName)));
  return result ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL Java_org_cef_network_CefCookieManager_1N_N_1SetStoragePath
  (JNIEnv *env, jobject obj, jstring jpath, jboolean jpersistSessionCookies) {
  CefRefPtr<CefCookieManager> manager =
      GetCefFromJNIObject<CefCookieManager>(env, obj, "CefCookieManager");
  if (!manager.get())
    return JNI_FALSE;
  bool result = manager->SetStoragePath(GetJNIString(env, jpath),
                                        (jpersistSessionCookies != JNI_FALSE));
  return result ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL Java_org_cef_network_CefCookieManager_1N_N_1FlushStore
  (JNIEnv *env, jobject obj, jobject jhandler) {
  CefRefPtr<CefCookieManager> manager =
      GetCefFromJNIObject<CefCookieManager>(env, obj, "CefCookieManager");
  if (!manager.get())
    return JNI_FALSE;
  CefRefPtr<CefCompletionCallback> callback = new CompletionCallback(env, jhandler);
  bool result = manager->FlushStore(callback);
  return result ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT void JNICALL Java_org_cef_network_CefCookieManager_1N_N_1CefCookieManager_1DTOR
  (JNIEnv *env, jobject obj) {
  SetCefForJNIObject<CefCookieManager>(env, obj, NULL, "CefCookieManager");
}
