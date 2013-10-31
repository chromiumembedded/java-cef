// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef CEF_TESTS_CEFCLIENT_JNI_UTIL_H_
#define CEF_TESTS_CEFCLIENT_JNI_UTIL_H_

#include <jni.h>
#include <vector>
#include "include/cef_base.h"
#include "util.h"

// Set the global JVM reference.
void SetJVM(JavaVM* jvm);

// Retrieve the JNIEnv for the current thread.
JNIEnv* GetJNIEnv();

// Create a new JNI object and call the default constructor.
jobject NewJNIObject(JNIEnv* env, jclass cls);
jobject NewJNIObject(JNIEnv* env, const char* class_name);

// Retrieve a String value.
CefString GetJNIString(JNIEnv* env, jstring jstr);

// Create a new String value.
jstring NewJNIString(JNIEnv* env, const CefString& str);

// Create a new array of String values.
jobjectArray NewJNIStringArray(JNIEnv* env,
								               const std::vector<CefString>& vals);

// Retrieve the int value stored in the |field_name| field of |cls|.
bool GetJNIFieldInt(JNIEnv* env, jclass cls, jobject obj,
                    const char* field_name, int* value);

// Set the int value stored in the |field_name| field of |cls|.
bool SetJNIFieldInt(JNIEnv* env, jclass cls, jobject obj,
                    const char* field_name, int value);

// Retrieve the static int value stored in the |field_name| field of |cls|.
bool GetJNIFieldStaticInt(JNIEnv* env, jclass cls,
                          const char* field_name, int* value);

// Call a JNI method that returns an int and accepts no arguments.
bool CallJNIMethodI_V(JNIEnv* env, jclass cls, jobject obj,
                      const char* method_name, int* value);

// Call a JNI method that returns a char and accepts no arguments.
bool CallJNIMethodC_V(JNIEnv* env, jclass cls, jobject obj,
                      const char* method_name, char* value);

// Retrieve the CefRect equivalent of a java.awt.Rectangle.
CefRect GetJNIRect(JNIEnv* env, jobject obj);

// Create a new java.awt.Rectangle.
jobject NewJNIRect(JNIEnv* env, const CefRect& rect);

// create a new array of java.awt.Rectangle.
jobjectArray NewJNIRectArray(JNIEnv* env,
								             const std::vector<CefRect>& vals);

// Retrieve the value of a java.awt.Point.
bool GetJNIPoint(JNIEnv* env, jobject obj, int* x, int* y);

// Create a new java.awt.Point.
jobject NewJNIPoint(JNIEnv* env, int x, int y);


// Helper macros for defining and retrieving static ints.
#define JNI_STATIC(name) _static_##name

#define JNI_STATIC_DEFINE_INT(env, cls, name) \
  JNI_STATIC_DEFINE_INT_RV(env, cls, name, );

#define JNI_STATIC_DEFINE_INT_RV(env, cls, name, rv) \
  static int JNI_STATIC(name) = -1; \
  if (JNI_STATIC(name) == -1 && \
      !GetJNIFieldStaticInt(env, cls, #name, &JNI_STATIC(name))) \
    return rv;


// Set the CEF base object for an existing JNI object. A reference will be
// added to the base object. If a previous base object existed a reference
// will be removed from that object.
template <class T>
bool SetCefForJNIObject(JNIEnv* env, jobject obj, T* base) {
  jclass cls = env->GetObjectClass(obj);
	jfieldID field = env->GetFieldID(cls, "N_CefHandle", "J");
	if (field) {
	  T* oldbase = reinterpret_cast<T*>(env->GetLongField(obj, field));
	  if(oldbase) {
		  // Remove a reference from the previous base object.
		  oldbase->Release();
	  }

	  env->SetLongField(obj, field, (jlong)base);
	  if(base) {
		  // Add a reference to the new base object.
		  base->AddRef();
	  }
	  return true;
  }

  env->ExceptionClear();
  return false;
}

// Retrieve the CEF base object from an existing JNI object.
template <class T>
T* GetCefFromJNIObject(JNIEnv* env, jobject obj) {
  jclass cls = env->GetObjectClass(obj);
	jfieldID field = env->GetFieldID(cls, "N_CefHandle", "J");
  if (field)
    return reinterpret_cast<T*>(env->GetLongField(obj, field));

	env->ExceptionClear();
  return NULL;
}

#endif // CEF_TESTS_CEFCLIENT_JNI_UTIL_H_
