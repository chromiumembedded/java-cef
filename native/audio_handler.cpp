// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "audio_handler.h"

#include "jni_util.h"

AudioHandler::AudioHandler(JNIEnv* env, jobject handler)
    : handle_(env, handler) {}

int AudioHandler::GetAudioParameters(CefRefPtr<CefBrowser> browser,
                                     cef_audio_parameters_t* params) {
  ScopedJNIEnv env;
  if (!env)
    return 0;

  ScopedJNIBrowser jbrowser(env, browser);

  return JNI_CALL_INT_METHOD(env, handle_, "getAudioParameters",
                       "(Lorg/cef/browser/CefBrowser;Ljava/lang/Object;)I",
                       jbrowser.get(), nullptr);
}

void AudioHandler::OnAudioStreamStarted(CefRefPtr<CefBrowser> browser,
                                   cef_audio_parameters_t params, int channels) {
  ScopedJNIEnv env;
  if (!env)
    return;

  ScopedJNIBrowser jbrowser(env, browser);

  JNI_CALL_VOID_METHOD(env, handle_, "onAudioStreamStarted",
                       "(Lorg/cef/browser/CefBrowser;Ljava/lang/Object;I)V",
                       jbrowser.get(), nullptr, channels);
}

void AudioHandler::OnAudioStreamPacket(CefRefPtr<CefBrowser> browser, float** data, int frames, long long pts) {
  ScopedJNIEnv env;
  if (!env)
    return false;

  ScopedJNIBrowser jbrowser(env, browser);
  ScopedJNIString jtext(env, text);
  jboolean jreturn = JNI_FALSE;

  // TODO: this is based on a bit of an assumption
  JNI_CALL_METHOD(env, handle_, "onAudioStreamPacket",
                  "(Lorg/cef/browser/CefBrowser;Ljava/lang/String;[[FIL]])Z", Boolean,
                  jreturn, jbrowser.get(), jtext.get()); // TODO:
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
    return false;

  ScopedJNIBrowser jbrowser(env, browser);
  ScopedJNIString jtext(env, text);

  JNI_CALL_VOID_METHOD(env, handle_, "onAudioStreamError",
                       "(Lorg/cef/browser/CefBrowser;Ljava/lang/String;)V",
                       jbrowser.get(), text.get());
}
