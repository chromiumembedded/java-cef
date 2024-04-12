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

void BrowserProcessHandler::OnScheduleMessagePumpWork(int64_t delay_ms) {
  if (!handle_)
    return;

  ScopedJNIEnv env;
  if (!env)
    return;

  JNI_CALL_VOID_METHOD(env, handle_, "onScheduleMessagePumpWork", "(J)V",
                       delay_ms);
}

bool BrowserProcessHandler::OnAlreadyRunningAppRelaunch(
    CefRefPtr<CefCommandLine> command_line,
    const CefString& current_directory) {
  if (!handle_)
    return false;

  ScopedJNIEnv env;
  if (!env)
    return false;

  ScopedJNIObject<CefCommandLine> jcommandLine(
      env, command_line, "org/cef/callback/CefCommandLine_N", "CefCommandLine");
  jcommandLine.SetTemporary();
  ScopedJNIString jcurrentDirectory(env, current_directory);

  jboolean jresult = 0;

  JNI_CALL_BOOLEAN_METHOD(
      jresult, env, handle_, "onAlreadyRunningAppRelaunch",
      "(Lorg/cef/callback/CefCommandLine;Ljava/lang/String;)Z",
      jcommandLine.get(), jcurrentDirectory.get());

  return jresult;
}

// static
CefRefPtr<CefListValue> BrowserProcessHandler::GetMessageRouterConfigs() {
  int idx = 0;
  static std::set<CefMessageRouterConfig, cmpCfg>::iterator iter;

  base::AutoLock lock_scope(router_cfg_lock_);
  if (router_cfg_.empty())
    return nullptr;

  // Configuration pased to CefHelperApp::OnBrowserCreated.
  auto router_configs = CefListValue::Create();
  for (iter = router_cfg_.begin(); iter != router_cfg_.end(); ++iter) {
    CefRefPtr<CefDictionaryValue> dict = CefDictionaryValue::Create();
    dict->SetString("js_query_function", iter->js_query_function);
    dict->SetString("js_cancel_function", iter->js_cancel_function);
    router_configs->SetDictionary(idx, dict);
    idx++;
  }

  return router_configs;
}

// static
void BrowserProcessHandler::AddMessageRouterConfig(
    const CefMessageRouterConfig& cfg) {
  base::AutoLock lock_scope(router_cfg_lock_);
  router_cfg_.insert(cfg);
}

// static
void BrowserProcessHandler::RemoveMessageRouterConfig(
    const CefMessageRouterConfig& cfg) {
  base::AutoLock lock_scope(router_cfg_lock_);
  router_cfg_.erase(cfg);
}
