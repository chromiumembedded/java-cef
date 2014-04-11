// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "load_handler.h"
#include "client_handler.h"

#include "jni_util.h"
#include "util.h"

LoadHandler::LoadHandler(JNIEnv* env, jobject handler) {
  jhandler_ = env->NewGlobalRef(handler);
}

LoadHandler::~LoadHandler() {
  JNIEnv* env = GetJNIEnv();
  env->DeleteGlobalRef(jhandler_);
}

void LoadHandler::OnLoadingStateChange(CefRefPtr<CefBrowser> browser,
                                       bool isLoading,
                                       bool canGoBack,
                                       bool canGoForward) {
  JNIEnv* env = GetJNIEnv();
  if (!env)
    return;
  JNI_CALL_VOID_METHOD(env, jhandler_,
                       "onLoadingStateChange",
                       "(Lorg/cef/CefBrowser;ZZZ)V",
                       GetJNIBrowser(browser),
                       (isLoading ? JNI_TRUE : JNI_FALSE),
                       (canGoBack ? JNI_TRUE : JNI_FALSE),
                       (canGoForward ? JNI_TRUE : JNI_FALSE));
}

void LoadHandler::OnLoadStart(CefRefPtr<CefBrowser> browser,
                              CefRefPtr<CefFrame> frame) {
  JNIEnv* env = GetJNIEnv();
  if (!env)
    return;
  JNI_CALL_VOID_METHOD(env, jhandler_,
                       "onLoadStart",
                       "(Lorg/cef/CefBrowser;I)V",
                       GetJNIBrowser(browser),
                       frame->GetIdentifier());
}

void LoadHandler::OnLoadEnd(CefRefPtr<CefBrowser> browser,
                            CefRefPtr<CefFrame> frame,
                            int httpStatusCode) {
  JNIEnv* env = GetJNIEnv();
  if (!env)
    return;
  JNI_CALL_VOID_METHOD(env, jhandler_,
                       "onLoadEnd",
                       "(Lorg/cef/CefBrowser;II)V",
                       GetJNIBrowser(browser),
                       frame->GetIdentifier(),
                       httpStatusCode);
}

void LoadHandler::OnLoadError(CefRefPtr<CefBrowser> browser,
                              CefRefPtr<CefFrame> frame,
                              ErrorCode errorCode,
                              const CefString& errorText,
                              const CefString& failedUrl) {
  JNIEnv* env = GetJNIEnv();
  if (!env)
    return;
  jobject jerrorCode = NULL;
  switch (errorCode) {
    case ERR_NONE:
      jerrorCode = GetJNIEnumValue(env,
                                   "org/cef/CefLoadHandler$ErrorCode",
                                   "ERR_NONE");
      break;
    case ERR_FAILED:
      jerrorCode = GetJNIEnumValue(env,
                                   "org/cef/CefLoadHandler$ErrorCode",
                                   "ERR_FAILED");
      break;
    case ERR_ABORTED:
      jerrorCode = GetJNIEnumValue(env,
                                   "org/cef/CefLoadHandler$ErrorCode",
                                   "ERR_ABORTED");
      break;
    case ERR_INVALID_ARGUMENT:
      jerrorCode = GetJNIEnumValue(env,
                                   "org/cef/CefLoadHandler$ErrorCode",
                                   "ERR_INVALID_ARGUMENT");
      break;
    case ERR_INVALID_HANDLE:
      jerrorCode = GetJNIEnumValue(env,
                                   "org/cef/CefLoadHandler$ErrorCode",
                                   "ERR_INVALID_HANDLE");
      break;
    case ERR_FILE_NOT_FOUND:
      jerrorCode = GetJNIEnumValue(env,
                                   "org/cef/CefLoadHandler$ErrorCode",
                                   "ERR_FILE_NOT_FOUND");
      break;
    case ERR_TIMED_OUT:
      jerrorCode = GetJNIEnumValue(env,
                                   "org/cef/CefLoadHandler$ErrorCode",
                                   "ERR_TIMED_OUT");
      break;
    case ERR_FILE_TOO_BIG:
      jerrorCode = GetJNIEnumValue(env,
                                   "org/cef/CefLoadHandler$ErrorCode",
                                   "ERR_FILE_TOO_BIG");
      break;
    case ERR_UNEXPECTED:
      jerrorCode = GetJNIEnumValue(env,
                                   "org/cef/CefLoadHandler$ErrorCode",
                                   "ERR_UNEXPECTED");
      break;
    case ERR_ACCESS_DENIED:
      jerrorCode = GetJNIEnumValue(env,
                                   "org/cef/CefLoadHandler$ErrorCode",
                                   "ERR_ACCESS_DENIED");
      break;
    case ERR_NOT_IMPLEMENTED:
      jerrorCode = GetJNIEnumValue(env,
                                   "org/cef/CefLoadHandler$ErrorCode",
                                   "ERR_NOT_IMPLEMENTED");
      break;
    case ERR_CONNECTION_CLOSED:
      jerrorCode = GetJNIEnumValue(env,
                                   "org/cef/CefLoadHandler$ErrorCode",
                                   "ERR_CONNECTION_CLOSED");
      break;
    case ERR_CONNECTION_RESET:
      jerrorCode = GetJNIEnumValue(env,
                                   "org/cef/CefLoadHandler$ErrorCode",
                                   "ERR_CONNECTION_RESET");
      break;
    case ERR_CONNECTION_REFUSED:
      jerrorCode = GetJNIEnumValue(env,
                                   "org/cef/CefLoadHandler$ErrorCode",
                                   "ERR_CONNECTION_REFUSED");
      break;
    case ERR_CONNECTION_ABORTED:
      jerrorCode = GetJNIEnumValue(env,
                                   "org/cef/CefLoadHandler$ErrorCode",
                                   "ERR_CONNECTION_ABORTED");
      break;
    case ERR_CONNECTION_FAILED:
      jerrorCode = GetJNIEnumValue(env,
                                   "org/cef/CefLoadHandler$ErrorCode",
                                   "ERR_CONNECTION_FAILED");
      break;
    case ERR_NAME_NOT_RESOLVED:
      jerrorCode = GetJNIEnumValue(env,
                                   "org/cef/CefLoadHandler$ErrorCode",
                                   "ERR_NAME_NOT_RESOLVED");
      break;
    case ERR_INTERNET_DISCONNECTED:
      jerrorCode = GetJNIEnumValue(env,
                                   "org/cef/CefLoadHandler$ErrorCode",
                                   "ERR_INTERNET_DISCONNECTED");
      break;
    case ERR_SSL_PROTOCOL_ERROR:
      jerrorCode = GetJNIEnumValue(env,
                                   "org/cef/CefLoadHandler$ErrorCode",
                                   "ERR_SSL_PROTOCOL_ERROR");
      break;
    case ERR_ADDRESS_INVALID:
      jerrorCode = GetJNIEnumValue(env,
                                   "org/cef/CefLoadHandler$ErrorCode",
                                   "ERR_ADDRESS_INVALID");
      break;
    case ERR_ADDRESS_UNREACHABLE:
      jerrorCode = GetJNIEnumValue(env,
                                   "org/cef/CefLoadHandler$ErrorCode",
                                   "ERR_ADDRESS_UNREACHABLE");
      break;
    case ERR_SSL_CLIENT_AUTH_CERT_NEEDED:
      jerrorCode = GetJNIEnumValue(env,
                                   "org/cef/CefLoadHandler$ErrorCode",
                                   "ERR_SSL_CLIENT_AUTH_CERT_NEEDED");
      break;
    case ERR_TUNNEL_CONNECTION_FAILED:
      jerrorCode = GetJNIEnumValue(env,
                                   "org/cef/CefLoadHandler$ErrorCode",
                                   "ERR_TUNNEL_CONNECTION_FAILED");
      break;
    case ERR_NO_SSL_VERSIONS_ENABLED:
      jerrorCode = GetJNIEnumValue(env,
                                   "org/cef/CefLoadHandler$ErrorCode",
                                   "ERR_NO_SSL_VERSIONS_ENABLED");
      break;
    case ERR_SSL_VERSION_OR_CIPHER_MISMATCH:
      jerrorCode = GetJNIEnumValue(env,
                                   "org/cef/CefLoadHandler$ErrorCode",
                                   "ERR_SSL_VERSION_OR_CIPHER_MISMATCH");
      break;
    case ERR_SSL_RENEGOTIATION_REQUESTED:
      jerrorCode = GetJNIEnumValue(env,
                                   "org/cef/CefLoadHandler$ErrorCode",
                                   "ERR_SSL_RENEGOTIATION_REQUESTED");
      break;
    case ERR_CERT_COMMON_NAME_INVALID:
      jerrorCode = GetJNIEnumValue(env,
                                   "org/cef/CefLoadHandler$ErrorCode",
                                   "ERR_CERT_COMMON_NAME_INVALID");
      break;
    case ERR_CERT_DATE_INVALID:
      jerrorCode = GetJNIEnumValue(env,
                                   "org/cef/CefLoadHandler$ErrorCode",
                                   "ERR_CERT_DATE_INVALID");
      break;
    case ERR_CERT_AUTHORITY_INVALID:
      jerrorCode = GetJNIEnumValue(env,
                                   "org/cef/CefLoadHandler$ErrorCode",
                                   "ERR_CERT_AUTHORITY_INVALID");
      break;
    case ERR_CERT_CONTAINS_ERRORS:
      jerrorCode = GetJNIEnumValue(env,
                                   "org/cef/CefLoadHandler$ErrorCode",
                                   "ERR_CERT_CONTAINS_ERRORS");
      break;
    case ERR_CERT_NO_REVOCATION_MECHANISM:
      jerrorCode = GetJNIEnumValue(env,
                                   "org/cef/CefLoadHandler$ErrorCode",
                                   "ERR_CERT_NO_REVOCATION_MECHANISM");
      break;
    case ERR_CERT_UNABLE_TO_CHECK_REVOCATION:
      jerrorCode = GetJNIEnumValue(env,
                                   "org/cef/CefLoadHandler$ErrorCode",
                                   "ERR_CERT_UNABLE_TO_CHECK_REVOCATION");
      break;
    case ERR_CERT_REVOKED:
      jerrorCode = GetJNIEnumValue(env,
                                   "org/cef/CefLoadHandler$ErrorCode",
                                   "ERR_CERT_REVOKED");
      break;
    case ERR_CERT_INVALID:
      jerrorCode = GetJNIEnumValue(env,
                                   "org/cef/CefLoadHandler$ErrorCode",
                                   "ERR_CERT_INVALID");
      break;
    case ERR_CERT_END:
      jerrorCode = GetJNIEnumValue(env,
                                   "org/cef/CefLoadHandler$ErrorCode",
                                   "ERR_CERT_END");
      break;
    case ERR_INVALID_URL:
      jerrorCode = GetJNIEnumValue(env,
                                   "org/cef/CefLoadHandler$ErrorCode",
                                   "ERR_INVALID_URL");
      break;
    case ERR_DISALLOWED_URL_SCHEME:
      jerrorCode = GetJNIEnumValue(env,
                                   "org/cef/CefLoadHandler$ErrorCode",
                                   "ERR_DISALLOWED_URL_SCHEME");
      break;
    case ERR_UNKNOWN_URL_SCHEME:
      jerrorCode = GetJNIEnumValue(env,
                                   "org/cef/CefLoadHandler$ErrorCode",
                                   "ERR_UNKNOWN_URL_SCHEME");
      break;
    case ERR_TOO_MANY_REDIRECTS:
      jerrorCode = GetJNIEnumValue(env,
                                   "org/cef/CefLoadHandler$ErrorCode",
                                   "ERR_TOO_MANY_REDIRECTS");
      break;
    case ERR_UNSAFE_REDIRECT:
      jerrorCode = GetJNIEnumValue(env,
                                   "org/cef/CefLoadHandler$ErrorCode",
                                   "ERR_UNSAFE_REDIRECT");
      break;
    case ERR_UNSAFE_PORT:
      jerrorCode = GetJNIEnumValue(env,
                                   "org/cef/CefLoadHandler$ErrorCode",
                                   "ERR_UNSAFE_PORT");
      break;
    case ERR_INVALID_RESPONSE:
      jerrorCode = GetJNIEnumValue(env,
                                   "org/cef/CefLoadHandler$ErrorCode",
                                   "ERR_INVALID_RESPONSE");
      break;
    case ERR_INVALID_CHUNKED_ENCODING:
      jerrorCode = GetJNIEnumValue(env,
                                   "org/cef/CefLoadHandler$ErrorCode",
                                   "ERR_INVALID_CHUNKED_ENCODING");
      break;
    case ERR_METHOD_NOT_SUPPORTED:
      jerrorCode = GetJNIEnumValue(env,
                                   "org/cef/CefLoadHandler$ErrorCode",
                                   "ERR_METHOD_NOT_SUPPORTED");
      break;
    case ERR_UNEXPECTED_PROXY_AUTH:
      jerrorCode = GetJNIEnumValue(env,
                                   "org/cef/CefLoadHandler$ErrorCode",
                                   "ERR_UNEXPECTED_PROXY_AUTH");
      break;
    case ERR_EMPTY_RESPONSE:
      jerrorCode = GetJNIEnumValue(env,
                                   "org/cef/CefLoadHandler$ErrorCode",
                                   "ERR_EMPTY_RESPONSE");
      break;
    case ERR_RESPONSE_HEADERS_TOO_BIG:
      jerrorCode = GetJNIEnumValue(env,
                                   "org/cef/CefLoadHandler$ErrorCode",
                                   "ERR_RESPONSE_HEADERS_TOO_BIG");
      break;
    case ERR_CACHE_MISS:
      jerrorCode = GetJNIEnumValue(env,
                                   "org/cef/CefLoadHandler$ErrorCode",
                                   "ERR_CACHE_MISS");
      break;
    case ERR_INSECURE_RESPONSE:
      jerrorCode = GetJNIEnumValue(env,
                                   "org/cef/CefLoadHandler$ErrorCode",
                                   "ERR_INSECURE_RESPONSE");
      break;
    default:
      break;
  }

  JNI_CALL_VOID_METHOD(env, jhandler_,
                       "onLoadError",
                       "(Lorg/cef/CefBrowser;ILorg/cef/CefLoadHandler$ErrorCode;Ljava/lang/String;Ljava/lang/String;)V",
                       GetJNIBrowser(browser),
                       frame->GetIdentifier(),
                       jerrorCode,
                       NewJNIString(env, errorText),
                       NewJNIString(env, failedUrl));
}
