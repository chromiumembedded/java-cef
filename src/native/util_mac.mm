// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#import "util_mac.mm"
#import <Cocoa/Cocoa.h>
#include <objc/runtime.h>
#include "include/cef_application_mac.h"

namespace {

BOOL g_handling_send_event = false;

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

}  // namespace util_mac
