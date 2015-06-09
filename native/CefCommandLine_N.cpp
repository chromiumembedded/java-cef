// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "CefCommandLine_N.h"
#include "include/cef_command_line.h"
#include "jni_util.h"
#include "util.h"

JNIEXPORT void JNICALL Java_org_cef_callback_CefCommandLine_1N_N_1reset
  (JNIEnv *env, jobject obj) {
    CefRefPtr<CefCommandLine> commandLine = GetCefFromJNIObject<CefCommandLine>(env, obj, "CefCommandLine");
    if (!commandLine.get())
      return;
    commandLine->Reset();
}

JNIEXPORT jstring JNICALL Java_org_cef_callback_CefCommandLine_1N_N_1getProgram
  (JNIEnv *env, jobject obj) {
    CefRefPtr<CefCommandLine> commandLine = GetCefFromJNIObject<CefCommandLine>(env, obj, "CefCommandLine");
    if (!commandLine.get())
      return env->NewStringUTF("");
    return NewJNIString(env, commandLine->GetProgram());
}

JNIEXPORT void JNICALL Java_org_cef_callback_CefCommandLine_1N_N_1setProgram
  (JNIEnv *env, jobject obj, jstring program) {
    CefRefPtr<CefCommandLine> commandLine = GetCefFromJNIObject<CefCommandLine>(env, obj, "CefCommandLine");
    if (!commandLine.get())
      return;
    commandLine->SetProgram(GetJNIString(env, program));
}

JNIEXPORT jboolean JNICALL Java_org_cef_callback_CefCommandLine_1N_N_1hasSwitches
  (JNIEnv *env, jobject obj) {
    CefRefPtr<CefCommandLine> commandLine = GetCefFromJNIObject<CefCommandLine>(env, obj, "CefCommandLine");
    if (!commandLine.get())
      return JNI_FALSE;
    return commandLine->HasSwitches() ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL Java_org_cef_callback_CefCommandLine_1N_N_1hasSwitch
  (JNIEnv *env, jobject obj, jstring name) {
    CefRefPtr<CefCommandLine> commandLine = GetCefFromJNIObject<CefCommandLine>(env, obj, "CefCommandLine");
    if (!commandLine.get())
      return JNI_FALSE;
    return commandLine->HasSwitch(GetJNIString(env, name)) ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jstring JNICALL Java_org_cef_callback_CefCommandLine_1N_N_1getSwitchValue
  (JNIEnv *env, jobject obj, jstring name) {
    CefRefPtr<CefCommandLine> commandLine = GetCefFromJNIObject<CefCommandLine>(env, obj, "CefCommandLine");
    if (!commandLine.get())
      return env->NewStringUTF("");
    return NewJNIString(env, commandLine->GetSwitchValue(GetJNIString(env, name)));
}

JNIEXPORT jobject JNICALL Java_org_cef_callback_CefCommandLine_1N_N_1getSwitches
  (JNIEnv *env, jobject obj) {
    CefRefPtr<CefCommandLine> commandLine = GetCefFromJNIObject<CefCommandLine>(env, obj, "CefCommandLine");
    CefCommandLine::SwitchMap switches;
    if (commandLine.get())
      commandLine->GetSwitches(switches);

    jobject hashMap = NewJNIObject(env, "java/util/HashMap");
    if (!hashMap)
      return NULL;

    for (CefCommandLine::SwitchMap::iterator iter = switches.begin(); iter != switches.end(); ++iter) {
      jstring jkey = NewJNIString(env, iter->first);
      jstring jvalue = NewJNIString(env, iter->second);
      jobject returnIgn = NULL;
      JNI_CALL_METHOD(env,
                      hashMap,
                      "put",
                      "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;",
                      Object,
                      returnIgn,
                      jkey,
                      jvalue);
      UNUSED(returnIgn);
    }
    return hashMap;
}

JNIEXPORT void JNICALL Java_org_cef_callback_CefCommandLine_1N_N_1appendSwitch
  (JNIEnv *env, jobject obj, jstring name) {
    CefRefPtr<CefCommandLine> commandLine = GetCefFromJNIObject<CefCommandLine>(env, obj, "CefCommandLine");
    if (!commandLine.get())
      return;
    commandLine->AppendSwitch(GetJNIString(env, name));
}

JNIEXPORT void JNICALL Java_org_cef_callback_CefCommandLine_1N_N_1appendSwitchWithValue
  (JNIEnv *env, jobject obj, jstring name, jstring value) {
    CefRefPtr<CefCommandLine> commandLine = GetCefFromJNIObject<CefCommandLine>(env, obj, "CefCommandLine");
    if (!commandLine.get())
      return;
    commandLine->AppendSwitchWithValue(GetJNIString(env, name), GetJNIString(env, value));
}

JNIEXPORT jboolean JNICALL Java_org_cef_callback_CefCommandLine_1N_N_1hasArguments
  (JNIEnv *env, jobject obj) {
    CefRefPtr<CefCommandLine> commandLine = GetCefFromJNIObject<CefCommandLine>(env, obj, "CefCommandLine");
    if (!commandLine.get())
      return JNI_FALSE;
    return commandLine->HasArguments() ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jobject JNICALL Java_org_cef_callback_CefCommandLine_1N_N_1getArguments
  (JNIEnv *env, jobject obj) {
    CefRefPtr<CefCommandLine> commandLine = GetCefFromJNIObject<CefCommandLine>(env, obj, "CefCommandLine");
    CefCommandLine::ArgumentList arguments;
    if (commandLine.get())
      commandLine->GetArguments(arguments);

    jobject vector = NewJNIObject(env, "java/util/Vector");
    if (!vector)
      return NULL;

    for (CefCommandLine::ArgumentList::iterator iter = arguments.begin(); iter != arguments.end(); ++iter) {
      jstring argument = NewJNIString(env, *iter);
      jboolean succ = JNI_FALSE;
      JNI_CALL_METHOD(env, vector, "add", "(Ljava/lang/object;)Z", Boolean, succ, argument);
      UNUSED(succ);
    }
    return vector;
}

JNIEXPORT void JNICALL Java_org_cef_callback_CefCommandLine_1N_N_1appendArgument
  (JNIEnv *env, jobject obj, jstring argument) {
    CefRefPtr<CefCommandLine> commandLine = GetCefFromJNIObject<CefCommandLine>(env, obj, "CefCommandLine");
    if (!commandLine.get())
      return;
    commandLine->AppendArgument(GetJNIString(env, argument));
}
