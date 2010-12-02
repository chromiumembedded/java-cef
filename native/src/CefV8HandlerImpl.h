// Copyright (c) 2010 Lime Wire, LLC. All rights reserved. Use of this source
// code is governed by a BSD-style license that can be found in the LICENSE file.

#ifndef _CEFV8HANDLERIMPL_H
#define _CEFV8HANDLERIMPL_H

#include <jni.h>
#include "cef.h"

class CefV8HandlerImpl : public CefThreadSafeBase<CefV8Handler>
{
public:
	CefV8HandlerImpl(JNIEnv* env, jobject handler);
	virtual ~CefV8HandlerImpl();

	virtual bool Execute(const CefString& name,
                       CefRefPtr<CefV8Value> object,
                       const CefV8ValueList& arguments,
                       CefRefPtr<CefV8Value>& retval,
                       CefString& exception);

	jobject handler() { return jhandler_; }

private:
	jobject jhandler_;
};

#endif // _CEFV8HANDLERIMPL_H
