// Copyright (c) 2010 Lime Wire, LLC. All rights reserved. Use of this source
// code is governed by a BSD-style license that can be found in the LICENSE file.

#include "CefBrowser_mac.h"
#include "SystemUtilities_mac.h"

void GetWindowInfo(JNIEnv *env, jobject obj, int width, int height,
                   CefWindowInfo& windowInfo)
{
  NSView* view = GetNSView(env, obj);
  windowInfo.SetAsChild(view, 0, 0, width, height);
}

void SetWindowPos(CefWindowHandle window, int width, int height)
{
  // Nothing to do here.
}

void SetWindowText(CefWindowHandle window, const CefString& title)
{
  NSView* view = (NSView*)window;
  std::string titleStr(title);
  NSString* str = [NSString stringWithUTF8String:titleStr.c_str()];
  [[view window] setTitle:str];
}
