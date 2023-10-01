// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef JCEF_NATIVE_CLIENT_HANDLER_H_
#define JCEF_NATIVE_CLIENT_HANDLER_H_
#pragma once

#include <jni.h>

#include <set>

#include "include/base/cef_lock.h"
#include "include/cef_base.h"
#include "include/cef_client.h"

#include "jni_scoped_helpers.h"
#include "message_router_handler.h"
#include "window_handler.h"

// ClientHandler implementation.
class ClientHandler : public CefClient {
 public:
  ClientHandler(JNIEnv* env, jobject handler);

  // CefClient methods
  CefRefPtr<CefContextMenuHandler> GetContextMenuHandler() override;
  CefRefPtr<CefDialogHandler> GetDialogHandler() override;
  CefRefPtr<CefDisplayHandler> GetDisplayHandler() override;
  CefRefPtr<CefAudioHandler> GetAudioHandler() override;
  CefRefPtr<CefDownloadHandler> GetDownloadHandler() override;
  CefRefPtr<CefDragHandler> GetDragHandler() override;
  CefRefPtr<CefFocusHandler> GetFocusHandler() override;
  CefRefPtr<CefJSDialogHandler> GetJSDialogHandler() override;
  CefRefPtr<CefKeyboardHandler> GetKeyboardHandler() override;
  CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override;
  CefRefPtr<CefLoadHandler> GetLoadHandler() override;
  CefRefPtr<CefPrintHandler> GetPrintHandler() override;
  CefRefPtr<CefRenderHandler> GetRenderHandler() override;
  CefRefPtr<CefRequestHandler> GetRequestHandler() override;

  bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
                                CefRefPtr<CefFrame> frame,
                                CefProcessId source_process,
                                CefRefPtr<CefProcessMessage> message) override;

  // Additional handler for windowed rendering mode
  CefRefPtr<WindowHandler> GetWindowHandler();

  // Methods to deal with message router bindings
  void AddMessageRouter(JNIEnv* env, jobject jmessageRouter);
  void RemoveMessageRouter(JNIEnv* env, jobject jmessageRouter);

  // Methods to set and remove a browser ref.
  void OnAfterCreated();
  void OnBeforeClose(CefRefPtr<CefBrowser> browser);
  void OnBeforeBrowse(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame);
  void OnRenderProcessTerminated(CefRefPtr<CefBrowser> browser);

  jobject getBrowser(JNIEnv* env, CefRefPtr<CefBrowser> browser);

 protected:
  typedef std::set<CefRefPtr<CefBrowser>> BrowserSet;
  BrowserSet GetAllBrowsers(JNIEnv* env);

  template <class T>
  CefRefPtr<T> GetHandler(const char* class_name);

  ScopedJNIObjectGlobal handle_;

  // The child browser window.
  std::map<int, jobject> browserMap_;

  using MessageRouterSet = std::set<CefRefPtr<CefMessageRouterBrowserSide>>;
  MessageRouterSet message_routers_;

  // Protects access to |message_routers_|.
  base::Lock message_router_lock_;

  // Include the default reference counting implementation.
  IMPLEMENT_REFCOUNTING(ClientHandler);
};

#endif  // JCEF_NATIVE_CLIENT_HANDLER_H_
