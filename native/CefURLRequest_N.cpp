// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "CefURLRequest_N.h"

#include "include/cef_request.h"
#include "include/cef_task.h"
#include "include/cef_urlrequest.h"

#include "critical_wait.h"
#include "jni_scoped_helpers.h"
#include "jni_util.h"
#include "url_request_client.h"

namespace {
class URLRequest : public CefTask {
 public:
  URLRequest(CefThreadId threadId,
             CefRefPtr<CefRequest> request,
             CefRefPtr<URLRequestClient> client)
      : threadId_(threadId),
        request_(request),
        client_(client),
        waitCond_(&lock_) {}

  bool Create() {
    if (!urlRequest_)
      Dispatch(REQ_CREATE);
    return (urlRequest_.get() != nullptr);
  }

  CefURLRequest::Status GetRequestStatus() {
    if (!urlRequest_)
      return UR_UNKNOWN;
    Dispatch(REQ_STATUS);
    return status_;
  }

  CefURLRequest::ErrorCode GetRequestError() {
    if (!urlRequest_)
      return ERR_FAILED;
    Dispatch(REQ_ERROR);
    return error_;
  }

  CefRefPtr<CefResponse> GetResponse() {
    if (!urlRequest_)
      return nullptr;
    Dispatch(REQ_RESPONSE);
    return response_;
  }

  void Cancel() {
    if (!urlRequest_)
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

  virtual void Execute() override {
    lock_.Lock();
    switch (mode_) {
      case REQ_CREATE:
        // TODO(JCEF): Add the ability to specify a CefRequestContext.
        urlRequest_ = CefURLRequest::Create(request_, client_.get(), nullptr);
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

const char kCefClassName[] = "CefURLRequest";

CefRefPtr<URLRequest> GetSelf(jlong self) {
  return reinterpret_cast<URLRequest*>(self);
}

}  // namespace

JNIEXPORT void JNICALL
Java_org_cef_network_CefURLRequest_1N_N_1Create(JNIEnv* env,
                                                jobject obj,
                                                jobject jrequest,
                                                jobject jRequestClient) {
  ScopedJNIRequest requestObj(env);
  requestObj.SetHandle(jrequest, false /* should_delete */);
  CefRefPtr<CefRequest> request = requestObj.GetCefObject();
  if (!request)
    return;

  CefRefPtr<URLRequestClient> client =
      URLRequestClient::Create(env, jRequestClient, obj);

  CefRefPtr<URLRequest> urlRequest = new URLRequest(TID_UI, request, client);
  if (!urlRequest->Create())
    return;
  SetCefForJNIObject(env, obj, urlRequest.get(), kCefClassName);
}

JNIEXPORT void JNICALL
Java_org_cef_network_CefURLRequest_1N_N_1Dispose(JNIEnv* env,
                                                 jobject obj,
                                                 jlong self) {
  SetCefForJNIObject<URLRequest>(env, obj, nullptr, kCefClassName);
}

JNIEXPORT jobject JNICALL
Java_org_cef_network_CefURLRequest_1N_N_1GetRequestStatus(JNIEnv* env,
                                                          jobject obj,
                                                          jlong self) {
  CefRefPtr<URLRequest> urlRequest = GetSelf(self);
  if (!urlRequest)
    return nullptr;

  ScopedJNIURLRequestStatus status(env, urlRequest->GetRequestStatus());
  return status.Release();
}

JNIEXPORT jobject JNICALL
Java_org_cef_network_CefURLRequest_1N_N_1GetRequestError(JNIEnv* env,
                                                         jobject obj,
                                                         jlong self) {
  CefRefPtr<URLRequest> urlRequest = GetSelf(self);
  cef_errorcode_t err = ERR_FAILED;
  if (urlRequest)
    err = urlRequest->GetRequestError();
  return NewJNIErrorCode(env, err);
}

JNIEXPORT jobject JNICALL
Java_org_cef_network_CefURLRequest_1N_N_1GetResponse(JNIEnv* env,
                                                     jobject obj,
                                                     jlong self) {
  CefRefPtr<URLRequest> urlRequest = GetSelf(self);
  if (!urlRequest)
    return nullptr;

  CefRefPtr<CefResponse> response = urlRequest->GetResponse();
  if (!response)
    return nullptr;

  ScopedJNIResponse jresponse(env, response);
  return jresponse.Release();
}

JNIEXPORT void JNICALL
Java_org_cef_network_CefURLRequest_1N_N_1Cancel(JNIEnv* env,
                                                jobject obj,
                                                jlong self) {
  CefRefPtr<URLRequest> urlRequest = GetSelf(self);
  if (!urlRequest)
    return;
  urlRequest->Cancel();
}
