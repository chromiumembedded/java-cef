// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "CefBrowser_N.h"
#include "include/cef_browser.h"
#include "jni_util.h"

#if defined(OS_LINUX)
#include <gdk/gdkkeysyms.h>
#endif

#if defined(OS_MACOSX)
#include <Carbon/Carbon.h>
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

#if defined(OS_MACOSX)
// A convenient array for getting symbol characters on the number keys.
const char kShiftCharsForNumberKeys[] = ")!@#$%^&*(";

// Convert an ANSI character to a Mac key code.
int GetMacKeyCodeFromChar(int key_char) {
  switch (key_char) {
    case ' ': return kVK_Space;

    case '0': case ')': return kVK_ANSI_0;
    case '1': case '!': return kVK_ANSI_1;
    case '2': case '@': return kVK_ANSI_2;
    case '3': case '#': return kVK_ANSI_3;
    case '4': case '$': return kVK_ANSI_4;
    case '5': case '%': return kVK_ANSI_5;
    case '6': case '^': return kVK_ANSI_6;
    case '7': case '&': return kVK_ANSI_7;
    case '8': case '*': return kVK_ANSI_8;
    case '9': case '(': return kVK_ANSI_9;

    case 'a': case 'A': return kVK_ANSI_A;
    case 'b': case 'B': return kVK_ANSI_B;
    case 'c': case 'C': return kVK_ANSI_C;
    case 'd': case 'D': return kVK_ANSI_D;
    case 'e': case 'E': return kVK_ANSI_E;
    case 'f': case 'F': return kVK_ANSI_F;
    case 'g': case 'G': return kVK_ANSI_G;
    case 'h': case 'H': return kVK_ANSI_H;
    case 'i': case 'I': return kVK_ANSI_I;
    case 'j': case 'J': return kVK_ANSI_J;
    case 'k': case 'K': return kVK_ANSI_K;
    case 'l': case 'L': return kVK_ANSI_L;
    case 'm': case 'M': return kVK_ANSI_M;
    case 'n': case 'N': return kVK_ANSI_N;
    case 'o': case 'O': return kVK_ANSI_O;
    case 'p': case 'P': return kVK_ANSI_P;
    case 'q': case 'Q': return kVK_ANSI_Q;
    case 'r': case 'R': return kVK_ANSI_R;
    case 's': case 'S': return kVK_ANSI_S;
    case 't': case 'T': return kVK_ANSI_T;
    case 'u': case 'U': return kVK_ANSI_U;
    case 'v': case 'V': return kVK_ANSI_V;
    case 'w': case 'W': return kVK_ANSI_W;
    case 'x': case 'X': return kVK_ANSI_X;
    case 'y': case 'Y': return kVK_ANSI_Y;
    case 'z': case 'Z': return kVK_ANSI_Z;

    // U.S. Specific mappings.  Mileage may vary.
    case ';': case ':': return kVK_ANSI_Semicolon;
    case '=': case '+': return kVK_ANSI_Equal;
    case ',': case '<': return kVK_ANSI_Comma;
    case '-': case '_': return kVK_ANSI_Minus;
    case '.': case '>': return kVK_ANSI_Period;
    case '/': case '?': return kVK_ANSI_Slash;
    case '`': case '~': return kVK_ANSI_Grave;
    case '[': case '{': return kVK_ANSI_LeftBracket;
    case '\\': case '|': return kVK_ANSI_Backslash;
    case ']': case '}': return kVK_ANSI_RightBracket;
    case '\'': case '"': return kVK_ANSI_Quote;
  }
  
  return -1;
}
#endif  // defined(OS_MACOSX)

}  // namespace

JNIEXPORT void JNICALL Java_org_cef_CefBrowser_1N_N_1Close
  (JNIEnv *env, jobject obj) {
  CefRefPtr<CefBrowser> browser = GetCefFromJNIObject<CefBrowser>(env, obj, "CefBrowser");
  if(!browser.get())
    return;

  browser->GetHost()->CloseBrowser(true);

  // Clear the browser pointer member of the Java object. This call will
  // release the extra reference to the object added in
  // CefContext::CreateBrowser.
  SetCefForJNIObject<CefBrowser>(env, obj, NULL, "CefBrowser");
}

JNIEXPORT void JNICALL Java_org_cef_CefBrowser_1N_N_1GoBack
  (JNIEnv *env, jobject obj) {
  CefRefPtr<CefBrowser> browser = GetCefFromJNIObject<CefBrowser>(env, obj, "CefBrowser");
  if(!browser.get())
    return;

  browser->GoBack();
}

JNIEXPORT void JNICALL Java_org_cef_CefBrowser_1N_N_1GoForward
  (JNIEnv *env, jobject obj) {
  CefRefPtr<CefBrowser> browser = GetCefFromJNIObject<CefBrowser>(env, obj, "CefBrowser");
  if(!browser.get())
    return;

  browser->GoForward();
}

JNIEXPORT jint JNICALL Java_org_cef_CefBrowser_1N_N_1GetIdentifier
  (JNIEnv *env, jobject obj) {
  CefRefPtr<CefBrowser> browser = GetCefFromJNIObject<CefBrowser>(env, obj, "CefBrowser");
  if(!browser.get())
    return -1;

  return browser->GetIdentifier();
}

JNIEXPORT void JNICALL Java_org_cef_CefBrowser_1N_N_1LoadURL
  (JNIEnv *env, jobject obj, jstring url) {
  CefRefPtr<CefBrowser> browser = GetCefFromJNIObject<CefBrowser>(env, obj, "CefBrowser");
  if(!browser.get())
    return;

  browser->GetMainFrame()->LoadURL(GetJNIString(env, url));
}

JNIEXPORT void JNICALL Java_org_cef_CefBrowser_1N_N_1WasResized
  (JNIEnv *env, jobject obj, jint width, jint height) {
  CefRefPtr<CefBrowser> browser = GetCefFromJNIObject<CefBrowser>(env, obj, "CefBrowser");
  if(!browser.get())
    return;

  if( browser->GetHost()->IsWindowRenderingDisabled() ) {
    browser->GetHost()->WasResized();
  }
#if defined(OS_WIN) 
  else {
    HWND handle = browser->GetHost()->GetWindowHandle();
    SetWindowPos(handle, NULL, 0, 0, width, height, SWP_NOZORDER | SWP_NOMOVE);
  }
#endif
}

JNIEXPORT void JNICALL Java_org_cef_CefBrowser_1N_N_1Invalidate
  (JNIEnv *env, jobject obj, jobject rect) {
  CefRefPtr<CefBrowser> browser = GetCefFromJNIObject<CefBrowser>(env, obj, "CefBrowser");
  if(!browser.get())
    return;

  CefRect dirtyRect = GetJNIRect(env, rect);
  browser->GetHost()->Invalidate(dirtyRect, PET_VIEW);
}

JNIEXPORT void JNICALL Java_org_cef_CefBrowser_1N_N_1SendKeyEvent
  (JNIEnv *env, jobject obj, jobject key_event) {
  CefRefPtr<CefBrowser> browser = GetCefFromJNIObject<CefBrowser>(env, obj, "CefBrowser");
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
  cef_event.modifiers = GetCefModifiers(env, cls, modifiers);

#if defined(OS_WIN)
  BYTE VkCode = LOBYTE(VkKeyScanA(key_char));
  UINT scanCode = MapVirtualKey(VkCode, MAPVK_VK_TO_VSC);
  cef_event.native_key_code = (scanCode << 16) |  // key scan code
                              1;  // key repeat count
#elif defined(OS_LINUX) || defined(OS_MACOSX)
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

#if defined(OS_LINUX)
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
#elif defined(OS_MACOSX)
  if (key_code == JNI_STATIC(VK_BACK_SPACE)) {
    cef_event.native_key_code = kVK_Delete;
    cef_event.unmodified_character = kBackspaceCharCode;
  } else if (key_code == JNI_STATIC(VK_DELETE)) {
    cef_event.native_key_code = kVK_ForwardDelete;
    cef_event.unmodified_character = kDeleteCharCode;
  } else if (key_code == JNI_STATIC(VK_DOWN)) {
    cef_event.native_key_code = kVK_DownArrow;
    cef_event.unmodified_character = /* NSDownArrowFunctionKey */ 0xF701;
  } else if (key_code == JNI_STATIC(VK_ENTER)) {
    cef_event.native_key_code = kVK_Return;
    cef_event.unmodified_character = kReturnCharCode;
  } else if (key_code == JNI_STATIC(VK_ESCAPE)) {
    cef_event.native_key_code = kVK_Escape;
    cef_event.unmodified_character = kEscapeCharCode;
  } else if (key_code == JNI_STATIC(VK_LEFT)) {
    cef_event.native_key_code = kVK_LeftArrow;
    cef_event.unmodified_character = /* NSLeftArrowFunctionKey */ 0xF702;
  } else if (key_code == JNI_STATIC(VK_RIGHT)) {
    cef_event.native_key_code = kVK_RightArrow;
    cef_event.unmodified_character = /* NSRightArrowFunctionKey */ 0xF703;
  } else if (key_code == JNI_STATIC(VK_TAB)) {
    cef_event.native_key_code = kVK_Tab;
    cef_event.unmodified_character = kTabCharCode;
  } else if (key_code == JNI_STATIC(VK_UP)) {
    cef_event.native_key_code = kVK_UpArrow;
    cef_event.unmodified_character = /* NSUpArrowFunctionKey */ 0xF700;
  } else {
    cef_event.native_key_code = GetMacKeyCodeFromChar(key_char);
    if (cef_event.native_key_code == -1)
      return;
    
    cef_event.unmodified_character = key_char;
  }

  cef_event.character = cef_event.unmodified_character;

  // Fill in |character| according to flags.
  if (cef_event.modifiers & EVENTFLAG_SHIFT_DOWN) {
    if (key_char >= '0' && key_char <= '9') {
      cef_event.character = kShiftCharsForNumberKeys[key_char - '0'];
    } else if (key_char >= 'A' && key_char <= 'Z') {
      cef_event.character = 'A' + (key_char - 'A');
    } else {
      switch (cef_event.native_key_code) {
        case kVK_ANSI_Grave:
          cef_event.character = '~';
          break;
        case kVK_ANSI_Minus:
          cef_event.character = '_';
          break;
        case kVK_ANSI_Equal:
          cef_event.character = '+';
          break;
        case kVK_ANSI_LeftBracket:
          cef_event.character = '{';
          break;
        case kVK_ANSI_RightBracket:
          cef_event.character = '}';
          break;
        case kVK_ANSI_Backslash:
          cef_event.character = '|';
          break;
        case kVK_ANSI_Semicolon:
          cef_event.character = ':';
          break;
        case kVK_ANSI_Quote:
          cef_event.character = '\"';
          break;
        case kVK_ANSI_Comma:
          cef_event.character = '<';
          break;
        case kVK_ANSI_Period:
          cef_event.character = '>';
          break;
        case kVK_ANSI_Slash:
          cef_event.character = '?';
          break;
        default:
          break;
      }
    }
  }

  // Control characters.
  if (cef_event.modifiers & EVENTFLAG_CONTROL_DOWN) {
    if (key_char >= 'A' && key_char <= 'Z')
      cef_event.character = 1 + key_char - 'A';
    else if (cef_event.native_key_code == kVK_ANSI_LeftBracket)
      cef_event.character = 27;
    else if (cef_event.native_key_code == kVK_ANSI_Backslash)
      cef_event.character = 28;
    else if (cef_event.native_key_code == kVK_ANSI_RightBracket)
      cef_event.character = 29;
  }
#endif  // defined(OS_MACOSX)
#endif  // defined(OS_LINUX) || defined(OS_MACOSX)

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
  CefRefPtr<CefBrowser> browser = GetCefFromJNIObject<CefBrowser>(env, obj, "CefBrowser");
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
  CefRefPtr<CefBrowser> browser = GetCefFromJNIObject<CefBrowser>(env, obj, "CefBrowser");
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

JNIEXPORT void JNICALL Java_org_cef_CefBrowser_1N_N_1SetFocus
  (JNIEnv *env, jobject obj, jboolean enable) {
  CefRefPtr<CefBrowser> browser = GetCefFromJNIObject<CefBrowser>(env, obj, "CefBrowser");
  if(!browser.get())
    return;

  if( browser->GetHost()->IsWindowRenderingDisabled() ) {
    browser->GetHost()->SendFocusEvent(enable == JNI_TRUE);
  } else {
    browser->GetHost()->SetFocus(enable == JNI_TRUE);
  }
#if defined(OS_WIN)
  if (enable == JNI_FALSE)
    SetFocus(browser->GetHost()->GetOpenerWindowHandle());
#endif
}
