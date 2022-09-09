// Copyright (c) 2019 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef JCEF_NATIVE_JNI_SCOPED_HELPERS_H_
#define JCEF_NATIVE_JNI_SCOPED_HELPERS_H_

#include <jni.h>

#include <string>

#include "include/cef_auth_callback.h"
#include "include/cef_browser.h"
#include "include/cef_drag_data.h"
#include "include/cef_frame.h"
#include "include/cef_menu_model.h"
#include "include/cef_print_settings.h"
#include "include/cef_request.h"
#include "include/cef_resource_request_handler.h"
#include "include/cef_response.h"
#include "include/wrapper/cef_message_router.h"

//
// --------
// OVERVIEW
// --------
//
// JCEF uses JNI bindings to support two-way communication between Java objects
// and native C++ objects. The life span of the binding may differ depending
// on the intended usage, and the Java and/or C++ objects may still exist after
// the binding has been terminated. Consequently, care must be taken to follow
// the intended usage for each object type.
//
// Some common usage patterns are:
//
// - Objects created from C++ code with life span terminated after execution
//   of a single Java method callback (see `EXAMPLE 1` below).
// - Objects created from Java code with life span terminated via execution of
//   the Java dispose() or finalize() method (see `EXAMPLE 2` below).
// - C++ objects that wrap Java objects for the purpose of executing Java
//   method callbacks with life span terminated on deletion of the C++ object
//   (see `EXAMPLE 3` below).
//
// The Java object representing a binding is often associated with the C++
// object via the CefNative Java interface. When a C++ object is created the
// native code increments the C++ reference count and calls the setNativeRef
// method on the JNI handle. The C++ pointer address is passed to JNI using the
// `jlong` type and stored in the Java object using a `long N_CefHandle` member.
// This `long` value can be passed into future JNI function calls from Java code
// or retrieved from C++ code by calling the getNativeRef method on the JNI
// handle. When the binding is terminated (via a call to dispose()/finalize() or
// from C++ code) the setNativeRef method is called with a value of 0 and the
// C++ reference count is decremented, possibly triggering deletion of the C++
// object.
//
// A C++ object that executes callbacks on a Java object will hold a global
// reference to that Java object's JNI handle. This will keep the Java object
// alive until after all of the associated C++ objects have been deleted. In
// most cases these Java objects will not extend CefNative and a new C++ object
// will be created on each callback.
//
// CefClient is a special case where the C++ object wraps a JNI handle and the
// Java object extends CefNative to avoid creation of a new C++ object on
// each callback. Care is taken with CefClient to explicitly release the C++
// references on shutdown (via remove*Handler methods) and thereby avoid a
// reference loop between the C++ object and JNI handle.
//
// Below are examples of the common usage patterns for JCEF bindings.
//
//
// ---------
// EXAMPLE 1
// ---------
//
// An object implemented in native code that is only ever created from native
// code and then passed to a Java callback.
//
// In this example CefSomeObject is implemented in native code and exposed to
// Java via an equivalent interface. The native object is associated with the
// Java object via CefNative, with life span controlled by the native caller.
// Because the Java object is scoped to the native method there's no need to
// support destruction via dispose() or finalize() from the Java side.
//
// For a complete example see CefContextMenuHandler.onBeforeContextMenu and
// CefMenuModel.
//
// Java client usage:
//
//   // Extending a CefSomeHandler interface that has an onCallback method.
//   class MyHandler extends CefSomeHandler {
//     // Client callback that will executed from native code.
//     @Override
//     public void onCallback(CefSomeObject someObject) {
//       // |someObject| is backed by a native CEF object and only valid for the
//       // scope of this method.
//       int something = someObject.getSomething();
//     }
//   }
//
// Java implementation of org.cef.handler.CefSomeObject:
//
//   // Only instantiated via CefSomeObject_N.
//   public interface CefSomeObject {
//     // Public interface methods...
//     public abstract int getSomething();
//   }
//
// Java implementation of org.cef.handler.CefSomeObject_N:
//
//   // Created from SomeHandler::OnCallback in native code.
//   class CefSomeObject_N extends CefNativeAdapter implements CefSomeObject {
//     CefSomeObject_N() {}
//
//     // CefSomeObject implementation:
//     @Override
//     public int getSomething() {
//       try {
//         // Pass the native handle as a parameter for faster lookup.
//         return N_GetSomething(getNativeRef(null));
//       } catch (UnsatisfiedLinkError ule) {
//         ule.printStackTrace();
//         return -1;
//       }
//     }
//
//     // This becomes a native JNI binding.
//     private final native int N_GetSomething(long self);
//   }
//
// C++ implementation of CefSomeObject_N.cpp:
//
//   CefRefPtr<CefSomeObject> GetSelf(jlong self) {
//     return reinterpret_cast<CefSomeObject*>(self);
//   }
//
//   // Native implementation of N_GetSomething().
//   JNIEXPORT jint JNICALL
//   Java_org_cef_handler_CefSomeObject_1N_N_1GetSomething(JNIEnv* env,
//                                                         jobject obj,
//                                                         jlong self) {
//     CefRefPtr<CefSomeObject> object = GetSelf(self);
//     if (!object)
//       return -1;
//     return object->GetSomething();
//   }
//
// C++ implementation of the SomeHandler::OnCallback method:
//
//   // Helper for creating a CefSomeObject JNI handle.
//   class ScopedJNISomeObject : public ScopedJNIObject<CefSomeObject> {
//    public:
//     ScopedJNISomeObject(JNIEnv* env, CefRefPtr<CefSomeObject> obj)
//         : ScopedJNIObject<CefSomeObject>(env,
//                                          obj,
//                                          "org/cef/handler/CefSomeObject_N",
//                                          "CefSomeObject") {}
//   };
//
//   void SomeHandler::OnCallback() {
//     // Create the JNI instance of CefSomeObject.
//     ScopedJNISomeObject jsomeObject(env, model);
//     // Disassociate from the Java object when |jsomeObject| is destroyed.
//     jsomeObject.SetTemporary();
//
//     // Pass the CefSomeObject instance to CefSomeHandler.onCallback.
//     JNI_CALL_VOID_METHOD(env, handle_, "onCallback",
//                         "(Lorg/cef/handler/CefSomeObject;)V",
//                         jsomeObject.get());
//   }
//
//
// ---------
// EXAMPLE 2
// ---------
//
// An object implemented in native code that can be created and accessed from
// Java code.
//
// In this example CefSomeObject is implemented in native code and exposed to
// Java via an equivalent interface. The native object is associated with the
// Java object via CefNative, with life span controlled by the Java object.
//
// For a complete example see CefDragData.
//
// Java client usage:
//
//   // Create and use a CefSomeObject which is backed by a native CEF object.
//   CefSomeObject someObject = CefSomeObject.create();
//   int something = someObject.getSomething();
//
//   // Explicitly release the native CEF object when done. If we don't do this
//   // the object may eventually be released via finalize() if/when garbage
//   // collection is triggered.
//   someObject.dispose();
//
// Java implementation of org.cef.handler.CefSomeObject:
//
//   public abstract class CefSomeObject {
//     // Public interface methods...
//     public abstract int getSomething();
//
//     // This constructor can't be called directly. Use create() instead.
//     CefSomeObject() {}
//
//     // Cleanup in case the user forgets to call dispose().
//     @Override
//     protected void finalize() throws Throwable {
//       dispose();
//       super.finalize();
//     }
//
//     // Implemented in CefSomeObject_N to terminate the native binding.
//     public abstract void dispose();
//
//     public static final CefSomeObject create() {
//       return CefSomeObject_N.createNative();
//     }
//   }
//
// Java implementation of org.cef.handler.CefSomeObject_N:
//
//   class CefSomeObject_N extends CefSomeObject implements CefNative {
//     // CefNative implementation:
//     // Used internally to store a pointer to the CEF object.
//     private long N_CefHandle = 0;
//
//     @Override
//     public void setNativeRef(String identifer, long nativeRef) {
//       N_CefHandle = nativeRef;
//     }
//
//     @Override
//     public long getNativeRef(String identifer) {
//       return N_CefHandle;
//     }
//
//     CefSomeObject_N() {
//       super();
//     }
//
//     // CefSomeObject implementation:
//     @Override
//     public void dispose() {
//       try {
//         N_Dispose();
//       } catch (UnsatisfiedLinkError ule) {
//         ule.printStackTrace();
//       }
//     }
//
//     public static CefDragData createNative() {
//       try {
//         return CefSomeObject_N.N_Create();
//       } catch (UnsatisfiedLinkError ule) {
//         ule.printStackTrace();
//         return null;
//       }
//     }
//
//     @Override
//     public int getSomething() {
//       try {
//         // Pass the native handle as a parameter for faster lookup.
//         return N_GetSomething(N_CefHandle);
//       } catch (UnsatisfiedLinkError ule) {
//         ule.printStackTrace();
//         return -1;
//       }
//     }
//
//     // These become native JNI bindings.
//     private final native static CefSomeObject_N N_Create();
//     private final native void N_Dispose();
//     private final native int N_GetSomething(long self);
//   }
//
// C++ implementation of CefSomeObject_N.cpp:
//
//   const char kCefClassName[] = "CefSomeObject";
//
//   CefRefPtr<CefSomeObject> GetSelf(JNIEnv* env, jlong self) {
//     return reinterpret_cast<CefSomeObject*>(self);
//   }
//
//   // Helper for creating a CefSomeObject JNI handle.
//   class ScopedJNISomeObject : public ScopedJNIObject<CefSomeObject> {
//    public:
//     ScopedJNISomeObject(JNIEnv* env, CefRefPtr<CefSomeObject> obj)
//         : ScopedJNIObject<CefSomeObject>(env,
//                                          obj,
//                                          "org/cef/handler/CefSomeObject_N",
//                                          kCefClassName) {}
//   };
//
//   JNIEXPORT void JNICALL
//   Java_org_cef_handler_CefSomeObject_1N_N_1Dispose(JNIEnv* env,
//                                                    jobject obj) {
//     // Terminate the binding and release the native object reference that
//     // was taken by ScopedJNISomeObject. The native object may be destroyed.
//     SetCefForJNIObject<CefSomeObject>(env, obj, nullptr, kCefClassName);
//   }
//
//   JNIEXPORT jobject JNICALL
//   Java_org_cef_handler_CefSomeObject_1N_N_1Create(JNIEnv* env, jclass cls) {
//     CefRefPtr<CefSomeObject> object = CefSomeObject::Create();
//
//     // Create and return a new JNI handle that holds a reference to |object|.
//     ScopedJNISomeObject jobject(env, object);
//     return jobject.Release();
//   }
//
//   JNIEXPORT jint JNICALL
//   Java_org_cef_handler_CefSomeObject_1N_N_1GetSomething(JNIEnv* env,
//                                                         jobject obj,
//                                                         jlong self) {
//     CefRefPtr<CefSomeObject> object = GetSelf(env, self);
//     if (!object)
//       return -1;
//     return object->GetSomething();
//   }
//
//
// ---------
// EXAMPLE 3
// ---------
//
// A native interface that delegates callbacks to a Java interface. The native
// object is not associated with the Java object so multiple calls to
// getSomeHandler will create a new native object.
//
// In this example the CefSomeHandler interface exists in both native code and
// Java code. The client returns an instance of CefSomeHandler from the
// CefSourceHandler.getSomeHandler Java method. Native code then calls the
// CefSomeHandler.doSomething method at a later time.
//
// For a complete example see CefDisplayHandler.
//
// Java client usage:
//
//   // Client implementation of CefSomeHandler.
//   class MySomeHandler extends CefSomeHandlerAdapter {
//     MySomeHandler() {}
//
//     @Override
//     public void doSomething() { /* Do something here... */ }
//   }
//
//   // Client implementation of an interface that returns a CefSomeHandler.
//   class MySourceHandler extends CefSourceHandlerAdapter {
//     @Override
//     public CefSomeHandler getSomeHandler() { return new MySomeHandler(); }
//   }
//
//   // Somehow install the MySourceHandler and trigger a call to
//   // getSomeHandler() from native code.
//   CefContext.somehowInstallAndExecuteSourceHandler(new MySourceHandler());
//
// Java implementation of org.cef.handler.CefSomeHandler:
//
//   public interface CefSomeHandler {
//     public void DoSomething();
//   }
//
// Java implementation of org.cef.handler.CefSomeHandlerAdapter:
//
//   public abstract class CefSomeHandlerAdapter implements CefSomeHandler {
//     @Override
//     public void doSomething() {}
//   }
//
// C++ implementation of some_handler.h/cpp:
//
//   class SomeHandler : public CefSomeHandler {
//    public:
//     SomeHandler(JNIEnv* env, jobject handler);
//
//     // CefSomeHandler methods:
//     void DoSomething() override;
//
//    private:
//     // Manages a global JNI reference to the Java object.
//     ScopedJNIObjectGlobal handle_;
//   };
//
//   SomeHandler::SomeHandler(JNIEnv* env, jobject handler)
//       : handle_(env, handler) {}
//
//   void SomeHandler::DoSomething() {
//     ScopedJNIEnv env;
//     if (!env)
//       return;
//     JNI_CALL_VOID_METHOD(env, handle_, "doSomething", "()V");
//   }
//
// C++ implementation of the SourceHandler::GetSomeHandler method:
//
//   CefRefPtr<CefSomeHandler> SourceHandler::GetSomeHandler() {
//     ScopedJNIEnv env;
//     if (!env)
//       return nullptr;
//
//     ScopedJNIObjectResult jresult(env);
//
//     // Call CefSourceHandler.getSomeHandler() in Java code to get a
//     // CefSomeHandler object.
//     JNI_CALL_METHOD(env, handle_, "getSomeHandler",
//                     "()Lorg/cef/handler/CefSomeHandler;", Object, jresult);
//
//     // Create and return a new SomeHandler C++ object that wraps the Java
//     // object.
//     return new SomeHandler(env, jresult);
//   }
//

// Type specialization helpers for SetCefForJNIObject.
struct SetCefForJNIObjectHelper {
  static inline void AddRef(CefBaseScoped* obj) {}
  static inline void Release(CefBaseScoped* obj) {}

  template <class T>
  static inline T* Get(CefRawPtr<T> obj) {
    return obj;
  }

  static inline void AddRef(CefBaseRefCounted* obj) { obj->AddRef(); }
  static inline void Release(CefBaseRefCounted* obj) { obj->Release(); }

  template <class T>
  static inline T* Get(CefRefPtr<T> obj) {
    return obj.get();
  }

  // For ref-counted implementations that don't derive from CefBaseRefCounted.
  template <class T>
  static inline void AddRef(base::RefCountedThreadSafe<T>* obj) {
    obj->AddRef();
  }
  template <class T>
  static inline void Release(base::RefCountedThreadSafe<T>* obj) {
    obj->Release();
  }
};

// Forward declarations required by the below template types.
jobject NewJNIObject(JNIEnv* env, const char* class_name);
template <class T>
bool SetCefForJNIObject(JNIEnv* env, jobject obj, T* base, const char* varName);
template <class T>
T* GetCefFromJNIObject(JNIEnv* env, jobject obj, const char* varName);

class ScopedJNIEnv {
 public:
  static const int kDefaultLocalCapacity;

  // Retrieve the JNIEnv for the current thread or attach the VM to the current
  // thread if necessary.
  // If |local_capacity| > 0 a local frame will be created with the specified
  // maximum number of local references. Otherwise, no local frame will be
  // created.
  ScopedJNIEnv(jint local_capacity = kDefaultLocalCapacity);

  // Like above, but using an already known JNIEnv.
  ScopedJNIEnv(JNIEnv* env, jint local_capacity = kDefaultLocalCapacity);

  ~ScopedJNIEnv();

  // If a local frame was created, export |result| to the previous local
  // reference frame on destruction.
  void set_export_result(jobject* result) { export_result_ = result; }

  // Explicit return.
  JNIEnv* get() const { return jenv_; }

  // Implicit cast works in most cases.
  operator JNIEnv*() const { return jenv_; }

  // Pointer override for forwarding calls to the JNIEnv.
  JNIEnv* operator->() const {
    DCHECK(jenv_);
    return jenv_;
  }

 private:
  JNIEnv* jenv_;
  jint local_capacity_;
  bool should_detach_ = false;
  jobject* export_result_ = nullptr;
};

// Used by the native counterpart of JCEF objects to hold a global JNI handle.
class ScopedJNIObjectGlobal {
 public:
  // Creates a global reference to |handle|.
  ScopedJNIObjectGlobal(JNIEnv* env, jobject handle);
  ~ScopedJNIObjectGlobal();

  // Explicit return.
  jobject get() const;

  // Implicit cast works in most cases.
  operator jobject() const;

 protected:
  jobject jhandle_;
};

// Base class for scoped JNI types. Most derived classes will populate
// |jhandle_| in the constructor.
template <class T>
class ScopedJNIBase {
 public:
  ScopedJNIBase(JNIEnv* env, bool should_delete = true)
      : env_(env), jhandle_(nullptr), delete_ref_(should_delete) {
    DCHECK(env_);
  }

  virtual ~ScopedJNIBase() {
    if (jhandle_ && delete_ref_) {
      env_->DeleteLocalRef(jhandle_);
    }
  }

  // Explicit return for use with JNI_CALL_METHOD.
  T get() const { return jhandle_; }

  // Implicit cast works in most cases.
  operator T() const { return jhandle_; }

  // Returns and disassociates from the underlying handle.
  T Release() {
    T temp = jhandle_;
    jhandle_ = nullptr;
    return temp;
  }

 protected:
  JNIEnv* const env_;
  T jhandle_;
  bool delete_ref_;
};

// Manages a local handle to a JNI object. Use ScopedJNIObject<T> instead if
// the Java class implements CefNative.
class ScopedJNIObjectLocal : public ScopedJNIBase<jobject> {
 public:
  // A local reference to |handle| should already exist.
  ScopedJNIObjectLocal(JNIEnv* env, jobject handle);

  // Create a new instance of the specified |class_name|.
  ScopedJNIObjectLocal(JNIEnv* env, const char* class_name);
};

// Used for later assignment of a handle whose lifespan will then be managed.
// This will usually be a JNI object result parameter from JNI_CALL_METHOD.
class ScopedJNIObjectResult : public ScopedJNIBase<jobject> {
 public:
  explicit ScopedJNIObjectResult(JNIEnv* env);

  jobject& operator=(const jobject& obj) {
    DCHECK(!jhandle_);
    jhandle_ = obj;
    return jhandle_;
  }
};

// Used for JNI objects that implement CefNative.
template <class T, class PtrT = CefRefPtr<T>>
class ScopedJNIObject : public ScopedJNIBase<jobject> {
 public:
  // Attach to an existing JNI object handle. |should_delete| should be false
  // if the handle is a parameter to or return value from a JNI function that
  // this scoped object resides within.
  ScopedJNIObject(JNIEnv* env,
                  jobject handle,
                  bool should_delete,
                  const char* cef_class_name)
      : ScopedJNIBase<jobject>(env, should_delete),
        cef_class_name_(cef_class_name),
        temporary_(false),
        created_handle_(false) {
    DCHECK(handle);
    jhandle_ = handle;
  }

  // Create a new JNI object handle that attaches to an existing CEF object. A
  // reference will be taken to |obj| and associated with the JNI handle via the
  // CefNative Java interface. The reference will later be invalidated using one
  // of the following approaches, depending on the object's intended usage:
  // 1. Via a JNI Dispose callback that is executed from the Java object's
  //    dispose() method. This is the preferred approach for long-lived objects
  //    as it gives the Java application explicit control over the CEF object's
  //    life span.
  // 2. Via execution of a JNI callback that represents completion of a single
  //    use Cef*Callback object.
  // 3. Via this object's destructor if SetTemporary() was called. This approach
  //    can be used to create Java objects that are only valid for the duration
  //    of a JNI method call.
  // 4. Via a JNI Dispose callback that is executed from the Java object's
  //    finalize() method. This is the catch-all case if none of the above
  //    conditions trigger and should be used with caution because the
  //    finalize() method is not guaranteed to be called in a timely manner.
  ScopedJNIObject(JNIEnv* env,
                  PtrT obj,
                  const char* jni_class_name,
                  const char* cef_class_name)
      : ScopedJNIBase<jobject>(env),
        object_(obj),
        cef_class_name_(cef_class_name),
        temporary_(false),
        created_handle_(false) {
    if (obj) {
      jhandle_ = NewJNIObject(env_, jni_class_name);
      if (jhandle_) {
        created_handle_ = true;
        SetCefForJNIObject(env_, jhandle_, SetCefForJNIObjectHelper::Get(obj),
                           cef_class_name);
      }
    }
  }

  virtual ~ScopedJNIObject() {
    if (temporary_ && created_handle_) {
      // Invalidate the Java object.
      SetCefForJNIObject<T>(env_, jhandle_, nullptr, cef_class_name_);
    }
  }

  // Attach to the specified |handle|. |should_delete| should be false if the
  // handle is a parameter to or return value from a JNI callback function.
  void SetHandle(jobject handle, bool should_delete) {
    DCHECK(!jhandle_);
    DCHECK(handle);
    jhandle_ = handle;
    delete_ref_ = should_delete;
  }

  // Invalidate the Java object on destruction.
  void SetTemporary() {
    DCHECK(created_handle_);
    temporary_ = true;
  }

  // Get an existing native CEF object.
  PtrT GetCefObject() {
    if (object_)
      return object_;
    if (jhandle_)
      object_ = GetCefFromJNIObject<T>(env_, jhandle_, cef_class_name_);
    return object_;
  }

  // Get an existing or create a new native CEF object.
  PtrT GetOrCreateCefObject() {
    PtrT object = GetCefObject();
    if (!object && jhandle_) {
      object = new T(env_, jhandle_);
      SetCefForJNIObject(env_, jhandle_, SetCefForJNIObjectHelper::Get(object),
                         cef_class_name_);
      object_ = object;
    }
    return object;
  }

 protected:
  PtrT object_;
  const char* const cef_class_name_;
  bool temporary_;
  bool created_handle_;
};

// JNI class. Finding |class_name| may fail.
class ScopedJNIClass : public ScopedJNIBase<jclass> {
 public:
  ScopedJNIClass(JNIEnv* env, const char* class_name);
  ScopedJNIClass(JNIEnv* env, const jclass& cls);
};

// JNI string.
class ScopedJNIString : public ScopedJNIBase<jstring> {
 public:
  ScopedJNIString(JNIEnv* env, const std::string& str);
};

// JNI date.
class ScopedJNIDate : public ScopedJNIBase<jobject> {
 public:
  ScopedJNIDate(JNIEnv* env, const CefBaseTime& time);
};

// JNI cookie.
class ScopedJNICookie : public ScopedJNIBase<jobject> {
 public:
  ScopedJNICookie(JNIEnv* env, const CefCookie& cookie);
};

// JNI TransitionType.
class ScopedJNITransitionType : public ScopedJNIBase<jobject> {
 public:
  ScopedJNITransitionType(JNIEnv* env,
                          CefRequest::TransitionType transitionType);
};

// JNI URLRequestStatus.
class ScopedJNIURLRequestStatus : public ScopedJNIBase<jobject> {
 public:
  ScopedJNIURLRequestStatus(JNIEnv* env,
                            CefResourceRequestHandler::URLRequestStatus status);
};

// Used for assignment of a handle whose lifespan will then be managed.
class ScopedJNIStringResult : public ScopedJNIBase<jstring> {
 public:
  explicit ScopedJNIStringResult(JNIEnv* env);
  ScopedJNIStringResult(JNIEnv* env, const jstring& str);

  jstring& operator=(const jstring& str) {
    DCHECK(!jhandle_);
    jhandle_ = str;
    return jhandle_;
  }

  jobject& operator=(const jobject& obj) {
    return (jobject&)operator=((jstring)obj);
  }

  // Get the associated CEF string.
  CefString GetCefString() const;
};

// JNI CefBrowser object. This is a special case where the CefBrowser object
// must always already exist.
class ScopedJNIBrowser : public ScopedJNIBase<jobject> {
 public:
  // If |obj| is nullptr the SetHandle method should be used.
  ScopedJNIBrowser(JNIEnv* env, CefRefPtr<CefBrowser> obj = nullptr);

  // Attach to the specified |handle|.
  void SetHandle(jobject handle, bool should_delete);

  // Get the associated CEF object.
  CefRefPtr<CefBrowser> GetCefObject() const;
};

// JNI CefAuthCallback object.
class ScopedJNIAuthCallback : public ScopedJNIObject<CefAuthCallback> {
 public:
  // If |obj| is nullptr the SetHandle method should be used.
  ScopedJNIAuthCallback(JNIEnv* env, CefRefPtr<CefAuthCallback> obj = nullptr);
};

// JNI CefDragData object.
class ScopedJNIDragData : public ScopedJNIObject<CefDragData> {
 public:
  // If |obj| is nullptr the SetHandle method should be used.
  ScopedJNIDragData(JNIEnv* env, CefRefPtr<CefDragData> obj = nullptr);
};

// JNI CefFrame object.
class ScopedJNIFrame : public ScopedJNIObject<CefFrame> {
 public:
  // If |obj| is nullptr the SetHandle method should be used.
  ScopedJNIFrame(JNIEnv* env, CefRefPtr<CefFrame> obj = nullptr);
};

// JNI CefMenuModel object.
class ScopedJNIMenuModel : public ScopedJNIObject<CefMenuModel> {
 public:
  // |obj| must be non-nullptr.
  ScopedJNIMenuModel(JNIEnv* env, CefRefPtr<CefMenuModel> obj);
};

// JNI CefMessageRouter object.
using CefMessageRouter = CefMessageRouterBrowserSide;
class ScopedJNIMessageRouter : public ScopedJNIObject<CefMessageRouter> {
 public:
  // If |obj| is nullptr the SetHandle method should be used.
  ScopedJNIMessageRouter(JNIEnv* env,
                         CefRefPtr<CefMessageRouter> obj = nullptr);
};

// JNI CefPostData object.
class ScopedJNIPostData : public ScopedJNIObject<CefPostData> {
 public:
  // If |obj| is nullptr the SetHandle method should be used.
  ScopedJNIPostData(JNIEnv* env, CefRefPtr<CefPostData> obj = nullptr);
};

// JNI CefPostDataElement object.
class ScopedJNIPostDataElement : public ScopedJNIObject<CefPostDataElement> {
 public:
  // If |obj| is nullptr the SetHandle method should be used.
  ScopedJNIPostDataElement(JNIEnv* env,
                           CefRefPtr<CefPostDataElement> obj = nullptr);
};

// JNI CefPrintSettings object.
class ScopedJNIPrintSettings : public ScopedJNIObject<CefPrintSettings> {
 public:
  // If |obj| is nullptr the SetHandle method should be used.
  ScopedJNIPrintSettings(JNIEnv* env,
                         CefRefPtr<CefPrintSettings> obj = nullptr);
};

// JNI CefRequest object.
class ScopedJNIRequest : public ScopedJNIObject<CefRequest> {
 public:
  // If |obj| is nullptr the SetHandle method should be used.
  ScopedJNIRequest(JNIEnv* env, CefRefPtr<CefRequest> obj = nullptr);
};

// JNI CefResponse object.
class ScopedJNIResponse : public ScopedJNIObject<CefResponse> {
 public:
  // If |obj| is nullptr the SetHandle method should be used.
  ScopedJNIResponse(JNIEnv* env, CefRefPtr<CefResponse> obj = nullptr);
};

// JNI CefCallback object.
class ScopedJNICallback : public ScopedJNIObject<CefCallback> {
 public:
  // If |obj| is nullptr the SetHandle method should be used.
  ScopedJNICallback(JNIEnv* env, CefRefPtr<CefCallback> obj = nullptr);
};

// JNI BoolRef object.
class ScopedJNIBoolRef : public ScopedJNIBase<jobject> {
 public:
  ScopedJNIBoolRef(JNIEnv* env, bool value);

  // Implicit retrieval of the underlying value.
  operator bool() const;
};

// JNI IntRef object.
class ScopedJNIIntRef : public ScopedJNIBase<jobject> {
 public:
  ScopedJNIIntRef(JNIEnv* env, int value);

  // Implicit retrieval of the underlying value.
  operator int() const;
};

// JNI StringRef object.
class ScopedJNIStringRef : public ScopedJNIBase<jobject> {
 public:
  ScopedJNIStringRef(JNIEnv* env, const CefString& value);

  // Implicit retrieval of the underlying value.
  operator CefString() const;
};

// Helper macros to call a method on the java side.
#define JNI_CALL_METHOD(env, obj, method, sig, type, storeIn, ...)        \
  {                                                                       \
    if (env && obj) {                                                     \
      ScopedJNIClass _cls(env, env->GetObjectClass(obj));                 \
      jmethodID _methodId = env->GetMethodID(_cls, method, sig);          \
      if (_methodId != nullptr) {                                         \
        storeIn = env->Call##type##Method(obj, _methodId, ##__VA_ARGS__); \
      }                                                                   \
      if (env->ExceptionOccurred()) {                                     \
        env->ExceptionDescribe();                                         \
        env->ExceptionClear();                                            \
      }                                                                   \
    }                                                                     \
  }

#define JNI_CALL_VOID_METHOD_EX(env, obj, method, sig, ...)      \
  {                                                              \
    if (env && obj) {                                            \
      ScopedJNIClass _cls(env, env->GetObjectClass(obj));        \
      jmethodID _methodId = env->GetMethodID(_cls, method, sig); \
      if (_methodId != nullptr) {                                \
        env->CallVoidMethod(obj, _methodId, ##__VA_ARGS__);      \
      }                                                          \
    }                                                            \
  }

#define JNI_CALL_VOID_METHOD(env, obj, method, sig, ...)         \
  {                                                              \
    if (env && obj) {                                            \
      ScopedJNIClass _cls(env, env->GetObjectClass(obj));        \
      jmethodID _methodId = env->GetMethodID(_cls, method, sig); \
      if (_methodId != nullptr) {                                \
        env->CallVoidMethod(obj, _methodId, ##__VA_ARGS__);      \
      }                                                          \
      if (env->ExceptionOccurred()) {                            \
        env->ExceptionDescribe();                                \
        env->ExceptionClear();                                   \
      }                                                          \
    }                                                            \
  }

#define JNI_CALL_BOOLEAN_METHOD(out, env, obj, method, sig, ...)     \
  {                                                                  \
    if (env && obj) {                                                \
      ScopedJNIClass _cls(env, env->GetObjectClass(obj));            \
      jmethodID _methodId = env->GetMethodID(_cls, method, sig);     \
      if (_methodId != nullptr) {                                    \
        out = env->CallBooleanMethod(obj, _methodId, ##__VA_ARGS__); \
      }                                                              \
      if (env->ExceptionOccurred()) {                                \
        env->ExceptionDescribe();                                    \
        env->ExceptionClear();                                       \
      }                                                              \
    }                                                                \
  }

// Set the CEF base object for an existing JNI object. A reference will be
// added to the base object. If a previous base object existed a reference
// will be removed from that object.
template <class T>
bool SetCefForJNIObject(JNIEnv* env,
                        jobject obj,
                        T* base,
                        const char* varName) {
  if (!obj)
    return false;

  ScopedJNIString identifer(env, varName);
  jlong previousValue = 0;
  JNI_CALL_METHOD(env, obj, "getNativeRef", "(Ljava/lang/String;)J", Long,
                  previousValue, identifer.get());
  if (previousValue != 0) {
    // Remove a reference from the previous base object.
    SetCefForJNIObjectHelper::Release(reinterpret_cast<T*>(previousValue));
  }

  JNI_CALL_VOID_METHOD(env, obj, "setNativeRef", "(Ljava/lang/String;J)V",
                       identifer.get(), (jlong)base);
  if (base) {
    // Add a reference to the new base object.
    SetCefForJNIObjectHelper::AddRef(base);
  }
  return true;
}

// Retrieve the CEF base object from an existing JNI object.
template <class T>
T* GetCefFromJNIObject(JNIEnv* env, jobject obj, const char* varName) {
  if (!obj)
    return nullptr;

  ScopedJNIString identifer(env, varName);
  jlong previousValue = 0;
  JNI_CALL_METHOD(env, obj, "getNativeRef", "(Ljava/lang/String;)J", Long,
                  previousValue, identifer.get());
  if (previousValue != 0)
    return reinterpret_cast<T*>(previousValue);
  return nullptr;
}

#endif  // JCEF_NATIVE_JNI_SCOPED_HELPERS_H_
