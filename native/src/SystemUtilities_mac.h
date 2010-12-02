// Copyright (c) 2010 Lime Wire, LLC. All rights reserved. Use of this source
// code is governed by a BSD-style license that can be found in the LICENSE file.

#ifndef _SYSTEM_UTILITIES_MAC_H
#define _SYSTEM_UTILITIES_MAC_H

#include <jni.h>
#import <Cocoa/Cocoa.h>

NSView* GetNSView(JNIEnv *env, jobject javaWindow);

#endif // _SYSTEM_UTILITIES_MAC_H
