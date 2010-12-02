// Copyright (c) 2010 Lime Wire, LLC. All rights reserved. Use of this source
// code is governed by a BSD-style license that can be found in the LICENSE file.

#include "CefContext.h"
#include "SystemUtilities.h"
#include "CefBaseImpl.h"
#include "CefV8HandlerImpl.h"
#include "cef.h"

// Path to Java DLL.
wchar_t g_JavaPath[512] = {0};

// JNI environment pointers for each thread.
JNIEnv* g_PerThreadEnv[3] = {0};

JNIEnv* GetJNIEnv()
{
	if(CefCurrentlyOn(TID_UI))
		return g_PerThreadEnv[TID_UI];
	else if(CefCurrentlyOn(TID_IO))
		return g_PerThreadEnv[TID_IO];
	else if(CefCurrentlyOn(TID_FILE))
		return g_PerThreadEnv[TID_FILE];
  
	return NULL;
}

// Task used to attach Java to CEF threads.
class JavaThreadInitTask : public CefThreadSafeBase<CefTask>
{
public:
	JavaThreadInitTask() {}
	virtual void Execute(CefThreadId threadId)
	{
		JavaVM* jvm = GetJavaVM();
		JNIEnv* jenv;
		if(jvm && jvm->AttachCurrentThread((void**)&jenv, NULL) == 0)
		{
			g_PerThreadEnv[threadId] = jenv;
		}
	}
};

// Task used to detach Java from CEF threads.
class JavaThreadDestroyTask : public CefThreadSafeBase<CefTask>
{
public:
	JavaThreadDestroyTask() {}
	virtual void Execute(CefThreadId threadId)
	{
		JavaVM* jvm = GetJavaVM();
		if(jvm && g_PerThreadEnv[threadId])
		{
			jvm->DetachCurrentThread();
			g_PerThreadEnv[threadId] = NULL;
		}
	}
};

JNIEXPORT jboolean JNICALL Java_org_limewire_cef_CefContext_N_1Initialize
  (JNIEnv *env, jclass c, jstring argPathToJavaDLL, jstring cachePath)
{
	// Store the path to the Java DLL globally.
  std::wstring path = GetStringFromJNI(env, argPathToJavaDLL);
#ifdef _WIN32
	wcsncpy_s(g_JavaPath, 512, path.c_str(), _TRUNCATE);
#else
  wcsncpy(g_JavaPath, path.c_str(), 512);
#endif

	CefSettings settings;
	CefBrowserSettings browserSettings;

#ifdef _WIN32
	settings.multi_threaded_message_loop = true;
#endif // _WIN32

	CefString cachePathStr(GetStringFromJNI(env, cachePath));
	cef_string_set(cachePathStr.c_str(), cachePathStr.length(),
		&settings.cache_path, true);

	// Initialize CEF.
	if(!CefInitialize(settings, browserSettings))
		return JNI_FALSE;

	CefRefPtr<CefTask> initTask(new JavaThreadInitTask);
	CefPostTask(TID_UI, initTask);
	CefPostTask(TID_IO, initTask);
	CefPostTask(TID_FILE, initTask);

	return JNI_TRUE;
}

JNIEXPORT void JNICALL Java_org_limewire_cef_CefContext_N_1Shutdown
  (JNIEnv *env, jclass)
{
	CefRefPtr<CefTask> destroyTask(new JavaThreadDestroyTask);
	CefPostTask(TID_UI, destroyTask);
	CefPostTask(TID_IO, destroyTask);
	CefPostTask(TID_FILE, destroyTask);
	
	CefShutdown();
}

JNIEXPORT jobject JNICALL Java_org_limewire_cef_CefContext_N_1CreateV8Undefined
  (JNIEnv *env, jclass)
{
	CefRefPtr<CefV8Value> value(CefV8Value::CreateUndefined());
	return CreateJNIObject(env, "org/limewire/cef/CefV8Value_N", value.get());
}

JNIEXPORT jobject JNICALL Java_org_limewire_cef_CefContext_N_1CreateV8Null
  (JNIEnv *env, jclass)
{
	CefRefPtr<CefV8Value> value(CefV8Value::CreateNull());
	return CreateJNIObject(env, "org/limewire/cef/CefV8Value_N", value.get());
}

JNIEXPORT jobject JNICALL Java_org_limewire_cef_CefContext_N_1CreateV8Bool
  (JNIEnv *env, jclass, jboolean val)
{
	CefRefPtr<CefV8Value> value(
		CefV8Value::CreateBool(val==JNI_TRUE?true:false));
	return CreateJNIObject(env, "org/limewire/cef/CefV8Value_N", value.get());
}

JNIEXPORT jobject JNICALL Java_org_limewire_cef_CefContext_N_1CreateV8Int
  (JNIEnv *env, jclass, jint val)
{
	CefRefPtr<CefV8Value> value(CefV8Value::CreateInt(val));
	return CreateJNIObject(env, "org/limewire/cef/CefV8Value_N", value.get());
}

JNIEXPORT jobject JNICALL Java_org_limewire_cef_CefContext_N_1CreateV8Double
  (JNIEnv *env, jclass, jdouble val)
{
	CefRefPtr<CefV8Value> value(CefV8Value::CreateDouble(val));
	return CreateJNIObject(env, "org/limewire/cef/CefV8Value_N", value.get());
}

JNIEXPORT jobject JNICALL Java_org_limewire_cef_CefContext_N_1CreateV8String
  (JNIEnv *env, jclass, jstring val)
{
	CefRefPtr<CefV8Value> value(
		CefV8Value::CreateString(GetStringFromJNI(env, val)));
	return CreateJNIObject(env, "org/limewire/cef/CefV8Value_N", value.get());
}

JNIEXPORT jobject JNICALL Java_org_limewire_cef_CefContext_N_1CreateV8Object
  (JNIEnv *env, jclass, jobject user_data)
{
	CefRefPtr<CefBase> base;
	if(user_data)
		base = new CefBaseImpl(env, user_data);
	CefRefPtr<CefV8Value> value(CefV8Value::CreateObject(base));
	return CreateJNIObject(env, "org/limewire/cef/CefV8Value_N", value.get());
}

JNIEXPORT jobject JNICALL Java_org_limewire_cef_CefContext_N_1CreateV8Array
  (JNIEnv *env, jclass)
{
	CefRefPtr<CefV8Value> value(CefV8Value::CreateArray());
	return CreateJNIObject(env, "org/limewire/cef/CefV8Value_N", value.get());
}

JNIEXPORT jobject JNICALL Java_org_limewire_cef_CefContext_N_1CreateV8Function
  (JNIEnv *env, jclass, jstring name, jobject hobj)
{
	if(!hobj)
		return NULL;

	CefRefPtr<CefV8Handler> handler(new CefV8HandlerImpl(env, hobj));
	CefRefPtr<CefV8Value> value(
		CefV8Value::CreateFunction(GetStringFromJNI(env, name), handler));
	return CreateJNIObject(env, "org/limewire/cef/CefV8Value_N", value.get());
}
