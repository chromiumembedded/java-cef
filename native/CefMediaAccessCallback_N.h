#ifndef JCEF_NATIVE_CEF_MEDIA_ACCESS_CALLBACK_N_H_
#define JCEF_NATIVE_CEF_MEDIA_ACCESS_CALLBACK_N_H_
#pragma once

#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT void JNICALL
Java_org_cef_callback_CefMediaAccessCallback_1N_N_1Continue(JNIEnv*, jobject, jlong, jint);

JNIEXPORT void JNICALL
Java_org_cef_callback_CefMediaAccessCallback_1N_N_1Cancel(JNIEnv*, jobject, jlong);

#ifdef __cplusplus
}
#endif

#endif
