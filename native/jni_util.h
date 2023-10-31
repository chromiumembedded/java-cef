// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef JCEF_NATIVE_JNI_UTIL_H_
#define JCEF_NATIVE_JNI_UTIL_H_

#include <jni.h>
#include <vector>
#include "include/cef_base.h"
#include "include/cef_browser.h"
#include "include/cef_frame.h"
#include "include/wrapper/cef_message_router.h"
#include "util.h"

// Set the global JVM reference.
void SetJVM(JavaVM* jvm);
JavaVM* GetJVM();

// Sets the java class loader to use for creating java objects in native code.
// We have to use a class loader instead of the JNIEnv::FindClass method
// because JNIEnv::FindClass always uses the system class loader if called
// from a non-Java thread, which will not work if the embedding Java code
// uses a custom class loader for JCEF classes (e.g. in JavaWebStart).
void SetJavaClassLoader(JNIEnv* env, jobject javaClassLoader);
jobject GetJavaClassLoader();

// Create a new JNI object and call the default constructor.
jobject NewJNIObject(JNIEnv* env, jclass cls);
jobject NewJNIObject(JNIEnv* env, const char* class_name);
jobject NewJNIObject(JNIEnv* env, const char* class_name, const char* sig, ...);

// Retrieve primitive reference values
bool GetJNIBoolRef(JNIEnv* env, jobject jboolRef);
int GetJNIIntRef(JNIEnv* env, jobject jintRef);
CefString GetJNIStringRef(JNIEnv* env, jobject jstringRef);

// Set primitive reference values
void SetJNIBoolRef(JNIEnv* env, jobject jboolRef, bool boolValue);
void SetJNIIntRef(JNIEnv* env, jobject jintRef, int intValue);
void SetJNIStringRef(JNIEnv* env,
                     jobject jstringRef,
                     const CefString& initValue);

// Create a new String value.
jstring NewJNIString(JNIEnv* env, const std::string& str);

// Retrieve a String value.
CefString GetJNIString(JNIEnv* env, jstring jstr);

// Create a new array of String values.
jobjectArray NewJNIStringArray(JNIEnv* env, const std::vector<CefString>& vals);

// Retrieve the String values from an array.
void GetJNIStringArray(JNIEnv* env,
                       jobjectArray jarray,
                       std::vector<CefString>& vals);

// Create a new vector of String values.
jobject NewJNIStringVector(JNIEnv* env, const std::vector<CefString>& vals);

// Add a String value to the Vector.
void AddJNIStringToVector(JNIEnv* env, jobject jvector, const CefString& str);

// Retrieve the String values from a Vector.
void GetJNIStringVector(JNIEnv* env,
                        jobject jvector,
                        std::vector<CefString>& vals);

// Create a new Map<String, String> of String values.
jobject NewJNIStringMap(JNIEnv* env,
                        const std::map<CefString, CefString>& vals);

// |jheaderMap| is expected to be a Map<String, String>.
void GetJNIStringMultiMap(JNIEnv* env,
                          jobject jheaderMap,
                          std::multimap<CefString, CefString>& vals);

// |jheaderMap| is expected to be a Map<String, String>.
void SetJNIStringMultiMap(JNIEnv* env,
                          jobject jheaderMap,
                          const std::multimap<CefString, CefString>& vals);

CefMessageRouterConfig GetJNIMessageRouterConfig(JNIEnv* env, jobject jConfig);

// Create a new JNI error code.
jobject NewJNIErrorCode(JNIEnv* env, cef_errorcode_t errorCode);
cef_errorcode_t GetJNIErrorCode(JNIEnv* env, jobject jerrorCode);

bool GetJNIFieldObject(JNIEnv* env,
                       jclass cls,
                       jobject obj,
                       const char* field_name,
                       jobject* value,
                       const char* object_type);

bool GetJNIFieldString(JNIEnv* env,
                       jclass cls,
                       jobject obj,
                       const char* field_name,
                       CefString* value);

bool GetJNIFieldDate(JNIEnv* env,
                     jclass cls,
                     jobject obj,
                     const char* field_name,
                     CefBaseTime* value);

bool GetJNIFieldBoolean(JNIEnv* env,
                        jclass cls,
                        jobject obj,
                        const char* field_name,
                        int* value);

// Retrieve the int value stored in the |field_name| field of |cls|.
bool GetJNIFieldInt(JNIEnv* env,
                    jclass cls,
                    jobject obj,
                    const char* field_name,
                    int* value);

bool GetJNIFieldDouble(JNIEnv* env,
                       jclass cls,
                       jobject obj,
                       const char* field_name,
                       double* value);

// Retrieve the long value stored in the |field_name| field of |cls|.
bool GetJNIFieldLong(JNIEnv* env,
                     jclass cls,
                     jobject obj,
                     const char* field_name,
                     jlong* value);

// Set the int value stored in the |field_name| field of |cls|.
bool SetJNIFieldInt(JNIEnv* env,
                    jclass cls,
                    jobject obj,
                    const char* field_name,
                    int value);
bool SetJNIFieldDouble(JNIEnv* env,
                       jclass cls,
                       jobject obj,
                       const char* field_name,
                       double value);
bool SetJNIFieldBoolean(JNIEnv* env,
                        jclass cls,
                        jobject obj,
                        const char* field_name,
                        int value);

// Retrieve the static int value stored in the |field_name| field of |cls|.
bool GetJNIFieldStaticInt(JNIEnv* env,
                          jclass cls,
                          const char* field_name,
                          int* value);

// Call a JNI method that returns an int and accepts no arguments.
bool CallJNIMethodI_V(JNIEnv* env,
                      jclass cls,
                      jobject obj,
                      const char* method_name,
                      int* value);

// Call a JNI method that returns a char and accepts no arguments.
bool CallJNIMethodC_V(JNIEnv* env,
                      jclass cls,
                      jobject obj,
                      const char* method_name,
                      char16_t* value);

// Call a JNI method that returns a double and accepts no arguments.
bool CallJNIMethodD_V(JNIEnv* env,
                      jclass cls,
                      jobject obj,
                      const char* method_name,
                      double* value);

// Rertieve the CefSize equivalent of a java.awt.Dimension.
CefSize GetJNISize(JNIEnv* env, jobject obj);

// Retrieve the CefRect equivalent of a java.awt.Rectangle.
CefRect GetJNIRect(JNIEnv* env, jobject obj);

// Retrieve the value of a java.awt.Point.
bool GetJNIPoint(JNIEnv* env, jobject obj, int* x, int* y);

// Get the existing CEF browser counterpart.
CefRefPtr<CefBrowser> GetJNIBrowser(JNIEnv* env, jobject jbrowser);

jobject GetJNIEnumValue(JNIEnv* env,
                        const char* class_name,
                        const char* enum_valname);

bool IsJNIEnumValue(JNIEnv* env,
                    jobject jenum,
                    const char* class_name,
                    const char* enum_valname);

// Helper macro for handling jni enum values in a switch statement
#define JNI_CASE(env, cls, type, result)       \
  case type:                                   \
    result = GetJNIEnumValue(env, cls, #type); \
    break;

// Helper macros for defining and retrieving static ints.
#define JNI_STATIC(name) _static_##name

#define JNI_STATIC_DEFINE_INT(env, cls, name) \
  JNI_STATIC_DEFINE_INT_RV(env, cls, name, );

#define JNI_STATIC_DEFINE_INT_RV(env, cls, name, rv)             \
  static int JNI_STATIC(name) = -1;                              \
  if (JNI_STATIC(name) == -1 &&                                  \
      !GetJNIFieldStaticInt(env, cls, #name, &JNI_STATIC(name))) \
    return rv;

#define JNI_GET_BROWSER_OR_RETURN(env, jbrowser, ...) \
  GetJNIBrowser(env, jbrowser);                       \
  if (!browser.get())                                 \
    return __VA_ARGS__;

#endif  // JCEF_NATIVE_JNI_UTIL_H_
