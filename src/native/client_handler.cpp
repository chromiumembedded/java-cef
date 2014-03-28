// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "client_handler.h"

#include <stdio.h>
#include <algorithm>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include "display_handler.h"
#include "focus_handler.h"
#include "life_span_handler.h"
#include "message_router_handler.h"
#include "render_handler.h"

#include "include/cef_browser.h"
#include "include/cef_frame.h"
#include "include/cef_path_util.h"
#include "include/cef_process_util.h"
#include "include/cef_runnable.h"
#include "include/cef_trace.h"
#include "include/cef_url.h"
#include "include/wrapper/cef_stream_resource_handler.h"
#include "jni_util.h"
#include "util.h"

ClientHandler::ClientHandler(JNIEnv* env, jobject handler)
  : jbrowser_(NULL) {
  jhandler_ = env->NewGlobalRef(handler);
}

ClientHandler::~ClientHandler() {
}

CefRefPtr<CefDisplayHandler> ClientHandler::GetDisplayHandler() {
  CefRefPtr<CefDisplayHandler> result = NULL;
  BEGIN_ENV(env)
  jobject handler = NULL;
  JNI_CALL_METHOD(env, jhandler_, "getDisplayHandler", "()Lorg/cef/CefDisplayHandler;", Object, handler);
  if (handler) {
    result = GetCefFromJNIObject<CefDisplayHandler>(env, handler, "CefDisplayHandler");
    if (!result.get()) {
      result = new DisplayHandler(env, handler);
      SetCefForJNIObject(env, handler, result.get(), "CefDisplayHandler");
    }
  }
  END_ENV(env)
  return result;
}

CefRefPtr<CefLifeSpanHandler> ClientHandler::GetLifeSpanHandler() {
  CefRefPtr<CefLifeSpanHandler> result = NULL;
  BEGIN_ENV(env)
  jobject handler = NULL;
  JNI_CALL_METHOD(env, jhandler_, "getLifeSpanHandler", "()Lorg/cef/CefLifeSpanHandler;", Object, handler);
  if (handler) {
    result = GetCefFromJNIObject<CefLifeSpanHandler>(env, handler, "CefLifeSpanHandler");
    if (!result.get()) {
      result = new LifeSpanHandler(env, handler);
      SetCefForJNIObject(env, handler, result.get(), "CefLifeSpanHandler");
    }
  }
  END_ENV(env)
  return result;
}

CefRefPtr<CefRenderHandler> ClientHandler::GetRenderHandler() {
  CefRefPtr<CefRenderHandler> result = NULL;
  BEGIN_ENV(env)
  jobject handler = NULL;
  JNI_CALL_METHOD(env, jhandler_, "getRenderHandler", "()Lorg/cef/CefRenderHandler;", Object, handler);
  if (handler) {
    result = GetCefFromJNIObject<CefRenderHandler>(env, handler, "CefRenderHandler");
    if (!result.get()) {
      result = new RenderHandler(env, handler);
      SetCefForJNIObject(env, handler, result.get(), "CefRenderHandler");
    }
  }
  END_ENV(env)
  return result;
}

CefRefPtr<CefFocusHandler> ClientHandler::GetFocusHandler() {
  CefRefPtr<CefFocusHandler> result = NULL;
  BEGIN_ENV(env)
  jobject handler = NULL;
  JNI_CALL_METHOD(env, jhandler_, "getFocusHandler", "()Lorg/cef/CefFocusHandler;", Object, handler);
  if (handler) {
    result = GetCefFromJNIObject<CefFocusHandler>(env, handler, "CefFocusHandler");
    if (!result.get()) {
      result = new FocusHandler(env, handler);
      SetCefForJNIObject(env, handler, result.get(), "CefFocusHandler");
    }
  }
  END_ENV(env)
  return result;
}

CefRefPtr<MessageRouterHandler> ClientHandler::GetMessageRouterHandler() {
  CefRefPtr<MessageRouterHandler> result = NULL;
  BEGIN_ENV(env)
  jobject handler = NULL;
  JNI_CALL_METHOD(env, jhandler_, "getMessageRouterHandler", "()Lorg/cef/CefMessageRouterHandler;", Object, handler);
  if (handler) {
    result = GetCefFromJNIObject<MessageRouterHandler>(env, handler, "CefMessageRouterHandler");
    if (!result.get()) {
      result = new MessageRouterHandler(env, handler);
      SetCefForJNIObject(env, handler, result.get(), "CefMessageRouterHandler");
    }
  }
  END_ENV(env)
  return result;
}

bool ClientHandler::OnProcessMessageReceived(
    CefRefPtr<CefBrowser> browser,
    CefProcessId source_process,
    CefRefPtr<CefProcessMessage> message) {
  if (message_router_)
    return message_router_->OnProcessMessageReceived(
        browser, source_process,  message);
  return false;
}

void ClientHandler::OnBeforeContextMenu(
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    CefRefPtr<CefContextMenuParams> params,
    CefRefPtr<CefMenuModel> model) {
}

bool ClientHandler::OnContextMenuCommand(
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    CefRefPtr<CefContextMenuParams> params,
    int command_id,
    EventFlags event_flags) {
  return false;
}

void ClientHandler::OnBeforeDownload(
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefDownloadItem> download_item,
    const CefString& suggested_name,
    CefRefPtr<CefBeforeDownloadCallback> callback) {
  REQUIRE_UI_THREAD();
}

void ClientHandler::OnDownloadUpdated(
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefDownloadItem> download_item,
    CefRefPtr<CefDownloadItemCallback> callback) {
  REQUIRE_UI_THREAD();
}

bool ClientHandler::OnDragEnter(CefRefPtr<CefBrowser> browser,
                                CefRefPtr<CefDragData> dragData,
                                DragOperationsMask mask) {
  REQUIRE_UI_THREAD();
  return false;
}

void ClientHandler::OnRequestGeolocationPermission(
      CefRefPtr<CefBrowser> browser,
      const CefString& requesting_url,
      int request_id,
      CefRefPtr<CefGeolocationCallback> callback) {
  // Allow geolocation access from all websites.
  callback->Continue(true);
}

bool ClientHandler::OnPreKeyEvent(CefRefPtr<CefBrowser> browser,
                                  const CefKeyEvent& event,
                                  CefEventHandle os_event,
                                  bool* is_keyboard_shortcut) {
  return false;
}

void ClientHandler::SetBrowser(CefRefPtr<CefBrowser> browser) { 
  REQUIRE_UI_THREAD();

  AutoLock lock_scope(this);
  if (!browser_.get())   {
    // We need to keep the main child window, but not popup windows.
    browser_ = browser;
  }

  if (!message_router_) {
    CefRefPtr<MessageRouterHandler> handler;
    handler = GetMessageRouterHandler();
    if (handler.get()) {
      // Create the browser-side router for query handling.
      CefMessageRouterConfig config;
      message_router_ = CefMessageRouterBrowserSide::Create(config);
      message_router_->AddHandler(handler.get(), false);
    }
  }
}

void ClientHandler::RemoveBrowser(CefRefPtr<CefBrowser> browser) {
  REQUIRE_UI_THREAD();

  AutoLock lock_scope(this);
  if (browser_->GetIdentifier() == browser->GetIdentifier()) {
    // Free the browser pointer so that the browser can be destroyed.
    browser_ = NULL;

    JNIEnv* env = GetJNIEnv();
    if(env) {
      env->DeleteGlobalRef(jbrowser_);
      env->DeleteGlobalRef(jhandler_);
      jbrowser_ = jhandler_ = NULL;
    }
  }
  if (message_router_)
    message_router_->OnBeforeClose(browser);
}

void ClientHandler::OnLoadStart(CefRefPtr<CefBrowser> browser,
                                CefRefPtr<CefFrame> frame) {
  REQUIRE_UI_THREAD();
}

void ClientHandler::OnLoadEnd(CefRefPtr<CefBrowser> browser,
                              CefRefPtr<CefFrame> frame,
                              int httpStatusCode) {
  REQUIRE_UI_THREAD();
}

void ClientHandler::OnLoadError(CefRefPtr<CefBrowser> browser,
                                CefRefPtr<CefFrame> frame,
                                ErrorCode errorCode,
                                const CefString& errorText,
                                const CefString& failedUrl) {
  REQUIRE_UI_THREAD();

  // Don't display an error for downloaded files.
  if (errorCode == ERR_ABORTED)
    return;

  // Display a load error message.
  std::stringstream ss;
  ss << "<html><body><h2>Failed to load URL " << std::string(failedUrl) <<
        " with error " << std::string(errorText) << " (" << errorCode <<
        ").</h2></body></html>";
  frame->LoadString(ss.str(), failedUrl);
}

void ClientHandler::OnRenderProcessTerminated(CefRefPtr<CefBrowser> browser,
                                              TerminationStatus status) {
  if (message_router_)
    message_router_->OnRenderProcessTerminated(browser);
}

bool ClientHandler::OnBeforeBrowse(CefRefPtr<CefBrowser> browser,
                                   CefRefPtr<CefFrame> frame,
                                   CefRefPtr<CefRequest> request,
                                   bool is_redirect) {
  if (message_router_)
    message_router_->OnBeforeBrowse(browser, frame);
  return false;
}

CefRefPtr<CefResourceHandler> ClientHandler::GetResourceHandler(
      CefRefPtr<CefBrowser> browser,
      CefRefPtr<CefFrame> frame,
      CefRefPtr<CefRequest> request) {
  return NULL;
}

bool ClientHandler::OnQuotaRequest(CefRefPtr<CefBrowser> browser,
                                   const CefString& origin_url,
                                   int64 new_size,
                                   CefRefPtr<CefQuotaCallback> callback) {
  return false;
}

void ClientHandler::OnProtocolExecution(CefRefPtr<CefBrowser> browser,
                                        const CefString& url,
                                        bool& allow_os_execution) {
}

void ClientHandler::SetJBrowser(jobject jbrowser) { 
  JNIEnv* env = GetJNIEnv();
  jbrowser_ = env->NewGlobalRef(jbrowser);
}
