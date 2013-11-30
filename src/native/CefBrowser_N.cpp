// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "CefBrowser_N.h"
#include "include/cef_browser.h"
#include "jni_util.h"

#if defined(OS_LINUX)
#include <gdk/gdkkeysyms.h>
#endif

#if defined(OS_WIN)
#undef MOUSE_MOVED
#endif

namespace {

int GetCefModifiers(JNIEnv *env, jclass cls, int modifiers) {
  JNI_STATIC_DEFINE_INT_RV(env, cls, ALT_DOWN_MASK, 0);
  JNI_STATIC_DEFINE_INT_RV(env, cls, BUTTON1_DOWN_MASK, 0);
  JNI_STATIC_DEFINE_INT_RV(env, cls, BUTTON2_DOWN_MASK, 0);
  JNI_STATIC_DEFINE_INT_RV(env, cls, BUTTON3_DOWN_MASK, 0);
  JNI_STATIC_DEFINE_INT_RV(env, cls, CTRL_DOWN_MASK, 0);
  JNI_STATIC_DEFINE_INT_RV(env, cls, META_DOWN_MASK, 0);
  JNI_STATIC_DEFINE_INT_RV(env, cls, SHIFT_DOWN_MASK, 0);

  int cef_modifiers = 0;
  if (modifiers & JNI_STATIC(ALT_DOWN_MASK))
    cef_modifiers |= EVENTFLAG_ALT_DOWN;
  if (modifiers & JNI_STATIC(BUTTON1_DOWN_MASK))
    cef_modifiers |= EVENTFLAG_LEFT_MOUSE_BUTTON;
  if (modifiers & JNI_STATIC(BUTTON2_DOWN_MASK))
    cef_modifiers |= EVENTFLAG_MIDDLE_MOUSE_BUTTON;
  if (modifiers & JNI_STATIC(BUTTON3_DOWN_MASK))
    cef_modifiers |= EVENTFLAG_RIGHT_MOUSE_BUTTON;
  if (modifiers & JNI_STATIC(CTRL_DOWN_MASK))
    cef_modifiers |= EVENTFLAG_CONTROL_DOWN;
  if (modifiers & JNI_STATIC(META_DOWN_MASK))
    cef_modifiers |= EVENTFLAG_COMMAND_DOWN;
  if (modifiers & JNI_STATIC(SHIFT_DOWN_MASK))
    cef_modifiers |= EVENTFLAG_SHIFT_DOWN;

  return cef_modifiers;
}

}  // namespace

JNIEXPORT void JNICALL Java_org_cef_CefBrowser_1N_N_1Close
  (JNIEnv *env, jobject obj) {
  CefRefPtr<CefBrowser> browser = GetCefFromJNIObject<CefBrowser>(env, obj);
  if(!browser.get())
    return;

  browser->GetHost()->CloseBrowser(true);

  // Clear the browser pointer member of the Java object. This call will
  // release the extra reference to the object added in
  // CefContext::CreateBrowser.
  SetCefForJNIObject<CefBrowser>(env, obj, NULL);
}

JNIEXPORT void JNICALL Java_org_cef_CefBrowser_1N_N_1GoBack
  (JNIEnv *env, jobject obj) {
  CefRefPtr<CefBrowser> browser = GetCefFromJNIObject<CefBrowser>(env, obj);
  if(!browser.get())
    return;

  browser->GoBack();
}

JNIEXPORT void JNICALL Java_org_cef_CefBrowser_1N_N_1GoForward
  (JNIEnv *env, jobject obj) {
  CefRefPtr<CefBrowser> browser = GetCefFromJNIObject<CefBrowser>(env, obj);
  if(!browser.get())
    return;

  browser->GoForward();
}

JNIEXPORT jint JNICALL Java_org_cef_CefBrowser_1N_N_1GetIdentifier
  (JNIEnv *env, jobject obj) {
  CefRefPtr<CefBrowser> browser = GetCefFromJNIObject<CefBrowser>(env, obj);
  if(!browser.get())
    return -1;

  return browser->GetIdentifier();
}

JNIEXPORT void JNICALL Java_org_cef_CefBrowser_1N_N_1LoadURL
  (JNIEnv *env, jobject obj, jstring url) {
  CefRefPtr<CefBrowser> browser = GetCefFromJNIObject<CefBrowser>(env, obj);
  if(!browser.get())
    return;

  browser->GetMainFrame()->LoadURL(GetJNIString(env, url));
}

JNIEXPORT void JNICALL Java_org_cef_CefBrowser_1N_N_1WasResized
  (JNIEnv *env, jobject obj) {
  CefRefPtr<CefBrowser> browser = GetCefFromJNIObject<CefBrowser>(env, obj);
  if(!browser.get())
    return;

  browser->GetHost()->WasResized();
}

JNIEXPORT void JNICALL Java_org_cef_CefBrowser_1N_N_1Invalidate
  (JNIEnv *env, jobject obj, jobject rect) {
  CefRefPtr<CefBrowser> browser = GetCefFromJNIObject<CefBrowser>(env, obj);
  if(!browser.get())
    return;

  CefRect dirtyRect = GetJNIRect(env, rect);
  browser->GetHost()->Invalidate(dirtyRect, PET_VIEW);
}

JNIEXPORT void JNICALL Java_org_cef_CefBrowser_1N_N_1SendKeyEvent
  (JNIEnv *env, jobject obj, jobject key_event) {
  CefRefPtr<CefBrowser> browser = GetCefFromJNIObject<CefBrowser>(env, obj);
  if(!browser.get())
    return;

  jclass cls = env->GetObjectClass(key_event);
  if (!cls)
    return;

  JNI_STATIC_DEFINE_INT(env, cls, KEY_PRESSED);
  JNI_STATIC_DEFINE_INT(env, cls, KEY_RELEASED);
  JNI_STATIC_DEFINE_INT(env, cls, KEY_TYPED);

  int event_type, modifiers;
  char key_char;
  if (!CallJNIMethodI_V(env, cls, key_event, "getID", &event_type) ||
      !CallJNIMethodC_V(env, cls, key_event, "getKeyChar", &key_char) ||
      !CallJNIMethodI_V(env, cls, key_event, "getModifiersEx", &modifiers)) {
    return;
  }

  CefKeyEvent cef_event;
#if defined(OS_WIN)
  BYTE VkCode = LOBYTE(VkKeyScanA(key_char));
  UINT scanCode = MapVirtualKey(VkCode, MAPVK_VK_TO_VSC);
  cef_event.native_key_code = (scanCode << 16) |  // key scan code
                              1;  // key repeat count
#elif defined(OS_LINUX)
  int key_code;
  if (!CallJNIMethodI_V(env, cls, key_event, "getKeyCode", &key_code)) {
    return;
  }

  JNI_STATIC_DEFINE_INT(env, cls, VK_BACK_SPACE);
  JNI_STATIC_DEFINE_INT(env, cls, VK_DELETE);
  JNI_STATIC_DEFINE_INT(env, cls, VK_DOWN);
  JNI_STATIC_DEFINE_INT(env, cls, VK_ENTER);
  JNI_STATIC_DEFINE_INT(env, cls, VK_ESCAPE);
  JNI_STATIC_DEFINE_INT(env, cls, VK_LEFT);
  JNI_STATIC_DEFINE_INT(env, cls, VK_RIGHT);
  JNI_STATIC_DEFINE_INT(env, cls, VK_TAB);
  JNI_STATIC_DEFINE_INT(env, cls, VK_UP);

  if (key_code == JNI_STATIC(VK_BACK_SPACE))
    cef_event.native_key_code = GDK_BackSpace;
  else if (key_code == JNI_STATIC(VK_DELETE))
    cef_event.native_key_code = GDK_Delete;
  else if (key_code == JNI_STATIC(VK_DOWN))
    cef_event.native_key_code = GDK_Down;
  else if (key_code == JNI_STATIC(VK_ENTER))
    cef_event.native_key_code = GDK_KEY_KP_Enter;
  else if (key_code == JNI_STATIC(VK_ESCAPE))
    cef_event.native_key_code = GDK_Escape;
  else if (key_code == JNI_STATIC(VK_LEFT))
    cef_event.native_key_code = GDK_Left;
  else if (key_code == JNI_STATIC(VK_RIGHT))
    cef_event.native_key_code = GDK_Right;
  else if (key_code == JNI_STATIC(VK_TAB))
    cef_event.native_key_code = GDK_Tab;
  else if (key_code == JNI_STATIC(VK_UP))
    cef_event.native_key_code = GDK_Up;
  else
    cef_event.native_key_code = key_char;
#endif  // defined(OS_LINUX)

  cef_event.modifiers = GetCefModifiers(env, cls, modifiers);

  if (event_type == JNI_STATIC(KEY_PRESSED)) {
#if defined(OS_WIN)
    cef_event.windows_key_code = VkCode;
#endif
    cef_event.type = KEYEVENT_RAWKEYDOWN;
  } else if (event_type == JNI_STATIC(KEY_RELEASED)) {
#if defined(OS_WIN)
    cef_event.windows_key_code = VkCode;
    // bits 30 and 31 should always be 1 for WM_KEYUP
    cef_event.native_key_code |= 0xC0000000;
#endif
    cef_event.type = KEYEVENT_KEYUP;
  } else if (event_type == JNI_STATIC(KEY_TYPED)) {
#if defined(OS_WIN)
    cef_event.windows_key_code = key_char;
#endif
    cef_event.type = KEYEVENT_CHAR;
  } else {
    return;
  }

  browser->GetHost()->SendKeyEvent(cef_event);
}

JNIEXPORT void JNICALL Java_org_cef_CefBrowser_1N_N_1SendMouseEvent
  (JNIEnv *env, jobject obj, jobject mouse_event) {
  CefRefPtr<CefBrowser> browser = GetCefFromJNIObject<CefBrowser>(env, obj);
  if(!browser.get())
    return;

  jclass cls = env->GetObjectClass(mouse_event);
  if (!cls)
    return;

  JNI_STATIC_DEFINE_INT(env, cls, BUTTON1);
  JNI_STATIC_DEFINE_INT(env, cls, BUTTON2);
  JNI_STATIC_DEFINE_INT(env, cls, BUTTON3);
  JNI_STATIC_DEFINE_INT(env, cls, MOUSE_DRAGGED);
  JNI_STATIC_DEFINE_INT(env, cls, MOUSE_ENTERED);
  JNI_STATIC_DEFINE_INT(env, cls, MOUSE_EXITED);
  JNI_STATIC_DEFINE_INT(env, cls, MOUSE_MOVED);
  JNI_STATIC_DEFINE_INT(env, cls, MOUSE_PRESSED);
  JNI_STATIC_DEFINE_INT(env, cls, MOUSE_RELEASED);

  int event_type, x, y, modifiers;
  if (!CallJNIMethodI_V(env, cls, mouse_event, "getID", &event_type) ||
      !CallJNIMethodI_V(env, cls, mouse_event, "getX", &x) ||
      !CallJNIMethodI_V(env, cls, mouse_event, "getY", &y) ||
      !CallJNIMethodI_V(env, cls, mouse_event, "getModifiersEx", &modifiers)) {
    return;
  }

  CefMouseEvent cef_event;
  cef_event.x = x;
  cef_event.y = y;

  cef_event.modifiers = GetCefModifiers(env, cls, modifiers);

  if (event_type == JNI_STATIC(MOUSE_PRESSED) ||
      event_type == JNI_STATIC(MOUSE_RELEASED)) {
    int click_count, button;
    if (!CallJNIMethodI_V(env, cls, mouse_event, "getClickCount", &click_count) ||
        !CallJNIMethodI_V(env, cls, mouse_event, "getButton", &button)) {
      return;
    }

    CefBrowserHost::MouseButtonType cef_mbt;
    if (button == JNI_STATIC(BUTTON1))
      cef_mbt = MBT_LEFT;
    else if (button == JNI_STATIC(BUTTON2))
      cef_mbt = MBT_MIDDLE;
    else if (button == JNI_STATIC(BUTTON3))
      cef_mbt = MBT_RIGHT;
    else
      return;

    browser->GetHost()->SendMouseClickEvent(cef_event, cef_mbt,
        (event_type == JNI_STATIC(MOUSE_RELEASED)), click_count);
  } else if (event_type == JNI_STATIC(MOUSE_MOVED) ||
             event_type == JNI_STATIC(MOUSE_DRAGGED) ||
             event_type == JNI_STATIC(MOUSE_ENTERED) ||
             event_type == JNI_STATIC(MOUSE_EXITED)) {
    browser->GetHost()->SendMouseMoveEvent(cef_event,
        (event_type == JNI_STATIC(MOUSE_EXITED)));
  }
}

JNIEXPORT void JNICALL Java_org_cef_CefBrowser_1N_N_1SendMouseWheelEvent
  (JNIEnv *env, jobject obj, jobject mouse_wheel_event) {
  CefRefPtr<CefBrowser> browser = GetCefFromJNIObject<CefBrowser>(env, obj);
  if(!browser.get())
    return;

  jclass cls = env->GetObjectClass(mouse_wheel_event);
  if (!cls)
    return;

  JNI_STATIC_DEFINE_INT(env, cls, WHEEL_UNIT_SCROLL);

  int scroll_type, delta, x, y, modifiers;
  if (!CallJNIMethodI_V(env, cls, mouse_wheel_event, "getScrollType", &scroll_type) ||
      !CallJNIMethodI_V(env, cls, mouse_wheel_event, "getWheelRotation", &delta) ||
      !CallJNIMethodI_V(env, cls, mouse_wheel_event, "getX", &x) ||
      !CallJNIMethodI_V(env, cls, mouse_wheel_event, "getY", &y) ||
      !CallJNIMethodI_V(env, cls, mouse_wheel_event, "getModifiersEx", &modifiers)) {
    return;
  }

  CefMouseEvent cef_event;
  cef_event.x = x;
  cef_event.y = y;

  cef_event.modifiers = GetCefModifiers(env, cls, modifiers);

  if (scroll_type == JNI_STATIC(WHEEL_UNIT_SCROLL)) {
    // Use the smarter version that considers platform settings.
    CallJNIMethodI_V(env, cls, mouse_wheel_event, "getUnitsToScroll", &delta);
  }

  double deltaX = 0, deltaY = 0;
  if (cef_event.modifiers & EVENTFLAG_SHIFT_DOWN)
    deltaX = delta;
  else
    deltaY = delta;

  browser->GetHost()->SendMouseWheelEvent(cef_event, deltaX, deltaY);
}
