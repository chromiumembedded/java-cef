#include "CefPermissionPromptCallback_N.h"

#include "include/cef_permission_handler.h"

#include "jni_scoped_helpers.h"
#include "jni_util.h"

namespace {
    CefRefPtr<CefPermissionPromptCallback> GetSelf(jlong self) {
      return reinterpret_cast<CefPermissionPromptCallback*>(self);
    }

    void ClearSelf(JNIEnv* env, jobject obj) {
      SetCefForJNIObject<CefPermissionPromptCallback>(env, obj, nullptr, "CefPermissionPromptCallback");
    }
}

JNIEXPORT void JNICALL
Java_org_cef_callback_CefPermissionPromptCallback_1N_N_1Continue(JNIEnv* env, jobject obj, jlong self, jint result) {
  CefRefPtr<CefPermissionPromptCallback> callback = GetSelf(self);
  if (!callback)
    return;

  callback->Continue(static_cast<cef_permission_request_result_t>(result));
  ClearSelf(env, obj);
}
