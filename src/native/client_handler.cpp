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
#include "load_handler.h"
#include "message_router_handler.h"
#include "render_handler.h"
#include "dialog_handler.h"
#include "download_handler.h"
#include "context_menu_handler.h"
#include "drag_handler.h"
#include "geolocation_handler.h"
#include "jsdialog_handler.h"

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

ClientHandler::ClientHandler(JNIEnv* env, jobject handler) {
  jhandler_ = env->NewGlobalRef(handler);
}

ClientHandler::~ClientHandler() {
  BEGIN_ENV(env)
  env->DeleteGlobalRef(jhandler_);
  END_ENV(env)
}
CefRefPtr<CefContextMenuHandler> ClientHandler::GetContextMenuHandler() {
  CefRefPtr<CefContextMenuHandler> result = NULL;
  BEGIN_ENV(env)
  jobject handler = NULL;
  JNI_CALL_METHOD(env, jhandler_, "getContextMenuHandler", "()Lorg/cef/handler/CefContextMenuHandler;", Object, handler);
  if (handler) {
    result = GetCefFromJNIObject<CefContextMenuHandler>(env, handler, "CefContextMenuHandler");
    if (!result.get()) {
      result = new ContextMenuHandler(env, handler);
      SetCefForJNIObject(env, handler, result.get(), "CefContextMenuHandler");
    }
  }
  END_ENV(env)
  return result;
}

CefRefPtr<CefDialogHandler> ClientHandler::GetDialogHandler() {
  CefRefPtr<CefDialogHandler> result = NULL;
  BEGIN_ENV(env)
  jobject handler = NULL;
  JNI_CALL_METHOD(env, jhandler_, "getDialogHandler", "()Lorg/cef/handler/CefDialogHandler;", Object, handler);
  if (handler) {
    result = GetCefFromJNIObject<CefDialogHandler>(env, handler, "CefDialogHandler");
    if (!result.get()) {
      result = new DialogHandler(env, handler);
      SetCefForJNIObject(env, handler, result.get(), "CefDialogHandler");
    }
  }
  END_ENV(env)
  return result;
}

CefRefPtr<CefDisplayHandler> ClientHandler::GetDisplayHandler() {
  CefRefPtr<CefDisplayHandler> result = NULL;
  BEGIN_ENV(env)
  jobject handler = NULL;
  JNI_CALL_METHOD(env, jhandler_, "getDisplayHandler", "()Lorg/cef/handler/CefDisplayHandler;", Object, handler);
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

CefRefPtr<CefDownloadHandler> ClientHandler::GetDownloadHandler() {
  CefRefPtr<CefDownloadHandler> result = NULL;
  BEGIN_ENV(env)
  jobject handler = NULL;
  JNI_CALL_METHOD(env, jhandler_, "getDownloadHandler", "()Lorg/cef/handler/CefDownloadHandler;", Object, handler);
  if (handler) {
    result = GetCefFromJNIObject<CefDownloadHandler>(env, handler, "CefDownloadHandler");
    if (!result.get()) {
      result = new DownloadHandler(env, handler);
      SetCefForJNIObject(env, handler, result.get(), "CefDownloadHandler");
    }
  }
  END_ENV(env)
  return result;
}

CefRefPtr<CefGeolocationHandler> ClientHandler::GetGeolocationHandler() {
  CefRefPtr<CefGeolocationHandler> result = NULL;
  BEGIN_ENV(env)
  jobject handler = NULL;
  JNI_CALL_METHOD(env, jhandler_, "getGeolocationHandler", "()Lorg/cef/handler/CefGeolocationHandler;", Object, handler);
  if (handler) {
    result = GetCefFromJNIObject<CefGeolocationHandler>(env, handler, "CefGeolocationHandler");
    if (!result.get()) {
      result = new GeolocationHandler(env, handler);
      SetCefForJNIObject(env, handler, result.get(), "CefGeolocationHandler");
    }
  }
  END_ENV(env)
  return result;
}

CefRefPtr<CefJSDialogHandler> ClientHandler::GetJSDialogHandler() {
  CefRefPtr<CefJSDialogHandler> result = NULL;
  BEGIN_ENV(env)
  jobject handler = NULL;
  JNI_CALL_METHOD(env, jhandler_, "getJSDialogHandler", "()Lorg/cef/handler/CefJSDialogHandler;", Object, handler);
  if (handler) {
    result = GetCefFromJNIObject<CefJSDialogHandler>(env, handler, "CefJSDialogHandler");
    if (!result.get()) {
      result = new JSDialogHandler(env, handler);
      SetCefForJNIObject(env, handler, result.get(), "CefJSDialogHandler");
    }
  }
  END_ENV(env)
  return result;
}

CefRefPtr<CefDragHandler> ClientHandler::GetDragHandler() {
  CefRefPtr<CefDragHandler> result = NULL;
  BEGIN_ENV(env)
  jobject handler = NULL;
  JNI_CALL_METHOD(env, jhandler_, "getDragHandler", "()Lorg/cef/handler/CefDragHandler;", Object, handler);
  if (handler) {
    result = GetCefFromJNIObject<CefDragHandler>(env, handler, "CefDragHandler");
    if (!result.get()) {
      result = new DragHandler(env, handler);
      SetCefForJNIObject(env, handler, result.get(), "CefDragHandler");
    }
  }
  END_ENV(env)
  return result;
}

CefRefPtr<CefLifeSpanHandler> ClientHandler::GetLifeSpanHandler() {
  CefRefPtr<CefLifeSpanHandler> result = NULL;
  BEGIN_ENV(env)
  jobject handler = NULL;
  JNI_CALL_METHOD(env, jhandler_, "getLifeSpanHandler", "()Lorg/cef/handler/CefLifeSpanHandler;", Object, handler);
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

CefRefPtr<CefLoadHandler> ClientHandler::GetLoadHandler() {
  CefRefPtr<CefLoadHandler> result = NULL;
  BEGIN_ENV(env)
  jobject handler = NULL;
  JNI_CALL_METHOD(env, jhandler_, "getLoadHandler", "()Lorg/cef/handler/CefLoadHandler;", Object, handler);
  if (handler) {
    result = GetCefFromJNIObject<CefLoadHandler>(env, handler, "CefLoadHandler");
    if (!result.get()) {
      result = new LoadHandler(env, handler);
      SetCefForJNIObject(env, handler, result.get(), "CefLoadHandler");
    }
  }
  END_ENV(env)
  return result;
}

CefRefPtr<CefRenderHandler> ClientHandler::GetRenderHandler() {
  CefRefPtr<CefRenderHandler> result = NULL;
  BEGIN_ENV(env)
  jobject handler = NULL;
  JNI_CALL_METHOD(env, jhandler_, "getRenderHandler", "()Lorg/cef/handler/CefRenderHandler;", Object, handler);
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
  JNI_CALL_METHOD(env, jhandler_, "getFocusHandler", "()Lorg/cef/handler/CefFocusHandler;", Object, handler);
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
  JNI_CALL_METHOD(env, jhandler_, "getMessageRouterHandler", "()Lorg/cef/handler/CefMessageRouterHandler;", Object, handler);
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

bool ClientHandler::OnPreKeyEvent(CefRefPtr<CefBrowser> browser,
                                  const CefKeyEvent& event,
                                  CefEventHandle os_event,
                                  bool* is_keyboard_shortcut) {
  return false;
}

void ClientHandler::OnAfterCreated() {
  REQUIRE_UI_THREAD();

  AutoLock lock_scope(this);
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

void ClientHandler::OnBeforeClose(CefRefPtr<CefBrowser> browser) {
  REQUIRE_UI_THREAD();

  AutoLock lock_scope(this);
  if (message_router_)
    message_router_->OnBeforeClose(browser);
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

void ClientHandler::OnRenderProcessTerminated(CefRefPtr<CefBrowser> browser,
                                              TerminationStatus status) {
  if (message_router_)
    message_router_->OnRenderProcessTerminated(browser);
}

jobject ClientHandler::getBrowser(CefRefPtr<CefBrowser> browser) {
  jobject jbrowser = NULL;
  BEGIN_ENV(env)
  JNI_CALL_METHOD(env, 
                  jhandler_, 
                  "getBrowser", 
                  "(I)Lorg/cef/browser/CefBrowser;", 
                  Object, 
                  jbrowser, 
                  browser->GetIdentifier());
  END_ENV(env)
  return jbrowser;
}
