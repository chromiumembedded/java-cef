// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.


#include "CefURLRequest_N.h"
#include "critical_wait.h"
#include "include/cef_task.h"
#include "include/cef_request.h"
#include "include/cef_urlrequest.h"
#include "url_request_client.h"
#include "jni_util.h"

namespace {
class URLRequest : public CefTask {
 public:

  explicit URLRequest(CefThreadId threadId,
                      CefRefPtr<CefRequest> request,
                      CefRefPtr<URLRequestClient> client)
      : threadId_(threadId),
        request_(request),
        client_(client),
        waitCond_(&lock_) {
  }

  virtual ~URLRequest() {
  } 

  bool Create() {
    if (!urlRequest_.get())
      Dispatch(REQ_CREATE);
    return (urlRequest_.get() != NULL);
  }

  CefURLRequest::Status GetRequestStatus() {
    if (!urlRequest_.get())
      return UR_UNKNOWN;
    Dispatch(REQ_STATUS);
    return status_;
  }

  CefURLRequest::ErrorCode GetRequestError() {
    if (!urlRequest_.get())
      return ERR_FAILED;
    Dispatch(REQ_ERROR);
    return error_;
  }

  CefRefPtr<CefResponse> GetResponse() {
    if (!urlRequest_.get())
      return NULL;
    Dispatch(REQ_RESPONSE);
    return response_;
  }

  void Cancel() {
    if(!urlRequest_.get())
      return;
    Dispatch(REQ_CANCEL);
  }

 private:
  enum URLRequestMode {
    REQ_CREATE,
    REQ_STATUS,
    REQ_ERROR,
    REQ_RESPONSE,
    REQ_CANCEL,
  };

  CefThreadId threadId_;
  CefRefPtr<CefRequest> request_;
  CefRefPtr<URLRequestClient> client_;

  // sync method calls
  CriticalLock lock_;
  CriticalWait waitCond_;
  URLRequestMode mode_;

  // result values
  CefRefPtr<CefURLRequest> urlRequest_;
  CefURLRequest::Status status_;
  CefURLRequest::ErrorCode error_;
  CefRefPtr<CefResponse> response_;

  void Dispatch(URLRequestMode mode) {
    mode_ = mode;
    if (CefCurrentlyOn(threadId_)) {
      Execute();
    } else {
      lock_.Lock();
      CefPostTask(threadId_, this);
      waitCond_.Wait();
      lock_.Unlock();
    }
  }

  virtual void Execute() OVERRIDE {
    lock_.Lock();
    switch (mode_) {
      case REQ_CREATE:
        // TODO(JCEF): Add the ability to specify a CefRequestContext.
        urlRequest_ =  CefURLRequest::Create(request_, client_.get(), NULL);
        break;
      case REQ_STATUS:
        status_ = urlRequest_->GetRequestStatus();
        break;
      case REQ_ERROR:
        error_ = urlRequest_->GetRequestError();
        break;
      case REQ_RESPONSE:
        response_ = urlRequest_->GetResponse();
        break;
      case REQ_CANCEL:
        urlRequest_->Cancel();
        break;
    }
    waitCond_.WakeUp();
    lock_.Unlock();
  }

  IMPLEMENT_REFCOUNTING(URLRequest);
};


}  // namespace


JNIEXPORT void JNICALL Java_org_cef_network_CefURLRequest_1N_N_1CefURLRequest_1CTOR
  (JNIEnv *env, jobject obj, jobject jrequest, jobject jRequestClient) {
  CefRefPtr<URLRequestClient> client = URLRequestClient::Create(env, jRequestClient, obj);
  CefRefPtr<CefRequest> request = GetCefFromJNIObject<CefRequest>(env, jrequest, "CefRequest");

  CefRefPtr<URLRequest> urlRequest = new URLRequest(TID_UI, request, client);
  if (!urlRequest->Create())
    return;
  SetCefForJNIObject(env, obj, urlRequest.get(), "CefURLRequest");
}

JNIEXPORT jobject JNICALL Java_org_cef_network_CefURLRequest_1N_N_1GetRequestStatus
  (JNIEnv *env, jobject obj) {
  jobject result = GetJNIEnumValue(env,
                                   "org/cef/network/CefURLRequest$Status",
                                   "UR_UNKNOWN");
  CefRefPtr<URLRequest> urlRequest =
      GetCefFromJNIObject<URLRequest>(env, obj, "CefURLRequest");
  if (!urlRequest.get())
    return result;

  switch (urlRequest->GetRequestStatus()) {
    default:
    JNI_CASE(env, "org/cef/network/CefURLRequest$Status", UR_UNKNOWN, result);
    JNI_CASE(env, "org/cef/network/CefURLRequest$Status", UR_SUCCESS, result);
    JNI_CASE(env, "org/cef/network/CefURLRequest$Status", UR_IO_PENDING, result);
    JNI_CASE(env, "org/cef/network/CefURLRequest$Status", UR_CANCELED, result);
    JNI_CASE(env, "org/cef/network/CefURLRequest$Status", UR_FAILED, result);
  }
  return result;
}

JNIEXPORT jobject JNICALL Java_org_cef_network_CefURLRequest_1N_N_1GetRequestError
  (JNIEnv *env, jobject obj) {
  CefRefPtr<URLRequest> urlRequest =
      GetCefFromJNIObject<URLRequest>(env, obj, "CefURLRequest");
  if (!urlRequest.get())
    return NewJNIErrorCode(env, ERR_FAILED);
  return NewJNIErrorCode(env, urlRequest->GetRequestError());
}

JNIEXPORT jobject JNICALL Java_org_cef_network_CefURLRequest_1N_N_1GetResponse
  (JNIEnv *env, jobject obj) {
  CefRefPtr<URLRequest> urlRequest =
      GetCefFromJNIObject<URLRequest>(env, obj, "CefURLRequest");
  if (!urlRequest.get())
    return NULL;

  CefRefPtr<CefResponse> response = urlRequest->GetResponse();
  if (!response.get())
    return NULL;

  jobject jresponse = NewJNIObject(env, "org/cef/network/CefResponse_N");
  SetCefForJNIObject<CefResponse>(env, jresponse, response, "CefResponse");
  return jresponse;
}

JNIEXPORT void JNICALL Java_org_cef_network_CefURLRequest_1N_N_1Cancel
  (JNIEnv *env, jobject obj) {
  CefRefPtr<URLRequest> urlRequest =
      GetCefFromJNIObject<URLRequest>(env, obj, "CefURLRequest");
  if (!urlRequest.get())
    return;
  urlRequest->Cancel();
}

JNIEXPORT void JNICALL Java_org_cef_network_CefURLRequest_1N_N_1CefURLRequest_1DTOR
  (JNIEnv *env, jobject obj) {
  SetCefForJNIObject<URLRequest>(env, obj, NULL, "CefURLRequest");
}
