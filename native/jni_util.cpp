// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "jni_util.h"

#include <algorithm>

#include "jni_scoped_helpers.h"

#include "include/cef_base.h"

namespace {

JavaVM* g_jvm = nullptr;

jobject g_javaClassLoader = nullptr;

}  // namespace

void SetJVM(JavaVM* jvm) {
  ASSERT(!g_jvm);
  g_jvm = jvm;
}

JavaVM* GetJVM() {
  return g_jvm;
}

void SetJavaClassLoader(JNIEnv* env, jobject javaClassLoader) {
  ASSERT(!g_javaClassLoader);
  g_javaClassLoader = env->NewGlobalRef(javaClassLoader);
}

jobject GetJavaClassLoader() {
  return g_javaClassLoader;
}

jobject NewJNIObject(JNIEnv* env, jclass cls) {
  jmethodID initID = env->GetMethodID(cls, "<init>", "()V");
  if (initID == 0) {
    env->ExceptionClear();
    return nullptr;
  }

  jobject obj = env->NewObject(cls, initID);
  if (obj == nullptr) {
    env->ExceptionClear();
    return nullptr;
  }

  return obj;
}

jobject NewJNIObject(JNIEnv* env, const char* class_name) {
  ScopedJNIClass cls(env, class_name);
  if (!cls)
    return nullptr;

  return NewJNIObject(env, cls);
}

jobject NewJNIObject(JNIEnv* env,
                     const char* class_name,
                     const char* sig,
                     ...) {
  ScopedJNIClass cls(env, class_name);
  if (!cls)
    return nullptr;

  jmethodID initID = env->GetMethodID(cls, "<init>", sig);
  if (initID == 0) {
    env->ExceptionClear();
    return nullptr;
  }

  va_list ap;
  va_start(ap, sig);

  jobject obj = env->NewObjectV(cls, initID, ap);
  if (obj == nullptr) {
    env->ExceptionClear();
    return nullptr;
  }

  return obj;
}

bool GetJNIBoolRef(JNIEnv* env, jobject jboolRef) {
  jboolean boolRefRes = JNI_FALSE;
  JNI_CALL_METHOD(env, jboolRef, "get", "()Z", Boolean, boolRefRes);
  return (boolRefRes != JNI_FALSE);
}

int GetJNIIntRef(JNIEnv* env, jobject jintRef) {
  jint intRefRes = -1;
  JNI_CALL_METHOD(env, jintRef, "get", "()I", Int, intRefRes);
  return intRefRes;
}

CefString GetJNIStringRef(JNIEnv* env, jobject jstringRef) {
  ScopedJNIStringResult str(env);
  JNI_CALL_METHOD(env, jstringRef, "get", "()Ljava/lang/String;", Object, str);
  return str.GetCefString();
}

void SetJNIBoolRef(JNIEnv* env, jobject jboolRef, bool boolValue) {
  JNI_CALL_VOID_METHOD(env, jboolRef, "set", "(Z)V",
                       (boolValue ? JNI_TRUE : JNI_FALSE));
}

void SetJNIIntRef(JNIEnv* env, jobject jintRef, int intValue) {
  JNI_CALL_VOID_METHOD(env, jintRef, "set", "(I)V", intValue);
}

void SetJNIStringRef(JNIEnv* env,
                     jobject jstringRef,
                     const CefString& stringValue) {
  JNI_CALL_VOID_METHOD(env, jstringRef, "set", "(Ljava/lang/String;)V",
                       NewJNIString(env, stringValue));
}

jstring NewJNIString(JNIEnv* env, const std::string& str) {
  return env->NewStringUTF(str.c_str());
}

CefString GetJNIString(JNIEnv* env, jstring jstr) {
  CefString cef_str;
  const char* chr = nullptr;
  if (jstr)
    chr = env->GetStringUTFChars(jstr, nullptr);
  if (chr)
    cef_str = chr;
  if (jstr)
    env->ReleaseStringUTFChars(jstr, chr);
  return cef_str;
}

jobjectArray NewJNIStringArray(JNIEnv* env,
                               const std::vector<CefString>& vals) {
  if (vals.empty())
    return nullptr;

  ScopedJNIClass cls(env, "java/lang/String");
  if (!cls)
    return nullptr;

  const jsize size = static_cast<jsize>(vals.size());
  jobjectArray arr = env->NewObjectArray(size, cls, nullptr);

  for (jsize i = 0; i < size; i++) {
    ScopedJNIString str(env, vals[i]);
    env->SetObjectArrayElement(arr, i, str);
  }

  return arr;
}

void GetJNIStringArray(JNIEnv* env,
                       jobjectArray jarray,
                       std::vector<CefString>& vals) {
  jsize argc = env->GetArrayLength(jarray);
  for (jsize i = 0; i < argc; ++i) {
    ScopedJNIStringResult str(env,
                              (jstring)env->GetObjectArrayElement(jarray, i));
    vals.push_back(str.GetCefString());
  }
}

jobject NewJNIStringVector(JNIEnv* env, const std::vector<CefString>& vals) {
  ScopedJNIObjectLocal jvector(env, "java/util/Vector");
  if (!jvector)
    return nullptr;

  std::vector<CefString>::const_iterator iter;
  for (iter = vals.begin(); iter != vals.end(); ++iter) {
    AddJNIStringToVector(env, jvector, *iter);
  }
  return jvector.Release();
}

void AddJNIStringToVector(JNIEnv* env, jobject jvector, const CefString& str) {
  ScopedJNIString argument(env, str);
  JNI_CALL_VOID_METHOD(env, jvector, "addElement", "(Ljava/lang/Object;)V",
                       argument.get());
}

void GetJNIStringVector(JNIEnv* env,
                        jobject jvector,
                        std::vector<CefString>& vals) {
  if (!jvector)
    return;

  jint jsize = 0;
  JNI_CALL_METHOD(env, jvector, "size", "()I", Int, jsize);

  for (jint index = 0; index < jsize; index++) {
    ScopedJNIStringResult jstr(env);
    JNI_CALL_METHOD(env, jvector, "get", "(I)Ljava/lang/Object;", Object, jstr,
                    index);
    vals.push_back(jstr.GetCefString());
  }
}

jobject NewJNIStringMap(JNIEnv* env,
                        const std::map<CefString, CefString>& vals) {
  ScopedJNIObjectLocal jmap(env, "java/util/HashMap");
  if (!jmap)
    return nullptr;

  for (auto iter = vals.begin(); iter != vals.end(); ++iter) {
    ScopedJNIString jkey(env, iter->first);
    ScopedJNIString jvalue(env, iter->second);
    ScopedJNIObjectResult jresult(env);
    JNI_CALL_METHOD(env, jmap, "put",
                    "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;",
                    Object, jresult, jkey.get(), jvalue.get());
  }
  return jmap.Release();
}

void GetJNIStringMultiMap(JNIEnv* env,
                          jobject jheaderMap,
                          std::multimap<CefString, CefString>& vals) {
  if (!jheaderMap)
    return;

  // public abstract java.util.Set<java.util.Map$Entry<K, V>> entrySet();
  ScopedJNIObjectResult jentrySet(env);
  JNI_CALL_METHOD(env, jheaderMap, "entrySet", "()Ljava/util/Set;", Object,
                  jentrySet);
  if (!jentrySet)
    return;

  // public abstract java.lang.Object[] toArray();
  ScopedJNIObjectResult jentrySetValues(env);
  JNI_CALL_METHOD(env, jentrySet, "toArray", "()[Ljava/lang/Object;", Object,
                  jentrySetValues);
  if (!jentrySetValues)
    return;

  jint length = env->GetArrayLength((jobjectArray)jentrySetValues.get());
  for (jint i = 0; i < length; i++) {
    ScopedJNIObjectLocal jmapEntry(
        env,
        env->GetObjectArrayElement((jobjectArray)jentrySetValues.get(), i));
    if (!jmapEntry)
      return;
    ScopedJNIStringResult jkey(env);
    ScopedJNIStringResult jvalue(env);
    JNI_CALL_METHOD(env, jmapEntry, "getKey", "()Ljava/lang/Object;", Object,
                    jkey);
    JNI_CALL_METHOD(env, jmapEntry, "getValue", "()Ljava/lang/Object;", Object,
                    jvalue);
    vals.insert(std::make_pair(jkey.GetCefString(), jvalue.GetCefString()));
  }
}

void SetJNIStringMultiMap(JNIEnv* env,
                          jobject jheaderMap,
                          const std::multimap<CefString, CefString>& vals) {
  for (auto it = vals.begin(); it != vals.end(); ++it) {
    ScopedJNIString jkey(env, it->first);
    ScopedJNIString jvalue(env, it->second);
    ScopedJNIObjectResult jresult(env);
    JNI_CALL_METHOD(env, jheaderMap, "put",
                    "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;",
                    Object, jresult, jkey.get(), jvalue.get());
  }
}

CefMessageRouterConfig GetJNIMessageRouterConfig(JNIEnv* env, jobject jConfig) {
  CefMessageRouterConfig config;

  if (jConfig == nullptr)
    return config;
  ScopedJNIClass cls(env,
                     "org/cef/browser/CefMessageRouter$CefMessageRouterConfig");
  if (cls == nullptr)
    return config;

  GetJNIFieldString(env, cls, jConfig, "jsQueryFunction",
                    &config.js_query_function);
  GetJNIFieldString(env, cls, jConfig, "jsCancelFunction",
                    &config.js_cancel_function);
  return config;
}

jobject NewJNIErrorCode(JNIEnv* env, cef_errorcode_t errorCode) {
  ScopedJNIObjectResult jerrorCode(env);
  switch (errorCode) {
    default:
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode", ERR_NONE,
               jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode", ERR_IO_PENDING,
               jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode", ERR_FAILED,
               jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode", ERR_ABORTED,
               jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_INVALID_ARGUMENT, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_INVALID_HANDLE, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_FILE_NOT_FOUND, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode", ERR_TIMED_OUT,
               jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_FILE_TOO_BIG, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode", ERR_UNEXPECTED,
               jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_ACCESS_DENIED, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_NOT_IMPLEMENTED, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_INSUFFICIENT_RESOURCES, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_OUT_OF_MEMORY, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_UPLOAD_FILE_CHANGED, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_SOCKET_NOT_CONNECTED, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode", ERR_FILE_EXISTS,
               jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_FILE_PATH_TOO_LONG, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_FILE_NO_SPACE, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_FILE_VIRUS_INFECTED, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_BLOCKED_BY_CLIENT, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_NETWORK_CHANGED, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_BLOCKED_BY_ADMINISTRATOR, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_SOCKET_IS_CONNECTED, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_UPLOAD_STREAM_REWIND_NOT_SUPPORTED, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_CONTEXT_SHUT_DOWN, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_BLOCKED_BY_RESPONSE, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_CLEARTEXT_NOT_PERMITTED, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_CONNECTION_CLOSED, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_CONNECTION_RESET, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_CONNECTION_REFUSED, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_CONNECTION_ABORTED, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_CONNECTION_FAILED, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_NAME_NOT_RESOLVED, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_INTERNET_DISCONNECTED, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_SSL_PROTOCOL_ERROR, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_ADDRESS_INVALID, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_ADDRESS_UNREACHABLE, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_SSL_CLIENT_AUTH_CERT_NEEDED, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_TUNNEL_CONNECTION_FAILED, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_NO_SSL_VERSIONS_ENABLED, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_SSL_VERSION_OR_CIPHER_MISMATCH, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_SSL_RENEGOTIATION_REQUESTED, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_PROXY_AUTH_UNSUPPORTED, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_BAD_SSL_CLIENT_AUTH_CERT, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_CONNECTION_TIMED_OUT, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_HOST_RESOLVER_QUEUE_TOO_LARGE, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_SOCKS_CONNECTION_FAILED, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_SOCKS_CONNECTION_HOST_UNREACHABLE, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_ALPN_NEGOTIATION_FAILED, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_SSL_NO_RENEGOTIATION, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_WINSOCK_UNEXPECTED_WRITTEN_BYTES, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_SSL_DECOMPRESSION_FAILURE_ALERT, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_SSL_BAD_RECORD_MAC_ALERT, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_PROXY_AUTH_REQUESTED, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_PROXY_CONNECTION_FAILED, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_MANDATORY_PROXY_CONFIGURATION_FAILED, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_PRECONNECT_MAX_SOCKET_LIMIT, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_SSL_CLIENT_AUTH_PRIVATE_KEY_ACCESS_DENIED, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_SSL_CLIENT_AUTH_CERT_NO_PRIVATE_KEY, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_PROXY_CERTIFICATE_INVALID, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_NAME_RESOLUTION_FAILED, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_NETWORK_ACCESS_DENIED, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_TEMPORARILY_THROTTLED, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_HTTPS_PROXY_TUNNEL_RESPONSE_REDIRECT, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_SSL_CLIENT_AUTH_SIGNATURE_FAILED, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode", ERR_MSG_TOO_BIG,
               jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_WS_PROTOCOL_ERROR, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_ADDRESS_IN_USE, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_SSL_HANDSHAKE_NOT_COMPLETED, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_SSL_BAD_PEER_PUBLIC_KEY, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_SSL_PINNED_KEY_NOT_IN_CERT_CHAIN, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_CLIENT_AUTH_CERT_TYPE_UNSUPPORTED, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_SSL_DECRYPT_ERROR_ALERT, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_WS_THROTTLE_QUEUE_TOO_LARGE, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_SSL_SERVER_CERT_CHANGED, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_SSL_UNRECOGNIZED_NAME_ALERT, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_SOCKET_SET_RECEIVE_BUFFER_SIZE_ERROR, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_SOCKET_SET_SEND_BUFFER_SIZE_ERROR, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_SOCKET_RECEIVE_BUFFER_SIZE_UNCHANGEABLE, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_SOCKET_SEND_BUFFER_SIZE_UNCHANGEABLE, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_SSL_CLIENT_AUTH_CERT_BAD_FORMAT, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_ICANN_NAME_COLLISION, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_SSL_SERVER_CERT_BAD_FORMAT, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_CT_STH_PARSING_FAILED, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_CT_STH_INCOMPLETE, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_UNABLE_TO_REUSE_CONNECTION_FOR_PROXY_AUTH, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_CT_CONSISTENCY_PROOF_PARSING_FAILED, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_SSL_OBSOLETE_CIPHER, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode", ERR_WS_UPGRADE,
               jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_READ_IF_READY_NOT_IMPLEMENTED, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_NO_BUFFER_SPACE, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_SSL_CLIENT_AUTH_NO_COMMON_ALGORITHMS, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_EARLY_DATA_REJECTED, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_WRONG_VERSION_ON_EARLY_DATA, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_TLS13_DOWNGRADE_DETECTED, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_SSL_KEY_USAGE_INCOMPATIBLE, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_INVALID_ECH_CONFIG_LIST, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_ECH_NOT_NEGOTIATED, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_ECH_FALLBACK_CERTIFICATE_INVALID, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_CERT_COMMON_NAME_INVALID, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_CERT_DATE_INVALID, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_CERT_AUTHORITY_INVALID, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_CERT_CONTAINS_ERRORS, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_CERT_NO_REVOCATION_MECHANISM, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_CERT_UNABLE_TO_CHECK_REVOCATION, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_CERT_REVOKED, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_CERT_INVALID, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_CERT_WEAK_SIGNATURE_ALGORITHM, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_CERT_NON_UNIQUE_NAME, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_CERT_WEAK_KEY, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_CERT_NAME_CONSTRAINT_VIOLATION, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_CERT_VALIDITY_TOO_LONG, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_CERTIFICATE_TRANSPARENCY_REQUIRED, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_CERT_SYMANTEC_LEGACY, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_CERT_KNOWN_INTERCEPTION_BLOCKED, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode", ERR_CERT_END,
               jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode", ERR_INVALID_URL,
               jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_DISALLOWED_URL_SCHEME, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_UNKNOWN_URL_SCHEME, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_INVALID_REDIRECT, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_TOO_MANY_REDIRECTS, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_UNSAFE_REDIRECT, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode", ERR_UNSAFE_PORT,
               jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_INVALID_RESPONSE, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_INVALID_CHUNKED_ENCODING, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_METHOD_NOT_SUPPORTED, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_UNEXPECTED_PROXY_AUTH, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_EMPTY_RESPONSE, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_RESPONSE_HEADERS_TOO_BIG, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_PAC_SCRIPT_FAILED, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_REQUEST_RANGE_NOT_SATISFIABLE, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_MALFORMED_IDENTITY, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_CONTENT_DECODING_FAILED, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_NETWORK_IO_SUSPENDED, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_SYN_REPLY_NOT_RECEIVED, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_ENCODING_CONVERSION_FAILED, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_UNRECOGNIZED_FTP_DIRECTORY_LISTING_FORMAT, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_NO_SUPPORTED_PROXIES, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_HTTP2_PROTOCOL_ERROR, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_INVALID_AUTH_CREDENTIALS, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_UNSUPPORTED_AUTH_SCHEME, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_ENCODING_DETECTION_FAILED, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_MISSING_AUTH_CREDENTIALS, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_UNEXPECTED_SECURITY_LIBRARY_STATUS, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_MISCONFIGURED_AUTH_ENVIRONMENT, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_UNDOCUMENTED_SECURITY_LIBRARY_STATUS, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_RESPONSE_BODY_TOO_BIG_TO_DRAIN, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_RESPONSE_HEADERS_MULTIPLE_CONTENT_LENGTH, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_INCOMPLETE_HTTP2_HEADERS, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_PAC_NOT_IN_DHCP, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_RESPONSE_HEADERS_MULTIPLE_CONTENT_DISPOSITION, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_RESPONSE_HEADERS_MULTIPLE_LOCATION, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_HTTP2_SERVER_REFUSED_STREAM, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_HTTP2_PING_FAILED, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_CONTENT_LENGTH_MISMATCH, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_INCOMPLETE_CHUNKED_ENCODING, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_QUIC_PROTOCOL_ERROR, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_RESPONSE_HEADERS_TRUNCATED, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_QUIC_HANDSHAKE_FAILED, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_HTTP2_INADEQUATE_TRANSPORT_SECURITY, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_HTTP2_FLOW_CONTROL_ERROR, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_HTTP2_FRAME_SIZE_ERROR, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_HTTP2_COMPRESSION_ERROR, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_PROXY_AUTH_REQUESTED_WITH_NO_CONNECTION, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_HTTP_1_1_REQUIRED, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_PROXY_HTTP_1_1_REQUIRED, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_PAC_SCRIPT_TERMINATED, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_INVALID_HTTP_RESPONSE, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_CONTENT_DECODING_INIT_FAILED, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_HTTP2_RST_STREAM_NO_ERROR_RECEIVED, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_TOO_MANY_RETRIES, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_HTTP2_STREAM_CLOSED, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_HTTP_RESPONSE_CODE_FAILURE, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_QUIC_CERT_ROOT_NOT_KNOWN, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_QUIC_GOAWAY_REQUEST_CAN_BE_RETRIED, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode", ERR_CACHE_MISS,
               jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_CACHE_READ_FAILURE, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_CACHE_WRITE_FAILURE, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_CACHE_OPERATION_NOT_SUPPORTED, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_CACHE_OPEN_FAILURE, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_CACHE_CREATE_FAILURE, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode", ERR_CACHE_RACE,
               jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_CACHE_CHECKSUM_READ_FAILURE, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_CACHE_CHECKSUM_MISMATCH, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_CACHE_LOCK_TIMEOUT, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_CACHE_AUTH_FAILURE_AFTER_READ, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_CACHE_ENTRY_NOT_SUITABLE, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_CACHE_DOOM_FAILURE, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_CACHE_OPEN_OR_CREATE_FAILURE, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_INSECURE_RESPONSE, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_NO_PRIVATE_KEY_FOR_CERT, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_ADD_USER_CERT_FAILED, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_INVALID_SIGNED_EXCHANGE, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_INVALID_WEB_BUNDLE, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_TRUST_TOKEN_OPERATION_FAILED, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_TRUST_TOKEN_OPERATION_SUCCESS_WITHOUT_SENDING_REQUEST,
               jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_PKCS12_IMPORT_BAD_PASSWORD, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_PKCS12_IMPORT_FAILED, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_IMPORT_CA_CERT_NOT_CA, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_IMPORT_CERT_ALREADY_EXISTS, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_IMPORT_CA_CERT_FAILED, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_IMPORT_SERVER_CERT_FAILED, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_PKCS12_IMPORT_INVALID_MAC, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_PKCS12_IMPORT_INVALID_FILE, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_PKCS12_IMPORT_UNSUPPORTED, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_KEY_GENERATION_FAILED, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_PRIVATE_KEY_EXPORT_FAILED, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_SELF_SIGNED_CERT_GENERATION_FAILED, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_CERT_DATABASE_CHANGED, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_DNS_MALFORMED_RESPONSE, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_DNS_SERVER_REQUIRES_TCP, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_DNS_SERVER_FAILED, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_DNS_TIMED_OUT, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_DNS_CACHE_MISS, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_DNS_SEARCH_EMPTY, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_DNS_SORT_ERROR, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_DNS_SECURE_RESOLVER_HOSTNAME_RESOLUTION_FAILED, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_DNS_NAME_HTTPS_ONLY, jerrorCode);
      JNI_CASE(env, "org/cef/handler/CefLoadHandler$ErrorCode",
               ERR_DNS_REQUEST_CANCELLED, jerrorCode);
  }
  return jerrorCode.Release();
}

cef_errorcode_t GetJNIErrorCode(JNIEnv* env, jobject jerrorCode) {
  cef_errorcode_t errorCode = ERR_NONE;

  if (jerrorCode) {
    jint jcode = 0;
    JNI_CALL_METHOD(env, jerrorCode, "getCode", "()I", Int, jcode);
    errorCode = static_cast<cef_errorcode_t>(jcode);
  }

  return errorCode;
}

bool GetJNIFieldObject(JNIEnv* env,
                       jclass cls,
                       jobject obj,
                       const char* field_name,
                       jobject* value,
                       const char* object_type) {
  jfieldID field = env->GetFieldID(cls, field_name, object_type);
  if (field) {
    *value = env->GetObjectField(obj, field);
    return *value != nullptr;
  }
  env->ExceptionClear();
  return false;
}

bool GetJNIFieldString(JNIEnv* env,
                       jclass cls,
                       jobject obj,
                       const char* field_name,
                       CefString* value) {
  jobject fieldobj = nullptr;
  if (GetJNIFieldObject(env, cls, obj, field_name, &fieldobj,
                        "Ljava/lang/String;")) {
    ScopedJNIStringResult str(env, (jstring)fieldobj);
    *value = str.GetCefString();
    return true;
  }
  return false;
}

bool GetJNIFieldDate(JNIEnv* env,
                     jclass cls,
                     jobject obj,
                     const char* field_name,
                     CefBaseTime* value) {
  jobject fieldobj = nullptr;
  if (GetJNIFieldObject(env, cls, obj, field_name, &fieldobj,
                        "Ljava/util/Date;")) {
    ScopedJNIObjectLocal jdate(env, fieldobj);
    long timestamp = 0;
    JNI_CALL_METHOD(env, jdate, "getTime", "()J", Long, timestamp);
    CefTime cef_time;
    cef_time.SetDoubleT((double)(timestamp / 1000));
    cef_time_to_basetime(&cef_time, value);
    return true;
  }
  return false;
}

bool GetJNIFieldBoolean(JNIEnv* env,
                        jclass cls,
                        jobject obj,
                        const char* field_name,
                        int* value) {
  jfieldID field = env->GetFieldID(cls, field_name, "Z");
  if (field) {
    *value = env->GetBooleanField(obj, field) != JNI_FALSE ? 1 : 0;
    return true;
  }
  env->ExceptionClear();
  return false;
}

bool GetJNIFieldDouble(JNIEnv* env,
                       jclass cls,
                       jobject obj,
                       const char* field_name,
                       double* value) {
  jfieldID field = env->GetFieldID(cls, field_name, "D");
  if (field) {
    *value = env->GetDoubleField(obj, field);
    return true;
  }
  env->ExceptionClear();
  return false;
}

bool GetJNIFieldInt(JNIEnv* env,
                    jclass cls,
                    jobject obj,
                    const char* field_name,
                    int* value) {
  jfieldID field = env->GetFieldID(cls, field_name, "I");
  if (field) {
    *value = env->GetIntField(obj, field);
    return true;
  }
  env->ExceptionClear();
  return false;
}

bool GetJNIFieldLong(JNIEnv* env,
                     jclass cls,
                     jobject obj,
                     const char* field_name,
                     jlong* value) {
  jfieldID field = env->GetFieldID(cls, field_name, "J");
  if (field) {
    *value = env->GetLongField(obj, field);
    return true;
  }
  env->ExceptionClear();
  return false;
}

bool SetJNIFieldInt(JNIEnv* env,
                    jclass cls,
                    jobject obj,
                    const char* field_name,
                    int value) {
  jfieldID field = env->GetFieldID(cls, field_name, "I");
  if (field) {
    env->SetIntField(obj, field, value);
    return true;
  }
  env->ExceptionClear();
  return false;
}

bool SetJNIFieldDouble(JNIEnv* env,
                       jclass cls,
                       jobject obj,
                       const char* field_name,
                       double value) {
  jfieldID field = env->GetFieldID(cls, field_name, "D");
  if (field) {
    env->SetDoubleField(obj, field, value);
    return true;
  }
  env->ExceptionClear();
  return false;
}

bool SetJNIFieldBoolean(JNIEnv* env,
                        jclass cls,
                        jobject obj,
                        const char* field_name,
                        int value) {
  jfieldID field = env->GetFieldID(cls, field_name, "Z");
  if (field) {
    env->SetBooleanField(obj, field, value == 0 ? 0 : 1);
    return true;
  }
  env->ExceptionClear();
  return false;
}

bool GetJNIFieldStaticInt(JNIEnv* env,
                          jclass cls,
                          const char* field_name,
                          int* value) {
  jfieldID field = env->GetStaticFieldID(cls, field_name, "I");
  if (field) {
    *value = env->GetStaticIntField(cls, field);
    return true;
  }
  env->ExceptionClear();
  return false;
}

bool CallJNIMethodI_V(JNIEnv* env,
                      jclass cls,
                      jobject obj,
                      const char* method_name,
                      int* value) {
  jmethodID methodID = env->GetMethodID(cls, method_name, "()I");
  if (methodID) {
    *value = env->CallIntMethod(obj, methodID);
    return true;
  }
  env->ExceptionClear();
  return false;
}

bool CallJNIMethodC_V(JNIEnv* env,
                      jclass cls,
                      jobject obj,
                      const char* method_name,
                      char16_t* value) {
  jmethodID methodID = env->GetMethodID(cls, method_name, "()C");
  if (methodID) {
    *value = env->CallCharMethod(obj, methodID);
    return true;
  }
  env->ExceptionClear();
  return false;
}

bool CallJNIMethodD_V(JNIEnv* env,
                      jclass cls,
                      jobject obj,
                      const char* method_name,
                      double* value) {
  jmethodID methodID = env->GetMethodID(cls, method_name, "()D");
  if (methodID) {
    *value = env->CallDoubleMethod(obj, methodID);
    return true;
  }
  env->ExceptionClear();
  return false;
}

CefSize GetJNISize(JNIEnv* env, jobject obj) {
  CefSize size;

  ScopedJNIClass cls(env, "java/awt/Dimension");
  if (!cls)
    return size;

  int width, height;
  if (GetJNIFieldInt(env, cls, obj, "width", &width) &&
      GetJNIFieldInt(env, cls, obj, "height", &height)) {
    size.Set(width, height);
  }
  return size;
}

CefRect GetJNIRect(JNIEnv* env, jobject obj) {
  CefRect rect;

  ScopedJNIClass cls(env, "java/awt/Rectangle");
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

bool GetJNIPoint(JNIEnv* env, jobject obj, int* x, int* y) {
  ScopedJNIClass cls(env, "java/awt/Point");
  if (!cls)
    return false;

  if (GetJNIFieldInt(env, cls, obj, "x", x) &&
      GetJNIFieldInt(env, cls, obj, "y", y)) {
    return true;
  }

  return false;
}

CefRefPtr<CefBrowser> GetJNIBrowser(JNIEnv* env, jobject jbrowser) {
  return GetCefFromJNIObject<CefBrowser>(env, jbrowser, "CefBrowser");
}

jobject GetJNIEnumValue(JNIEnv* env,
                        const char* class_name,
                        const char* enum_valname) {
  ScopedJNIClass cls(env, class_name);
  if (!cls)
    return nullptr;

  std::string tmp;
  tmp.append("L").append(class_name).append(";");

  jfieldID fieldId = env->GetStaticFieldID(cls, enum_valname, tmp.c_str());
  if (!fieldId)
    return nullptr;

  return env->GetStaticObjectField(cls, fieldId);
}

bool IsJNIEnumValue(JNIEnv* env,
                    jobject jenum,
                    const char* class_name,
                    const char* enum_valname) {
  if (!jenum)
    return false;

  ScopedJNIObjectLocal compareTo(
      env, GetJNIEnumValue(env, class_name, enum_valname));
  if (compareTo) {
    jboolean isEqual = JNI_FALSE;
    JNI_CALL_METHOD(env, jenum, "equals", "(Ljava/lang/Object;)Z", Boolean,
                    isEqual, compareTo.get());
    return (isEqual != JNI_FALSE);
  }
  return false;
}
