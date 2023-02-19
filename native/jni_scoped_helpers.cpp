// Copyright (c) 2019 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include <algorithm>
#include <mutex>

#include "jni_scoped_helpers.h"

#include "client_handler.h"
#include "jni_util.h"

namespace {

// Retrieves the JNIEnv for the current thread. Attaches the VM to the current
// thread if necessary. Sets |mustDetach| to true if DetachJNIEnv must be
// called.
jint GetJNIEnv(JNIEnv** env, bool* mustDetach) {
  *env = nullptr;
  *mustDetach = false;

  JavaVM* jvm = GetJVM();
  if (!jvm)
    return JNI_ERR;

  jint result = jvm->GetEnv((void**)env, JNI_VERSION_1_6);
  if (result == JNI_EDETACHED) {
    result = jvm->AttachCurrentThreadAsDaemon((void**)env, nullptr);
    if (result == JNI_OK) {
      *mustDetach = true;
    }
  }

  return result;
}

// Detaches the current thread from the VM. Should only be called if
// |mustDetach| was set to true by GetJNIEnv.
void DetachJNIEnv() {
  JavaVM* jvm = GetJVM();
  if (jvm) {
    jvm->DetachCurrentThread();
  }
}

// Using a simple cache to store global refs to loaded classes, since we
// need to load the same classes over and over, which should neither change
// on the JVM side nor be GCed...
std::map<std::string, jobject> classCache_;
std::mutex classCacheMutex_;
// ...except if there's a change in the classloader to use by JCEF, in which
// case the cache can be invalidated
jobject classCacheClassLoader_;

// Returns a class with the given fully qualified |class_name| (with '/' as
// separator).
jclass FindClass(JNIEnv* env, const char* class_name) {
  std::string classNameSeparatedByDots(class_name);
  std::replace(classNameSeparatedByDots.begin(), classNameSeparatedByDots.end(),
               '/', '.');

  jobject classLoader = GetJavaClassLoader();
  ASSERT(classLoader);

  // std::map is not thread-safe with regard to concurrent reading and writing
  std::lock_guard<std::mutex> guard(classCacheMutex_);

  if (classLoader != classCacheClassLoader_) {
    for (std::pair<const std::string, jobject>& entry : classCache_) {
      env->DeleteGlobalRef(entry.second);
    }
    classCache_.clear();
    classCacheClassLoader_ = classLoader;
  }

  std::map<std::string, jobject>::iterator it =
      classCache_.find(classNameSeparatedByDots);
  if (it != classCache_.end()) {
    return static_cast<jclass>(env->NewLocalRef(it->second));
  }

  ScopedJNIString classNameJString(env, classNameSeparatedByDots);
  jobject result = nullptr;

  JNI_CALL_METHOD(env, classLoader, "loadClass",
                  "(Ljava/lang/String;)Ljava/lang/Class;", Object, result,
                  classNameJString.get());

  // Make a global reference out of the local reference to allow for caching.
  // This produces a non-garbage-collectable class, since this global ref is
  // never released! However, for the classes that are requested by JCEF via
  // this mechanism, that should be acceptable, because they aren't candidates
  // to be GCed anyway.
  classCache_[classNameSeparatedByDots] = env->NewGlobalRef(result);

  return static_cast<jclass>(result);
}

jobject NewJNIBoolRef(JNIEnv* env, bool initValue) {
  ScopedJNIObjectLocal jboolRef(env, "org/cef/misc/BoolRef");
  if (!jboolRef)
    return nullptr;
  SetJNIBoolRef(env, jboolRef, initValue);
  return jboolRef.Release();
}

jobject NewJNIIntRef(JNIEnv* env, int initValue) {
  ScopedJNIObjectLocal jintRef(env, "org/cef/misc/IntRef");
  if (!jintRef)
    return nullptr;
  SetJNIIntRef(env, jintRef, initValue);
  return jintRef.Release();
}

jobject NewJNIStringRef(JNIEnv* env, const CefString& initValue) {
  ScopedJNIObjectLocal jstringRef(env, "org/cef/misc/StringRef");
  if (!jstringRef)
    return nullptr;
  SetJNIStringRef(env, jstringRef, initValue);
  return jstringRef.Release();
}

jobject NewJNIDate(JNIEnv* env, const CefBaseTime& time) {
  ScopedJNIObjectLocal jdate(env, "java/util/Date");
  if (!jdate)
    return nullptr;
  CefTime cef_time;
  cef_time_from_basetime(time, &cef_time);
  double timestamp = cef_time.GetDoubleT() * 1000;
  JNI_CALL_VOID_METHOD(env, jdate, "setTime", "(J)V", (jlong)timestamp);
  return jdate.Release();
}

jobject NewJNICookie(JNIEnv* env, const CefCookie& cookie) {
  ScopedJNIString name(env, CefString(&cookie.name));
  ScopedJNIString value(env, CefString(&cookie.value));
  ScopedJNIString domain(env, CefString(&cookie.domain));
  ScopedJNIString path(env, CefString(&cookie.path));
  const bool hasExpires = (cookie.has_expires != 0);
  ScopedJNIObjectLocal expires(
      env, hasExpires ? NewJNIDate(env, cookie.expires) : nullptr);
  ScopedJNIDate creation(env, cookie.creation);
  ScopedJNIDate last_access(env, cookie.last_access);

  return NewJNIObject(env, "org/cef/network/CefCookie",
                      "(Ljava/lang/String;Ljava/lang/String;"
                      "Ljava/lang/String;Ljava/lang/String;"
                      "ZZLjava/util/Date;Ljava/util/Date;"
                      "ZLjava/util/Date;)V",
                      name.get(), value.get(), domain.get(), path.get(),
                      (cookie.secure != 0 ? JNI_TRUE : JNI_FALSE),
                      (cookie.httponly != 0 ? JNI_TRUE : JNI_FALSE),
                      creation.get(), last_access.get(),
                      (hasExpires ? JNI_TRUE : JNI_FALSE), expires.get());
}

jobject NewJNITransitionType(JNIEnv* env,
                             CefRequest::TransitionType transitionType) {
  ScopedJNIObjectResult result(env);
  switch (transitionType & TT_SOURCE_MASK) {
    default:
      JNI_CASE(env, "org/cef/network/CefRequest$TransitionType", TT_LINK,
               result);
      JNI_CASE(env, "org/cef/network/CefRequest$TransitionType", TT_EXPLICIT,
               result);
      JNI_CASE(env, "org/cef/network/CefRequest$TransitionType",
               TT_AUTO_SUBFRAME, result);
      JNI_CASE(env, "org/cef/network/CefRequest$TransitionType",
               TT_MANUAL_SUBFRAME, result);
      JNI_CASE(env, "org/cef/network/CefRequest$TransitionType", TT_FORM_SUBMIT,
               result);
      JNI_CASE(env, "org/cef/network/CefRequest$TransitionType", TT_RELOAD,
               result);
  }

  if (result) {
    const int qualifiers = (transitionType & TT_QUALIFIER_MASK);
    JNI_CALL_VOID_METHOD(env, result, "addQualifiers", "(I)V", qualifiers);
  }

  return result.Release();
}

jobject NewJNIURLRequestStatus(
    JNIEnv* env,
    CefResourceRequestHandler::URLRequestStatus status) {
  ScopedJNIObjectResult result(env);
  switch (status) {
    default:
      JNI_CASE(env, "org/cef/network/CefURLRequest$Status", UR_UNKNOWN, result);
      JNI_CASE(env, "org/cef/network/CefURLRequest$Status", UR_SUCCESS, result);
      JNI_CASE(env, "org/cef/network/CefURLRequest$Status", UR_IO_PENDING,
               result);
      JNI_CASE(env, "org/cef/network/CefURLRequest$Status", UR_CANCELED,
               result);
      JNI_CASE(env, "org/cef/network/CefURLRequest$Status", UR_FAILED, result);
  }
  return result.Release();
}

jobject GetJNIBrowser(JNIEnv* env, CefRefPtr<CefBrowser> browser) {
  if (!browser)
    return nullptr;
  CefRefPtr<ClientHandler> client =
      (ClientHandler*)browser->GetHost()->GetClient().get();
  return client->getBrowser(env, browser);
}

}  // namespace

// static
const int ScopedJNIEnv::kDefaultLocalCapacity = 1024;

ScopedJNIEnv::ScopedJNIEnv(jint local_capacity)
    : ScopedJNIEnv(nullptr, local_capacity) {}

ScopedJNIEnv::ScopedJNIEnv(JNIEnv* env, jint local_capacity)
    : jenv_(env), local_capacity_(local_capacity) {
  if (!jenv_) {
    if (GetJNIEnv(&jenv_, &should_detach_) != JNI_OK || !jenv_) {
      NOTREACHED() << "Failed to retrieve JNIEnv";
      return;
    }
  }
  if (local_capacity_ > 0) {
    if (jenv_->EnsureLocalCapacity(local_capacity_) != JNI_OK ||
        jenv_->PushLocalFrame(local_capacity_) != JNI_OK) {
      LOG(WARNING) << "Failed to create local frame with capacity "
                   << local_capacity_;
      local_capacity_ = 0;
    }
  }
}

ScopedJNIEnv::~ScopedJNIEnv() {
  if (!jenv_)
    return;
  if (local_capacity_ > 0) {
    if (export_result_) {
      *export_result_ = jenv_->PopLocalFrame(*export_result_);
    } else {
      jenv_->PopLocalFrame(nullptr);
    }
  }
  if (should_detach_) {
    DetachJNIEnv();
  }
}

ScopedJNIObjectGlobal::ScopedJNIObjectGlobal(JNIEnv* env, jobject handle)
    : jhandle_(nullptr) {
  if (handle) {
    jhandle_ = env->NewGlobalRef(handle);
    DCHECK(jhandle_);
  }
}

ScopedJNIObjectGlobal::~ScopedJNIObjectGlobal() {
  if (jhandle_) {
    ScopedJNIEnv env;
    if (env)
      env->DeleteGlobalRef(jhandle_);
  }
}

jobject ScopedJNIObjectGlobal::get() const {
  return jhandle_;
}

ScopedJNIObjectGlobal::operator jobject() const {
  return jhandle_;
}

ScopedJNIObjectLocal::ScopedJNIObjectLocal(JNIEnv* env, jobject handle)
    : ScopedJNIBase<jobject>(env) {
  jhandle_ = handle;
}

ScopedJNIObjectLocal::ScopedJNIObjectLocal(JNIEnv* env, const char* class_name)
    : ScopedJNIObjectLocal(env, NewJNIObject(env, class_name)) {}

ScopedJNIObjectResult::ScopedJNIObjectResult(JNIEnv* env)
    : ScopedJNIBase<jobject>(env) {}

ScopedJNIClass::ScopedJNIClass(JNIEnv* env, const char* class_name)
    : ScopedJNIClass(env, FindClass(env, class_name)) {}

ScopedJNIClass::ScopedJNIClass(JNIEnv* env, const jclass& cls)
    : ScopedJNIBase<jclass>(env) {
  jhandle_ = cls;
}

ScopedJNIString::ScopedJNIString(JNIEnv* env, const std::string& str)
    : ScopedJNIBase<jstring>(env) {
  jhandle_ = NewJNIString(env, str);
  DCHECK(jhandle_);
}

ScopedJNIDate::ScopedJNIDate(JNIEnv* env, const CefBaseTime& time)
    : ScopedJNIBase<jobject>(env) {
  jhandle_ = NewJNIDate(env, time);
  DCHECK(jhandle_);
}

ScopedJNICookie::ScopedJNICookie(JNIEnv* env, const CefCookie& cookie)
    : ScopedJNIBase<jobject>(env) {
  jhandle_ = NewJNICookie(env, cookie);
  DCHECK(jhandle_);
}

ScopedJNITransitionType::ScopedJNITransitionType(
    JNIEnv* env,
    CefRequest::TransitionType transitionType)
    : ScopedJNIBase<jobject>(env) {
  jhandle_ = NewJNITransitionType(env, transitionType);
  DCHECK(jhandle_);
}

ScopedJNIURLRequestStatus::ScopedJNIURLRequestStatus(
    JNIEnv* env,
    CefResourceRequestHandler::URLRequestStatus status)
    : ScopedJNIBase<jobject>(env) {
  jhandle_ = NewJNIURLRequestStatus(env, status);
  DCHECK(jhandle_);
}

ScopedJNIStringResult::ScopedJNIStringResult(JNIEnv* env)
    : ScopedJNIBase<jstring>(env) {}

ScopedJNIStringResult::ScopedJNIStringResult(JNIEnv* env, const jstring& str)
    : ScopedJNIStringResult(env) {
  jhandle_ = str;
}

CefString ScopedJNIStringResult::GetCefString() const {
  if (!jhandle_)
    return CefString();
  return GetJNIString(env_, jhandle_);
}

ScopedJNIBrowser::ScopedJNIBrowser(JNIEnv* env, CefRefPtr<CefBrowser> obj)
    : ScopedJNIBase<jobject>(env) {
  if (obj) {
    // Will return nullptr for browsers that represent native popup windows.
    jhandle_ = GetJNIBrowser(env_, obj);
  } else {
    jhandle_ = nullptr;
  }
}

void ScopedJNIBrowser::SetHandle(jobject handle, bool should_delete) {
  DCHECK(!jhandle_);
  DCHECK(handle);
  jhandle_ = handle;
  delete_ref_ = should_delete;
}

CefRefPtr<CefBrowser> ScopedJNIBrowser::GetCefObject() const {
  if (!jhandle_)
    return nullptr;
  return GetJNIBrowser(env_, jhandle_);
}

ScopedJNIAuthCallback::ScopedJNIAuthCallback(JNIEnv* env,
                                             CefRefPtr<CefAuthCallback> obj)
    : ScopedJNIObject<CefAuthCallback>(env,
                                       obj,
                                       "org/cef/callback/CefAuthCallback_N",
                                       "CefAuthCallback") {}

ScopedJNIDragData::ScopedJNIDragData(JNIEnv* env, CefRefPtr<CefDragData> obj)
    : ScopedJNIObject<CefDragData>(env,
                                   obj,
                                   "org/cef/callback/CefDragData_N",
                                   "CefDragData") {}

ScopedJNIFrame::ScopedJNIFrame(JNIEnv* env, CefRefPtr<CefFrame> obj)
    : ScopedJNIObject<CefFrame>(env,
                                obj,
                                "org/cef/browser/CefFrame_N",
                                "CefFrame") {}

ScopedJNIMenuModel::ScopedJNIMenuModel(JNIEnv* env, CefRefPtr<CefMenuModel> obj)
    : ScopedJNIObject<CefMenuModel>(env,
                                    obj,
                                    "org/cef/callback/CefMenuModel_N",
                                    "CefMenuModel") {}

ScopedJNIMessageRouter::ScopedJNIMessageRouter(JNIEnv* env,
                                               CefRefPtr<CefMessageRouter> obj)
    : ScopedJNIObject<CefMessageRouter>(env,
                                        obj,
                                        "org/cef/browser/CefMessageRouter_N",
                                        "CefMessageRouter") {}

ScopedJNIPostData::ScopedJNIPostData(JNIEnv* env, CefRefPtr<CefPostData> obj)
    : ScopedJNIObject<CefPostData>(env,
                                   obj,
                                   "org/cef/network/CefPostData_N",
                                   "CefPostData") {}

ScopedJNIPostDataElement::ScopedJNIPostDataElement(
    JNIEnv* env,
    CefRefPtr<CefPostDataElement> obj)
    : ScopedJNIObject<CefPostDataElement>(
          env,
          obj,
          "org/cef/network/CefPostDataElement_N",
          "CefPostDataElement") {}

ScopedJNIPrintSettings::ScopedJNIPrintSettings(JNIEnv* env,
                                               CefRefPtr<CefPrintSettings> obj)
    : ScopedJNIObject<CefPrintSettings>(env,
                                        obj,
                                        "org/cef/misc/CefPrintSettings_N",
                                        "CefPrintSettings") {}

ScopedJNIRequest::ScopedJNIRequest(JNIEnv* env, CefRefPtr<CefRequest> obj)
    : ScopedJNIObject<CefRequest>(env,
                                  obj,
                                  "org/cef/network/CefRequest_N",
                                  "CefRequest") {}

ScopedJNIResponse::ScopedJNIResponse(JNIEnv* env, CefRefPtr<CefResponse> obj)
    : ScopedJNIObject<CefResponse>(env,
                                   obj,
                                   "org/cef/network/CefResponse_N",
                                   "CefResponse") {}

ScopedJNICallback::ScopedJNICallback(JNIEnv* env, CefRefPtr<CefCallback> obj)
    : ScopedJNIObject<CefCallback>(env,
                                   obj,
                                   "org/cef/callback/CefCallback_N",
                                   "CefCallback") {}

ScopedJNIBoolRef::ScopedJNIBoolRef(JNIEnv* env, bool value)
    : ScopedJNIBase<jobject>(env) {
  jhandle_ = NewJNIBoolRef(env, value);
  DCHECK(jhandle_);
}

ScopedJNIBoolRef::operator bool() const {
  return GetJNIBoolRef(env_, jhandle_);
}

ScopedJNIIntRef::ScopedJNIIntRef(JNIEnv* env, int value)
    : ScopedJNIBase<jobject>(env) {
  jhandle_ = NewJNIIntRef(env, value);
  DCHECK(jhandle_);
}

ScopedJNIIntRef::operator int() const {
  return GetJNIIntRef(env_, jhandle_);
}

ScopedJNIStringRef::ScopedJNIStringRef(JNIEnv* env, const CefString& value)
    : ScopedJNIBase<jobject>(env) {
  jhandle_ = NewJNIStringRef(env, value);
  DCHECK(jhandle_);
}

ScopedJNIStringRef::operator CefString() const {
  return GetJNIStringRef(env_, jhandle_);
}
