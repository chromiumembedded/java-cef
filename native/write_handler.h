// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef JCEF_NATIVE_WRITE_HANDLER_H_
#define JCEF_NATIVE_WRITE_HANDLER_H_
#pragma once

#include <jni.h>
#include "include/cef_stream.h"
#include "include/base/cef_lock.h"

// WriteHandler implementation.
class WriteHandler : public CefWriteHandler {
 public:
  WriteHandler(JNIEnv* env, jobject jOutputStream);
  virtual ~WriteHandler();

  // CefWriteHandler methods
  virtual size_t Write(const void* ptr, size_t size, size_t n) OVERRIDE;
  virtual int Seek(int64 offset, int whence) OVERRIDE;
  virtual int64 Tell() OVERRIDE;
  virtual int Flush() OVERRIDE;
  virtual bool MayBlock() OVERRIDE;

 protected:
  jobject jOutputStream_;
  size_t offset_;

  base::Lock lock_;

  // Include the default reference counting implementation.
  IMPLEMENT_REFCOUNTING(WriteHandler);
};

#endif  // JCEF_NATIVE_WRITE_HANDLER_H_
