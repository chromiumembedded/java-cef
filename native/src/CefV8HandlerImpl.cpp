// Copyright (c) 2010 Lime Wire, LLC. All rights reserved. Use of this source
// code is governed by a BSD-style license that can be found in the LICENSE file.

#include "CefV8HandlerImpl.h"
#include "SystemUtilities.h"

CefV8HandlerImpl::CefV8HandlerImpl(JNIEnv* env, jobject handler)
{
	// Create a new global reference.
	jhandler_ = env->NewGlobalRef(handler);
}

CefV8HandlerImpl::~CefV8HandlerImpl()
{
	// Release the global reference.
	JNIEnv* env = GetJNIEnv();
	if(env)
		env->DeleteGlobalRef(jhandler_);
}

bool CefV8HandlerImpl::Execute(const CefString& name,
							   CefRefPtr<CefV8Value> object,
							   const CefV8ValueList& arguments,
							   CefRefPtr<CefV8Value>& retval,
							   CefString& exception)
{
	JNIEnv* env = GetJNIEnv();
	if(!env)
		return false;

	jobject params = CreateJNIObject(env,
		"org/limewire/cef/CefV8FunctionParams", NULL);
	if(!params)
		return false;

	jclass params_cls = env->GetObjectClass(params);

	// Set the object member.
	{
		jfieldID fieldID = env->GetFieldID(params_cls, "object",
			"Lorg/limewire/cef/CefV8Value;");
		if(fieldID != 0)
		{
			jobject obj = NULL;
			if(object.get())
			{
				obj = CreateJNIObject(env, "org/limewire/cef/CefV8Value_N",
					object.get());
			}
			env->SetObjectField(params, fieldID, obj);
		}
		else
		{
			env->ExceptionClear();
		}
	}

	// Set the arguments member.
	{
		jfieldID fieldID = env->GetFieldID(params_cls, "arguments",
			"[Lorg/limewire/cef/CefV8Value;");
		if(fieldID != 0)
		{
			jobjectArray obj = NULL;
			size_t size = arguments.size();
			if(size > 0)
			{
				jclass cls = env->FindClass("org/limewire/cef/CefV8Value_N");
				obj = env->NewObjectArray(size, cls, NULL);
				for(size_t i = 0; i < size; i++)
				{
					env->SetObjectArrayElement(obj, i,
						CreateJNIObject(env, "org/limewire/cef/CefV8Value_N",
							arguments[i]));
				}
			}
			
			env->SetObjectField(params, fieldID, obj);
		}
		else
		{
			env->ExceptionClear();
		}
	}

	bool ret = false;

	// Execute the method.
	{
		jclass cls = env->GetObjectClass(jhandler_);
		jmethodID methodID = env->GetMethodID(cls, "executeFunction",
			"(Ljava/lang/String;Lorg/limewire/cef/CefV8FunctionParams;)Z");
		if(methodID != 0)
		{
			ret = (env->CallBooleanMethod(jhandler_, methodID,
				CreateJNIString(env, name), params) == JNI_TRUE);
		}
		
		if(env->ExceptionOccurred())
		{
			env->ExceptionDescribe();
			env->ExceptionClear();
		}
	}

	// Get the return value member.
	{
		jfieldID fieldID = env->GetFieldID(params_cls, "retval",
			"Lorg/limewire/cef/CefV8Value;");
		if(fieldID != 0)
		{
			jobject obj = env->GetObjectField(params, fieldID);
			if(obj)
				retval = (CefV8Value*)GetCefBaseFromJNIObject(env, obj);
		}
		else
		{
			env->ExceptionClear();
		}
	}

	// Get the exception member.
	{
		jfieldID fieldID = env->GetFieldID(params_cls, "exception",
			"Ljava/lang/String;");
		if(fieldID != 0)
		{
			jstring str = (jstring)env->GetObjectField(params, fieldID);
			if(str)
				exception = GetStringFromJNI(env, str);
		}
		else
		{
			env->ExceptionClear();
		}
	}

	return ret;
}
