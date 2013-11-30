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

ClientHandler::ClientHandler(JNIEnv* env, jobject browser, jobject handler)
  : browser_id_(0) {
  jbrowser_ = env->NewGlobalRef(browser);
  jhandler_ = env->NewGlobalRef(handler);
}

ClientHandler::~ClientHandler() {
}

bool ClientHandler::OnProcessMessageReceived(
    CefRefPtr<CefBrowser> browser,
    CefProcessId source_process,
    CefRefPtr<CefProcessMessage> message) {
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

void ClientHandler::OnLoadingStateChange(CefRefPtr<CefBrowser> browser,
                                         bool isLoading,
                                         bool canGoBack,
                                         bool canGoForward) {
  REQUIRE_UI_THREAD();
}

void ClientHandler::OnAddressChange(CefRefPtr<CefBrowser> browser,
                                    CefRefPtr<CefFrame> frame,
                                    const CefString& url) {
  JNIEnv* env = GetJNIEnv();
	if(env) {
		jclass cls = env->GetObjectClass(jhandler_);
		jmethodID methodID = env->GetMethodID(cls, "onAddressChange",
			"(Lorg/cef/CefBrowser;Ljava/lang/String;)V");
		if(methodID != 0) {
			env->CallVoidMethod(jhandler_, methodID, jbrowser_,
				  NewJNIString(env, url));
		}
		
		if(env->ExceptionOccurred()) {
			env->ExceptionDescribe();
			env->ExceptionClear();
		}
	}
}

void ClientHandler::OnTitleChange(CefRefPtr<CefBrowser> browser,
                                  const CefString& title) {
  JNIEnv* env = GetJNIEnv();
	if(env) {
		jclass cls = env->GetObjectClass(jhandler_);
		jmethodID methodID = env->GetMethodID(cls, "onTitleChange",
			"(Lorg/cef/CefBrowser;Ljava/lang/String;)V");
		if(methodID != 0) {
			env->CallVoidMethod(jhandler_, methodID, jbrowser_,
				  NewJNIString(env, title));
		}
		
		if(env->ExceptionOccurred()) {
			env->ExceptionDescribe();
			env->ExceptionClear();
		}
	}
}

bool ClientHandler::OnConsoleMessage(CefRefPtr<CefBrowser> browser,
                                     const CefString& message,
                                     const CefString& source,
                                     int line) {
  REQUIRE_UI_THREAD();
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

bool ClientHandler::OnBeforePopup(CefRefPtr<CefBrowser> browser,
                                  CefRefPtr<CefFrame> frame,
                                  const CefString& target_url,
                                  const CefString& target_frame_name,
                                  const CefPopupFeatures& popupFeatures,
                                  CefWindowInfo& windowInfo,
                                  CefRefPtr<CefClient>& client,
                                  CefBrowserSettings& settings,
                                  bool* no_javascript_access) {
  if (browser->GetHost()->IsWindowRenderingDisabled()) {
    // Cancel popups in off-screen rendering mode.
    return true;
  }
  return false;
}

void ClientHandler::OnAfterCreated(CefRefPtr<CefBrowser> browser) {
  REQUIRE_UI_THREAD();

  AutoLock lock_scope(this);
  if (!browser_.get())   {
    // We need to keep the main child window, but not popup windows.
    browser_ = browser;
    browser_id_ = browser->GetIdentifier();
  }
}

bool ClientHandler::DoClose(CefRefPtr<CefBrowser> browser) {
  REQUIRE_UI_THREAD();
  return false;
}

void ClientHandler::OnBeforeClose(CefRefPtr<CefBrowser> browser) {
  REQUIRE_UI_THREAD();

  AutoLock lock_scope(this);
  if (browser_id_ == browser->GetIdentifier()) {
    // Free the browser pointer so that the browser can be destroyed.
    browser_ = NULL;
    browser_id_ = 0;

    JNIEnv* env = GetJNIEnv();
	  if(env) {
      env->DeleteGlobalRef(jbrowser_);
      env->DeleteGlobalRef(jhandler_);
      jbrowser_ = jhandler_ = NULL;
    }
  }
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

bool ClientHandler::GetRootScreenRect(CefRefPtr<CefBrowser> browser,
    CefRect& rect) {
  return GetViewRect(browser, rect);
}

bool ClientHandler::GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect) {
  JNIEnv* env = GetJNIEnv();
	if(!env)
    return false;

	jclass cls = env->GetObjectClass(jhandler_);
  jmethodID methodID = env->GetMethodID(cls, "getViewRect",
			"(Lorg/cef/CefBrowser;)Ljava/awt/Rectangle;");
	if(methodID == 0)
    return false;

	bool success = false;
		
  jobject retval = env->CallObjectMethod(jhandler_, methodID, jbrowser_);
		
	if(env->ExceptionOccurred()) {
    env->ExceptionDescribe();
		env->ExceptionClear();
	} else if (retval) {
    rect = GetJNIRect(env, retval);
    success = true;
  }

  if (retval)
    env->DeleteLocalRef(retval);

  return success;
}

bool ClientHandler::GetScreenPoint(CefRefPtr<CefBrowser> browser,
                                   int viewX,
                                   int viewY,
                                   int& screenX,
                                   int& screenY) {
  JNIEnv* env = GetJNIEnv();
	if(!env)
    return false;

	jclass cls = env->GetObjectClass(jhandler_);
  jmethodID methodID = env->GetMethodID(cls, "getScreenPoint",
			"(Lorg/cef/CefBrowser;Ljava/awt/Point;)Ljava/awt/Point;");
	if(methodID == 0)
    return false;

	bool success = false;

  jobject point_obj = NewJNIPoint(env, viewX, viewY);
  if (point_obj == NULL)
    return false;
		
  jobject retval = env->CallObjectMethod(jhandler_, methodID, jbrowser_, point_obj);
		
	if(env->ExceptionOccurred()) {
    env->ExceptionDescribe();
		env->ExceptionClear();
	} else if (retval) {
    GetJNIPoint(env, retval, &screenX, &screenY);
    success = true;
  }

  env->DeleteLocalRef(point_obj);
  if (retval)
    env->DeleteLocalRef(retval);

  return success;
}

bool ClientHandler::GetScreenInfo(CefRefPtr<CefBrowser> browser,
                                  CefScreenInfo& screen_info) {
  return false;
}

void ClientHandler::OnPopupShow(CefRefPtr<CefBrowser> browser,
                                bool show) {
  JNIEnv* env = GetJNIEnv();
	if(!env)
    return;

  jclass cls = env->GetObjectClass(jhandler_);
  jmethodID methodID = env->GetMethodID(cls, "onPopupShow",
			"(Lorg/cef/CefBrowser;Z)V");
	if(methodID == 0)
    return;

  env->CallVoidMethod(jhandler_, methodID, jbrowser_,
      show ? JNI_TRUE : JNI_FALSE);

	if(env->ExceptionOccurred()) {
    env->ExceptionDescribe();
		env->ExceptionClear();
	}
}

void ClientHandler::OnPopupSize(CefRefPtr<CefBrowser> browser,
                                const CefRect& rect) {
  JNIEnv* env = GetJNIEnv();
	if(!env)
    return;

  jclass cls = env->GetObjectClass(jhandler_);
  jmethodID methodID = env->GetMethodID(cls, "onPopupSize",
			"(Lorg/cef/CefBrowser;Ljava/awt/Rectangle;)V");
	if(methodID == 0)
    return;

  jobject rect_obj = NewJNIRect(env, rect);
  if (rect_obj == NULL)
    return;

  env->CallVoidMethod(jhandler_, methodID, jbrowser_,
      rect_obj);

	if(env->ExceptionOccurred()) {
    env->ExceptionDescribe();
		env->ExceptionClear();
	}

  env->DeleteLocalRef(rect_obj);
}

void ClientHandler::OnPaint(CefRefPtr<CefBrowser> browser,
                            PaintElementType type,
                            const RectList& dirtyRects,
                            const void* buffer,
                            int width,
                            int height) {
  JNIEnv* env = GetJNIEnv();
	if(!env)
    return;

  jclass cls = env->GetObjectClass(jhandler_);
  jmethodID methodID = env->GetMethodID(cls, "onPaint",
			"(Lorg/cef/CefBrowser;Z[Ljava/awt/Rectangle;Ljava/nio/ByteBuffer;II)V");
	if(methodID == 0)
    return;

  jobjectArray rect_array = NewJNIRectArray(env, dirtyRects);
  jobject direct_buffer =
      env->NewDirectByteBuffer(const_cast<void*>(buffer), width * height * 4);

  env->CallVoidMethod(jhandler_, methodID, jbrowser_,
      type == PET_VIEW ? JNI_FALSE : JNI_TRUE,
      rect_array, direct_buffer, width, height);

	if(env->ExceptionOccurred()) {
    env->ExceptionDescribe();
		env->ExceptionClear();
	}

  env->DeleteLocalRef(rect_array);
  env->DeleteLocalRef(direct_buffer);
}

void ClientHandler::OnCursorChange(CefRefPtr<CefBrowser> browser,
                                   CefCursorHandle cursor) {
  JNIEnv* env = GetJNIEnv();
	if(!env)
    return;

  jclass cls = env->GetObjectClass(jhandler_);
  jmethodID methodID = env->GetMethodID(cls, "onCursorChange",
			"(Lorg/cef/CefBrowser;I)V");
	if(methodID == 0)
    return;

  const int cursorId = NativeGetCursorId(cursor);
  env->CallVoidMethod(jhandler_, methodID, jbrowser_, cursorId);

	if(env->ExceptionOccurred()) {
    env->ExceptionDescribe();
		env->ExceptionClear();
	}
}
