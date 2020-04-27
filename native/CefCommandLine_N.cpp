// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "CefCommandLine_N.h"
#include "include/cef_command_line.h"
#include "jni_scoped_helpers.h"
#include "jni_util.h"

namespace {

CefRefPtr<CefCommandLine> GetSelf(jlong self) {
  return reinterpret_cast<CefCommandLine*>(self);
}

}  // namespace

JNIEXPORT void JNICALL
Java_org_cef_callback_CefCommandLine_1N_N_1Reset(JNIEnv* env,
                                                 jobject obj,
                                                 jlong self) {
  CefRefPtr<CefCommandLine> commandLine = GetSelf(self);
  if (!commandLine)
    return;
  commandLine->Reset();
}

JNIEXPORT jstring JNICALL
Java_org_cef_callback_CefCommandLine_1N_N_1GetProgram(JNIEnv* env,
                                                      jobject obj,
                                                      jlong self) {
  CefRefPtr<CefCommandLine> commandLine = GetSelf(self);
  if (!commandLine)
    return env->NewStringUTF("");
  return NewJNIString(env, commandLine->GetProgram());
}

JNIEXPORT void JNICALL
Java_org_cef_callback_CefCommandLine_1N_N_1SetProgram(JNIEnv* env,
                                                      jobject obj,
                                                      jlong self,
                                                      jstring program) {
  CefRefPtr<CefCommandLine> commandLine = GetSelf(self);
  if (!commandLine)
    return;
  commandLine->SetProgram(GetJNIString(env, program));
}

JNIEXPORT jboolean JNICALL
Java_org_cef_callback_CefCommandLine_1N_N_1HasSwitches(JNIEnv* env,
                                                       jobject obj,
                                                       jlong self) {
  CefRefPtr<CefCommandLine> commandLine = GetSelf(self);
  if (!commandLine)
    return JNI_FALSE;
  return commandLine->HasSwitches() ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL
Java_org_cef_callback_CefCommandLine_1N_N_1HasSwitch(JNIEnv* env,
                                                     jobject obj,
                                                     jlong self,
                                                     jstring name) {
  CefRefPtr<CefCommandLine> commandLine = GetSelf(self);
  if (!commandLine)
    return JNI_FALSE;
  return commandLine->HasSwitch(GetJNIString(env, name)) ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jstring JNICALL
Java_org_cef_callback_CefCommandLine_1N_N_1GetSwitchValue(JNIEnv* env,
                                                          jobject obj,
                                                          jlong self,
                                                          jstring name) {
  CefRefPtr<CefCommandLine> commandLine = GetSelf(self);
  if (!commandLine)
    return env->NewStringUTF("");
  return NewJNIString(env,
                      commandLine->GetSwitchValue(GetJNIString(env, name)));
}

JNIEXPORT jobject JNICALL
Java_org_cef_callback_CefCommandLine_1N_N_1GetSwitches(JNIEnv* env,
                                                       jobject obj,
                                                       jlong self) {
  CefRefPtr<CefCommandLine> commandLine = GetSelf(self);
  CefCommandLine::SwitchMap switches;
  if (commandLine)
    commandLine->GetSwitches(switches);
  return NewJNIStringMap(env, switches);
}

JNIEXPORT void JNICALL
Java_org_cef_callback_CefCommandLine_1N_N_1AppendSwitch(JNIEnv* env,
                                                        jobject obj,
                                                        jlong self,
                                                        jstring name) {
  CefRefPtr<CefCommandLine> commandLine = GetSelf(self);
  if (!commandLine)
    return;
  commandLine->AppendSwitch(GetJNIString(env, name));
}

JNIEXPORT void JNICALL
Java_org_cef_callback_CefCommandLine_1N_N_1AppendSwitchWithValue(
    JNIEnv* env,
    jobject obj,
    jlong self,
    jstring name,
    jstring value) {
  CefRefPtr<CefCommandLine> commandLine = GetSelf(self);
  if (!commandLine)
    return;
  commandLine->AppendSwitchWithValue(GetJNIString(env, name),
                                     GetJNIString(env, value));
}

JNIEXPORT jboolean JNICALL
Java_org_cef_callback_CefCommandLine_1N_N_1HasArguments(JNIEnv* env,
                                                        jobject obj,
                                                        jlong self) {
  CefRefPtr<CefCommandLine> commandLine = GetSelf(self);
  if (!commandLine)
    return JNI_FALSE;
  return commandLine->HasArguments() ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jobject JNICALL
Java_org_cef_callback_CefCommandLine_1N_N_1GetArguments(JNIEnv* env,
                                                        jobject obj,
                                                        jlong self) {
  CefRefPtr<CefCommandLine> commandLine = GetSelf(self);
  CefCommandLine::ArgumentList arguments;
  if (commandLine)
    commandLine->GetArguments(arguments);
  return NewJNIStringVector(env, arguments);
}

JNIEXPORT void JNICALL
Java_org_cef_callback_CefCommandLine_1N_N_1AppendArgument(JNIEnv* env,
                                                          jobject obj,
                                                          jlong self,
                                                          jstring argument) {
  CefRefPtr<CefCommandLine> commandLine = GetSelf(self);
  if (!commandLine)
    return;
  commandLine->AppendArgument(GetJNIString(env, argument));
}
