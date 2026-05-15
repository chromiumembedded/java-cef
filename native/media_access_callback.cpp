#include "media_access_callback.h"

#include "jni_scoped_helpers.h"
#include "jni_util.h"

MediaAccessCallback::MediaAccessCallback(CefRefPtr<CefMediaAccessCallback> callback) : callback_(callback) {}

void MediaAccessCallback::Continue(uint32_t allowed_permissions) {
  if (callback_)
    callback_->Continue(allowed_permissions);
}

void MediaAccessCallback::Cancel() {
  if (callback_)
    callback_->Cancel();
}

jobject NewJNIMediaAccessCallback(JNIEnv* env, CefRefPtr<CefMediaAccessCallback> callback) {
  if (!callback)
    return nullptr;

  jobject jcallback = NewJNIObject(env, "org/cef/callback/CefMediaAccessCallback_N");
  if (!jcallback)
    return nullptr;

  SetCefForJNIObject<CefMediaAccessCallback>(env, jcallback, callback.get(), "CefMediaAccessCallback");

  return jcallback;
}
