// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef JCEF_NATIVE_DISPLAY_HANDLER_H_
#define JCEF_NATIVE_DISPLAY_HANDLER_H_
#pragma once

#include <jni.h>

#include "include/cef_audio_handler_capi.h"

#include "jni_scoped_helpers.h"

// DisplayHandler implementation.
class AudioHandler : public CefAudioHandler {
 public:
  AudioHandler(JNIEnv* env, jobject handler);

  // CefDisplayHandler methods:
  int GetAudioParameters(CefRefPtr<CefBrowser> browser,
                       cef_audio_parameters_t params) override;
  void OnAudioStreamStarted(CefRefPtr<CefBrowser> browser,
                     cef_audio_parameters_t params, int channels) override;
  bool OnAudioStreamPacket(CefRefPtr<CefBrowser> browser, float** data, int frames, long long pts) override;
  void OnAudioStreamStopped(CefRefPtr<CefBrowser> browser) override;
  void OnAudioStreamError(CefRefPtr<CefBrowser> browser, CefString& text) override;

 protected:
  ScopedJNIObjectGlobal handle_;

  // Include the default reference counting implementation.
  IMPLEMENT_REFCOUNTING(AudioHandler);
};

#endif  // JCEF_NATIVE_DISPLAY_HANDLER_H_
