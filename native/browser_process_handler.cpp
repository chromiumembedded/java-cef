// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "browser_process_handler.h"

#include "client_handler.h"
#include "context.h"
#include "jni_scoped_helpers.h"
#include "jni_util.h"
#include "print_handler.h"
#include "util.h"

#include "include/cef_base.h"

// instantiate static values
std::set<CefMessageRouterConfig, cmpCfg> BrowserProcessHandler::router_cfg_;
base::Lock BrowserProcessHandler::router_cfg_lock_;

BrowserProcessHandler::BrowserProcessHandler(JNIEnv* env, jobject app_handler)
    : handle_(env, app_handler) {}

BrowserProcessHandler::~BrowserProcessHandler() {
  base::AutoLock lock_scope(router_cfg_lock_);
  router_cfg_.clear();
}

void BrowserProcessHandler::OnContextInitialized() {
  Context::GetInstance()->OnContextInitialized();

  if (!handle_)
    return;

  ScopedJNIEnv env;
  if (!env)
    return;

  JNI_CALL_VOID_METHOD(env, handle_, "onContextInitialized", "()V");
}

void BrowserProcessHandler::OnRenderProcessThreadCreated(
    CefRefPtr<CefListValue> extra_info) {
  int idx = 0;
  static std::set<CefMessageRouterConfig, cmpCfg>::iterator iter;

  // Delegate creation of the renderer-side router for query handling.
  base::AutoLock lock_scope(router_cfg_lock_);
  for (iter = router_cfg_.begin(); iter != router_cfg_.end(); ++iter) {
    CefRefPtr<CefDictionaryValue> dict = CefDictionaryValue::Create();
    dict->SetString("js_query_function", iter->js_query_function);
    dict->SetString("js_cancel_function", iter->js_cancel_function);
    extra_info->SetDictionary(idx, dict);
    idx++;
  }
}

CefRefPtr<CefPrintHandler> BrowserProcessHandler::GetPrintHandler() {
  CefRefPtr<CefPrintHandler> result;
  ScopedJNIEnv env;
  if (!env)
    return nullptr;

  ScopedJNIObjectResult jresult(env);
  JNI_CALL_METHOD(env, handle_, "getPrintHandler",
                  "()Lorg/cef/handler/CefPrintHandler;", Object, jresult);

  if (jresult) {
    ScopedJNIObject<PrintHandler> jprintHandler(
        env, jresult.Release(), true /* should_delete */, "CefPrintHandler");
    result = jprintHandler.GetOrCreateCefObject();
  }

  return result;
}

void BrowserProcessHandler::OnScheduleMessagePumpWork(int64 delay_ms) {
  if (!handle_)
    return;

  ScopedJNIEnv env;
  if (!env)
    return;

  JNI_CALL_VOID_METHOD(env, handle_, "onScheduleMessagePumpWork", "(J)V",
                       delay_ms);
}

void BrowserProcessHandler::AddMessageRouterConfig(
    const CefMessageRouterConfig& cfg) {
  base::AutoLock lock_scope(router_cfg_lock_);
  router_cfg_.insert(cfg);
}

void BrowserProcessHandler::RemoveMessageRouterConfig(
    const CefMessageRouterConfig& cfg) {
  base::AutoLock lock_scope(router_cfg_lock_);
  router_cfg_.erase(cfg);
}
