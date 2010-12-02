// Copyright (c) 2010 Lime Wire, LLC. All rights reserved. Use of this source
// code is governed by a BSD-style license that can be found in the LICENSE file.

#include "SystemUtilities.h"
#include <jawt.h>
#include <string>

CefString GetStringFromJNI(JNIEnv* env, jstring jstr)
{
	CefString wstr;
	const char* chr = NULL;
	if(jstr)
		chr = env->GetStringUTFChars(jstr, NULL);
	if(chr)
		wstr = chr;
	env->ReleaseStringUTFChars(jstr, chr);
	return wstr;
}

jstring CreateJNIString(JNIEnv* env, const CefString& str)
{
  std::string cstr(str);
	return env->NewStringUTF(cstr.c_str());
}

jobjectArray CreateJNIStringArray(JNIEnv* env,
								  const std::vector<CefString>& vals)
{
	if(vals.empty())
		return NULL;

	jobjectArray arr = env->NewObjectArray(vals.size(),
		env->FindClass("java/lang/String"),
		CreateJNIString(env, CefString()));

	for(size_t i = 0; i < vals.size(); i++)
		env->SetObjectArrayElement(arr, i, CreateJNIString(env, vals[i]));

	return arr;
}

jobject CreateJNIObject(JNIEnv* env, const char* class_name, CefBase* cef_obj)
{
	jclass cls = env->FindClass(class_name);
	if(!cls)
	{
		env->ExceptionClear();
		return NULL;
	}

	jmethodID initID = env->GetMethodID(cls, "<init>", "()V");
	if(initID == 0)
	{
		env->ExceptionClear();
		return NULL;
	}

	jobject obj = env->NewObject(cls, initID);
	if(obj == NULL)
	{
		env->ExceptionClear();
		return NULL;
	}

	if(cef_obj && !SetCefBaseForJNIObject(env, obj, cef_obj))
		return NULL;

	return obj;
}

bool SetCefBaseForJNIObject(JNIEnv* env, jobject obj, CefBase* base)
{
	jclass cls = env->GetObjectClass(obj);
	jfieldID handleID = env->GetFieldID(cls, "N_CefHandle", "J");
	if(handleID == 0)
	{
		env->ExceptionClear();
		return false;
	}

	CefBase* oldbase = (CefBase*)env->GetLongField(obj, handleID);
	if(oldbase)
	{
		// Remove a reference from the previous base object.
		oldbase->Release();
	}

	env->SetLongField(obj, handleID, (jlong)base);
	if(base)
	{
		// Add a reference to the new base object.
		base->AddRef();
	}
	return true;
}

CefBase* GetCefBaseFromJNIObject(JNIEnv* env, jobject obj)
{
	jclass cls = env->GetObjectClass(obj);
	jfieldID handleID = env->GetFieldID(cls, "N_CefHandle", "J");
	if(handleID == 0)
	{
		env->ExceptionClear();
		return NULL;
	}

	return (CefBase*)env->GetLongField(obj, handleID);
}
