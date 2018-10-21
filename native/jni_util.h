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
#include "include/cef_request_handler.h"
#include "include/wrapper/cef_message_router.h"
#include "util.h"

// Set the global JVM reference.
void SetJVM(JavaVM* jvm);

// Retrieve the JNIEnv for the current thread.
JNIEnv* GetJNIEnv();

// Determines whether the current thread is already attached to the VM,
// and tells the caller if it needs to later DetachCurrentThread.
//
// INSTEAD OF USING THIS FUNCTION DIRECTLY, USE THE HELPER MACRO
// BEGIN_ENV(e) INSTEAD.
jint GetJNIEnv(JNIEnv** env, bool* mustDetach);

// Detaches the current thread from the VM.
//
// INSTEAD OF USING THIS FUNCTION DIRECTLY; USE THE HELPER MACRO
// END_ENV(e) INSTEAD.
void DetachFromThread(bool* mustDetach);

// Sets the java class loader to use for creating java objects in native code.
// We have to use a class loader instead of the JNIEnv::FindClass method
// because JNIEnv::FindClass always uses the system class loader if called
// from a non-Java thread, which will not work if the embedding Java code
// uses a custom class loader for JCEF classes (e.g. in JavaWebStart).
void SetJavaClassLoader(JNIEnv* env, jobject javaClassLoader);

// Returns a class with the given fully qualified |class_name| (with '/' as
// separator).
jclass FindClass(JNIEnv* env, const char* class_name);

// Helper macros to bind and release the JNI environment
// to other threads than the JNI function was called on.
#define BEGIN_ENV(e)           \
  JNIEnv* e = NULL;            \
  bool __shouldDetach = false; \
  if (GetJNIEnv(&e, &__shouldDetach) == JNI_OK && e != NULL) {
#define END_ENV(e)                   \
  DetachFromThread(&__shouldDetach); \
  }

#if defined(OS_WIN)
HWND GetHwndOfCanvas(jobject canvas, JNIEnv* env);
#elif defined(OS_LINUX)
unsigned long GetDrawableOfCanvas(jobject canvas, JNIEnv* env);
#endif

// Create a new JNI object and call the default constructor.
jobject NewJNIObject(JNIEnv* env, jclass cls);
jobject NewJNIObject(JNIEnv* env, const char* class_name);
jobject NewJNIObject(JNIEnv* env, const char* class_name, const char* sig, ...);

// Create a new primitive reference
jobject NewJNIBoolRef(JNIEnv* env, bool initValue);
jobject NewJNIIntRef(JNIEnv* env, int initValue);
jobject NewJNIStringRef(JNIEnv* env, const CefString& initValue);

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

// Create a new date type
jobject NewJNIDate(JNIEnv* env, const CefTime& time);

// Create a new cookie object
jobject NewJNICookie(JNIEnv* env, const CefCookie& cookie);

// Retrieve a Cookie value
CefCookie GetJNICookie(JNIEnv* env, jobject jcookie);

// Retrieve a String value.
CefString GetJNIString(JNIEnv* env, jstring jstr);

// Retrieve a String array.
void GetJNIStringArray(JNIEnv* env,
                       jobjectArray jarray,
                       std::vector<CefString>& vals);

CefMessageRouterConfig GetJNIMessageRouterConfig(JNIEnv* env, jobject jConfig);
CefMessageRouterConfig GetJNIMessageRouterConfigFromRouter(JNIEnv* env,
                                                           jobject jRouter);

// Create a new JNI error code.
jobject NewJNIErrorCode(JNIEnv* env, cef_errorcode_t errorCode);
cef_errorcode_t GetJNIErrorCode(JNIEnv* env, jobject jerrorCode);

// Create a new String value.
jstring NewJNIString(JNIEnv* env, const CefString& str);

jobject NewJNILong(JNIEnv* env, const int64& val);

// Create a new array of String values.
jobjectArray NewJNIStringArray(JNIEnv* env, const std::vector<CefString>& vals);

jobject NewJNIStringVector(JNIEnv* env, const std::vector<CefString>& vals);

void AddJNIStringToVector(JNIEnv* env, jobject jvector, const CefString& str);

jobject NewJNILongVector(JNIEnv* env, const std::vector<int64>& vals);

void AddJNILongToVector(JNIEnv* env, jobject jvector, const int64& val);

void GetJNIStringVector(JNIEnv* env,
                        jobject jvector,
                        std::vector<CefString>& vals);

bool GetJNIFieldString(JNIEnv* env,
                       jclass cls,
                       jobject obj,
                       const char* field_name,
                       CefString* value);

bool GetJNIFieldDate(JNIEnv* env,
                     jclass cls,
                     jobject obj,
                     const char* field_name,
                     CefTime* value);

bool GetJNIFieldBoolean(JNIEnv* env,
                        jclass cls,
                        jobject obj,
                        const char* field_name,
                        int* value);

bool GetJNIFieldObject(JNIEnv* env,
                       jclass cls,
                       jobject obj,
                       const char* field_name,
                       jobject* value,
                       const char* object_type);

// Retrieve the int value stored in the |field_name| field of |cls|.
bool GetJNIFieldInt(JNIEnv* env,
                    jclass cls,
                    jobject obj,
                    const char* field_name,
                    int* value);

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
                      char* value);

// Retrieve the CefPageRange equivalent of a org.cef.misc.CefPageRange
CefRange GetJNIPageRange(JNIEnv* env, jobject obj);

// Create a new org.cef.misc.CefPageRange
jobject NewJNIPageRange(JNIEnv* env, const CefRange& range);

// Rertieve the CefSize equivalent of a java.awt.Dimension.
CefSize GetJNISize(JNIEnv* env, jobject obj);

// Retrieve the CefRect equivalent of a java.awt.Rectangle.
CefRect GetJNIRect(JNIEnv* env, jobject obj);

// Create a new java.awt.Rectangle.
jobject NewJNIRect(JNIEnv* env, const CefRect& rect);

// create a new array of java.awt.Rectangle.
jobjectArray NewJNIRectArray(JNIEnv* env, const std::vector<CefRect>& vals);

// Retrieve the value of a java.awt.Point.
bool GetJNIPoint(JNIEnv* env, jobject obj, int* x, int* y);

// Create a new java.awt.Point.
jobject NewJNIPoint(JNIEnv* env, int x, int y);

CefSettings GetJNISettings(JNIEnv* env, jobject obj);

CefPdfPrintSettings GetJNIPdfPrintSettings(JNIEnv* env, jobject obj);

// Get java browser counterpart
jobject GetJNIBrowser(CefRefPtr<CefBrowser>);
jobjectArray GetAllJNIBrowser(JNIEnv* env, jobject jclientHandler);

jobject GetJNIFrame(JNIEnv* env, CefRefPtr<CefFrame>);

jobject NewJNITransitionType(JNIEnv* env,
                             CefRequest::TransitionType transitionType);

jobject NewJNIURLRequestStatus(JNIEnv* env,
                               CefRequestHandler::URLRequestStatus);

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

// Helper macros to call a method on the java side
#define JNI_CALL_METHOD(env, obj, method, sig, type, storeIn, ...)        \
  {                                                                       \
    if (env && obj) {                                                     \
      jclass _cls = env->GetObjectClass(obj);                             \
      jmethodID _methodId = env->GetMethodID(_cls, method, sig);          \
      if (_methodId != NULL) {                                            \
        storeIn = env->Call##type##Method(obj, _methodId, ##__VA_ARGS__); \
      }                                                                   \
      if (env->ExceptionOccurred()) {                                     \
        env->ExceptionDescribe();                                         \
        env->ExceptionClear();                                            \
      }                                                                   \
    }                                                                     \
  }

#define JNI_CALL_VOID_METHOD_EX(env, obj, method, sig, ...)      \
  {                                                              \
    if (env && obj) {                                            \
      jclass _cls = env->GetObjectClass(obj);                    \
      jmethodID _methodId = env->GetMethodID(_cls, method, sig); \
      if (_methodId != NULL) {                                   \
        env->CallVoidMethod(obj, _methodId, ##__VA_ARGS__);      \
      }                                                          \
    }                                                            \
  }

#define JNI_CALL_VOID_METHOD(env, obj, method, sig, ...)         \
  {                                                              \
    if (env && obj) {                                            \
      jclass _cls = env->GetObjectClass(obj);                    \
      jmethodID _methodId = env->GetMethodID(_cls, method, sig); \
      if (_methodId != NULL) {                                   \
        env->CallVoidMethod(obj, _methodId, ##__VA_ARGS__);      \
      }                                                          \
      if (env->ExceptionOccurred()) {                            \
        env->ExceptionDescribe();                                \
        env->ExceptionClear();                                   \
      }                                                          \
    }                                                            \
  }

#define JNI_GET_BROWSER_OR_RETURN(env, obj, ...)           \
  GetCefFromJNIObject<CefBrowser>(env, obj, "CefBrowser"); \
  if (!browser.get())                                      \
    return __VA_ARGS__;

// Type specialization helpers for SetCefForJNIObject.
struct SetCefForJNIObjectHelper {
  static inline void AddRef(CefBaseScoped* obj) {}
  static inline void Release(CefBaseScoped* obj) {}
  static inline void AddRef(CefBaseRefCounted* obj) { obj->AddRef(); }
  static inline void Release(CefBaseRefCounted* obj) { obj->Release(); }

  // For ref-counted implementations that don't derive from CefBaseRefCounted.
  template <class T>
  static inline void AddRef(base::RefCountedThreadSafe<T>* obj) {
    obj->AddRef();
  }
  template <class T>
  static inline void Release(base::RefCountedThreadSafe<T>* obj) {
    obj->Release();
  }
};

// Set the CEF base object for an existing JNI object. A reference will be
// added to the base object. If a previous base object existed a reference
// will be removed from that object.
template <class T>
bool SetCefForJNIObject(JNIEnv* env,
                        jobject obj,
                        T* base,
                        const char* varName) {
  if (!obj)
    return false;
  jstring identifer = env->NewStringUTF(varName);
  jlong previousValue = 0;
  JNI_CALL_METHOD(env, obj, "getNativeRef", "(Ljava/lang/String;)J", Long,
                  previousValue, identifer);

  T* oldbase = reinterpret_cast<T*>(previousValue);
  if (oldbase) {
    // Remove a reference from the previous base object.
    SetCefForJNIObjectHelper::Release(oldbase);
  }

  JNI_CALL_VOID_METHOD(env, obj, "setNativeRef", "(Ljava/lang/String;J)V",
                       identifer, (jlong)base);
  if (base) {
    // Add a reference to the new base object.
    SetCefForJNIObjectHelper::AddRef(base);
  }
  env->DeleteLocalRef(identifer);
  return true;
}

// Retrieve the CEF base object from an existing JNI object.
template <class T>
T* GetCefFromJNIObject(JNIEnv* env, jobject obj, const char* varName) {
  jstring identifer = env->NewStringUTF(varName);
  jlong previousValue = 0;
  if (obj != NULL)
    JNI_CALL_METHOD(env, obj, "getNativeRef", "(Ljava/lang/String;)J", Long,
                    previousValue, identifer);

  env->DeleteLocalRef(identifer);
  if (previousValue != 0)
    return reinterpret_cast<T*>(previousValue);
  return NULL;
}

#endif  // JCEF_NATIVE_JNI_UTIL_H_
