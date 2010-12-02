// Copyright (c) 2010 Lime Wire, LLC. All rights reserved. Use of this source
// code is governed by a BSD-style license that can be found in the LICENSE file.

#include "CefBaseImpl.h"
#include "SystemUtilities.h"

CefBaseImpl::CefBaseImpl(JNIEnv* env, jobject object)
{
	// Create a new global reference.
	jobject_ = env->NewGlobalRef(object);
}

CefBaseImpl::~CefBaseImpl()
{
	// Release the global reference.
	JNIEnv* env = GetJNIEnv();
	if(env)
		env->DeleteGlobalRef(jobject_);
}
