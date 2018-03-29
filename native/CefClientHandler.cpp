// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "CefClientHandler.h"
#include "client_handler.h"
#include "jni_util.h"
#include "message_router_handler.h"

JNIEXPORT void JNICALL
Java_org_cef_handler_CefClientHandler_N_1CefClientHandler_1CTOR(
    JNIEnv* env,
    jobject clientHandler) {
  CefRefPtr<ClientHandler> client = new ClientHandler(env, clientHandler);
  SetCefForJNIObject(env, clientHandler, client.get(), "CefClientHandler");
}

JNIEXPORT void JNICALL
Java_org_cef_handler_CefClientHandler_N_1addMessageRouter(
    JNIEnv* env,
    jobject clientHandler,
    jobject jmessageRouter) {
  CefRefPtr<ClientHandler> client = GetCefFromJNIObject<ClientHandler>(
      env, clientHandler, "CefClientHandler");
  if (!client.get())
    return;
  client->AddMessageRouter(env, jmessageRouter);
}

JNIEXPORT void JNICALL
Java_org_cef_handler_CefClientHandler_N_1removeContextMenuHandler(
    JNIEnv* env,
    jobject clientHandler,
    jobject contextMenuHandler) {
  SetCefForJNIObject<CefContextMenuHandler>(env, contextMenuHandler, NULL,
                                            "CefContextMenuHandler");
}

JNIEXPORT void JNICALL
Java_org_cef_handler_CefClientHandler_N_1removeDialogHandler(
    JNIEnv* env,
    jobject clientHandler,
    jobject dialogHandler) {
  SetCefForJNIObject<CefDialogHandler>(env, dialogHandler, NULL,
                                       "CefDialogHandler");
}

JNIEXPORT void JNICALL
Java_org_cef_handler_CefClientHandler_N_1removeDisplayHandler(
    JNIEnv* env,
    jobject clientHandler,
    jobject displayHandler) {
  SetCefForJNIObject<CefDisplayHandler>(env, displayHandler, NULL,
                                        "CefDisplayHandler");
}

JNIEXPORT void JNICALL
Java_org_cef_handler_CefClientHandler_N_1removeDownloadHandler(
    JNIEnv* env,
    jobject clientHandler,
    jobject downloadHandler) {
  SetCefForJNIObject<CefDownloadHandler>(env, downloadHandler, NULL,
                                         "CefDownloadHandler");
}

JNIEXPORT void JNICALL
Java_org_cef_handler_CefClientHandler_N_1removeDragHandler(
    JNIEnv* env,
    jobject clientHandler,
    jobject dragHandler) {
  SetCefForJNIObject<CefDragHandler>(env, dragHandler, NULL, "CefDragHandler");
}

JNIEXPORT void JNICALL
Java_org_cef_handler_CefClientHandler_N_1removeFocusHandler(
    JNIEnv* env,
    jobject clientHandler,
    jobject focusHandler) {
  SetCefForJNIObject<CefFocusHandler>(env, focusHandler, NULL,
                                      "CefFocusHandler");
}

JNIEXPORT void JNICALL
Java_org_cef_handler_CefClientHandler_N_1removeJSDialogHandler(
    JNIEnv* env,
    jobject clientHandler,
    jobject jsdialogHandler) {
  SetCefForJNIObject<CefJSDialogHandler>(env, jsdialogHandler, NULL,
                                         "CefJSDialogHandler");
}

JNIEXPORT void JNICALL
Java_org_cef_handler_CefClientHandler_N_1removeKeyboardHandler(
    JNIEnv* env,
    jobject clientHandler,
    jobject keyboardHandler) {
  SetCefForJNIObject<CefKeyboardHandler>(env, keyboardHandler, NULL,
                                         "CefKeyboardHandler");
}

JNIEXPORT void JNICALL
Java_org_cef_handler_CefClientHandler_N_1removeLifeSpanHandler(
    JNIEnv* env,
    jobject clientHandler,
    jobject lifeSpanHandler) {
  SetCefForJNIObject<CefLifeSpanHandler>(env, lifeSpanHandler, NULL,
                                         "CefLifeSpanHandler");
}

JNIEXPORT void JNICALL
Java_org_cef_handler_CefClientHandler_N_1removeLoadHandler(
    JNIEnv* env,
    jobject clientHandler,
    jobject loadHandler) {
  SetCefForJNIObject<CefLoadHandler>(env, loadHandler, NULL, "CefLoadHandler");
}

JNIEXPORT void JNICALL
Java_org_cef_handler_CefClientHandler_N_1removeMessageRouter(
    JNIEnv* env,
    jobject clientHandler,
    jobject jmessageRouter) {
  CefRefPtr<ClientHandler> client = GetCefFromJNIObject<ClientHandler>(
      env, clientHandler, "CefClientHandler");
  if (!client.get())
    return;
  client->RemoveMessageRouter(env, jmessageRouter);
}

JNIEXPORT void JNICALL
Java_org_cef_handler_CefClientHandler_N_1removeRenderHandler(
    JNIEnv* env,
    jobject clientHandler,
    jobject renderHandler) {
  SetCefForJNIObject<CefRenderHandler>(env, renderHandler, NULL,
                                       "CefRenderHandler");
}

JNIEXPORT void JNICALL
Java_org_cef_handler_CefClientHandler_N_1removeRequestHandler(
    JNIEnv* env,
    jobject clientHandler,
    jobject requestHandler) {
  SetCefForJNIObject<CefRequestHandler>(env, requestHandler, NULL,
                                        "CefRequestHandler");
}

JNIEXPORT void JNICALL
Java_org_cef_handler_CefClientHandler_N_1removeWindowHandler(
    JNIEnv* env,
    jobject clientHandler,
    jobject windowHandler) {
  SetCefForJNIObject<WindowHandler>(env, windowHandler, NULL, "WindowHandler");
}

JNIEXPORT void JNICALL
Java_org_cef_handler_CefClientHandler_N_1CefClientHandler_1DTOR(
    JNIEnv* env,
    jobject clientHandler) {
  // delete reference to the native client handler
  SetCefForJNIObject<ClientHandler>(env, clientHandler, NULL,
                                    "CefClientHandler");
}
