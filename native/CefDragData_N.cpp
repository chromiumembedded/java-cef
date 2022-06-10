// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "CefDragData_N.h"

#include "include/cef_drag_handler.h"

#include "jni_scoped_helpers.h"
#include "jni_util.h"
#include "write_handler.h"

namespace {

const char kCefClassName[] = "CefDragData";

CefRefPtr<CefDragData> GetSelf(jlong self) {
  return reinterpret_cast<CefDragData*>(self);
}

}  // namespace

JNIEXPORT jobject JNICALL
Java_org_cef_callback_CefDragData_1N_N_1Create(JNIEnv* env, jclass cls) {
  CefRefPtr<CefDragData> dragData = CefDragData::Create();
  ScopedJNIDragData jdragData(env, dragData);
  return jdragData.Release();
}

JNIEXPORT jobject JNICALL
Java_org_cef_callback_CefDragData_1N_N_1Clone(JNIEnv* env,
                                              jobject obj,
                                              jlong self) {
  CefRefPtr<CefDragData> dragData = GetSelf(self);
  if (!dragData)
    return nullptr;

  CefRefPtr<CefDragData> cloneData = dragData->Clone();
  ScopedJNIDragData jcloneData(env, cloneData);
  return jcloneData.Release();
}

JNIEXPORT void JNICALL
Java_org_cef_callback_CefDragData_1N_N_1Dispose(JNIEnv* env,
                                                jobject obj,
                                                jlong self) {
  SetCefForJNIObject<CefDragData>(env, obj, nullptr, kCefClassName);
}

JNIEXPORT jboolean JNICALL
Java_org_cef_callback_CefDragData_1N_N_1IsReadOnly(JNIEnv* env,
                                                   jobject obj,
                                                   jlong self) {
  CefRefPtr<CefDragData> dragData = GetSelf(self);
  if (!dragData)
    return JNI_TRUE;
  return dragData->IsReadOnly() ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL
Java_org_cef_callback_CefDragData_1N_N_1IsLink(JNIEnv* env,
                                               jobject obj,
                                               jlong self) {
  CefRefPtr<CefDragData> dragData = GetSelf(self);
  if (!dragData)
    return JNI_FALSE;
  return dragData->IsLink() ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL
Java_org_cef_callback_CefDragData_1N_N_1IsFragment(JNIEnv* env,
                                                   jobject obj,
                                                   jlong self) {
  CefRefPtr<CefDragData> dragData = GetSelf(self);
  if (!dragData)
    return JNI_FALSE;
  return dragData->IsFragment() ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL
Java_org_cef_callback_CefDragData_1N_N_1IsFile(JNIEnv* env,
                                               jobject obj,
                                               jlong self) {
  CefRefPtr<CefDragData> dragData = GetSelf(self);
  if (!dragData)
    return JNI_FALSE;
  return dragData->IsFile() ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jstring JNICALL
Java_org_cef_callback_CefDragData_1N_N_1GetLinkURL(JNIEnv* env,
                                                   jobject obj,
                                                   jlong self) {
  CefRefPtr<CefDragData> dragData = GetSelf(self);
  if (!dragData)
    return nullptr;
  return NewJNIString(env, dragData->GetLinkURL());
}

JNIEXPORT jstring JNICALL
Java_org_cef_callback_CefDragData_1N_N_1GetLinkTitle(JNIEnv* env,
                                                     jobject obj,
                                                     jlong self) {
  CefRefPtr<CefDragData> dragData = GetSelf(self);
  if (!dragData)
    return nullptr;
  return NewJNIString(env, dragData->GetLinkTitle());
}

JNIEXPORT jstring JNICALL
Java_org_cef_callback_CefDragData_1N_N_1GetLinkMetadata(JNIEnv* env,
                                                        jobject obj,
                                                        jlong self) {
  CefRefPtr<CefDragData> dragData = GetSelf(self);
  if (!dragData)
    return nullptr;
  return NewJNIString(env, dragData->GetLinkMetadata());
}

JNIEXPORT jstring JNICALL
Java_org_cef_callback_CefDragData_1N_N_1GetFragmentText(JNIEnv* env,
                                                        jobject obj,
                                                        jlong self) {
  CefRefPtr<CefDragData> dragData = GetSelf(self);
  if (!dragData)
    return nullptr;
  return NewJNIString(env, dragData->GetFragmentText());
}

JNIEXPORT jstring JNICALL
Java_org_cef_callback_CefDragData_1N_N_1GetFragmentHtml(JNIEnv* env,
                                                        jobject obj,
                                                        jlong self) {
  CefRefPtr<CefDragData> dragData = GetSelf(self);
  if (!dragData)
    return nullptr;
  return NewJNIString(env, dragData->GetFragmentHtml());
}

JNIEXPORT jstring JNICALL
Java_org_cef_callback_CefDragData_1N_N_1GetFragmentBaseURL(JNIEnv* env,
                                                           jobject obj,
                                                           jlong self) {
  CefRefPtr<CefDragData> dragData = GetSelf(self);
  if (!dragData)
    return nullptr;
  return NewJNIString(env, dragData->GetFragmentBaseURL());
}

JNIEXPORT jint JNICALL
Java_org_cef_callback_CefDragData_1N_N_1GetFileContents(JNIEnv* env,
                                                        jobject obj,
                                                        jlong self,
                                                        jobject jstream) {
  CefRefPtr<CefDragData> dragData = GetSelf(self);
  if (!dragData || !jstream)
    return 0;

  CefRefPtr<CefWriteHandler> handler = new WriteHandler(env, jstream);
  return (jint)dragData->GetFileContents(
      CefStreamWriter::CreateForHandler(handler));
}

JNIEXPORT jstring JNICALL
Java_org_cef_callback_CefDragData_1N_N_1GetFileName(JNIEnv* env,
                                                    jobject obj,
                                                    jlong self) {
  CefRefPtr<CefDragData> dragData = GetSelf(self);
  if (!dragData)
    return nullptr;
  return NewJNIString(env, dragData->GetFileName());
}

JNIEXPORT jboolean JNICALL
Java_org_cef_callback_CefDragData_1N_N_1GetFileNames(JNIEnv* env,
                                                     jobject obj,
                                                     jlong self,
                                                     jobject jfileNames) {
  CefRefPtr<CefDragData> dragData = GetSelf(self);
  if (!dragData)
    return JNI_FALSE;

  std::vector<CefString> fileNames;
  if (!dragData->GetFileNames(fileNames))
    return JNI_FALSE;

  for (size_t i = 0; i < fileNames.size(); ++i) {
    AddJNIStringToVector(env, jfileNames, fileNames.at(i));
  }
  return JNI_TRUE;
}

JNIEXPORT void JNICALL
Java_org_cef_callback_CefDragData_1N_N_1SetLinkURL(JNIEnv* env,
                                                   jobject obj,
                                                   jlong self,
                                                   jstring jurl) {
  CefRefPtr<CefDragData> dragData = GetSelf(self);
  if (!dragData)
    return;
  dragData->SetLinkURL(GetJNIString(env, jurl));
}

JNIEXPORT void JNICALL
Java_org_cef_callback_CefDragData_1N_N_1SetLinkTitle(JNIEnv* env,
                                                     jobject obj,
                                                     jlong self,
                                                     jstring jtitle) {
  CefRefPtr<CefDragData> dragData = GetSelf(self);
  if (!dragData)
    return;
  dragData->SetLinkTitle(GetJNIString(env, jtitle));
}

JNIEXPORT void JNICALL
Java_org_cef_callback_CefDragData_1N_N_1SetLinkMetadata(JNIEnv* env,
                                                        jobject obj,
                                                        jlong self,
                                                        jstring jmetaData) {
  CefRefPtr<CefDragData> dragData = GetSelf(self);
  if (!dragData)
    return;
  dragData->SetLinkMetadata(GetJNIString(env, jmetaData));
}

JNIEXPORT void JNICALL
Java_org_cef_callback_CefDragData_1N_N_1SetFragmentText(JNIEnv* env,
                                                        jobject obj,
                                                        jlong self,
                                                        jstring jfragment) {
  CefRefPtr<CefDragData> dragData = GetSelf(self);
  if (!dragData)
    return;
  dragData->SetFragmentText(GetJNIString(env, jfragment));
}

JNIEXPORT void JNICALL
Java_org_cef_callback_CefDragData_1N_N_1SetFragmentHtml(JNIEnv* env,
                                                        jobject obj,
                                                        jlong self,
                                                        jstring jfragment) {
  CefRefPtr<CefDragData> dragData = GetSelf(self);
  if (!dragData)
    return;
  dragData->SetFragmentHtml(GetJNIString(env, jfragment));
}

JNIEXPORT void JNICALL
Java_org_cef_callback_CefDragData_1N_N_1SetFragmentBaseURL(JNIEnv* env,
                                                           jobject obj,
                                                           jlong self,
                                                           jstring jfragment) {
  CefRefPtr<CefDragData> dragData = GetSelf(self);
  if (!dragData)
    return;
  dragData->SetFragmentBaseURL(GetJNIString(env, jfragment));
}

JNIEXPORT void JNICALL
Java_org_cef_callback_CefDragData_1N_N_1ResetFileContents(JNIEnv* env,
                                                          jobject obj,
                                                          jlong self) {
  CefRefPtr<CefDragData> dragData = GetSelf(self);
  if (!dragData)
    return;
  dragData->ResetFileContents();
}

JNIEXPORT void JNICALL
Java_org_cef_callback_CefDragData_1N_N_1AddFile(JNIEnv* env,
                                                jobject obj,
                                                jlong self,
                                                jstring jpath,
                                                jstring jdisplayName) {
  CefRefPtr<CefDragData> dragData = GetSelf(self);
  if (!dragData)
    return;
  dragData->AddFile(GetJNIString(env, jpath), GetJNIString(env, jdisplayName));
}
