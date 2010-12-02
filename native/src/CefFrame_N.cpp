// Copyright (c) 2010 Lime Wire, LLC. All rights reserved. Use of this source
// code is governed by a BSD-style license that can be found in the LICENSE file.

#include "CefFrame_N.h"
#include "SystemUtilities.h"

JNIEXPORT void JNICALL Java_org_limewire_cef_CefFrame_1N_N_1Destroy
  (JNIEnv *env, jobject obj)
{
	CefRefPtr<CefFrame> frame(
		(CefFrame*)GetCefBaseFromJNIObject(env, obj));
	if(!frame.get())
		return;

	// Clear the value pointer member of the Java object. This call will
	// release the extra reference to the value object added when the Java
	// object was created.
	SetCefBaseForJNIObject(env, obj, NULL);
}

JNIEXPORT void JNICALL Java_org_limewire_cef_CefFrame_1N_N_1LoadURL
  (JNIEnv *env, jobject obj, jstring url)
{
	CefRefPtr<CefFrame> frame(
		(CefFrame*)GetCefBaseFromJNIObject(env, obj));
	if(!frame.get())
		return;

	frame->LoadURL(GetStringFromJNI(env, url));
}

JNIEXPORT void JNICALL Java_org_limewire_cef_CefFrame_1N_N_1ExecuteJavaScript
  (JNIEnv *env, jobject obj, jstring jsCode, jstring scriptUrl, jint startLine)
{
	CefRefPtr<CefFrame> frame(
		(CefFrame*)GetCefBaseFromJNIObject(env, obj));
	if(!frame.get())
		return;

	frame->ExecuteJavaScript(GetStringFromJNI(env, jsCode),
		GetStringFromJNI(env, scriptUrl), startLine);
}

JNIEXPORT jboolean JNICALL Java_org_limewire_cef_CefFrame_1N_N_1IsMain
  (JNIEnv *env, jobject obj)
{
	CefRefPtr<CefFrame> frame(
		(CefFrame*)GetCefBaseFromJNIObject(env, obj));
	if(!frame.get())
		return JNI_FALSE;

	return frame->IsMain() ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL Java_org_limewire_cef_CefFrame_1N_N_1IsFocused
  (JNIEnv *env, jobject obj)
{
	CefRefPtr<CefFrame> frame(
		(CefFrame*)GetCefBaseFromJNIObject(env, obj));
	if(!frame.get())
		return JNI_FALSE;

	return frame->IsFocused() ? JNI_TRUE : JNI_FALSE;
}
