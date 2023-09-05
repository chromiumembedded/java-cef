// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "write_handler.h"

#include "jni_util.h"
#include "util.h"

WriteHandler::WriteHandler(JNIEnv* env, jobject jOutputStream)
    : handle_(env, jOutputStream) {}

WriteHandler::~WriteHandler() {
  ScopedJNIEnv env;
  if (!env)
    return;

  if (handle_) {
    JNI_CALL_VOID_METHOD(env, handle_, "close", "()V");
  }
}

size_t WriteHandler::Write(const void* ptr, size_t size, size_t n) {
  base::AutoLock lock_scope(lock_);
  size_t rv = n;

  ScopedJNIEnv env;
  if (!env)
    return rv;

  jbyteArray jbyteArray = env->NewByteArray((jsize)(size * n));
  env->SetByteArrayRegion(jbyteArray, 0, (jsize)(size * n), (const jbyte*)ptr);

  JNI_CALL_VOID_METHOD_EX(env, handle_, "write", "([B)V", jbyteArray);
  if (env->ExceptionOccurred()) {
    env->ExceptionClear();
    rv = 0;
  }
  offset_ += (rv * size);
  env->DeleteLocalRef(jbyteArray);

  return rv;
}

int WriteHandler::Seek(int64_t offset, int whence) {
  // not supported by Javas OutputStream
  return -1;
}

int64_t WriteHandler::Tell() {
  return offset_;
}

int WriteHandler::Flush() {
  ScopedJNIEnv env;
  if (env) {
    JNI_CALL_VOID_METHOD(env, handle_, "flush", "()V");
  }
  return 0;
}

bool WriteHandler::MayBlock() {
  return false;
}
