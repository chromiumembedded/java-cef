// Copyright (c) 2010 Lime Wire, LLC. All rights reserved. Use of this source
// code is governed by a BSD-style license that can be found in the LICENSE file.

#include "CefBrowser_N.h"
#include "SystemUtilities.h"
#include "CefHandlerImpl.h"

#if defined(__APPLE__)
#include "CefBrowser_mac.h"
#endif

JNIEXPORT void JNICALL Java_org_limewire_cef_CefBrowser_1N_N_1Destroy
  (JNIEnv *env, jobject obj)
{
	CefRefPtr<CefBrowser> browser(
		(CefBrowser*)GetCefBaseFromJNIObject(env, obj));
	if(!browser.get())
		return;

	// Clear the browser pointer member of the Java object. This call will
	// release the extra reference to the object added in
	// CefHandlerImpl::HandleAfterCreated.
	SetCefBaseForJNIObject(env, obj, NULL);
}

JNIEXPORT void JNICALL Java_org_limewire_cef_CefBrowser_1N_N_1GoBack
  (JNIEnv *env, jobject obj)
{
	CefRefPtr<CefBrowser> browser(
		(CefBrowser*)GetCefBaseFromJNIObject(env, obj));
	if(!browser.get())
		return;

	browser->GoBack();
}

JNIEXPORT void JNICALL Java_org_limewire_cef_CefBrowser_1N_N_1GoForward
  (JNIEnv *env, jobject obj)
{
	CefRefPtr<CefBrowser> browser(
		(CefBrowser*)GetCefBaseFromJNIObject(env, obj));
	if(!browser.get())
		return;

	browser->GoForward();
}

JNIEXPORT jlong JNICALL Java_org_limewire_cef_CefBrowser_1N_N_1GetWindowHandle
  (JNIEnv *env, jobject obj)
{
	CefRefPtr<CefBrowser> browser(
		(CefBrowser*)GetCefBaseFromJNIObject(env, obj));
	if(!browser.get())
		return 0;

	return (jlong)browser->GetWindowHandle();
}

JNIEXPORT jboolean JNICALL Java_org_limewire_cef_CefBrowser_1N_N_1IsPopup
  (JNIEnv *env, jobject obj)
{
	CefRefPtr<CefBrowser> browser(
		(CefBrowser*)GetCefBaseFromJNIObject(env, obj));
	if(!browser.get())
		return JNI_FALSE;

	return browser->IsPopup() ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jobject JNICALL Java_org_limewire_cef_CefBrowser_1N_N_1GetMainFrame
  (JNIEnv *env, jobject obj)
{
	CefRefPtr<CefBrowser> browser(
		(CefBrowser*)GetCefBaseFromJNIObject(env, obj));
	if(!browser.get())
		return NULL;

	CefRefPtr<CefFrame> frame = browser->GetMainFrame();
	if(!frame.get())
		return NULL;

	return CreateJNIObject(env, "org/limewire/cef/CefFrame_N", frame.get());
}

JNIEXPORT jobject JNICALL Java_org_limewire_cef_CefBrowser_1N_N_1GetFocusedFrame
  (JNIEnv *env, jobject obj)
{
	CefRefPtr<CefBrowser> browser(
		(CefBrowser*)GetCefBaseFromJNIObject(env, obj));
	if(!browser.get())
		return NULL;

	CefRefPtr<CefFrame> frame = browser->GetFocusedFrame();
	if(!frame.get())
		return NULL;

	return CreateJNIObject(env, "org/limewire/cef/CefFrame_N", frame.get());
}

JNIEXPORT void JNICALL Java_org_limewire_cef_CefBrowser_1N_N_1Paint
  (JNIEnv *env, jobject obj, jint width, jint height)
{
	CefRefPtr<CefBrowser> browser(
		(CefBrowser*)GetCefBaseFromJNIObject(env, obj));
	if(browser.get())
		return;

	jclass cls = env->GetObjectClass(obj);

	// Check if CEF object creation is pending.
	{
		jfieldID fieldID = env->GetFieldID(cls, "N_CefCreated", "Z");
		if(fieldID != 0)
		{
			// Object creation is currently pending so return.
			if(env->GetBooleanField(obj, fieldID) == JNI_TRUE)
				return;

			// Set object creation as pending.
			env->SetBooleanField(obj, fieldID, JNI_TRUE);
		}
		else
		{
			env->ExceptionClear();
		}
	}

	// Retrieve the handler member.
	jobject handler_obj = NULL;
	{
		jfieldID fieldID = env->GetFieldID(cls, "handler",
			"Lorg/limewire/cef/CefHandler;");
		if(fieldID != 0)
			handler_obj = env->GetObjectField(obj, fieldID);
		else
			env->ExceptionClear();
	}

	// Create the browser window the first time paint is called.
	CefWindowInfo windowInfo;
	CefRefPtr<CefHandler> handler(new CefHandlerImpl(env, obj, handler_obj));
	
	// Retrieve the URL member.
	CefString load_url;
	{
		jfieldID fieldID = env->GetFieldID(cls, "load_url",
			"Ljava/lang/String;");
		if(fieldID != 0)
		{
			jstring strobj = (jstring)env->GetObjectField(obj, fieldID);
			load_url = GetStringFromJNI(env, strobj);
		}
		else
		{
			env->ExceptionClear();
		}
	}

#if defined(_WIN32)
	RECT rect = {0, 0, width, height};
	HWND hwnd = GetJavaWindowHandle(env, env->GetObjectClass(obj), obj);
	if(!hwnd)
		return;

	windowInfo.SetAsChild(hwnd, rect);
#elif defined(__APPLE__)
  GetWindowInfo(env, obj, width, height, windowInfo);
#else
#error Please implement.
#endif
  
	CefBrowser::CreateBrowser(windowInfo, false, handler, load_url);
}

JNIEXPORT void JNICALL Java_org_limewire_cef_CefBrowser_1N_N_1SetWindowSize
  (JNIEnv *env, jobject obj, jint width, jint height)
{
	CefRefPtr<CefBrowser> browser(
		(CefBrowser*)GetCefBaseFromJNIObject(env, obj));
	if(!browser.get())
		return;

#if defined(_WIN32)
	SetWindowPos(browser->GetWindowHandle(), NULL, 0, 0, width, height,
		SWP_NOZORDER | SWP_NOMOVE);
#elif defined(__APPLE__)
  SetWindowPos(browser->GetWindowHandle(), width, height);
#else
#error Please implement.
#endif
}
