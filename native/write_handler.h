// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef JCEF_NATIVE_WRITE_HANDLER_H_
#define JCEF_NATIVE_WRITE_HANDLER_H_
#pragma once

#include <jni.h>

#include "include/base/cef_lock.h"
#include "include/cef_stream.h"

#include "jni_scoped_helpers.h"

// WriteHandler implementation.
class WriteHandler : public CefWriteHandler {
 public:
  WriteHandler(JNIEnv* env, jobject jOutputStream);
  virtual ~WriteHandler();

  // CefWriteHandler methods
  virtual size_t Write(const void* ptr, size_t size, size_t n) override;
  virtual int Seek(int64_t offset, int whence) override;
  virtual int64_t Tell() override;
  virtual int Flush() override;
  virtual bool MayBlock() override;

 protected:
  ScopedJNIObjectGlobal handle_;
  size_t offset_ = 0;

  base::Lock lock_;

  // Include the default reference counting implementation.
  IMPLEMENT_REFCOUNTING(WriteHandler);
};

#endif  // JCEF_NATIVE_WRITE_HANDLER_H_
