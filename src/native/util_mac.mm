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
#include "client_app.h"
#include "client_handler.h"
#include "render_handler.h"
#include "jni_util.h"

namespace {

static CefRefPtr<ClientApp> g_client_app_ = NULL;
BOOL g_handling_send_event = false;
bool g_init_result = false;
NSCondition *g_lock_initialize = NULL;
BOOL g_continue_terminate = false;
id g_last_sender = nil;

}  // namespace

// Java provides an NSApplicationAWT implementation that we can't access or
// override directly. Therefore add the necessary CrAppControlProtocol
// functionality to NSApplication using categories and swizzling.
@interface NSApplication (JCEFApplication)

- (BOOL)isHandlingSendEvent;
- (void)setHandlingSendEvent:(BOOL)handlingSendEvent;
- (void)_swizzled_sendEvent:(NSEvent*)event;
- (void)_swizzled_terminate:(id)sender;

@end

@implementation NSApplication (JCEFApplication)

// This selector is called very early during the application initialization.
+ (void)load {
  // Swap NSApplication::sendEvent with _swizzled_sendEvent.
  Method original = class_getInstanceMethod(self, @selector(sendEvent));
  Method swizzled =
      class_getInstanceMethod(self, @selector(_swizzled_sendEvent));
  method_exchangeImplementations(original, swizzled);

  Method originalTerm = class_getInstanceMethod(self, @selector(terminate:));
  Method swizzledTerm =
      class_getInstanceMethod(self, @selector(_swizzled_terminate:));
  method_exchangeImplementations(originalTerm, swizzledTerm);
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

- (void)_swizzled_terminate:(id)sender {
  bool continueTerminate = true;
  if (g_client_app_.get() && !g_continue_terminate) {
    g_last_sender = sender;
    continueTerminate = !g_client_app_->HandleTerminate();
  }
  if (continueTerminate) {
    [self _swizzled_terminate:sender];
  }
}

@end

// This is only a small stub class to transport data between
// JNI function calls and the Obj-C implementations. The Obj-C
// implementation is addressed by calling "performSelectorOnMainThread".
@interface SystemStub : NSObject {
  CefMainArgs args_;
  CefSettings settings_;
  CefRefPtr<ClientApp> application_;
  bool result_;
}

- (id) initWithValues:(const CefMainArgs&)args
             settings:(const CefSettings&) settings
          application:(CefRefPtr<ClientApp>) application;
- (const CefMainArgs&) args;
- (const CefSettings&) settings;
- (CefRefPtr<ClientApp>) application;
- (void) setResult:(bool)result;
- (bool) result;

@end  // interface SystemStub

@implementation SystemStub

- (id) initWithValues:(const CefMainArgs&) pArgs
             settings:(const CefSettings&) pSettings
          application:(CefRefPtr<ClientApp>)  pApplication {

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
- (CefRefPtr<ClientApp>) application {
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
  g_client_app_ = [stub application];
  g_init_result =
      CefInitialize([stub args], [stub settings], g_client_app_.get(), NULL);
  [g_lock_initialize signal];
  [g_lock_initialize unlock];

  // Run the application message loop.
  CefRunMessageLoop();
  g_client_app_ = NULL;

  // Shut down CEF.
  [g_lock_initialize lock];
  CefShutdown();
  [g_lock_initialize signal];
  [g_lock_initialize unlock];
}

+ (void) quitMessageLoop {
  CefQuitMessageLoop();
}

@end  // implementation CefHandler

// Instead of adding CefBrowser as child of the windows main view, a content
// view (CefBrowserContentView) is set between the main view and the
// CefBrowser. Why?
//
// The CefBrowserContentView defines the viewable part of the browser view.
// In most cases the content view has the same size as the browser view,
// but for example if you add CefBrowser to a JScrollPane, you only want to
// see the portion of the browser window you're scrolled to. In this case 
// the sizes of the content view and the browser view differ.
// 
// With other words the CefBrowserContentView clips the CefBrowser to its
// displayable content.
//
// +- - - - - - - - - - - - - - - - - - - - -+
// |/   / CefBrowser/   /   /   /   /   /   /|
//    +-------------------------+  /   /   / <--- invisible part of CefBrowser
// |  | CefBrowserContentView   | /   /   /  |
//   /|                         |/   /   /
// |/ |                         |   /   /   /|
//    |                       <------------------ visible part of CefBrowser
// |  |                         | /   /   /  |
//   /|                         |/   /   /
// |/ |                         |   /   /   /|
//    |                         |  /   /   /
// |  +-------------------------+ /   /   /  |
//   /   /   /   /   /   /   /   /   /   /
// |/   /   /   /   /   /   /   /   /   /   /|
//     /   /   /   /   /   /   /   /   /   / 
// |  /   /   /   /   /   /   /   /   /   /  |
// +- - - - - - - - - - - - - - - - - - - - -+
@interface CefBrowserContentView : NSView {
  CefRefPtr<CefBrowser> cefBrowser;
}

@property(readonly) BOOL isLiveResizing;

-(void) addCefBrowser:(CefRefPtr<CefBrowser>)browser;
-(void) removeCefBrowser;
-(void) updateView:(NSDictionary*)dict;
@end  // interface CefBrowserContentView

@implementation CefBrowserContentView

@synthesize isLiveResizing;

-(id)initWithFrame:(NSRect)frameRect {
  self = [super initWithFrame:frameRect];
  cefBrowser = NULL;
  return self;
}

-(void)dealloc {
  [[NSNotificationCenter defaultCenter] removeObserver:self];
  cefBrowser = NULL;
  [super dealloc];
}

-(void) setFrame:(NSRect)frameRect {
  // Instead of using the passed frame, get the visible rect from java because
  // the passed frame rect doesn't contain the clipped view part. Otherwise
  // we'll overlay some parts of the Java UI.
  if (cefBrowser.get()) {
    CefRefPtr<CefRenderHandler> renderer =
        cefBrowser->GetHost()->GetClient()->GetRenderHandler();

    CefRect rect;
    renderer->GetViewRect(cefBrowser, rect);
    util_mac::TranslateRect(self, rect);
    frameRect = (NSRect){{rect.x, rect.y}, {rect.width, rect.height}};
  }
  [super setFrame:frameRect];
}

-(void) addCefBrowser:(CefRefPtr<CefBrowser>)browser {
  cefBrowser = browser;
  // Register for the start and end events of "liveResize" to avoid
  // Java paint updates while someone is resizing the main window (e.g. by
  // pulling with the mouse cursor) 
  [[NSNotificationCenter defaultCenter] addObserver:self
      selector:@selector(windowWillStartLiveResize:)
      name:NSWindowWillStartLiveResizeNotification
      object:[self window]];
  [[NSNotificationCenter defaultCenter]
      addObserver:self
      selector:@selector(windowDidEndLiveResize:)
      name:NSWindowDidEndLiveResizeNotification
      object:[self window]];
}

-(void) removeCefBrowser {
  [[NSNotificationCenter defaultCenter] removeObserver:self];
  cefBrowser = NULL;
  [self removeFromSuperview];
}

- (void)windowWillStartLiveResize:(NSNotification *)notification {
  isLiveResizing = YES;
}

- (void)windowDidEndLiveResize:(NSNotification *)notification {
  isLiveResizing = NO;
  [self setFrame: [self frame]];
}

-(void) updateView:(NSDictionary*)dict {
  NSRect contentRect = NSRectFromString([dict valueForKey:@"content"]);
  NSRect browserRect = NSRectFromString([dict valueForKey:@"browser"]);

  NSArray* childs = [self subviews];
  for (NSView* child in childs) {
    [child setFrame:browserRect];
    [child setNeedsDisplay:YES];
  }
  [super setFrame:contentRect];
  [self setNeedsDisplay:YES];
}

@end  // implementation CefBrowserContentView


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

CefWindowHandle CreateBrowserContentView(CefWindowHandle cefWindow,
    CefRect& orig) {
  NSWindow* window = (NSWindow*)cefWindow;
  NSView* mainView = (CefWindowHandle)[window contentView];
  TranslateRect(mainView, orig);
  NSRect frame = {{orig.x,orig.y},{orig.width,orig.height}};

  CefBrowserContentView* contentView =
      [[CefBrowserContentView alloc] initWithFrame:frame];

  [mainView addSubview:contentView];
  [contentView setAutoresizingMask:(NSViewWidthSizable|NSViewHeightSizable)];
  [contentView setNeedsDisplay:YES];
  [contentView release];

  // Override origin bevore "orig" is returned because the new origin is
  // relative to the created CefBrowserContentView object 
  orig.x = 0;
  orig.y = 0;
  return contentView;
}

// translate java's window origin to Obj-C's window origin
void TranslateRect(CefWindowHandle view, CefRect& orig) {
  NSRect bounds = [[[view window] contentView] bounds];
  orig.y = bounds.size.height - orig.height - orig.y;
}

bool CefInitializeOnMainThread(const CefMainArgs& args,
                               const CefSettings& settings,
                               CefRefPtr<ClientApp> application) {
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
  if (g_client_app_ == NULL)
    return;

  g_lock_initialize = [[NSCondition alloc] init];
  [g_lock_initialize lock];
  [[CefHandler class] performSelectorOnMainThread:@selector(quitMessageLoop)
                                       withObject:nil
                                    waitUntilDone:NO];
  [g_lock_initialize wait];
  [g_lock_initialize unlock];
  [g_lock_initialize release];
}

void SetVisibility(CefWindowHandle handle, bool isVisible) {
  bool isHidden = [handle isHidden];
  if (isHidden == isVisible) {
    [handle setHidden:!isVisible];
    [handle needsDisplay];
    [[handle superview] display];
  }
}

void AddCefBrowser(CefRefPtr<CefBrowser> browser) {
  if (!browser.get())
    return;
  CefWindowHandle handle = browser->GetHost()->GetWindowHandle();
  CefBrowserContentView *browserImpl =
      (CefBrowserContentView*)[handle superview];
  [browserImpl addCefBrowser:browser];
}

void UpdateView(CefWindowHandle handle, CefRect contentRect,
    CefRect browserRect) {
  util_mac::TranslateRect(handle, contentRect);
  CefBrowserContentView *browser = (CefBrowserContentView*)[handle superview];
  browserRect.y = contentRect.height - browserRect.height - browserRect.y;

  // Only update the view if nobody is currently resizing the main window.
  // Otherwise the CefBrowser part may start flickering because there's a
  // significant delay between the native window resize event and the java
  // resize event
  if (![browser isLiveResizing]) {
    NSString *contentStr = [[NSString alloc] initWithFormat:@"{{%d,%d},{%d,%d}",
        contentRect.x, contentRect.y, contentRect.width, contentRect.height];
    NSString *browserStr = [[NSString alloc] initWithFormat:@"{{%d,%d},{%d,%d}",
        browserRect.x, browserRect.y, browserRect.width, browserRect.height];
    NSDictionary* dict =
        [[NSDictionary alloc] initWithObjectsAndKeys:contentStr,@"content",
                                                     browserStr,@"browser",
                                                     nil];
    [browser performSelectorOnMainThread:@selector(updateView:) withObject:dict
        waitUntilDone:NO];
  }
}

void RemoveCefBrowser(CefRefPtr<CefBrowser> browser) {
  if (!browser.get())
    return;
  CefWindowHandle handle = browser->GetHost()->GetWindowHandle();

  // There are some cases where the superview of CefBrowser isn't
  // a CefBrowserContentView. For example if another CefBrowser window was
  // created by calling "window.open()" in JavaScript.
  NSView* superView = [handle superview];
  if ([superView isKindOfClass:[CefBrowserContentView class]]) {
    CefBrowserContentView *browserView =
        (CefBrowserContentView*)[handle superview];
    [browserView removeCefBrowser];
  }
}

void ContinueDefaultTerminate() {
  g_continue_terminate = TRUE;
  [[NSApplication sharedApplication] performSelectorOnMainThread:
      @selector(terminate:) withObject:g_last_sender waitUntilDone:NO];
}

}  // namespace util_mac
