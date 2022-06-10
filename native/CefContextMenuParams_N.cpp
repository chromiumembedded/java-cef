// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "CefContextMenuParams_N.h"

#include "include/cef_context_menu_handler.h"

#include "jni_util.h"

namespace {

CefRefPtr<CefContextMenuParams> GetSelf(jlong self) {
  return reinterpret_cast<CefContextMenuParams*>(self);
}

}  // namespace

JNIEXPORT jint JNICALL
Java_org_cef_callback_CefContextMenuParams_1N_N_1GetXCoord(JNIEnv* env,
                                                           jobject obj,
                                                           jlong self) {
  CefRefPtr<CefContextMenuParams> menuParams = GetSelf(self);
  if (!menuParams)
    return 0;
  return menuParams->GetXCoord();
}

JNIEXPORT jint JNICALL
Java_org_cef_callback_CefContextMenuParams_1N_N_1GetYCoord(JNIEnv* env,
                                                           jobject obj,
                                                           jlong self) {
  CefRefPtr<CefContextMenuParams> menuParams = GetSelf(self);
  if (!menuParams)
    return 0;
  return menuParams->GetYCoord();
}

JNIEXPORT jint JNICALL
Java_org_cef_callback_CefContextMenuParams_1N_N_1GetTypeFlags(JNIEnv* env,
                                                              jobject obj,
                                                              jlong self) {
  CefRefPtr<CefContextMenuParams> menuParams = GetSelf(self);
  if (!menuParams)
    return 0;
  return (jint)menuParams->GetTypeFlags();
}

JNIEXPORT jstring JNICALL
Java_org_cef_callback_CefContextMenuParams_1N_N_1GetLinkUrl(JNIEnv* env,
                                                            jobject obj,
                                                            jlong self) {
  CefRefPtr<CefContextMenuParams> menuParams = GetSelf(self);
  if (!menuParams)
    return nullptr;
  return NewJNIString(env, menuParams->GetLinkUrl());
}

JNIEXPORT jstring JNICALL
Java_org_cef_callback_CefContextMenuParams_1N_N_1GetUnfilteredLinkUrl(
    JNIEnv* env,
    jobject obj,
    jlong self) {
  CefRefPtr<CefContextMenuParams> menuParams = GetSelf(self);
  if (!menuParams)
    return nullptr;
  return NewJNIString(env, menuParams->GetUnfilteredLinkUrl());
}

JNIEXPORT jstring JNICALL
Java_org_cef_callback_CefContextMenuParams_1N_N_1GetSourceUrl(JNIEnv* env,
                                                              jobject obj,
                                                              jlong self) {
  CefRefPtr<CefContextMenuParams> menuParams = GetSelf(self);
  if (!menuParams)
    return nullptr;
  return NewJNIString(env, menuParams->GetSourceUrl());
}

JNIEXPORT jboolean JNICALL
Java_org_cef_callback_CefContextMenuParams_1N_N_1HasImageContents(JNIEnv* env,
                                                                  jobject obj,
                                                                  jlong self) {
  CefRefPtr<CefContextMenuParams> menuParams = GetSelf(self);
  if (!menuParams)
    return JNI_FALSE;
  return menuParams->HasImageContents() ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jstring JNICALL
Java_org_cef_callback_CefContextMenuParams_1N_N_1GetPageUrl(JNIEnv* env,
                                                            jobject obj,
                                                            jlong self) {
  CefRefPtr<CefContextMenuParams> menuParams = GetSelf(self);
  if (!menuParams)
    return nullptr;
  return NewJNIString(env, menuParams->GetPageUrl());
}

JNIEXPORT jstring JNICALL
Java_org_cef_callback_CefContextMenuParams_1N_N_1GetFrameUrl(JNIEnv* env,
                                                             jobject obj,
                                                             jlong self) {
  CefRefPtr<CefContextMenuParams> menuParams = GetSelf(self);
  if (!menuParams)
    return nullptr;
  return NewJNIString(env, menuParams->GetFrameUrl());
}

JNIEXPORT jstring JNICALL
Java_org_cef_callback_CefContextMenuParams_1N_N_1GetFrameCharset(JNIEnv* env,
                                                                 jobject obj,
                                                                 jlong self) {
  CefRefPtr<CefContextMenuParams> menuParams = GetSelf(self);
  if (!menuParams)
    return nullptr;
  return NewJNIString(env, menuParams->GetFrameCharset());
}

JNIEXPORT jobject JNICALL
Java_org_cef_callback_CefContextMenuParams_1N_N_1GetMediaType(JNIEnv* env,
                                                              jobject obj,
                                                              jlong self) {
  CefRefPtr<CefContextMenuParams> menuParams = GetSelf(self);

  jobject result =
      GetJNIEnumValue(env, "org/cef/callback/CefContextMenuParams$MediaType",
                      "CM_MEDIATYPE_NONE");
  if (!menuParams)
    return result;

  switch (menuParams->GetMediaType()) {
    JNI_CASE(env, "org/cef/callback/CefContextMenuParams$MediaType",
             CM_MEDIATYPE_IMAGE, result);
    JNI_CASE(env, "org/cef/callback/CefContextMenuParams$MediaType",
             CM_MEDIATYPE_VIDEO, result);
    JNI_CASE(env, "org/cef/callback/CefContextMenuParams$MediaType",
             CM_MEDIATYPE_AUDIO, result);
    JNI_CASE(env, "org/cef/callback/CefContextMenuParams$MediaType",
             CM_MEDIATYPE_FILE, result);
    JNI_CASE(env, "org/cef/callback/CefContextMenuParams$MediaType",
             CM_MEDIATYPE_PLUGIN, result);
    default:
      JNI_CASE(env, "org/cef/callback/CefContextMenuParams$MediaType",
               CM_MEDIATYPE_NONE, result);
  }
  return result;
}

JNIEXPORT jint JNICALL
Java_org_cef_callback_CefContextMenuParams_1N_N_1GetMediaStateFlags(
    JNIEnv* env,
    jobject obj,
    jlong self) {
  CefRefPtr<CefContextMenuParams> menuParams = GetSelf(self);
  if (!menuParams)
    return 0;
  return (jint)menuParams->GetMediaStateFlags();
}

JNIEXPORT jstring JNICALL
Java_org_cef_callback_CefContextMenuParams_1N_N_1GetSelectionText(JNIEnv* env,
                                                                  jobject obj,
                                                                  jlong self) {
  CefRefPtr<CefContextMenuParams> menuParams = GetSelf(self);
  if (!menuParams)
    return nullptr;
  return NewJNIString(env, menuParams->GetSelectionText());
}

JNIEXPORT jstring JNICALL
Java_org_cef_callback_CefContextMenuParams_1N_N_1GetMisspelledWord(JNIEnv* env,
                                                                   jobject obj,
                                                                   jlong self) {
  CefRefPtr<CefContextMenuParams> menuParams = GetSelf(self);
  if (!menuParams)
    return nullptr;
  return NewJNIString(env, menuParams->GetMisspelledWord());
}

JNIEXPORT jboolean JNICALL
Java_org_cef_callback_CefContextMenuParams_1N_N_1GetDictionarySuggestions(
    JNIEnv* env,
    jobject obj,
    jlong self,
    jobject jsuggestions) {
  CefRefPtr<CefContextMenuParams> menuParams = GetSelf(self);
  if (!menuParams)
    return false;

  std::vector<CefString> suggestions;
  bool result = menuParams->GetDictionarySuggestions(suggestions);
  if (!result)
    return JNI_FALSE;

  for (std::vector<CefString>::size_type i = 0; i < suggestions.size(); ++i) {
    AddJNIStringToVector(env, jsuggestions, suggestions.at(i));
  }
  return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_org_cef_callback_CefContextMenuParams_1N_N_1IsEditable(JNIEnv* env,
                                                            jobject obj,
                                                            jlong self) {
  CefRefPtr<CefContextMenuParams> menuParams = GetSelf(self);
  if (!menuParams)
    return JNI_FALSE;
  return menuParams->IsEditable() ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL
Java_org_cef_callback_CefContextMenuParams_1N_N_1IsSpellCheckEnabled(
    JNIEnv* env,
    jobject obj,
    jlong self) {
  CefRefPtr<CefContextMenuParams> menuParams = GetSelf(self);
  if (!menuParams)
    return JNI_FALSE;
  return menuParams->IsSpellCheckEnabled() ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jint JNICALL
Java_org_cef_callback_CefContextMenuParams_1N_N_1GetEditStateFlags(JNIEnv* env,
                                                                   jobject obj,
                                                                   jlong self) {
  CefRefPtr<CefContextMenuParams> menuParams = GetSelf(self);
  if (!menuParams)
    return 0;
  return (jint)menuParams->GetEditStateFlags();
}
