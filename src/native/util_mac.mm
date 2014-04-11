// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#import "util_mac.h"
#import <Cocoa/Cocoa.h>
#import <Foundation/NSLock.h>
#import <jni.h>
#include <objc/runtime.h>
#include "include/cef_app.h"
#include "include/cef_application_mac.h"
#include "include/cef_browser.h"
#include "include/cef_path_util.h"
#include "client_handler.h"
#include "render_handler.h"
#include "jni_util.h"

namespace {

BOOL g_handling_send_event = false;
bool g_init_result = false;
NSCondition *g_lock_initialize = NULL;

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

@end  // interface CefHandler

@implementation CefHandler

+ (void) initialize:(SystemStub*)stub {
  // Initialize CEF
  [g_lock_initialize lock];
  CefRefPtr<CefApp> app = [stub application];
  g_init_result = CefInitialize([stub args], [stub settings], app, NULL);
  [g_lock_initialize signal];
  [g_lock_initialize unlock];

  // Run the application message loop.
  CefRunMessageLoop();
  // Shut down CEF.
  CefShutdown();
}

+ (void) quitMessageLoop {
  CefQuitMessageLoop();
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

CefWindowHandle GetParentView(CefWindowHandle childView) {
  NSWindow* window = (NSWindow*)childView;
  return (CefWindowHandle)[window contentView];
}

// translate java's window origin to Obj-C's window origin
void TranslateRect(CefWindowHandle view, CefRect& orig) {
  orig.y = [view bounds].size.height - orig.height - orig.y;
}

bool CefInitializeOnMainThread(const CefMainArgs& args,
                               const CefSettings& settings,
                               CefRefPtr<CefApp> application) {
  SystemStub* stub = [[SystemStub alloc] initWithValues:args
                                               settings:settings
                                            application:application];
  // we're not using waitUntilDone here because we want to return true
  // or false to the caller of this method.
  g_lock_initialize = [[NSCondition alloc] init];
  [g_lock_initialize lock];
  [[CefHandler class] performSelectorOnMainThread:@selector(initialize:)
                                       withObject:stub
                                    waitUntilDone:NO];
  [g_lock_initialize wait];
  [g_lock_initialize unlock];
  [g_lock_initialize release];
  [stub release];
  return g_init_result;
}

void CefQuitMessageLoopOnMainThread() {
  [[CefHandler class] performSelectorOnMainThread:@selector(quitMessageLoop)
                                       withObject:nil
                                    waitUntilDone:YES];
}
}  // namespace util_mac
