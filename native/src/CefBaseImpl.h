// Copyright (c) 2010 Lime Wire, LLC. All rights reserved. Use of this source
// code is governed by a BSD-style license that can be found in the LICENSE file.

#ifndef _CEFBASEIMPL_H
#define _CEFBASEIMPL_H

#include <jni.h>
#include "cef.h"

class CefBaseImpl : public CefThreadSafeBase<CefBase>
{
public:
	CefBaseImpl(JNIEnv* env, jobject object);
	virtual ~CefBaseImpl();

	jobject object() { return jobject_; }

private:
	jobject jobject_;
};

#endif // _CEFBASEIMPL_H
