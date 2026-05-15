#include "permission_handler.h"

#include "jni_util.h"
#include "media_access_callback.h"
#include "permission_prompt_callback.h"

PermissionHandler::PermissionHandler(JNIEnv* env, jobject handler) : handle_(env, handler) {}

bool PermissionHandler::OnRequestMediaAccessPermission(
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    const CefString& requesting_origin,
    uint32_t requested_permissions,
    CefRefPtr<CefMediaAccessCallback> callback) {
  ScopedJNIEnv env;
  if (!env)
    return false;

  ScopedJNIBrowser jbrowser(env, browser);
  ScopedJNIFrame jframe(env, frame);
  jframe.SetTemporary();
  ScopedJNIString jorigin(env, requesting_origin);

  jobject jcallback = NewJNIMediaAccessCallback(env, callback);

  jboolean jresult = JNI_FALSE;
  JNI_CALL_METHOD(env, handle_, "onRequestMediaAccessPermission",
                  "(Lorg/cef/browser/CefBrowser;"
                  "Lorg/cef/browser/CefFrame;"
                  "Ljava/lang/String;"
                  "ILorg/cef/callback/CefMediaAccessCallback;)Z",
                  Boolean, jresult,
                  jbrowser.get(),
                  jframe.get(),
                  jorigin.get(),
                  static_cast<jint>(requested_permissions),
                  jcallback);

  if (jcallback)
    env->DeleteLocalRef(jcallback);

  return (jresult != JNI_FALSE);
}

bool PermissionHandler::OnShowPermissionPrompt(
    CefRefPtr<CefBrowser> browser,
    uint64_t prompt_id,
    const CefString& requesting_origin,
    uint32_t requested_permissions,
    CefRefPtr<CefPermissionPromptCallback> callback) {
  ScopedJNIEnv env;
  if (!env)
    return false;

  ScopedJNIBrowser jbrowser(env, browser);
  ScopedJNIString jorigin(env, requesting_origin);

  jobject jpermissionSet = NewJNIPermissionRequestTypeEnumSet(env, requested_permissions);
  jobject jcallback = NewJNIPermissionPromptCallback(env, callback);

  jboolean jresult = JNI_FALSE;
  JNI_CALL_METHOD(env, handle_, "onShowPermissionPrompt",
                  "(Lorg/cef/browser/CefBrowser;"
                  "JLjava/lang/String;"
                  "Ljava/util/EnumSet;"
                  "Lorg/cef/callback/CefPermissionPromptCallback;)Z",
                  Boolean, jresult,
                  jbrowser.get(),
                  static_cast<jlong>(prompt_id),
                  jorigin.get(),
                  jpermissionSet,
                  jcallback);

  if (jpermissionSet)
    env->DeleteLocalRef(jpermissionSet);
  if (jcallback)
    env->DeleteLocalRef(jcallback);

  return (jresult != JNI_FALSE);
}

void PermissionHandler::OnDismissPermissionPrompt(
    CefRefPtr<CefBrowser> browser,
    uint64_t prompt_id,
    cef_permission_request_result_t result) {
  ScopedJNIEnv env;
  if (!env)
    return;

  ScopedJNIBrowser jbrowser(env, browser);
  ScopedJNIObjectLocal jresult(env, NewJNIPermissionRequestResult(env, result));

  JNI_CALL_VOID_METHOD(env, handle_, "onDismissPermissionPrompt",
                       "(Lorg/cef/browser/CefBrowser;J"
                       "Lorg/cef/handler/CefPermissionRequestResult;)V",
                       jbrowser.get(),
                       static_cast<jlong>(prompt_id),
                       jresult.get());
}
