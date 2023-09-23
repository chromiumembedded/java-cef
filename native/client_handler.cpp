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
#include "audio_handler.h"
#include "download_handler.h"
#include "drag_handler.h"
#include "focus_handler.h"
#include "jsdialog_handler.h"
#include "keyboard_handler.h"
#include "life_span_handler.h"
#include "load_handler.h"
#include "message_router_handler.h"
#include "print_handler.h"
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

namespace {

CefRefPtr<CefMessageRouter> GetMessageRouter(JNIEnv* env,
                                             jobject jmessageRouter) {
  ScopedJNIMessageRouter messageRouter(env);
  messageRouter.SetHandle(jmessageRouter, false /* should_delete */);
  return messageRouter.GetCefObject();
}

CefMessageRouterConfig GetMessageRouterConfig(JNIEnv* env,
                                              jobject jmessageRouter) {
  ScopedJNIObjectResult jrouterConfig(env);
  JNI_CALL_METHOD(env, jmessageRouter, "getMessageRouterConfig",
                  "()Lorg/cef/browser/CefMessageRouter$CefMessageRouterConfig;",
                  Object, jrouterConfig);
  return GetJNIMessageRouterConfig(env, jrouterConfig);
}

}  // namespace

ClientHandler::ClientHandler(JNIEnv* env, jobject handler)
    : handle_(env, handler) {}

template <class T>
CefRefPtr<T> ClientHandler::GetHandler(const char* class_name) {
  std::string methodName, className, methodSig;

  std::stringstream ss;
  ss << "get" << class_name;
  methodName = ss.str();

  ss.str(std::string());
  ss << "Cef" << class_name;
  className = ss.str();

  ss.str(std::string());
  ss << "()Lorg/cef/handler/" << className << ";";
  methodSig = ss.str();

  ScopedJNIEnv env;
  if (!env)
    return nullptr;

  CefRefPtr<T> result = nullptr;
  ScopedJNIObjectResult jresult(env);
  JNI_CALL_METHOD(env, handle_, methodName.c_str(), methodSig.c_str(), Object,
                  jresult);
  if (jresult) {
    ScopedJNIObject<T> jhandler(env, jresult.Release(),
                                true /* should_delete */, className.c_str());
    result = jhandler.GetOrCreateCefObject();
  }

  return result;
}

CefRefPtr<CefContextMenuHandler> ClientHandler::GetContextMenuHandler() {
  return GetHandler<ContextMenuHandler>("ContextMenuHandler");
}

CefRefPtr<CefDialogHandler> ClientHandler::GetDialogHandler() {
  return GetHandler<DialogHandler>("DialogHandler");
}

CefRefPtr<CefDisplayHandler> ClientHandler::GetDisplayHandler() {
  return GetHandler<DisplayHandler>("DisplayHandler");
}

 CefRefPtr<CefAudioHandler> ClientHandler::GetAudioHandler() {
   return GetHandler<AudioHandler>("AudioHandler");
 }

CefRefPtr<CefDownloadHandler> ClientHandler::GetDownloadHandler() {
  return GetHandler<DownloadHandler>("DownloadHandler");
}

CefRefPtr<CefDragHandler> ClientHandler::GetDragHandler() {
  return GetHandler<DragHandler>("DragHandler");
}

CefRefPtr<CefFocusHandler> ClientHandler::GetFocusHandler() {
  return GetHandler<FocusHandler>("FocusHandler");
}

CefRefPtr<CefJSDialogHandler> ClientHandler::GetJSDialogHandler() {
  return GetHandler<JSDialogHandler>("JSDialogHandler");
}

CefRefPtr<CefKeyboardHandler> ClientHandler::GetKeyboardHandler() {
  return GetHandler<KeyboardHandler>("KeyboardHandler");
}

CefRefPtr<CefLifeSpanHandler> ClientHandler::GetLifeSpanHandler() {
  return GetHandler<LifeSpanHandler>("LifeSpanHandler");
}

CefRefPtr<CefLoadHandler> ClientHandler::GetLoadHandler() {
  return GetHandler<LoadHandler>("LoadHandler");
}

CefRefPtr<CefPrintHandler> ClientHandler::GetPrintHandler() {
  return GetHandler<PrintHandler>("PrintHandler");
}

CefRefPtr<CefRenderHandler> ClientHandler::GetRenderHandler() {
  return GetHandler<RenderHandler>("RenderHandler");
}

CefRefPtr<CefRequestHandler> ClientHandler::GetRequestHandler() {
  return GetHandler<RequestHandler>("RequestHandler");
}

bool ClientHandler::OnProcessMessageReceived(
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    CefProcessId source_process,
    CefRefPtr<CefProcessMessage> message) {
  bool handled = false;

  // Iterate on a copy of |message_routers_| to avoid re-entrancy of
  // |message_router_lock_| if the client CefMessageRouterHandler impl
  // calls CefClientHandler.addMessageRouter/removeMessageRouter.
  MessageRouterSet message_routers;
  {
    base::AutoLock lock_scope(message_router_lock_);
    message_routers = message_routers_;
  }

  for (auto& router : message_routers) {
    handled = router->OnProcessMessageReceived(browser, frame, source_process,
                                               message);
    if (handled)
      break;
  }
  return handled;
}

CefRefPtr<WindowHandler> ClientHandler::GetWindowHandler() {
  return GetHandler<WindowHandler>("WindowHandler");
}

void ClientHandler::AddMessageRouter(JNIEnv* env, jobject jmessageRouter) {
  CefRefPtr<CefMessageRouter> router = GetMessageRouter(env, jmessageRouter);
  if (!router)
    return;

  CefMessageRouterConfig config = GetMessageRouterConfig(env, jmessageRouter);

  // 1) Add CefMessageRouterBrowserSide into the list.
  {
    base::AutoLock lock_scope(message_router_lock_);
    message_routers_.insert(router);
  }

  // 2) Update CefApp for new render-processes.
  BrowserProcessHandler::AddMessageRouterConfig(config);

  // 3) Update running render-processes.
  BrowserSet allBrowsers = GetAllBrowsers(env);
  if (allBrowsers.empty())
    return;

  CefRefPtr<CefProcessMessage> message =
      CefProcessMessage::Create("AddMessageRouter");
  CefRefPtr<CefListValue> args = message->GetArgumentList();
  args->SetString(0, config.js_query_function);
  args->SetString(1, config.js_cancel_function);

  BrowserSet::const_iterator it = allBrowsers.begin();
  for (; it != allBrowsers.end(); ++it) {
    (*it)->GetMainFrame()->SendProcessMessage(PID_RENDERER, message);
  }
}

void ClientHandler::RemoveMessageRouter(JNIEnv* env, jobject jmessageRouter) {
  CefRefPtr<CefMessageRouter> router = GetMessageRouter(env, jmessageRouter);
  if (!router)
    return;

  CefMessageRouterConfig config = GetMessageRouterConfig(env, jmessageRouter);

  // 1) Remove CefMessageRouterBrowserSide from the list.
  {
    base::AutoLock lock_scope(message_router_lock_);
    message_routers_.erase(router);
  }

  // 2) Update CefApp.
  BrowserProcessHandler::RemoveMessageRouterConfig(config);

  // 3) Update running render-processes.
  BrowserSet allBrowsers = GetAllBrowsers(env);
  if (allBrowsers.empty())
    return;

  CefRefPtr<CefProcessMessage> message =
      CefProcessMessage::Create("RemoveMessageRouter");
  CefRefPtr<CefListValue> args = message->GetArgumentList();
  args->SetString(0, config.js_query_function);
  args->SetString(1, config.js_cancel_function);

  BrowserSet::const_iterator it = allBrowsers.begin();
  for (; it != allBrowsers.end(); ++it) {
    (*it)->GetMainFrame()->SendProcessMessage(PID_RENDERER, message);
  }
}

void ClientHandler::OnAfterCreated() {}

void ClientHandler::OnBeforeClose(CefRefPtr<CefBrowser> browser) {
  REQUIRE_UI_THREAD();

  base::AutoLock lock_scope(message_router_lock_);
  for (auto& router : message_routers_) {
    router->OnBeforeClose(browser);
  }
}

void ClientHandler::OnBeforeBrowse(CefRefPtr<CefBrowser> browser,
                                   CefRefPtr<CefFrame> frame) {
  REQUIRE_UI_THREAD();

  base::AutoLock lock_scope(message_router_lock_);
  for (auto& router : message_routers_) {
    router->OnBeforeBrowse(browser, frame);
  }
}

void ClientHandler::OnRenderProcessTerminated(CefRefPtr<CefBrowser> browser) {
  REQUIRE_UI_THREAD();

  base::AutoLock lock_scope(message_router_lock_);
  for (auto& router : message_routers_) {
    router->OnRenderProcessTerminated(browser);
  }
}

jobject ClientHandler::getBrowser(JNIEnv* env, CefRefPtr<CefBrowser> browser) {
  jobject jbrowser = nullptr;
  JNI_CALL_METHOD(env, handle_, "getBrowser", "(I)Lorg/cef/browser/CefBrowser;",
                  Object, jbrowser, browser->GetIdentifier());
  return jbrowser;
}

ClientHandler::BrowserSet ClientHandler::GetAllBrowsers(JNIEnv* env) {
  BrowserSet result;

  jobject jbrowsers = nullptr;
  JNI_CALL_METHOD(env, handle_, "getAllBrowser", "()[Ljava/lang/Object;",
                  Object, jbrowsers);
  if (!jbrowsers)
    return result;

  jobjectArray jbrowserArray = (jobjectArray)jbrowsers;
  jint length = env->GetArrayLength(jbrowserArray);
  for (int i = 0; i < length; ++i) {
    jobject jbrowser = env->GetObjectArrayElement(jbrowserArray, i);
    if (!jbrowser)
      continue;

    ScopedJNIBrowser sbrowser(env);
    sbrowser.SetHandle(jbrowser, true /* should_delete */);
    CefRefPtr<CefBrowser> browser = sbrowser.GetCefObject();
    if (!browser)
      continue;

    result.insert(browser);
  }
  env->DeleteLocalRef(jbrowserArray);

  return result;
}
