// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "audio_handler.h"

#include "jni_util.h"

AudioHandler::AudioHandler(JNIEnv* env, jobject handler)
    : handle_(env, handler) {}

bool AudioHandler::GetAudioParameters(CefRefPtr<CefBrowser> browser,
                                     CefAudioParameters& params) {
  ScopedJNIEnv env;
  if (!env)
    return true;

  ScopedJNIBrowser jbrowser(env, browser);

  jboolean jreturn = JNI_FALSE;

  JNI_CALL_METHOD(env, handle_, "getAudioParameters",
                       "(Lorg/cef/browser/CefBrowser;Ljava/lang/Object;)Z", Boolean,
                       jreturn, jbrowser.get(), nullptr);

  return (jreturn != JNI_FALSE);
}

void AudioHandler::OnAudioStreamStarted(CefRefPtr<CefBrowser> browser,
                                   const CefAudioParameters& params, int channels) {
  ScopedJNIEnv env;
  if (!env)
    return;

  ScopedJNIBrowser jbrowser(env, browser);

  JNI_CALL_VOID_METHOD(env, handle_, "onAudioStreamStarted",
                       "(Lorg/cef/browser/CefBrowser;Ljava/lang/Object;I)V",
                       jbrowser.get(), nullptr, channels);
}

void AudioHandler::OnAudioStreamPacket(CefRefPtr<CefBrowser> browser, const float** data, int frames, int64_t pts) {
  ScopedJNIEnv env;
  if (!env)
    return;

  ScopedJNIBrowser jbrowser(env, browser);

  // TODO: this should truthfully be using a float buffer, but I'm not yet sure how to do that from JNI
  jfloatArray jArray = env->NewFloatArray(frames);
  int size = frames;
  jfloat* fill = (jfloat*) malloc(size * sizeof(jfloat));
  for (int i = 0; i < size; i++)
    fill[i] = data[0][i];
  env->SetFloatArrayRegion(jArray, 0, size, fill);

  JNI_CALL_VOID_METHOD(env, handle_, "onAudioStreamPacket",
                  "(Lorg/cef/browser/CefBrowser;[FIJ)V",
                  jbrowser.get(), jArray, frames, (long long) pts);
}

void AudioHandler::OnAudioStreamStopped(CefRefPtr<CefBrowser> browser) {
  ScopedJNIEnv env;
  if (!env)
    return;

  ScopedJNIBrowser jbrowser(env, browser);

  JNI_CALL_VOID_METHOD(env, handle_, "onAudioStreamStopped",
                       "(Lorg/cef/browser/CefBrowser;)V",
                       jbrowser.get());
}

void AudioHandler::OnAudioStreamError(CefRefPtr<CefBrowser> browser,
                                      const CefString& text) {
  ScopedJNIEnv env;
  if (!env)
    return;

  ScopedJNIBrowser jbrowser(env, browser);
  ScopedJNIString jtext(env, text);

  JNI_CALL_VOID_METHOD(env, handle_, "onAudioStreamError",
                       "(Lorg/cef/browser/CefBrowser;Ljava/lang/String;)V",
                       jbrowser.get(), jtext.get());
}
