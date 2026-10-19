#include "permission_prompt_callback.h"

#include "jni_scoped_helpers.h"
#include "jni_util.h"

PermissionPromptCallback::PermissionPromptCallback(CefRefPtr<CefPermissionPromptCallback> callback) : callback_(callback) {}

void PermissionPromptCallback::Continue(cef_permission_request_result_t result) {
  if (callback_)
    callback_->Continue(result);
}

jobject NewJNIPermissionPromptCallback(JNIEnv* env, CefRefPtr<CefPermissionPromptCallback> callback) {
  if (!callback)
    return nullptr;

  jobject jcallback = NewJNIObject(env, "org/cef/callback/CefPermissionPromptCallback_N");
  if (!jcallback)
    return nullptr;

  SetCefForJNIObject<CefPermissionPromptCallback>(env, jcallback, callback.get(), "CefPermissionPromptCallback");

  return jcallback;
}
