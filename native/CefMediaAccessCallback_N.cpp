#include "CefMediaAccessCallback_N.h"

#include "include/cef_permission_handler.h"

#include "jni_scoped_helpers.h"
#include "jni_util.h"

namespace {
    CefRefPtr<CefMediaAccessCallback> GetSelf(jlong self) {
      return reinterpret_cast<CefMediaAccessCallback*>(self);
    }

    void ClearSelf(JNIEnv* env, jobject obj) {
      SetCefForJNIObject<CefMediaAccessCallback>(env, obj, nullptr, "CefMediaAccessCallback");
    }
}

JNIEXPORT void JNICALL
Java_org_cef_callback_CefMediaAccessCallback_1N_N_1Continue(JNIEnv* env, jobject obj, jlong self, jint allowed_permissions) {
  CefRefPtr<CefMediaAccessCallback> callback = GetSelf(self);
  if (!callback)
    return;

  callback->Continue(static_cast<uint32_t>(allowed_permissions));
  ClearSelf(env, obj);
}

JNIEXPORT void JNICALL
Java_org_cef_callback_CefMediaAccessCallback_1N_N_1Cancel(JNIEnv* env, jobject obj, jlong self) {
  CefRefPtr<CefMediaAccessCallback> callback = GetSelf(self);
  if (!callback)
    return;

  callback->Cancel();
  ClearSelf(env, obj);
}
