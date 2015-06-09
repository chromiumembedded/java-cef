// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "write_handler.h"

#include "jni_util.h"
#include "util.h"

WriteHandler::WriteHandler(JNIEnv* env, jobject jOutputStream) {
  jOutputStream_ = env->NewGlobalRef(jOutputStream);
  offset_ = 0;
}

WriteHandler::~WriteHandler() {
  BEGIN_ENV(env)
  if (jOutputStream_) {
    JNI_CALL_VOID_METHOD(env, jOutputStream_, "close", "()V");
    env->DeleteGlobalRef(jOutputStream_);
  }
  END_ENV(env)
}

size_t WriteHandler::Write(const void* ptr, size_t size, size_t n) {
  base::AutoLock lock_scope(lock_);
  size_t rv = n;
  BEGIN_ENV(env)
  jbyteArray jbyteArray = env->NewByteArray((jsize)(size*n));
  env->SetByteArrayRegion(jbyteArray, 0, (jsize)(size*n), (const jbyte*)ptr);

  JNI_CALL_VOID_METHOD_EX(env, jOutputStream_, "write", "([B)V", jbyteArray);
  if (env->ExceptionOccurred()) {
    env->ExceptionClear();
    rv = 0;
  }
  offset_ += (rv*size);
  END_ENV(env)

  return rv;
}

int WriteHandler::Seek(int64 offset, int whence)  {
  // not supported by Javas OutputStream
  return -1;
}

int64 WriteHandler::Tell() {
  return offset_;
}

int WriteHandler::Flush() {
  BEGIN_ENV(env)
  JNI_CALL_VOID_METHOD(env, jOutputStream_, "flush", "()V");
  END_ENV(env)
  return 0;
}

bool WriteHandler::MayBlock() {
  return false;
}
