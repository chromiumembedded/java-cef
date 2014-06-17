// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "browser_process_handler.h"
#include "client_handler.h"

#include "jni_util.h"
#include "util.h"

#include "include/cef_base.h"

// instantiate static values
std::set<CefMessageRouterConfig, cmpCfg> BrowserProcessHandler::router_cfg_;
CefCriticalSection BrowserProcessHandler::router_cfg_lock_;

BrowserProcessHandler::BrowserProcessHandler(jobject app_handler) {
  app_handler_ = app_handler;
}

BrowserProcessHandler::~BrowserProcessHandler() {
  router_cfg_lock_.Lock();
  router_cfg_.clear();
  router_cfg_lock_.Unlock();
}

void BrowserProcessHandler::OnContextInitialized() {
  if (!app_handler_)
    return;

  BEGIN_ENV(env)
  JNI_CALL_VOID_METHOD(env, app_handler_, "onContextInitialized", "()V");
  END_ENV(env)
}

void BrowserProcessHandler::OnRenderProcessThreadCreated(
    CefRefPtr<CefListValue> extra_info) {
  int idx=0;
  static std::set<CefMessageRouterConfig, cmpCfg>::iterator iter;

  // Delegate creation of the renderer-side router for query handling.
  router_cfg_lock_.Lock();
  for (iter = router_cfg_.begin(); iter != router_cfg_.end(); ++iter) {
    CefRefPtr<CefDictionaryValue> dict = CefDictionaryValue::Create();
    dict->SetString("js_query_function", iter->js_query_function);
    dict->SetString("js_cancel_function", iter->js_cancel_function);
    extra_info->SetDictionary(idx, dict);
    idx++;
  }
  router_cfg_lock_.Unlock();
}

void BrowserProcessHandler::AddMessageRouterConfig(
    const CefMessageRouterConfig& cfg) {
  router_cfg_lock_.Lock();
  router_cfg_.insert(cfg);
  router_cfg_lock_.Unlock();
}

void BrowserProcessHandler::RemoveMessageRouterConfig(
    const CefMessageRouterConfig& cfg) {
  router_cfg_lock_.Lock();
  router_cfg_.erase(cfg);
  router_cfg_lock_.Unlock();
}
