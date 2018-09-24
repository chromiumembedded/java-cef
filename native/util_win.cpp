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

#include "include/cef_path_util.h"

#define XBUTTON1_HI (XBUTTON1 << 16)

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

static int getMouseEvent(const char* evtName) {
  int value = 0;
  BEGIN_ENV(env)
  jclass jcls = FindClass(env, "java/awt/event/MouseEvent");
  GetJNIFieldStaticInt(env, jcls, evtName, &value);
  END_ENV(env)
  return value;
}

static int getModifiers(BOOLEAN forceShift) {
  int alt = 0;
  int ctrl = 0;
  int shift = 0;
  int button1 = 0;
  int button2 = 0;
  int button3 = 0;
  BEGIN_ENV(env)
  jclass jcls = FindClass(env, "java/awt/event/InputEvent");
  if ((GetKeyState(VK_MENU) & 0x8000) != 0)
    GetJNIFieldStaticInt(env, jcls, "ALT_DOWN_MASK", &alt);
  if ((GetKeyState(VK_CONTROL) & 0x8000) != 0)
    GetJNIFieldStaticInt(env, jcls, "CTRL_DOWN_MASK", &ctrl);
  if (forceShift || (GetKeyState(VK_SHIFT) & 0x8000) != 0)
    GetJNIFieldStaticInt(env, jcls, "SHIFT_DOWN_MASK", &shift);
  if ((GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0)
    GetJNIFieldStaticInt(env, jcls, "BUTTON1_DOWN_MASK", &button1);
  if ((GetAsyncKeyState(VK_MBUTTON) & 0x8000) != 0)
    GetJNIFieldStaticInt(env, jcls, "BUTTON2_DOWN_MASK", &button2);
  if ((GetAsyncKeyState(VK_RBUTTON) & 0x8000) != 0)
    GetJNIFieldStaticInt(env, jcls, "BUTTON3_DOWN_MASK", &button3);
  END_ENV(env)
  return (alt | ctrl | shift | button1 | button2 | button3);
}

static int getMouseButton(WPARAM wParam) {
  int mouseButton = 0;
  BEGIN_ENV(env)
  jclass jcls = FindClass(env, "java/awt/event/MouseEvent");
  switch (wParam) {
    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP:
    case WM_LBUTTONDBLCLK:
      GetJNIFieldStaticInt(env, jcls, "BUTTON1", &mouseButton);
      break;
    case WM_MBUTTONDOWN:
    case WM_MBUTTONUP:
    case WM_MBUTTONDBLCLK:
      GetJNIFieldStaticInt(env, jcls, "BUTTON2", &mouseButton);
      break;
    case WM_RBUTTONDOWN:
    case WM_RBUTTONUP:
    case WM_RBUTTONDBLCLK:
      GetJNIFieldStaticInt(env, jcls, "BUTTON3", &mouseButton);
      break;
    default:
      GetJNIFieldStaticInt(env, jcls, "NOBUTTON", &mouseButton);
      break;
  }
  END_ENV(env)
  return mouseButton;
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

  // Get corresponding CefWindowHandle of Java-Canvas
  CefWindowHandle browser = pStruct->hwnd;
  CefRefPtr<CefBrowser> cefBrowser;
  std::map<CefWindowHandle, CefRefPtr<CefBrowser>>::iterator it;
  WaitForSingleObject(g_browsers_lock_, INFINITE);
  while (browser != NULL) {
    it = g_browsers_.find(browser);
    if (it != g_browsers_.end()) {
      cefBrowser = it->second;
      break;
    }
    browser = GetParent(browser);
  }
  ReleaseMutex(g_browsers_lock_);

  if (!browser)
    return CallNextHookEx(NULL, nCode, wParam, lParam);

  int mouseButton = getMouseButton(wParam);
  // Horizonal wheel event is the same as vertical wheel event
  // with pressed shift button.
  int modifiers = getModifiers(wParam == WM_MOUSEHWHEEL);
  int mouseEvent = 0;

  switch (wParam) {
    case WM_MOUSEMOVE:
      mouseEvent = getMouseEvent("MOUSE_MOVED");
      break;

    // Handle a double click like a single click.
    case WM_LBUTTONDBLCLK:
    case WM_MBUTTONDBLCLK:
    case WM_RBUTTONDBLCLK:
    // FALL THRU

    // Handle button down and up events.
    case WM_LBUTTONDOWN:
    case WM_MBUTTONDOWN:
    case WM_RBUTTONDOWN:
      mouseEvent = getMouseEvent("MOUSE_PRESSED");
      break;

    case WM_LBUTTONUP:
    case WM_MBUTTONUP:
    case WM_RBUTTONUP:
      mouseEvent = getMouseEvent("MOUSE_RELEASED");
      break;

    // Handle horizontal mouse wheel event.
    // The vertical mouse wheel is already recognized in Java.
    // case WM_MOUSEWHEEL:
    case WM_MOUSEHWHEEL:
      mouseEvent = getMouseEvent("MOUSE_WHEEL");
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
  CefWindowHandle handle = GetParent(browserHandle);
  std::pair<CefWindowHandle, CefRefPtr<CefBrowser>> pair =
      std::make_pair(handle, browser);
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
  g_browsers_.erase(GetParent(browserHandle));
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

CefWindowHandle GetWindowHandle(JNIEnv* env, jobject canvas) {
  return GetHwndOfCanvas(canvas, env);
}

void SetParent(CefWindowHandle browserHandle, CefWindowHandle parentHandle) {
  if (parentHandle == kNullWindowHandle)
    parentHandle = TempWindow::GetWindowHandle();
  if (parentHandle != kNullWindowHandle && browserHandle != kNullWindowHandle)
    ::SetParent(browserHandle, parentHandle);
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

void FocusParent(CefWindowHandle browserHandle) {
  HWND parent = GetParent(browserHandle);
  SetActiveWindow(parent);
  SetFocus(parent);
}

#endif  // USING_JAVA

}  // namespace util
