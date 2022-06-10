// Copyright (c) 2015 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "temp_window_win.h"

#include <windows.h>

#include "include/base/cef_logging.h"

namespace {

const wchar_t kWndClass[] = L"Client_TempWindow";

// Create the temp window.
HWND CreateTempWindow() {
  HINSTANCE hInstance = ::GetModuleHandle(nullptr);

  WNDCLASSEX wc = {0};
  wc.cbSize = sizeof(wc);
  wc.lpfnWndProc = DefWindowProc;
  wc.hInstance = hInstance;
  wc.lpszClassName = kWndClass;
  RegisterClassEx(&wc);

  // Create a 1x1 pixel hidden window.
  return CreateWindow(kWndClass, 0, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN, 0, 0,
                      1, 1, nullptr, nullptr, hInstance, nullptr);
}

TempWindowWin* g_temp_window = nullptr;

}  // namespace

TempWindowWin::TempWindowWin() : hwnd_(nullptr) {
  DCHECK(!g_temp_window);
  g_temp_window = this;

  hwnd_ = CreateTempWindow();
  CHECK(hwnd_);
}

TempWindowWin::~TempWindowWin() {
  g_temp_window = nullptr;
  DCHECK(hwnd_);
  DestroyWindow(hwnd_);
}

// static
CefWindowHandle TempWindowWin::GetWindowHandle() {
  if (!g_temp_window)
    return kNullWindowHandle;
  return g_temp_window->hwnd_;
}
