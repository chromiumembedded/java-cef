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

#include "browser_process_handler.h"
#include "context_menu_handler.h"
#include "dialog_handler.h"
#include "display_handler.h"
#include "download_handler.h"
#include "drag_handler.h"
#include "focus_handler.h"
#include "jsdialog_handler.h"
#include "keyboard_handler.h"
#include "life_span_handler.h"
#include "load_handler.h"
#include "message_router_handler.h"
#include "render_handler.h"
#include "request_handler.h"

#include "include/cef_browser.h"
#include "include/cef_frame.h"
#include "include/cef_parser.h"
#include "include/cef_path_util.h"
#include "include/cef_process_util.h"
#include "include/cef_trace.h"
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
  JNI_CALL_METHOD(env, jhandler_, "getContextMenuHandler",
                  "()Lorg/cef/handler/CefContextMenuHandler;", Object, handler);
  if (handler) {
    result = GetCefFromJNIObject<CefContextMenuHandler>(
        env, handler, "CefContextMenuHandler");
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
  JNI_CALL_METHOD(env, jhandler_, "getDialogHandler",
                  "()Lorg/cef/handler/CefDialogHandler;", Object, handler);
  if (handler) {
    result =
        GetCefFromJNIObject<CefDialogHandler>(env, handler, "CefDialogHandler");
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
  JNI_CALL_METHOD(env, jhandler_, "getDisplayHandler",
                  "()Lorg/cef/handler/CefDisplayHandler;", Object, handler);
  if (handler) {
    result = GetCefFromJNIObject<CefDisplayHandler>(env, handler,
                                                    "CefDisplayHandler");
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
  JNI_CALL_METHOD(env, jhandler_, "getDownloadHandler",
                  "()Lorg/cef/handler/CefDownloadHandler;", Object, handler);
  if (handler) {
    result = GetCefFromJNIObject<CefDownloadHandler>(env, handler,
                                                     "CefDownloadHandler");
    if (!result.get()) {
      result = new DownloadHandler(env, handler);
      SetCefForJNIObject(env, handler, result.get(), "CefDownloadHandler");
    }
  }
  END_ENV(env)
  return result;
}

CefRefPtr<CefDragHandler> ClientHandler::GetDragHandler() {
  CefRefPtr<CefDragHandler> result = NULL;
  BEGIN_ENV(env)
  jobject handler = NULL;
  JNI_CALL_METHOD(env, jhandler_, "getDragHandler",
                  "()Lorg/cef/handler/CefDragHandler;", Object, handler);
  if (handler) {
    result =
        GetCefFromJNIObject<CefDragHandler>(env, handler, "CefDragHandler");
    if (!result.get()) {
      result = new DragHandler(env, handler);
      SetCefForJNIObject(env, handler, result.get(), "CefDragHandler");
    }
  }
  END_ENV(env)
  return result;
}

CefRefPtr<CefFocusHandler> ClientHandler::GetFocusHandler() {
  CefRefPtr<CefFocusHandler> result = NULL;
  BEGIN_ENV(env)
  jobject handler = NULL;
  JNI_CALL_METHOD(env, jhandler_, "getFocusHandler",
                  "()Lorg/cef/handler/CefFocusHandler;", Object, handler);
  if (handler) {
    result =
        GetCefFromJNIObject<CefFocusHandler>(env, handler, "CefFocusHandler");
    if (!result.get()) {
      result = new FocusHandler(env, handler);
      SetCefForJNIObject(env, handler, result.get(), "CefFocusHandler");
    }
  }
  END_ENV(env)
  return result;
}

CefRefPtr<CefJSDialogHandler> ClientHandler::GetJSDialogHandler() {
  CefRefPtr<CefJSDialogHandler> result = NULL;
  BEGIN_ENV(env)
  jobject handler = NULL;
  JNI_CALL_METHOD(env, jhandler_, "getJSDialogHandler",
                  "()Lorg/cef/handler/CefJSDialogHandler;", Object, handler);
  if (handler) {
    result = GetCefFromJNIObject<CefJSDialogHandler>(env, handler,
                                                     "CefJSDialogHandler");
    if (!result.get()) {
      result = new JSDialogHandler(env, handler);
      SetCefForJNIObject(env, handler, result.get(), "CefJSDialogHandler");
    }
  }
  END_ENV(env)
  return result;
}

CefRefPtr<CefKeyboardHandler> ClientHandler::GetKeyboardHandler() {
  CefRefPtr<CefKeyboardHandler> result = NULL;
  BEGIN_ENV(env)
  jobject handler = NULL;
  JNI_CALL_METHOD(env, jhandler_, "getKeyboardHandler",
                  "()Lorg/cef/handler/CefKeyboardHandler;", Object, handler);
  if (handler) {
    result = GetCefFromJNIObject<CefKeyboardHandler>(env, handler,
                                                     "CefKeyboardHandler");
    if (!result.get()) {
      result = new KeyboardHandler(env, handler);
      SetCefForJNIObject(env, handler, result.get(), "CefKeyboardHandler");
    }
  }
  END_ENV(env)
  return result;
}

CefRefPtr<CefLifeSpanHandler> ClientHandler::GetLifeSpanHandler() {
  CefRefPtr<CefLifeSpanHandler> result = NULL;
  BEGIN_ENV(env)
  jobject handler = NULL;
  JNI_CALL_METHOD(env, jhandler_, "getLifeSpanHandler",
                  "()Lorg/cef/handler/CefLifeSpanHandler;", Object, handler);
  if (handler) {
    result = GetCefFromJNIObject<CefLifeSpanHandler>(env, handler,
                                                     "CefLifeSpanHandler");
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
  JNI_CALL_METHOD(env, jhandler_, "getLoadHandler",
                  "()Lorg/cef/handler/CefLoadHandler;", Object, handler);
  if (handler) {
    result =
        GetCefFromJNIObject<CefLoadHandler>(env, handler, "CefLoadHandler");
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
  JNI_CALL_METHOD(env, jhandler_, "getRenderHandler",
                  "()Lorg/cef/handler/CefRenderHandler;", Object, handler);
  if (handler) {
    result =
        GetCefFromJNIObject<CefRenderHandler>(env, handler, "CefRenderHandler");
    if (!result.get()) {
      result = new RenderHandler(env, handler);
      SetCefForJNIObject(env, handler, result.get(), "CefRenderHandler");
    }
  }
  END_ENV(env)
  return result;
}

CefRefPtr<CefRequestHandler> ClientHandler::GetRequestHandler() {
  CefRefPtr<CefRequestHandler> result = NULL;
  BEGIN_ENV(env)
  jobject handler = NULL;
  JNI_CALL_METHOD(env, jhandler_, "getRequestHandler",
                  "()Lorg/cef/handler/CefRequestHandler;", Object, handler);
  if (handler) {
    result = GetCefFromJNIObject<CefRequestHandler>(env, handler,
                                                    "CefRequestHandler");
    if (!result.get()) {
      result = new RequestHandler(env, handler);
      SetCefForJNIObject(env, handler, result.get(), "CefRequestHandler");
    }
  }
  END_ENV(env)
  return result;
}

bool ClientHandler::OnProcessMessageReceived(
    CefRefPtr<CefBrowser> browser,
    CefProcessId source_process,
    CefRefPtr<CefProcessMessage> message) {
  std::set<CefRefPtr<CefMessageRouterBrowserSide>>::iterator iter;
  bool handled = false;

  base::AutoLock lock_scope(lock_);
  for (iter = message_router_.begin(); iter != message_router_.end(); iter++) {
    handled =
        (*iter)->OnProcessMessageReceived(browser, source_process, message);
    if (handled)
      break;
  }
  return handled;
}

CefRefPtr<WindowHandler> ClientHandler::GetWindowHandler() {
  CefRefPtr<WindowHandler> result = NULL;
  BEGIN_ENV(env)
  jobject handler = NULL;
  JNI_CALL_METHOD(env, jhandler_, "getWindowHandler",
                  "()Lorg/cef/handler/CefWindowHandler;", Object, handler);
  if (handler) {
    result = GetCefFromJNIObject<WindowHandler>(env, handler, "WindowHandler");
    if (!result.get()) {
      result = new WindowHandler(env, handler);
      SetCefForJNIObject(env, handler, result.get(), "WindowHandler");
    }
  }
  END_ENV(env)
  return result;
}

void ClientHandler::AddMessageRouter(JNIEnv* env, jobject jmessageRouter) {
  CefRefPtr<CefMessageRouterBrowserSide> router =
      GetCefFromJNIObject<CefMessageRouterBrowserSide>(env, jmessageRouter,
                                                       "CefMessageRouter");
  if (!router.get())
    return;

  CefMessageRouterConfig config =
      GetJNIMessageRouterConfigFromRouter(env, jmessageRouter);

  base::AutoLock lock_scope(lock_);
  // 1) Add CefMessageRouterBrowserSide into the list.
  message_router_.insert(router);

  // 2) Update CefApp for new render-processes.
  BrowserProcessHandler::AddMessageRouterConfig(config);

  // 3) Update running render-processes.
  jobjectArray jbrowserArray = GetAllJNIBrowser(env, jhandler_);
  if (!jbrowserArray)
    return;

  CefRefPtr<CefProcessMessage> message =
      CefProcessMessage::Create("AddMessageRouter");
  CefRefPtr<CefListValue> args = message->GetArgumentList();
  args->SetString(0, config.js_query_function);
  args->SetString(1, config.js_cancel_function);

  jint length = env->GetArrayLength(jbrowserArray);
  for (int i = 0; i < length; ++i) {
    jobject jbrowser = env->GetObjectArrayElement(jbrowserArray, i);
    CefRefPtr<CefBrowser> browser =
        GetCefFromJNIObject<CefBrowser>(env, jbrowser, "CefBrowser");
    if (!browser.get())
      continue;
    browser->SendProcessMessage(PID_RENDERER, message);
  }
}

void ClientHandler::RemoveMessageRouter(JNIEnv* env, jobject jmessageRouter) {
  CefRefPtr<CefMessageRouterBrowserSide> router =
      GetCefFromJNIObject<CefMessageRouterBrowserSide>(env, jmessageRouter,
                                                       "CefMessageRouter");
  if (!router.get())
    return;

  CefMessageRouterConfig config =
      GetJNIMessageRouterConfigFromRouter(env, jmessageRouter);

  base::AutoLock lock_scope(lock_);
  // 1) Remove CefMessageRouterBrowserSide from the list.
  message_router_.erase(router);

  // 2) Update CefApp.
  BrowserProcessHandler::RemoveMessageRouterConfig(config);

  // 3) Update running render-processes.
  jobjectArray jbrowserArray = GetAllJNIBrowser(env, jhandler_);
  if (!jbrowserArray)
    return;

  CefRefPtr<CefProcessMessage> message =
      CefProcessMessage::Create("RemoveMessageRouter");
  CefRefPtr<CefListValue> args = message->GetArgumentList();
  args->SetString(0, config.js_query_function);
  args->SetString(1, config.js_cancel_function);

  jint length = env->GetArrayLength(jbrowserArray);
  for (int i = 0; i < length; ++i) {
    jobject jbrowser = env->GetObjectArrayElement(jbrowserArray, i);
    CefRefPtr<CefBrowser> browser =
        GetCefFromJNIObject<CefBrowser>(env, jbrowser, "CefBrowser");
    if (!browser.get())
      continue;
    browser->SendProcessMessage(PID_RENDERER, message);
  }
}

void ClientHandler::OnAfterCreated() {}

void ClientHandler::OnBeforeClose(CefRefPtr<CefBrowser> browser) {
  REQUIRE_UI_THREAD();

  base::AutoLock lock_scope(lock_);
  std::set<CefRefPtr<CefMessageRouterBrowserSide>>::iterator iter;
  for (iter = message_router_.begin(); iter != message_router_.end(); iter++) {
    (*iter)->OnBeforeClose(browser);
  }
}

void ClientHandler::OnBeforeBrowse(CefRefPtr<CefBrowser> browser,
                                   CefRefPtr<CefFrame> frame) {
  base::AutoLock lock_scope(lock_);
  std::set<CefRefPtr<CefMessageRouterBrowserSide>>::iterator iter;
  for (iter = message_router_.begin(); iter != message_router_.end(); iter++) {
    (*iter)->OnBeforeBrowse(browser, frame);
  }
}

void ClientHandler::OnRenderProcessTerminated(CefRefPtr<CefBrowser> browser) {
  base::AutoLock lock_scope(lock_);
  std::set<CefRefPtr<CefMessageRouterBrowserSide>>::iterator iter;
  for (iter = message_router_.begin(); iter != message_router_.end(); iter++) {
    (*iter)->OnRenderProcessTerminated(browser);
  }
}

jobject ClientHandler::getBrowser(CefRefPtr<CefBrowser> browser) {
  jobject jbrowser = NULL;
  BEGIN_ENV(env)
  JNI_CALL_METHOD(env, jhandler_, "getBrowser",
                  "(I)Lorg/cef/browser/CefBrowser;", Object, jbrowser,
                  browser->GetIdentifier());
  END_ENV(env)
  return jbrowser;
}
