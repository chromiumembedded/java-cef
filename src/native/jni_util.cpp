// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "jni_util.h"
#include "client_handler.h"
#include <jawt.h>
#include "util.h"

namespace {

JavaVM *g_jvm = NULL;

}  // namespace

void SetJVM(JavaVM* jvm) {
  ASSERT(!g_jvm);
  g_jvm = jvm;
}

JNIEnv* GetJNIEnv() {
  JNIEnv *env = NULL;
  if (g_jvm->GetEnv((void**)&env, JNI_VERSION_1_6) == JNI_EDETACHED &&
      g_jvm->AttachCurrentThread((void**)&env, NULL) != JNI_OK) {
    return NULL;
  }
  return env;
}

// Determines whether the current thread is already attached to the VM,
// and tells the caller if it needs to later DetachCurrentThread.
//
// CALL THIS ONCE WITHIN A FUNCTION SCOPE and use a local boolean
// for mustDetach; if you do not, the first call might attach, setting
// mustDetach to true, but the second will misleadingly set mustDetach
// to false, leaving a dangling JNIEnv.
jint GetJNIEnv(JNIEnv **env, bool *mustDetach) {
  jint getEnvErr = JNI_OK;
  *mustDetach = false;
  if (g_jvm) {
    getEnvErr = g_jvm->GetEnv((void **)env, JNI_VERSION_1_4);
    if (getEnvErr == JNI_EDETACHED) {
      getEnvErr = g_jvm->AttachCurrentThread((void **)env, NULL);
      if (getEnvErr == JNI_OK) {
        *mustDetach = true;
      }
    }
  }
  return getEnvErr;
}

void DetachFromThread(bool *mustDetach) {
  if (!g_jvm) {
    return;
  }
  if (*mustDetach)
    g_jvm->DetachCurrentThread();
}

jobject NewJNIObject(JNIEnv* env, jclass cls) {
  jmethodID initID = env->GetMethodID(cls, "<init>", "()V");
  if(initID == 0) {
    env->ExceptionClear();
    return NULL;
  }

  jobject obj = env->NewObject(cls, initID);
  if(obj == NULL) {
    env->ExceptionClear();
    return NULL;
  }

  return obj;
}

jobject NewJNIObject(JNIEnv* env, const char* class_name) {
  jclass cls = env->FindClass(class_name);
  if (!cls)
    return NULL;

  return NewJNIObject(env, cls);
}

jobject NewJNIObject(JNIEnv* env, const char* class_name, const char* sig, ...) {
  jclass cls = env->FindClass(class_name);
  if (!cls)
    return NULL;

  jmethodID initID = env->GetMethodID(cls, "<init>", sig);
  if (initID == 0) {
    env->ExceptionClear();
    return NULL;
  }

  va_list ap;
  va_start(ap, sig);

  jobject obj = env->NewObjectV(cls, initID, ap);
  if (obj == NULL) {
    env->ExceptionClear();
    return NULL;
  }

  return obj;
}

jobject NewJNIBoolRef(JNIEnv* env, bool initValue) {
  jobject jboolRef = NewJNIObject(env, "org/cef/misc/BoolRef");
  if (!jboolRef)
    return NULL;
  SetJNIBoolRef(env, jboolRef, initValue);
  return jboolRef;
}

jobject NewJNIIntRef(JNIEnv* env, int initValue) {
  jobject jintRef = NewJNIObject(env, "org/cef/misc/IntRef");
  if (!jintRef)
    return NULL;
  SetJNIIntRef(env, jintRef, initValue);
  return jintRef;
}

bool GetJNIBoolRef(JNIEnv* env, jobject jboolRef) {
  jboolean boolRefRes = JNI_FALSE;
  JNI_CALL_METHOD(env, jboolRef, 
                  "get",
                   "()Z",
                   Boolean,
                   boolRefRes);
  return (boolRefRes != JNI_FALSE);
}

int GetJNIIntRef(JNIEnv* env, jobject jintRef) {
  jint intRefRes = -1;
  JNI_CALL_METHOD(env, jintRef, 
                  "get",
                  "()I",
                  Int,
                  intRefRes);
  return intRefRes;
}

void SetJNIBoolRef(JNIEnv* env, jobject jboolRef, bool boolValue) {
  JNI_CALL_VOID_METHOD(env, jboolRef, 
                       "set",
                       "(Z)V",
                       (boolValue ? JNI_TRUE : JNI_FALSE));
}

void SetJNIIntRef(JNIEnv* env, jobject jintRef, int intValue) {
  JNI_CALL_VOID_METHOD(env, jintRef, 
                       "set",
                       "(I)V",
                       intValue);
}

CefString GetJNIString(JNIEnv* env, jstring jstr)
{
  CefString cef_str;
  const char* chr = NULL;
  if(jstr)
    chr = env->GetStringUTFChars(jstr, NULL);
  if(chr)
    cef_str = chr;
  env->ReleaseStringUTFChars(jstr, chr);
  return cef_str;
}

void GetJNIStringArray(JNIEnv* env, jobjectArray jarray,
                       std::vector<CefString>& vals) {
  jsize argc = env->GetArrayLength(jarray);
  for (jsize i = 0; i < argc; ++i) {
    jstring jstr = (jstring)env->GetObjectArrayElement(jarray, i);
    const char* cstr = env->GetStringUTFChars(jstr, NULL);
    CefString cef_str(cstr);
    vals.push_back(cef_str);
    env->ReleaseStringUTFChars(jstr, cstr);
  }
}

jobject NewJNIErrorCode(JNIEnv* env, cef_errorcode_t errorCode) {
  jobject jerrorCode = NULL;
  switch (errorCode) {
    default:
    case ERR_NONE:
      jerrorCode = GetJNIEnumValue(env,
                                   "org/cef/handler/CefLoadHandler$ErrorCode",
                                   "ERR_NONE");
      break;
    case ERR_FAILED:
      jerrorCode = GetJNIEnumValue(env,
                                   "org/cef/handler/CefLoadHandler$ErrorCode",
                                   "ERR_FAILED");
      break;
    case ERR_ABORTED:
      jerrorCode = GetJNIEnumValue(env,
                                   "org/cef/handler/CefLoadHandler$ErrorCode",
                                   "ERR_ABORTED");
      break;
    case ERR_INVALID_ARGUMENT:
      jerrorCode = GetJNIEnumValue(env,
                                   "org/cef/handler/CefLoadHandler$ErrorCode",
                                   "ERR_INVALID_ARGUMENT");
      break;
    case ERR_INVALID_HANDLE:
      jerrorCode = GetJNIEnumValue(env,
                                   "org/cef/handler/CefLoadHandler$ErrorCode",
                                   "ERR_INVALID_HANDLE");
      break;
    case ERR_FILE_NOT_FOUND:
      jerrorCode = GetJNIEnumValue(env,
                                   "org/cef/handler/CefLoadHandler$ErrorCode",
                                   "ERR_FILE_NOT_FOUND");
      break;
    case ERR_TIMED_OUT:
      jerrorCode = GetJNIEnumValue(env,
                                   "org/cef/handler/CefLoadHandler$ErrorCode",
                                   "ERR_TIMED_OUT");
      break;
    case ERR_FILE_TOO_BIG:
      jerrorCode = GetJNIEnumValue(env,
                                   "org/cef/handler/CefLoadHandler$ErrorCode",
                                   "ERR_FILE_TOO_BIG");
      break;
    case ERR_UNEXPECTED:
      jerrorCode = GetJNIEnumValue(env,
                                   "org/cef/handler/CefLoadHandler$ErrorCode",
                                   "ERR_UNEXPECTED");
      break;
    case ERR_ACCESS_DENIED:
      jerrorCode = GetJNIEnumValue(env,
                                   "org/cef/handler/CefLoadHandler$ErrorCode",
                                   "ERR_ACCESS_DENIED");
      break;
    case ERR_NOT_IMPLEMENTED:
      jerrorCode = GetJNIEnumValue(env,
                                   "org/cef/handler/CefLoadHandler$ErrorCode",
                                   "ERR_NOT_IMPLEMENTED");
      break;
    case ERR_CONNECTION_CLOSED:
      jerrorCode = GetJNIEnumValue(env,
                                   "org/cef/handler/CefLoadHandler$ErrorCode",
                                   "ERR_CONNECTION_CLOSED");
      break;
    case ERR_CONNECTION_RESET:
      jerrorCode = GetJNIEnumValue(env,
                                   "org/cef/handler/CefLoadHandler$ErrorCode",
                                   "ERR_CONNECTION_RESET");
      break;
    case ERR_CONNECTION_REFUSED:
      jerrorCode = GetJNIEnumValue(env,
                                   "org/cef/handler/CefLoadHandler$ErrorCode",
                                   "ERR_CONNECTION_REFUSED");
      break;
    case ERR_CONNECTION_ABORTED:
      jerrorCode = GetJNIEnumValue(env,
                                   "org/cef/handler/CefLoadHandler$ErrorCode",
                                   "ERR_CONNECTION_ABORTED");
      break;
    case ERR_CONNECTION_FAILED:
      jerrorCode = GetJNIEnumValue(env,
                                   "org/cef/handler/CefLoadHandler$ErrorCode",
                                   "ERR_CONNECTION_FAILED");
      break;
    case ERR_NAME_NOT_RESOLVED:
      jerrorCode = GetJNIEnumValue(env,
                                   "org/cef/handler/CefLoadHandler$ErrorCode",
                                   "ERR_NAME_NOT_RESOLVED");
      break;
    case ERR_INTERNET_DISCONNECTED:
      jerrorCode = GetJNIEnumValue(env,
                                   "org/cef/handler/CefLoadHandler$ErrorCode",
                                   "ERR_INTERNET_DISCONNECTED");
      break;
    case ERR_SSL_PROTOCOL_ERROR:
      jerrorCode = GetJNIEnumValue(env,
                                   "org/cef/handler/CefLoadHandler$ErrorCode",
                                   "ERR_SSL_PROTOCOL_ERROR");
      break;
    case ERR_ADDRESS_INVALID:
      jerrorCode = GetJNIEnumValue(env,
                                   "org/cef/handler/CefLoadHandler$ErrorCode",
                                   "ERR_ADDRESS_INVALID");
      break;
    case ERR_ADDRESS_UNREACHABLE:
      jerrorCode = GetJNIEnumValue(env,
                                   "org/cef/handler/CefLoadHandler$ErrorCode",
                                   "ERR_ADDRESS_UNREACHABLE");
      break;
    case ERR_SSL_CLIENT_AUTH_CERT_NEEDED:
      jerrorCode = GetJNIEnumValue(env,
                                   "org/cef/handler/CefLoadHandler$ErrorCode",
                                   "ERR_SSL_CLIENT_AUTH_CERT_NEEDED");
      break;
    case ERR_TUNNEL_CONNECTION_FAILED:
      jerrorCode = GetJNIEnumValue(env,
                                   "org/cef/handler/CefLoadHandler$ErrorCode",
                                   "ERR_TUNNEL_CONNECTION_FAILED");
      break;
    case ERR_NO_SSL_VERSIONS_ENABLED:
      jerrorCode = GetJNIEnumValue(env,
                                   "org/cef/handler/CefLoadHandler$ErrorCode",
                                   "ERR_NO_SSL_VERSIONS_ENABLED");
      break;
    case ERR_SSL_VERSION_OR_CIPHER_MISMATCH:
      jerrorCode = GetJNIEnumValue(env,
                                   "org/cef/handler/CefLoadHandler$ErrorCode",
                                   "ERR_SSL_VERSION_OR_CIPHER_MISMATCH");
      break;
    case ERR_SSL_RENEGOTIATION_REQUESTED:
      jerrorCode = GetJNIEnumValue(env,
                                   "org/cef/handler/CefLoadHandler$ErrorCode",
                                   "ERR_SSL_RENEGOTIATION_REQUESTED");
      break;
    case ERR_CERT_COMMON_NAME_INVALID:
      jerrorCode = GetJNIEnumValue(env,
                                   "org/cef/handler/CefLoadHandler$ErrorCode",
                                   "ERR_CERT_COMMON_NAME_INVALID");
      break;
    case ERR_CERT_DATE_INVALID:
      jerrorCode = GetJNIEnumValue(env,
                                   "org/cef/handler/CefLoadHandler$ErrorCode",
                                   "ERR_CERT_DATE_INVALID");
      break;
    case ERR_CERT_AUTHORITY_INVALID:
      jerrorCode = GetJNIEnumValue(env,
                                   "org/cef/handler/CefLoadHandler$ErrorCode",
                                   "ERR_CERT_AUTHORITY_INVALID");
      break;
    case ERR_CERT_CONTAINS_ERRORS:
      jerrorCode = GetJNIEnumValue(env,
                                   "org/cef/handler/CefLoadHandler$ErrorCode",
                                   "ERR_CERT_CONTAINS_ERRORS");
      break;
    case ERR_CERT_NO_REVOCATION_MECHANISM:
      jerrorCode = GetJNIEnumValue(env,
                                   "org/cef/handler/CefLoadHandler$ErrorCode",
                                   "ERR_CERT_NO_REVOCATION_MECHANISM");
      break;
    case ERR_CERT_UNABLE_TO_CHECK_REVOCATION:
      jerrorCode = GetJNIEnumValue(env,
                                   "org/cef/handler/CefLoadHandler$ErrorCode",
                                   "ERR_CERT_UNABLE_TO_CHECK_REVOCATION");
      break;
    case ERR_CERT_REVOKED:
      jerrorCode = GetJNIEnumValue(env,
                                   "org/cef/handler/CefLoadHandler$ErrorCode",
                                   "ERR_CERT_REVOKED");
      break;
    case ERR_CERT_INVALID:
      jerrorCode = GetJNIEnumValue(env,
                                   "org/cef/handler/CefLoadHandler$ErrorCode",
                                   "ERR_CERT_INVALID");
      break;
    case ERR_CERT_END:
      jerrorCode = GetJNIEnumValue(env,
                                   "org/cef/handler/CefLoadHandler$ErrorCode",
                                   "ERR_CERT_END");
      break;
    case ERR_INVALID_URL:
      jerrorCode = GetJNIEnumValue(env,
                                   "org/cef/handler/CefLoadHandler$ErrorCode",
                                   "ERR_INVALID_URL");
      break;
    case ERR_DISALLOWED_URL_SCHEME:
      jerrorCode = GetJNIEnumValue(env,
                                   "org/cef/handler/CefLoadHandler$ErrorCode",
                                   "ERR_DISALLOWED_URL_SCHEME");
      break;
    case ERR_UNKNOWN_URL_SCHEME:
      jerrorCode = GetJNIEnumValue(env,
                                   "org/cef/handler/CefLoadHandler$ErrorCode",
                                   "ERR_UNKNOWN_URL_SCHEME");
      break;
    case ERR_TOO_MANY_REDIRECTS:
      jerrorCode = GetJNIEnumValue(env,
                                   "org/cef/handler/CefLoadHandler$ErrorCode",
                                   "ERR_TOO_MANY_REDIRECTS");
      break;
    case ERR_UNSAFE_REDIRECT:
      jerrorCode = GetJNIEnumValue(env,
                                   "org/cef/handler/CefLoadHandler$ErrorCode",
                                   "ERR_UNSAFE_REDIRECT");
      break;
    case ERR_UNSAFE_PORT:
      jerrorCode = GetJNIEnumValue(env,
                                   "org/cef/handler/CefLoadHandler$ErrorCode",
                                   "ERR_UNSAFE_PORT");
      break;
    case ERR_INVALID_RESPONSE:
      jerrorCode = GetJNIEnumValue(env,
                                   "org/cef/handler/CefLoadHandler$ErrorCode",
                                   "ERR_INVALID_RESPONSE");
      break;
    case ERR_INVALID_CHUNKED_ENCODING:
      jerrorCode = GetJNIEnumValue(env,
                                   "org/cef/handler/CefLoadHandler$ErrorCode",
                                   "ERR_INVALID_CHUNKED_ENCODING");
      break;
    case ERR_METHOD_NOT_SUPPORTED:
      jerrorCode = GetJNIEnumValue(env,
                                   "org/cef/handler/CefLoadHandler$ErrorCode",
                                   "ERR_METHOD_NOT_SUPPORTED");
      break;
    case ERR_UNEXPECTED_PROXY_AUTH:
      jerrorCode = GetJNIEnumValue(env,
                                   "org/cef/handler/CefLoadHandler$ErrorCode",
                                   "ERR_UNEXPECTED_PROXY_AUTH");
      break;
    case ERR_EMPTY_RESPONSE:
      jerrorCode = GetJNIEnumValue(env,
                                   "org/cef/handler/CefLoadHandler$ErrorCode",
                                   "ERR_EMPTY_RESPONSE");
      break;
    case ERR_RESPONSE_HEADERS_TOO_BIG:
      jerrorCode = GetJNIEnumValue(env,
                                   "org/cef/handler/CefLoadHandler$ErrorCode",
                                   "ERR_RESPONSE_HEADERS_TOO_BIG");
      break;
    case ERR_CACHE_MISS:
      jerrorCode = GetJNIEnumValue(env,
                                   "org/cef/handler/CefLoadHandler$ErrorCode",
                                   "ERR_CACHE_MISS");
      break;
    case ERR_INSECURE_RESPONSE:
      jerrorCode = GetJNIEnumValue(env,
                                   "org/cef/handler/CefLoadHandler$ErrorCode",
                                   "ERR_INSECURE_RESPONSE");
      break;
  }
  return jerrorCode;
}

jstring NewJNIString(JNIEnv* env, const CefString& str)
{
  std::string cstr(str);
  return env->NewStringUTF(cstr.c_str());
}

jobjectArray NewJNIStringArray(JNIEnv* env,
                               const std::vector<CefString>& vals)
{
  if(vals.empty())
    return NULL;

  jclass cls = env->FindClass("java/lang/String");
  if (!cls)
    return NULL;

  jobjectArray arr = env->NewObjectArray(
      static_cast<jsize>(vals.size()),
      cls,
      NULL);

  for(jsize i = 0; i < static_cast<jsize>(vals.size()); i++)
    env->SetObjectArrayElement(arr, i, NewJNIString(env, vals[i]));

  return arr;
}

jobject NewJNIStringVector(JNIEnv* env,
                           const std::vector<CefString>& vals) {

  jobject jvector = NewJNIObject(env, "java/util/Vector");
  if (!jvector)
    return NULL;

  std::vector<CefString>::const_iterator iter;
  for (iter = vals.begin(); iter != vals.end(); ++iter) {
    AddJNIStringToVector(env, jvector, *iter);
  }
  return jvector;
}

void AddJNIStringToVector(JNIEnv* env, jobject jvector, 
                          const CefString &str) {
  jstring argument = NewJNIString(env, str);
  JNI_CALL_VOID_METHOD(env, jvector, "addElement", "(Ljava/lang/Object;)V",argument);
}

void GetJNIStringVector(JNIEnv* env, jobject jvector,
                        std::vector<CefString>& vals) {
  if (!jvector)
    return;

  jint jsize=0;
  JNI_CALL_METHOD(env, jvector, "size", "()I", Int, jsize);

  for (jint index=0; index< jsize; index++) {
    jobject str=NULL;
    JNI_CALL_METHOD(env, jvector, "get", "(I)Ljava/lang/Object;", Object, str, index);
    vals.push_back(GetJNIString(env, (jstring)str));
  }
}

bool GetJNIFieldInt(JNIEnv* env, jclass cls, jobject obj,
                    const char* field_name, int* value) {
  jfieldID field = env->GetFieldID(cls, field_name, "I");
  if (field) {
    *value = env->GetIntField(obj, field);
    return true;
  }
  env->ExceptionClear();
  return false;
}

bool SetJNIFieldInt(JNIEnv* env, jclass cls, jobject obj,
                    const char* field_name, int value) {
  jfieldID field = env->GetFieldID(cls, field_name, "I");
  if (field) {
    env->SetIntField(obj, field, value);
    return true;
  }
  env->ExceptionClear();
  return false;
}

bool GetJNIFieldStaticInt(JNIEnv* env, jclass cls,
                          const char* field_name, int* value) {
  jfieldID field = env->GetStaticFieldID(cls, field_name, "I");
  if (field) {
    *value = env->GetStaticIntField(cls, field);
    return true;
  }
  env->ExceptionClear();
  return false;
}

bool CallJNIMethodI_V(JNIEnv* env, jclass cls, jobject obj,
                      const char* method_name, int* value) {
  jmethodID methodID = env->GetMethodID(cls, method_name, "()I");
  if (methodID) {
    *value = env->CallIntMethod(obj, methodID);
    return true;
  }
  env->ExceptionClear();
  return false;
}

bool CallJNIMethodC_V(JNIEnv* env, jclass cls, jobject obj,
                      const char* method_name, char* value) {
  jmethodID methodID = env->GetMethodID(cls, method_name, "()C");
  if (methodID) {
    *value = env->CallCharMethod(obj, methodID);
    return true;
  }
  env->ExceptionClear();
  return false;
}

CefRect GetJNIRect(JNIEnv* env, jobject obj) {
  CefRect rect;

  jclass cls = env->FindClass("java/awt/Rectangle");
  if (!cls)
    return rect;

  int x, y, width, height;
  if (GetJNIFieldInt(env, cls, obj, "x", &x) &&
      GetJNIFieldInt(env, cls, obj, "y", &y) &&
      GetJNIFieldInt(env, cls, obj, "width", &width) &&
      GetJNIFieldInt(env, cls, obj, "height", &height)) {
    rect.Set(x, y, width, height);
    return rect;
  }

  return rect;
}

jobject NewJNIRect(JNIEnv* env, const CefRect& rect) {
  jclass cls = env->FindClass("java/awt/Rectangle");
  if (!cls)
    return NULL;

  jobject obj = NewJNIObject(env, cls);
  if (!obj)
    return NULL;

  if (SetJNIFieldInt(env, cls, obj, "x", rect.x) &&
      SetJNIFieldInt(env, cls, obj, "y", rect.y) &&
      SetJNIFieldInt(env, cls, obj, "width", rect.width) &&
      SetJNIFieldInt(env, cls, obj, "height", rect.height)) {
    return obj;
  }

  env->DeleteLocalRef(obj);
  return NULL;
}

jobjectArray NewJNIRectArray(JNIEnv* env,
                             const std::vector<CefRect>& vals) {
  if(vals.empty())
    return NULL;

  jclass cls = env->FindClass("java/awt/Rectangle");
  if (!cls)
    return NULL;

  jobjectArray arr = env->NewObjectArray(
      static_cast<jsize>(vals.size()),
      cls,
      NULL);

  for(jsize i = 0; i < static_cast<jsize>(vals.size()); i++)
    env->SetObjectArrayElement(arr, i, NewJNIRect(env, vals[i]));

  return arr;
}

bool GetJNIPoint(JNIEnv* env, jobject obj, int* x, int* y) {
  jclass cls = env->FindClass("java/awt/Rectangle");
  if (!cls)
    return false;

  if (GetJNIFieldInt(env, cls, obj, "x", x) &&
      GetJNIFieldInt(env, cls, obj, "y", y)) {
    return true;
  }

  return false;
}

// Create a new java.awt.Point.
jobject NewJNIPoint(JNIEnv* env, int x, int y) {
  jclass cls = env->FindClass("java/awt/Point");
  if (!cls)
    return NULL;

  jobject obj = NewJNIObject(env, cls);
  if (!obj)
    return NULL;

  if (SetJNIFieldInt(env, cls, obj, "x", x) &&
      SetJNIFieldInt(env, cls, obj, "y", y)) {
    return obj;
  }

  env->DeleteLocalRef(obj);
  return NULL;
}

jobject GetJNIBrowser(CefRefPtr<CefBrowser> browser) {
  if (!browser.get())
    return NULL;
  CefRefPtr<ClientHandler> client = (ClientHandler*)browser->GetHost()->GetClient().get();
  return client->getBrowser(browser);
}

jobject GetJNIEnumValue(JNIEnv* env, const char* class_name, const char* enum_valname) {
  jclass sourceCls = env->FindClass(class_name);
  if (!sourceCls)
    return NULL;

  std::string tmp;
  tmp.append("L").append(class_name).append(";");

  jfieldID fieldId = env->GetStaticFieldID(sourceCls, enum_valname, tmp.c_str());
  if (!fieldId)
    return NULL;

  jobject jsource = env->GetStaticObjectField(sourceCls, fieldId);
  return jsource;
}

bool IsJNIEnumValue(JNIEnv* env, jobject jenum, const char* class_name, const char* enum_valname) {
  if(!jenum)
    return false;

  jobject compareTo = GetJNIEnumValue(env, class_name, enum_valname);
  if (compareTo) {
    jboolean isEqual = JNI_FALSE;
    JNI_CALL_METHOD(env, jenum,
                    "equals",
                    "(Ljava/lang/Object;)Z",
                    Boolean,
                    isEqual,
                    compareTo);
    return (isEqual != JNI_FALSE);
  }
  return false;
}
