// Copyright (c) 2010 Lime Wire, LLC. All rights reserved. Use of this source
// code is governed by a BSD-style license that can be found in the LICENSE file.

#ifndef _SYSTEM_UTILITIES_H
#define _SYSTEM_UTILITIES_H

#include <jni.h>
#include "cef.h"

// Return the global JavaVM reference.
JavaVM* GetJavaVM();

// Return the global JNI environment reference for the specified CEF thread.
JNIEnv* GetJNIEnv();

// Retrieve a String value.
CefString GetStringFromJNI(JNIEnv* env, jstring jstr);

// Create a new String value.
jstring CreateJNIString(JNIEnv* env, const CefString& str);

// Create a new array of String values.
jobjectArray CreateJNIStringArray(JNIEnv* env,
								  const std::vector<CefString>& vals);

// Create a new JNI object.  If |cef_obj| is non-NULL SetCefBaseForJNIObject()
// will be also called for the object.
jobject CreateJNIObject(JNIEnv* env, const char* class_name, CefBase* cef_obj);

// Set the CEF base object for an existing JNI object.  A reference will be
// added to the base object.  If a previous base object existed a reference
// will be removed from that object.
bool SetCefBaseForJNIObject(JNIEnv* env, jobject obj, CefBase* base);

// Retrieve the CEF base object from an existing JNI object.
CefBase* GetCefBaseFromJNIObject(JNIEnv* env, jobject obj);

#endif // _SYSTEM_UTILITIES_H

