// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "util.h"

#include <windows.h>

#include <tchar.h>
#include <tlhelp32.h>
#include <windows.h>
#include <iostream>
#include <map>
#include <sstream>

#ifdef USING_JAVA
#include "client_handler.h"
#include "jni_util.h"
#include "temp_window.h"
#endif

#include "include/base/cef_callback.h"
#include "include/cef_path_util.h"

#define XBUTTON1_HI (XBUTTON1 << 16)
#undef MOUSE_MOVED

namespace util {

int GetPid() {
  return (int)GetCurrentProcessId();
}

int GetParentPid() {
  DWORD pid = GetCurrentProcessId();
  int ppid = 0;
  HANDLE hProcess;
  PROCESSENTRY32 pe32;

  hProcess = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
  if (hProcess == INVALID_HANDLE_VALUE)
    return ppid;

  pe32.dwSize = sizeof(PROCESSENTRY32);
  if (!Process32First(hProcess, &pe32)) {
    CloseHandle(hProcess);
    return ppid;
  }

  do {
    if (pe32.th32ProcessID == pid) {
      ppid = (int)pe32.th32ParentProcessID;
      break;
    }
  } while (Process32Next(hProcess, &pe32));

  CloseHandle(hProcess);
  return ppid;
}

std::string GetTempFileName(const std::string& identifer, bool useParentId) {
  std::stringstream tmpName;
  CefString tmpPath;
  if (!CefGetPath(PK_DIR_TEMP, tmpPath)) {
    TCHAR lpPathBuffer[MAX_PATH];
    GetTempPath(MAX_PATH, lpPathBuffer);
    tmpPath.FromWString(lpPathBuffer);
  }
  tmpName << tmpPath.ToString().c_str() << "\\";
  tmpName << "jcef-p" << (useParentId ? util::GetParentPid() : util::GetPid());
  tmpName << (identifer.empty() ? "" : "_") << identifer.c_str() << ".tmp";
  return tmpName.str();
}

#ifdef USING_JAVA

static int getModifiers(BOOLEAN forceShift) {
  ScopedJNIEnv env;
  if (!env)
    return 0;

  int modifiers = 0;

  const char* inputEventClassName = "java/awt/event/InputEvent";
  JNI_STATIC_DEFINE_INT_RV(env, ScopedJNIClass(env, inputEventClassName),
                           ALT_DOWN_MASK, 0);
  JNI_STATIC_DEFINE_INT_RV(env, ScopedJNIClass(env, inputEventClassName),
                           CTRL_DOWN_MASK, 0);
  JNI_STATIC_DEFINE_INT_RV(env, ScopedJNIClass(env, inputEventClassName),
                           SHIFT_DOWN_MASK, 0);
  JNI_STATIC_DEFINE_INT_RV(env, ScopedJNIClass(env, inputEventClassName),
                           BUTTON1_DOWN_MASK, 0);
  JNI_STATIC_DEFINE_INT_RV(env, ScopedJNIClass(env, inputEventClassName),
                           BUTTON2_DOWN_MASK, 0);
  JNI_STATIC_DEFINE_INT_RV(env, ScopedJNIClass(env, inputEventClassName),
                           BUTTON3_DOWN_MASK, 0);

  if ((GetKeyState(VK_MENU) & 0x8000) != 0)
    modifiers |= JNI_STATIC(ALT_DOWN_MASK);
  if ((GetKeyState(VK_CONTROL) & 0x8000) != 0)
    modifiers |= JNI_STATIC(CTRL_DOWN_MASK);
  if (forceShift || (GetKeyState(VK_SHIFT) & 0x8000) != 0)
    modifiers |= JNI_STATIC(SHIFT_DOWN_MASK);
  if ((GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0)
    modifiers |= JNI_STATIC(BUTTON1_DOWN_MASK);
  if ((GetAsyncKeyState(VK_MBUTTON) & 0x8000) != 0)
    modifiers |= JNI_STATIC(BUTTON2_DOWN_MASK);
  if ((GetAsyncKeyState(VK_RBUTTON) & 0x8000) != 0)
    modifiers |= JNI_STATIC(BUTTON3_DOWN_MASK);

  return modifiers;
}

static int getMouseButton(WPARAM wParam) {
  ScopedJNIEnv env;
  if (!env)
    return 0;

  const char* mouseEventClassName = "java/awt/event/MouseEvent";
  JNI_STATIC_DEFINE_INT_RV(env, ScopedJNIClass(env, mouseEventClassName),
                           BUTTON1, 0);
  JNI_STATIC_DEFINE_INT_RV(env, ScopedJNIClass(env, mouseEventClassName),
                           BUTTON2, 0);
  JNI_STATIC_DEFINE_INT_RV(env, ScopedJNIClass(env, mouseEventClassName),
                           BUTTON3, 0);
  JNI_STATIC_DEFINE_INT_RV(env, ScopedJNIClass(env, mouseEventClassName),
                           NOBUTTON, 0);

  switch (wParam) {
    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP:
    case WM_LBUTTONDBLCLK:
      return JNI_STATIC(BUTTON1);
    case WM_MBUTTONDOWN:
    case WM_MBUTTONUP:
    case WM_MBUTTONDBLCLK:
      return JNI_STATIC(BUTTON2);
    case WM_RBUTTONDOWN:
    case WM_RBUTTONUP:
    case WM_RBUTTONDBLCLK:
      return JNI_STATIC(BUTTON3);
    default:
      return JNI_STATIC(NOBUTTON);
  }
}

static int getMouseEvent(WPARAM wParam) {
  ScopedJNIEnv env;
  if (!env)
    return 0;

  const char* mouseEventClassName = "java/awt/event/MouseEvent";
  JNI_STATIC_DEFINE_INT_RV(env, ScopedJNIClass(env, mouseEventClassName),
                           MOUSE_MOVED, 0);
  JNI_STATIC_DEFINE_INT_RV(env, ScopedJNIClass(env, mouseEventClassName),
                           MOUSE_PRESSED, 0);
  JNI_STATIC_DEFINE_INT_RV(env, ScopedJNIClass(env, mouseEventClassName),
                           MOUSE_RELEASED, 0);
  JNI_STATIC_DEFINE_INT_RV(env, ScopedJNIClass(env, mouseEventClassName),
                           MOUSE_WHEEL, 0);

  switch (wParam) {
    case WM_MOUSEMOVE:
      return JNI_STATIC(MOUSE_MOVED);

    // Handle a double click like a single click.
    case WM_LBUTTONDBLCLK:
    case WM_MBUTTONDBLCLK:
    case WM_RBUTTONDBLCLK:
    // FALL THRU

    // Handle button down and up events.
    case WM_LBUTTONDOWN:
    case WM_MBUTTONDOWN:
    case WM_RBUTTONDOWN:
      return JNI_STATIC(MOUSE_PRESSED);

    case WM_LBUTTONUP:
    case WM_MBUTTONUP:
    case WM_RBUTTONUP:
      return JNI_STATIC(MOUSE_RELEASED);

    // Handle horizontal mouse wheel event.
    // The vertical mouse wheel is already recognized in Java.
    // case WM_MOUSEWHEEL:
    case WM_MOUSEHWHEEL:
      return JNI_STATIC(MOUSE_WHEEL);
    default:
      return 0;
  }
}

static std::map<CefWindowHandle, CefRefPtr<CefBrowser>> g_browsers_;
static HANDLE g_browsers_lock_ = CreateMutex(NULL, FALSE, NULL);
static HHOOK g_mouse_monitor_ = NULL;
static HHOOK g_proc_monitor_ = NULL;
static int g_mouse_monitor_refs_ = 0;
static BOOLEAN g_once_ = TRUE;

LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
  if (nCode != HC_ACTION || lParam == NULL)
    return CallNextHookEx(NULL, nCode, wParam, lParam);

  MOUSEHOOKSTRUCT* pStruct = (MOUSEHOOKSTRUCT*)lParam;

  // Find the CefBrowser associated with the current window hierarchy.
  CefWindowHandle windowHandle = pStruct->hwnd;
  CefRefPtr<CefBrowser> cefBrowser;
  std::map<CefWindowHandle, CefRefPtr<CefBrowser>>::iterator it;
  WaitForSingleObject(g_browsers_lock_, INFINITE);
  while (windowHandle != NULL) {
    it = g_browsers_.find(windowHandle);
    if (it != g_browsers_.end()) {
      cefBrowser = it->second;
      break;
    }
    windowHandle = GetParent(windowHandle);
  }
  ReleaseMutex(g_browsers_lock_);

  if (!cefBrowser)
    return CallNextHookEx(NULL, nCode, wParam, lParam);

  // We're creating the scoped JNI env here so it can be re-used for
  // the following subfunctions, which all need an env internally,
  // without having the invoking thread constantly attaching and
  // detaching from the JVM (which is ugly, especially if a debugger
  // is attached, but also generally detrimental to performance)
  ScopedJNIEnv env;
  if (!env)
    return CallNextHookEx(NULL, nCode, wParam, lParam);

  int mouseButton = getMouseButton(wParam);
  // Horizonal wheel event is the same as vertical wheel event
  // with pressed shift button.
  int modifiers = getModifiers(wParam == WM_MOUSEHWHEEL);

  int mouseEvent = getMouseEvent(wParam);
  switch (wParam) {
    // Handle horizontal mouse wheel event.
    // The vertical mouse wheel is already recognized in Java.
    // case WM_MOUSEWHEEL:
    case WM_MOUSEHWHEEL:
      mouseButton =
          GET_WHEEL_DELTA_WPARAM(((MOUSEHOOKSTRUCTEX*)pStruct)->mouseData);
      break;
    default:
      break;
  }

  if (mouseEvent != 0) {
    // Forward params to the CefWindowHandle of Java-Canvas
    CefRefPtr<ClientHandler> client =
        (ClientHandler*)cefBrowser->GetHost()->GetClient().get();
    CefRefPtr<WindowHandler> handler = client->GetWindowHandler();
    handler->OnMouseEvent(cefBrowser, mouseEvent, pStruct->pt.x, pStruct->pt.y,
                          modifiers, mouseButton);
  }

  return CallNextHookEx(NULL, nCode, wParam, lParam);
}

void AddCefBrowser(CefRefPtr<CefBrowser> browser) {
  if (!browser.get())
    return;
  CefWindowHandle browserHandle = browser->GetHost()->GetWindowHandle();
  if (!browserHandle)
    return;

  WaitForSingleObject(g_browsers_lock_, INFINITE);
  std::pair<CefWindowHandle, CefRefPtr<CefBrowser>> pair =
      std::make_pair(browserHandle, browser);
  g_browsers_.insert(pair);
  ReleaseMutex(g_browsers_lock_);

  if (g_mouse_monitor_ == NULL) {
    DWORD threadId = GetWindowThreadProcessId(browserHandle, NULL);
    g_mouse_monitor_ =
        SetWindowsHookEx(WH_MOUSE, util::MouseProc, NULL, threadId);
    g_mouse_monitor_refs_ = 1;
  } else {
    g_mouse_monitor_refs_++;
  }
}

void DestroyCefBrowser(CefRefPtr<CefBrowser> browser) {
  if (!browser.get())
    return;
  CefWindowHandle browserHandle = browser->GetHost()->GetWindowHandle();
  if (!browserHandle)
    return;

  WaitForSingleObject(g_browsers_lock_, INFINITE);
  size_t erased = g_browsers_.erase(browserHandle);
  DCHECK_EQ(1U, erased);
  ReleaseMutex(g_browsers_lock_);

  ::DestroyWindow(browserHandle);

  if (g_mouse_monitor_ == NULL)
    return;
  g_mouse_monitor_refs_--;
  if (g_mouse_monitor_refs_ <= 0) {
    UnhookWindowsHookEx(g_mouse_monitor_);
    g_mouse_monitor_ = NULL;
  }
}

void SetWindowBounds(CefWindowHandle browserHandle,
                     const CefRect& contentRect) {
  HRGN contentRgn = CreateRectRgn(contentRect.x, contentRect.y,
                                  contentRect.x + contentRect.width,
                                  contentRect.y + contentRect.height);
  SetWindowRgn(GetParent(browserHandle), contentRgn, TRUE);
}

void SetWindowSize(CefWindowHandle browserHandle, int width, int height) {
  SetWindowPos(browserHandle, NULL, 0, 0, width, height,
               SWP_NOZORDER | SWP_NOMOVE);
}

#endif  // USING_JAVA

}  // namespace util
