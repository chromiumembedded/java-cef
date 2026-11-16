#ifndef JCEF_NATIVE_CEF_PERMISSION_PROMPT_CALLBACK_N_H_
#define JCEF_NATIVE_CEF_PERMISSION_PROMPT_CALLBACK_N_H_
#pragma once

#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif
JNIEXPORT void JNICALL
Java_org_cef_callback_CefPermissionPromptCallback_1N_N_1Continue(JNIEnv*, jobject, jlong, jint);
#ifdef __cplusplus
}
#endif

#endif
