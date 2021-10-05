// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "request_context_handler.h"

#include "jni_util.h"
#include "resource_request_handler.h"
#include "util.h"

namespace {

// JNI CefWebPluginInfo object.
class ScopedJNIWebPluginInfo : public ScopedJNIObject<CefWebPluginInfo> {
 public:
  ScopedJNIWebPluginInfo(JNIEnv* env, CefRefPtr<CefWebPluginInfo> obj)
      : ScopedJNIObject<CefWebPluginInfo>(env,
                                          obj,
                                          "org/cef/network/CefWebPluginInfo_N",
                                          "CefWebPluginInfo") {}
};

}  // namespace

RequestContextHandler::RequestContextHandler(JNIEnv* env, jobject jhandler)
    : handle_(env, jhandler) {}

bool RequestContextHandler::OnBeforePluginLoad(
    const CefString& mime_type,
    const CefString& plugin_url,
    bool is_main_frame,
    const CefString& top_origin_url,
    CefRefPtr<CefWebPluginInfo> plugin_info,
    PluginPolicy* plugin_policy) {
  ScopedJNIEnv env;
  if (!env)
    return false;

  ScopedJNIString jmimeType(env, mime_type);
  ScopedJNIString jpluginUrl(env, plugin_url);
  ScopedJNIString jtopOriginUrl(env, top_origin_url);
  ScopedJNIWebPluginInfo jpluginInfo(env, plugin_info);
  jboolean jresult = JNI_FALSE;

  JNI_CALL_METHOD(env, handle_, "onBeforePluginLoad",
                  "Ljava/lang/String;Ljava/lang/String;ZLjava/lang/String;Lorg/"
                  "cef/network/CefWebPluginInfo;)Z",
                  Boolean, jresult, jmimeType.get(), jpluginUrl.get(),
                  is_main_frame ? JNI_TRUE : JNI_FALSE, jtopOriginUrl.get(),
                  jpluginInfo.get());

  if (jresult == JNI_FALSE) {
    // Allow the plugin load.
    if (*plugin_policy != PLUGIN_POLICY_ALLOW &&
        *plugin_policy != PLUGIN_POLICY_DETECT_IMPORTANT) {
      *plugin_policy = PLUGIN_POLICY_ALLOW;
      return true;
    }
  } else {
    // Block the plugin load.
    if (*plugin_policy != PLUGIN_POLICY_BLOCK &&
        *plugin_policy != PLUGIN_POLICY_DISABLE) {
      *plugin_policy = PLUGIN_POLICY_BLOCK;
      return true;
    }
  }

  return false;
}

CefRefPtr<CefResourceRequestHandler>
RequestContextHandler::GetResourceRequestHandler(
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    CefRefPtr<CefRequest> request,
    bool is_navigation,
    bool is_download,
    const CefString& request_initiator,
    bool& disable_default_handling) {
  ScopedJNIEnv env;
  if (!env)
    return nullptr;

  ScopedJNIBrowser jbrowser(env, browser);
  ScopedJNIFrame jframe(env, frame);
  jframe.SetTemporary();
  ScopedJNIRequest jrequest(env, request);
  jrequest.SetTemporary();
  ScopedJNIString jrequestInitiator(env, request_initiator);
  ScopedJNIBoolRef jdisableDefaultHandling(env, disable_default_handling);
  ScopedJNIObjectResult jresult(env);

  JNI_CALL_METHOD(env, handle_, "getResourceRequestHandler",
                  "(Lorg/cef/browser/CefBrowser;Lorg/cef/browser/CefFrame;Lorg/"
                  "cef/network/CefRequest;ZZLjava/lang/String;Lorg/cef/misc/"
                  "BoolRef;)Lorg/cef/handler/CefResourceRequestHandler;",
                  Object, jresult, jbrowser.get(), jframe.get(), jrequest.get(),
                  is_navigation ? JNI_TRUE : JNI_FALSE,
                  is_download ? JNI_TRUE : JNI_FALSE, jrequestInitiator.get(),
                  jdisableDefaultHandling.get());

  disable_default_handling = jdisableDefaultHandling;

  if (jresult)
    return new ResourceRequestHandler(env, jresult);
  return nullptr;
}
