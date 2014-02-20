// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#import "util_mac.mm"
#import <Cocoa/Cocoa.h>
#import <jni.h>
#include <objc/runtime.h>
#include <iostream>
#include "include/cef_app.h"
#include "include/cef_application_mac.h"
#include "include/cef_browser.h"
#include "include/cef_path_util.h"
#include "client_handler.h"
#include "jni_util.h"

namespace {

BOOL g_handling_send_event = false;
CefRefPtr<ClientHandler> g_client_handler = NULL;

}  // namespace

// Java provides an NSApplicationAWT implementation that we can't access or
// override directly. Therefore add the necessary CrAppControlProtocol
// functionality to NSApplication using categories and swizzling.
@interface NSApplication (JCEFApplication)

- (BOOL)isHandlingSendEvent;
- (void)setHandlingSendEvent:(BOOL)handlingSendEvent;
- (void)_swizzled_sendEvent:(NSEvent*)event;

@end

@implementation NSApplication (JCEFApplication)

// This selector is called very early during the application initialization.
+ (void)load {
  // Swap NSApplication::sendEvent with _swizzled_sendEvent.
  Method original = class_getInstanceMethod(self, @selector(sendEvent));
  Method swizzled =
      class_getInstanceMethod(self, @selector(_swizzled_sendEvent));
  method_exchangeImplementations(original, swizzled);
}

- (BOOL)isHandlingSendEvent {
  return g_handling_send_event;
}

- (void)setHandlingSendEvent:(BOOL)handlingSendEvent {
  g_handling_send_event = handlingSendEvent;
}

- (void)_swizzled_sendEvent:(NSEvent*)event {
  CefScopedSendingEvent sendingEventScoper;
  // Calls NSApplication::sendEvent due to the swizzling.
  [self _swizzled_sendEvent:event];
}

@end

// This is only a small stub class to transport data between
// JNI function calls and the Obj-C implementations. The Obj-C
// implementation is addressed by calling "performSelectorOnMainThread".
@interface BrowserStub : NSObject {
  jobject canvas_;
  jlong windowHandle_;
  jstring url_;
  jobject result_;
  jobject cefClient_;
  jboolean transparent_;
}

- (id) initWithValues:(jobject)cefClient
         windowHandle:(jlong)windowHandle
               urlStr:(jstring)url
        isTransparent:(jboolean)transparent
               canvas:(jobject)canvas;
- (jobject) canvas;
- (jlong) windowHandle;
- (jstring) url;
- (jboolean) transparent;
- (jobject) cefClient;
- (void) setResult:(jobject)result;
- (jobject) result;

@end  // interface BrowserStub

@implementation BrowserStub

- (id) initWithValues:(jobject)cefClient
         windowHandle:(jlong)windowHandle
               urlStr:(jstring)url
        isTransparent:(jboolean)transparent
               canvas:(jobject)canvas {
  if (self = [super init]) {
    cefClient_ = cefClient;
    windowHandle_ = windowHandle;
    url_ = url;
    transparent_ = transparent;
    canvas_ = canvas;
  }
  return self;
}
- (jobject) canvas {
  return canvas_;
}
- (jlong) windowHandle {
  return windowHandle_;
}
- (jstring) url {
  return url_;
}
- (jboolean) transparent {
  return transparent_;
}
- (jobject) cefClient {
  return cefClient_;
}
- (void) setResult:(jobject)result {
  result_ = result;
}
- (jobject) result {
  return result_;
}

@end  // implementation BrowserStub

@interface SystemStub : NSObject {
  CefMainArgs args_;
  CefSettings settings_;
  CefRefPtr<CefApp> application_;
  bool result_;
}

- (id) initWithValues:(const CefMainArgs&)args
             settings:(const CefSettings&) settings
          application:(CefRefPtr<CefApp>) application;
- (const CefMainArgs&) args;
- (const CefSettings&) settings;
- (CefRefPtr<CefApp>) application;
- (void) setResult:(bool)result;
- (bool) result;

@end  // interface SystemStub

@implementation SystemStub

- (id) initWithValues:(const CefMainArgs&) pArgs
             settings:(const CefSettings&) pSettings
          application:(CefRefPtr<CefApp>)  pApplication {

  if (self = [super init]) {
    args_ = pArgs;
    settings_ = pSettings;
    application_ = pApplication;
    result_ = false;
  }
  return self;
}
- (const CefMainArgs&) args {
  return args_;
}
- (const CefSettings&) settings {
  return settings_;
}
- (CefRefPtr<CefApp>) application {
  return application_;
}
- (void) setResult:(bool)result {
  result_ = result;
}
- (bool) result {
  return result_;
}

@end  // implementation SystemStub

// Obj-C Wrapper Class to be called by "performSelectorOnMainThread".
@interface CefHandler : NSObject { }

+ (void) initialize:(SystemStub*)stub;
+ (void) quitMessageLoop;
+ (void) createBrowser:(BrowserStub*)stub;

@end  // interface CefHandler

@implementation CefHandler

+ (void) initialize:(SystemStub*)stub {
  // Initialize CEF
  bool result = CefInitialize([stub args], [stub settings], [stub application],
                              NULL);
  [stub setResult:result];
  // Run the application message loop.
  CefRunMessageLoop();
  // Shut down CEF.
  CefShutdown();
  // Free memory.
  g_client_handler = NULL;
  [stub setResult:true];
}

+ (void) quitMessageLoop {
  CefQuitMessageLoop();
}

+ (void) createBrowser:(BrowserStub*)stub {
  BEGIN_ENV(env)
  jobject browser = NewJNIObject(env, "org/cef/CefBrowser_N");
  if (browser) {
    g_client_handler = new ClientHandler(env, browser, [stub cefClient]);
    CefWindowInfo windowInfo;
    if (![stub canvas]) {
      windowInfo.SetAsOffScreen((CefWindowHandle)[stub windowHandle]);
      windowInfo.SetTransparentPainting([stub transparent]);
    } else {
      CefRect rect;
      g_client_handler->GetViewRect(NULL, rect);
      int screenX, screenY;
      g_client_handler->GetScreenPoint(NULL, rect.x, rect.y, screenX, screenY);
      NSWindow* window = (NSWindow*)[stub windowHandle];
      NSView* parentView = [window contentView];
      NSRect bounds = [parentView bounds];
      // translate java's window origin to Obj-C's window origin
      int systemX = rect.x;
      int systemY = bounds.size.height - rect.height - rect.y;
      windowInfo.SetAsChild(parentView, systemX, systemY, rect.width, rect.height);
    }
    CefBrowserSettings settings;
    CefRefPtr<CefBrowser> browserObj;
    CefString strUrl = GetJNIString(env, [stub url]);
    browserObj = CefBrowserHost::CreateBrowserSync(windowInfo,
                                                   g_client_handler.get(),
                                                   strUrl,
                                                   settings,
                                                   NULL);
    if ([stub canvas]) {
      NSView* browserView = browserObj->GetHost()->GetWindowHandle();
      [browserView setWantsLayer:YES];
      AddLayerToComponent([stub canvas], env, browserView);
    }
    SetCefForJNIObject(env, browser, browserObj.get());
  }
  [stub setResult:browser];
  END_ENV(env)
}

@end  // implementation CefHandler

namespace util_mac {

std::string GetAbsPath(const std::string& path) {
  char full_path[PATH_MAX];
  if (realpath(path.c_str(), full_path) == NULL)
    return std::string();
  return full_path;
}

bool IsNSView(void* ptr) {
  id obj = (id)ptr;
  bool result = [obj isKindOfClass:[NSView class]];
  if (!result)
    NSLog(@"Expected NSView, found %@", NSStringFromClass([obj class]));
  return result;
}

bool CefInitializeOnMainThread(const CefMainArgs& args,
                               const CefSettings& settings,
                               CefRefPtr<CefApp> application) {
  SystemStub* stub = [[SystemStub alloc] initWithValues:args
                                               settings:settings
                                            application:application];
  [[CefHandler class] performSelectorOnMainThread:@selector(initialize:)
                                       withObject:stub
                                    waitUntilDone:YES];
  bool result = [stub result];
  [stub release];
  return result;
}

void CefQuitMessageLoopOnMainThread() {
  [[CefHandler class] performSelectorOnMainThread:@selector(quitMessageLoop)
                                       withObject:nil
                                    waitUntilDone:YES];
}

jobject CefCreateBrowserOnMainThread(jobject jCefClient,
                                     jlong windowHandle,
                                     jstring url,
                                     jboolean transparent,
                                     jobject canvas) {
  BrowserStub* stub = [[BrowserStub alloc] initWithValues:jCefClient
                                             windowHandle:windowHandle
                                                   urlStr:url
                                            isTransparent:transparent
                                                   canvas:canvas];
  [[CefHandler class] performSelectorOnMainThread:@selector(createBrowser:)
                                       withObject:stub
                                    waitUntilDone:YES];
  jobject result = [stub result];
  [stub release];
  return result;
}
}  // namespace util_mac
