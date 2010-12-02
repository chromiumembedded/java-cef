// Copyright (c) 2010 Lime Wire, LLC. All rights reserved. Use of this source
// code is governed by a BSD-style license that can be found in the LICENSE file.

#include "CefV8Value_N.h"
#include "SystemUtilities.h"
#include "CefBaseImpl.h"
#include "CefV8HandlerImpl.h"

JNIEXPORT void JNICALL Java_org_limewire_cef_CefV8Value_1N_N_1Destroy
  (JNIEnv *env, jobject obj)
{
	CefRefPtr<CefV8Value> value(
		(CefV8Value*)GetCefBaseFromJNIObject(env, obj));
	if(!value.get())
		return;

	// Clear the value pointer member of the Java object. This call will
	// release the extra reference to the value object added when the Java
	// object was created.
	SetCefBaseForJNIObject(env, obj, NULL);
}

JNIEXPORT jboolean JNICALL Java_org_limewire_cef_CefV8Value_1N_N_1IsUndefined
  (JNIEnv *env, jobject obj)
{
	CefRefPtr<CefV8Value> value(
		(CefV8Value*)GetCefBaseFromJNIObject(env, obj));
	if(!value.get())
		return JNI_FALSE;

	return value->IsUndefined() ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL Java_org_limewire_cef_CefV8Value_1N_N_1IsNull
  (JNIEnv *env, jobject obj)
{
	CefRefPtr<CefV8Value> value(
		(CefV8Value*)GetCefBaseFromJNIObject(env, obj));
	if(!value.get())
		return JNI_FALSE;

	return value->IsNull() ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL Java_org_limewire_cef_CefV8Value_1N_N_1IsBool
  (JNIEnv *env, jobject obj)
{
	CefRefPtr<CefV8Value> value(
		(CefV8Value*)GetCefBaseFromJNIObject(env, obj));
	if(!value.get())
		return JNI_FALSE;

	return value->IsBool() ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL Java_org_limewire_cef_CefV8Value_1N_N_1IsInt
  (JNIEnv *env, jobject obj)
{
	CefRefPtr<CefV8Value> value(
		(CefV8Value*)GetCefBaseFromJNIObject(env, obj));
	if(!value.get())
		return JNI_FALSE;

	return value->IsInt() ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL Java_org_limewire_cef_CefV8Value_1N_N_1IsDouble
  (JNIEnv *env, jobject obj)
{
	CefRefPtr<CefV8Value> value(
		(CefV8Value*)GetCefBaseFromJNIObject(env, obj));
	if(!value.get())
		return JNI_FALSE;

	return value->IsDouble() ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL Java_org_limewire_cef_CefV8Value_1N_N_1IsString
  (JNIEnv *env, jobject obj)
{
	CefRefPtr<CefV8Value> value(
		(CefV8Value*)GetCefBaseFromJNIObject(env, obj));
	if(!value.get())
		return JNI_FALSE;

	return value->IsString() ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL Java_org_limewire_cef_CefV8Value_1N_N_1IsObject
  (JNIEnv *env, jobject obj)
{
	CefRefPtr<CefV8Value> value(
		(CefV8Value*)GetCefBaseFromJNIObject(env, obj));
	if(!value.get())
		return JNI_FALSE;

	return value->IsObject() ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL Java_org_limewire_cef_CefV8Value_1N_N_1IsArray
  (JNIEnv *env, jobject obj)
{
	CefRefPtr<CefV8Value> value(
		(CefV8Value*)GetCefBaseFromJNIObject(env, obj));
	if(!value.get())
		return JNI_FALSE;

	return value->IsArray() ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL Java_org_limewire_cef_CefV8Value_1N_N_1IsFunction
  (JNIEnv *env, jobject obj)
{
	CefRefPtr<CefV8Value> value(
		(CefV8Value*)GetCefBaseFromJNIObject(env, obj));
	if(!value.get())
		return JNI_FALSE;

	return value->IsFunction() ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL Java_org_limewire_cef_CefV8Value_1N_N_1GetBoolValue
  (JNIEnv *env, jobject obj)
{
	CefRefPtr<CefV8Value> value(
		(CefV8Value*)GetCefBaseFromJNIObject(env, obj));
	if(!value.get())
		return JNI_FALSE;

	return value->GetBoolValue() ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jint JNICALL Java_org_limewire_cef_CefV8Value_1N_N_1GetIntValue
  (JNIEnv *env, jobject obj)
{
	CefRefPtr<CefV8Value> value(
		(CefV8Value*)GetCefBaseFromJNIObject(env, obj));
	if(!value.get())
		return 0;

	return value->GetIntValue();
}

JNIEXPORT jdouble JNICALL Java_org_limewire_cef_CefV8Value_1N_N_1GetDoubleValue
  (JNIEnv *env, jobject obj)
{
	CefRefPtr<CefV8Value> value(
		(CefV8Value*)GetCefBaseFromJNIObject(env, obj));
	if(!value.get())
		return 0;

	return value->GetDoubleValue();
}

JNIEXPORT jstring JNICALL Java_org_limewire_cef_CefV8Value_1N_N_1GetStringValue
  (JNIEnv *env, jobject obj)
{
	CefRefPtr<CefV8Value> value(
		(CefV8Value*)GetCefBaseFromJNIObject(env, obj));
	if(!value.get())
		return NULL;

	CefString str = value->GetStringValue();
	return env->NewString((const jchar*)str.c_str(), str.length());
}

JNIEXPORT jboolean JNICALL Java_org_limewire_cef_CefV8Value_1N_N_1HasValue__Ljava_lang_String_2
  (JNIEnv *env, jobject obj, jstring key)
{
	CefRefPtr<CefV8Value> value(
		(CefV8Value*)GetCefBaseFromJNIObject(env, obj));
	if(!value.get())
		return JNI_FALSE;

	return value->HasValue(GetStringFromJNI(env, key)) ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL Java_org_limewire_cef_CefV8Value_1N_N_1HasValue__I
  (JNIEnv *env, jobject obj, jint index)
{
	CefRefPtr<CefV8Value> value(
		(CefV8Value*)GetCefBaseFromJNIObject(env, obj));
	if(!value.get())
		return JNI_FALSE;

	return value->HasValue(index) ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL Java_org_limewire_cef_CefV8Value_1N_N_1DeleteValue__Ljava_lang_String_2
  (JNIEnv *env, jobject obj, jstring key)
{
	CefRefPtr<CefV8Value> value(
		(CefV8Value*)GetCefBaseFromJNIObject(env, obj));
	if(!value.get())
		return JNI_FALSE;

	return value->DeleteValue(GetStringFromJNI(env, key)) ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL Java_org_limewire_cef_CefV8Value_1N_N_1DeleteValue__I
  (JNIEnv *env, jobject obj, jint index)
{
	CefRefPtr<CefV8Value> value(
		(CefV8Value*)GetCefBaseFromJNIObject(env, obj));
	if(!value.get())
		return JNI_FALSE;

	return value->DeleteValue(index) ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jobject JNICALL Java_org_limewire_cef_CefV8Value_1N_N_1GetValue__Ljava_lang_String_2
  (JNIEnv *env, jobject obj, jstring key)
{
	CefRefPtr<CefV8Value> value(
		(CefV8Value*)GetCefBaseFromJNIObject(env, obj));
	if(!value.get())
		return NULL;

	CefRefPtr<CefV8Value> retval = value->GetValue(GetStringFromJNI(env, key));
	if(!retval.get())
		return NULL;

	return CreateJNIObject(env, "org/limewire/cef/CefV8Value_N", retval.get());
}

JNIEXPORT jobject JNICALL Java_org_limewire_cef_CefV8Value_1N_N_1GetValue__I
  (JNIEnv *env, jobject obj, jint index)
{
	CefRefPtr<CefV8Value> value(
		(CefV8Value*)GetCefBaseFromJNIObject(env, obj));
	if(!value.get())
		return NULL;

	CefRefPtr<CefV8Value> retval = value->GetValue(index);
	if(!retval.get())
		return NULL;

	return CreateJNIObject(env, "org/limewire/cef/CefV8Value_N", retval.get());
}

JNIEXPORT jboolean JNICALL Java_org_limewire_cef_CefV8Value_1N_N_1SetValue__Ljava_lang_String_2Lorg_limewire_cef_CefV8Value_2
  (JNIEnv *env, jobject obj, jstring key, jobject val)
{
	CefRefPtr<CefV8Value> value(
		(CefV8Value*)GetCefBaseFromJNIObject(env, obj));
	if(!value.get())
		return JNI_FALSE;

	CefRefPtr<CefV8Value> argval(
		(CefV8Value*)GetCefBaseFromJNIObject(env, val));
	if(!argval.get())
		return JNI_FALSE;

	return value->SetValue(GetStringFromJNI(env, key), argval) ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL Java_org_limewire_cef_CefV8Value_1N_N_1SetValue__ILorg_limewire_cef_CefV8Value_2
  (JNIEnv *env, jobject obj, jint index, jobject val)
{
	CefRefPtr<CefV8Value> value(
		(CefV8Value*)GetCefBaseFromJNIObject(env, obj));
	if(!value.get())
		return JNI_FALSE;

	CefRefPtr<CefV8Value> argval(
		(CefV8Value*)GetCefBaseFromJNIObject(env, val));
	if(!argval.get())
		return JNI_FALSE;

	return value->SetValue(index, argval) ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jobjectArray JNICALL Java_org_limewire_cef_CefV8Value_1N_N_1GetKeys
  (JNIEnv *env, jobject obj)
{
	CefRefPtr<CefV8Value> value(
		(CefV8Value*)GetCefBaseFromJNIObject(env, obj));
	if(!value.get())
		return NULL;

	std::vector<CefString> keys;
	value->GetKeys(keys);

	return CreateJNIStringArray(env, keys);	
}

JNIEXPORT jobject JNICALL Java_org_limewire_cef_CefV8Value_1N_N_1GetUserData
  (JNIEnv *env, jobject obj)
{
	CefRefPtr<CefV8Value> value(
		(CefV8Value*)GetCefBaseFromJNIObject(env, obj));
	if(!value.get())
		return NULL;

	CefRefPtr<CefBase> base = value->GetUserData();
	CefBaseImpl* impl = (CefBaseImpl*)base.get();
	if(impl)
		return impl->object();

	return NULL;
}

JNIEXPORT jint JNICALL Java_org_limewire_cef_CefV8Value_1N_N_1GetArrayLength
  (JNIEnv *env, jobject obj)
{
	CefRefPtr<CefV8Value> value(
		(CefV8Value*)GetCefBaseFromJNIObject(env, obj));
	if(!value.get())
		return 0;

	return value->GetArrayLength();
}

JNIEXPORT jstring JNICALL Java_org_limewire_cef_CefV8Value_1N_N_1GetFunctionName
  (JNIEnv *env, jobject obj)
{
	CefRefPtr<CefV8Value> value(
		(CefV8Value*)GetCefBaseFromJNIObject(env, obj));
	if(!value.get())
		return NULL;

	return CreateJNIString(env, value->GetFunctionName());
}

JNIEXPORT jobject JNICALL Java_org_limewire_cef_CefV8Value_1N_N_1GetFunctionHandler
  (JNIEnv *env, jobject obj)
{
	CefRefPtr<CefV8Value> value(
		(CefV8Value*)GetCefBaseFromJNIObject(env, obj));
	if(!value.get())
		return NULL;

	CefRefPtr<CefV8Handler> handler = value->GetFunctionHandler();
	CefV8HandlerImpl* impl = (CefV8HandlerImpl*)handler.get();
	if(impl)
		return impl->handler();

	return NULL;
}

JNIEXPORT jboolean JNICALL Java_org_limewire_cef_CefV8Value_1N_N_1ExecuteFunction
  (JNIEnv *env, jobject obj, jobject params)
{
	CefRefPtr<CefV8Value> value(
		(CefV8Value*)GetCefBaseFromJNIObject(env, obj));
	if(!value.get())
		return JNI_FALSE;

	jclass params_cls = env->GetObjectClass(params);

	// Retrieve the object member.
	jobject object_obj = NULL;
	{
		jfieldID fieldID = env->GetFieldID(params_cls, "object",
			"Lorg/limewire/cef/CefV8Value;");
		if(fieldID != 0)
			object_obj = env->GetObjectField(params, fieldID);
		else
			env->ExceptionClear();
	}
	if(!object_obj)
		return JNI_FALSE;

	CefRefPtr<CefV8Value> object(
		(CefV8Value*)GetCefBaseFromJNIObject(env, object_obj));
	if(!object.get())
		return JNI_FALSE;

	// Retrieve the argument member.
	jobjectArray arg_arr = NULL;
	{
		jfieldID fieldID = env->GetFieldID(params_cls, "owner",
			"[Lorg/limewire/cef/CefV8Value;");
		if(fieldID != 0)
			arg_arr = (jobjectArray)env->GetObjectField(params, fieldID);
		else
			env->ExceptionClear();
	}
	if(!arg_arr)
		return JNI_FALSE;

	CefV8ValueList argList;
	int len = env->GetArrayLength(arg_arr);

	for(int i = 0; i < len; i++)
	{
		jobject cobj = env->GetObjectArrayElement(arg_arr, i);
		if(!cobj)
			continue;

		CefRefPtr<CefV8Value> argobj(
			(CefV8Value*)GetCefBaseFromJNIObject(env, cobj));
		if(!argobj.get())
			continue;

		argList.push_back(argobj);
	}

	CefRefPtr<CefV8Value> retVal;
	CefString exception;
	
	// Execute the function.
	bool ret = value->ExecuteFunction(object, argList, retVal, exception);

	// Set the return value member.
	{
		jfieldID fieldID = env->GetFieldID(params_cls, "retval",
			"Lorg/limewire/cef/CefV8Value;");
		if(fieldID != 0)
		{
			jobject robj = NULL;
			if(retVal.get())
			{
				robj = CreateJNIObject(env, "org/limewire/cef/CefV8Value_N",
					retVal.get());
			}
			env->SetObjectField(params, fieldID, robj);
		}
		else
		{
			env->ExceptionClear();
		}
	}

	// Set the exception member.
	{
		jfieldID fieldID = env->GetFieldID(params_cls, "exception",
			"Ljava/lang/String;");
		if(fieldID != 0)
		{
			env->SetObjectField(params, fieldID,
				CreateJNIString(env, exception));
		}
		else
		{
			env->ExceptionClear();
		}
	}

	return ret ? JNI_TRUE : JNI_FALSE;
}
