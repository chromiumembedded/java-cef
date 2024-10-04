// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "CefBrowser_N.h"

#include "include/base/cef_callback.h"
#include "include/cef_browser.h"
#include "include/cef_parser.h"
#include "include/cef_task.h"
#include "include/wrapper/cef_closure_task.h"

#include "browser_process_handler.h"
#include "client_handler.h"
#include "critical_wait.h"
#include "devtools_message_observer.h"
#include "int_callback.h"
#include "jni_util.h"
#include "life_span_handler.h"
#include "pdf_print_callback.h"
#include "render_handler.h"
#include "run_file_dialog_callback.h"
#include "string_visitor.h"
#include "temp_window.h"
#include "window_handler.h"

#if defined(OS_LINUX)
#define XK_3270  // for XK_3270_BackTab
#include <X11/XF86keysym.h>
#include <X11/keysym.h>
#include <memory>
#endif

#if defined(OS_MACOSX)
#include <Carbon/Carbon.h>
#include "util_mac.h"
#endif

#if defined(OS_WIN)
#include <memory>
#undef MOUSE_MOVED
#endif

namespace {

int GetCefModifiersGlfw(JNIEnv* env, jclass cls, int modifiers) {
  JNI_STATIC_DEFINE_INT_RV(env, cls, GLFW_MOD_ALT, 0);
  JNI_STATIC_DEFINE_INT_RV(env, cls, GLFW_MOD_CONTROL, 0);
  JNI_STATIC_DEFINE_INT_RV(env, cls, GLFW_MOD_SUPER, 0);
  JNI_STATIC_DEFINE_INT_RV(env, cls, GLFW_MOD_SHIFT, 0);

  int cef_modifiers = 0;
  if (modifiers & JNI_STATIC(GLFW_MOD_ALT))
    cef_modifiers |= EVENTFLAG_ALT_DOWN;
  if (modifiers & 0x10) // BUTTON1_MASK
    cef_modifiers |= EVENTFLAG_LEFT_MOUSE_BUTTON;
  if (modifiers & 0x20) // BUTTON2_MASK
    cef_modifiers |= EVENTFLAG_MIDDLE_MOUSE_BUTTON;
  if (modifiers & 0x40) // BUTTON3_MASK
    cef_modifiers |= EVENTFLAG_RIGHT_MOUSE_BUTTON;
  if (modifiers & JNI_STATIC(GLFW_MOD_CONTROL))
    cef_modifiers |= EVENTFLAG_CONTROL_DOWN;
  if (modifiers & JNI_STATIC(GLFW_MOD_SUPER))
    cef_modifiers |= EVENTFLAG_COMMAND_DOWN;
  if (modifiers & JNI_STATIC(GLFW_MOD_SHIFT))
    cef_modifiers |= EVENTFLAG_SHIFT_DOWN;

  return cef_modifiers;
}

#if defined(OS_LINUX)

// From ui/events/keycodes/keyboard_codes_posix.h.
enum KeyboardCode {
  VKEY_BACK = 0x08,
  VKEY_TAB = 0x09,
  VKEY_BACKTAB = 0x0A,
  VKEY_CLEAR = 0x0C,
  VKEY_RETURN = 0x0D,
  VKEY_SHIFT = 0x10,
  VKEY_CONTROL = 0x11,
  VKEY_MENU = 0x12,
  VKEY_PAUSE = 0x13,
  VKEY_CAPITAL = 0x14,
  VKEY_KANA = 0x15,
  VKEY_HANGUL = 0x15,
  VKEY_JUNJA = 0x17,
  VKEY_FINAL = 0x18,
  VKEY_HANJA = 0x19,
  VKEY_KANJI = 0x19,
  VKEY_ESCAPE = 0x1B,
  VKEY_CONVERT = 0x1C,
  VKEY_NONCONVERT = 0x1D,
  VKEY_ACCEPT = 0x1E,
  VKEY_MODECHANGE = 0x1F,
  VKEY_SPACE = 0x20,
  VKEY_PRIOR = 0x21,
  VKEY_NEXT = 0x22,
  VKEY_END = 0x23,
  VKEY_HOME = 0x24,
  VKEY_LEFT = 0x25,
  VKEY_UP = 0x26,
  VKEY_RIGHT = 0x27,
  VKEY_DOWN = 0x28,
  VKEY_SELECT = 0x29,
  VKEY_PRINT = 0x2A,
  VKEY_EXECUTE = 0x2B,
  VKEY_SNAPSHOT = 0x2C,
  VKEY_INSERT = 0x2D,
  VKEY_DELETE = 0x2E,
  VKEY_HELP = 0x2F,
  VKEY_0 = 0x30,
  VKEY_1 = 0x31,
  VKEY_2 = 0x32,
  VKEY_3 = 0x33,
  VKEY_4 = 0x34,
  VKEY_5 = 0x35,
  VKEY_6 = 0x36,
  VKEY_7 = 0x37,
  VKEY_8 = 0x38,
  VKEY_9 = 0x39,
  VKEY_A = 0x41,
  VKEY_B = 0x42,
  VKEY_C = 0x43,
  VKEY_D = 0x44,
  VKEY_E = 0x45,
  VKEY_F = 0x46,
  VKEY_G = 0x47,
  VKEY_H = 0x48,
  VKEY_I = 0x49,
  VKEY_J = 0x4A,
  VKEY_K = 0x4B,
  VKEY_L = 0x4C,
  VKEY_M = 0x4D,
  VKEY_N = 0x4E,
  VKEY_O = 0x4F,
  VKEY_P = 0x50,
  VKEY_Q = 0x51,
  VKEY_R = 0x52,
  VKEY_S = 0x53,
  VKEY_T = 0x54,
  VKEY_U = 0x55,
  VKEY_V = 0x56,
  VKEY_W = 0x57,
  VKEY_X = 0x58,
  VKEY_Y = 0x59,
  VKEY_Z = 0x5A,
  VKEY_LWIN = 0x5B,
  VKEY_COMMAND = VKEY_LWIN,  // Provide the Mac name for convenience.
  VKEY_RWIN = 0x5C,
  VKEY_APPS = 0x5D,
  VKEY_SLEEP = 0x5F,
  VKEY_NUMPAD0 = 0x60,
  VKEY_NUMPAD1 = 0x61,
  VKEY_NUMPAD2 = 0x62,
  VKEY_NUMPAD3 = 0x63,
  VKEY_NUMPAD4 = 0x64,
  VKEY_NUMPAD5 = 0x65,
  VKEY_NUMPAD6 = 0x66,
  VKEY_NUMPAD7 = 0x67,
  VKEY_NUMPAD8 = 0x68,
  VKEY_NUMPAD9 = 0x69,
  VKEY_MULTIPLY = 0x6A,
  VKEY_ADD = 0x6B,
  VKEY_SEPARATOR = 0x6C,
  VKEY_SUBTRACT = 0x6D,
  VKEY_DECIMAL = 0x6E,
  VKEY_DIVIDE = 0x6F,
  VKEY_F1 = 0x70,
  VKEY_F2 = 0x71,
  VKEY_F3 = 0x72,
  VKEY_F4 = 0x73,
  VKEY_F5 = 0x74,
  VKEY_F6 = 0x75,
  VKEY_F7 = 0x76,
  VKEY_F8 = 0x77,
  VKEY_F9 = 0x78,
  VKEY_F10 = 0x79,
  VKEY_F11 = 0x7A,
  VKEY_F12 = 0x7B,
  VKEY_F13 = 0x7C,
  VKEY_F14 = 0x7D,
  VKEY_F15 = 0x7E,
  VKEY_F16 = 0x7F,
  VKEY_F17 = 0x80,
  VKEY_F18 = 0x81,
  VKEY_F19 = 0x82,
  VKEY_F20 = 0x83,
  VKEY_F21 = 0x84,
  VKEY_F22 = 0x85,
  VKEY_F23 = 0x86,
  VKEY_F24 = 0x87,
  VKEY_NUMLOCK = 0x90,
  VKEY_SCROLL = 0x91,
  VKEY_LSHIFT = 0xA0,
  VKEY_RSHIFT = 0xA1,
  VKEY_LCONTROL = 0xA2,
  VKEY_RCONTROL = 0xA3,
  VKEY_LMENU = 0xA4,
  VKEY_RMENU = 0xA5,
  VKEY_BROWSER_BACK = 0xA6,
  VKEY_BROWSER_FORWARD = 0xA7,
  VKEY_BROWSER_REFRESH = 0xA8,
  VKEY_BROWSER_STOP = 0xA9,
  VKEY_BROWSER_SEARCH = 0xAA,
  VKEY_BROWSER_FAVORITES = 0xAB,
  VKEY_BROWSER_HOME = 0xAC,
  VKEY_VOLUME_MUTE = 0xAD,
  VKEY_VOLUME_DOWN = 0xAE,
  VKEY_VOLUME_UP = 0xAF,
  VKEY_MEDIA_NEXT_TRACK = 0xB0,
  VKEY_MEDIA_PREV_TRACK = 0xB1,
  VKEY_MEDIA_STOP = 0xB2,
  VKEY_MEDIA_PLAY_PAUSE = 0xB3,
  VKEY_MEDIA_LAUNCH_MAIL = 0xB4,
  VKEY_MEDIA_LAUNCH_MEDIA_SELECT = 0xB5,
  VKEY_MEDIA_LAUNCH_APP1 = 0xB6,
  VKEY_MEDIA_LAUNCH_APP2 = 0xB7,
  VKEY_OEM_1 = 0xBA,
  VKEY_OEM_PLUS = 0xBB,
  VKEY_OEM_COMMA = 0xBC,
  VKEY_OEM_MINUS = 0xBD,
  VKEY_OEM_PERIOD = 0xBE,
  VKEY_OEM_2 = 0xBF,
  VKEY_OEM_3 = 0xC0,
  VKEY_OEM_4 = 0xDB,
  VKEY_OEM_5 = 0xDC,
  VKEY_OEM_6 = 0xDD,
  VKEY_OEM_7 = 0xDE,
  VKEY_OEM_8 = 0xDF,
  VKEY_OEM_102 = 0xE2,
  VKEY_OEM_103 = 0xE3,  // GTV KEYCODE_MEDIA_REWIND
  VKEY_OEM_104 = 0xE4,  // GTV KEYCODE_MEDIA_FAST_FORWARD
  VKEY_PROCESSKEY = 0xE5,
  VKEY_PACKET = 0xE7,
  VKEY_DBE_SBCSCHAR = 0xF3,
  VKEY_DBE_DBCSCHAR = 0xF4,
  VKEY_ATTN = 0xF6,
  VKEY_CRSEL = 0xF7,
  VKEY_EXSEL = 0xF8,
  VKEY_EREOF = 0xF9,
  VKEY_PLAY = 0xFA,
  VKEY_ZOOM = 0xFB,
  VKEY_NONAME = 0xFC,
  VKEY_PA1 = 0xFD,
  VKEY_OEM_CLEAR = 0xFE,
  VKEY_UNKNOWN = 0,

  // POSIX specific VKEYs. Note that as of Windows SDK 7.1, 0x97-9F, 0xD8-DA,
  // and 0xE8 are unassigned.
  VKEY_WLAN = 0x97,
  VKEY_POWER = 0x98,
  VKEY_BRIGHTNESS_DOWN = 0xD8,
  VKEY_BRIGHTNESS_UP = 0xD9,
  VKEY_KBD_BRIGHTNESS_DOWN = 0xDA,
  VKEY_KBD_BRIGHTNESS_UP = 0xE8,

  // Windows does not have a specific key code for AltGr. We use the unused 0xE1
  // (VK_OEM_AX) code to represent AltGr, matching the behaviour of Firefox on
  // Linux.
  VKEY_ALTGR = 0xE1,
  // Windows does not have a specific key code for Compose. We use the unused
  // 0xE6 (VK_ICO_CLEAR) code to represent Compose.
  VKEY_COMPOSE = 0xE6,
};

// From ui/events/keycodes/keyboard_code_conversion_x.cc.
KeyboardCode KeyboardCodeFromXKeysym(unsigned int keysym) {
  switch (keysym) {
    case XK_BackSpace:
      return VKEY_BACK;
    case XK_Delete:
    case XK_KP_Delete:
      return VKEY_DELETE;
    case XK_Tab:
    case XK_KP_Tab:
    case XK_ISO_Left_Tab:
    case XK_3270_BackTab:
      return VKEY_TAB;
    case XK_Linefeed:
    case XK_Return:
    case XK_KP_Enter:
    case XK_ISO_Enter:
      return VKEY_RETURN;
    case XK_Clear:
    case XK_KP_Begin:  // NumPad 5 without Num Lock, for crosbug.com/29169.
      return VKEY_CLEAR;
    case XK_KP_Space:
    case XK_space:
      return VKEY_SPACE;
    case XK_Home:
    case XK_KP_Home:
      return VKEY_HOME;
    case XK_End:
    case XK_KP_End:
      return VKEY_END;
    case XK_Page_Up:
    case XK_KP_Page_Up:  // aka XK_KP_Prior
      return VKEY_PRIOR;
    case XK_Page_Down:
    case XK_KP_Page_Down:  // aka XK_KP_Next
      return VKEY_NEXT;
    case XK_Left:
    case XK_KP_Left:
      return VKEY_LEFT;
    case XK_Right:
    case XK_KP_Right:
      return VKEY_RIGHT;
    case XK_Down:
    case XK_KP_Down:
      return VKEY_DOWN;
    case XK_Up:
    case XK_KP_Up:
      return VKEY_UP;
    case XK_Escape:
      return VKEY_ESCAPE;
    case XK_Kana_Lock:
    case XK_Kana_Shift:
      return VKEY_KANA;
    case XK_Hangul:
      return VKEY_HANGUL;
    case XK_Hangul_Hanja:
      return VKEY_HANJA;
    case XK_Kanji:
      return VKEY_KANJI;
    case XK_Henkan:
      return VKEY_CONVERT;
    case XK_Muhenkan:
      return VKEY_NONCONVERT;
    case XK_Zenkaku_Hankaku:
      return VKEY_DBE_DBCSCHAR;
    case XK_A:
    case XK_a:
      return VKEY_A;
    case XK_B:
    case XK_b:
      return VKEY_B;
    case XK_C:
    case XK_c:
      return VKEY_C;
    case XK_D:
    case XK_d:
      return VKEY_D;
    case XK_E:
    case XK_e:
      return VKEY_E;
    case XK_F:
    case XK_f:
      return VKEY_F;
    case XK_G:
    case XK_g:
      return VKEY_G;
    case XK_H:
    case XK_h:
      return VKEY_H;
    case XK_I:
    case XK_i:
      return VKEY_I;
    case XK_J:
    case XK_j:
      return VKEY_J;
    case XK_K:
    case XK_k:
      return VKEY_K;
    case XK_L:
    case XK_l:
      return VKEY_L;
    case XK_M:
    case XK_m:
      return VKEY_M;
    case XK_N:
    case XK_n:
      return VKEY_N;
    case XK_O:
    case XK_o:
      return VKEY_O;
    case XK_P:
    case XK_p:
      return VKEY_P;
    case XK_Q:
    case XK_q:
      return VKEY_Q;
    case XK_R:
    case XK_r:
      return VKEY_R;
    case XK_S:
    case XK_s:
      return VKEY_S;
    case XK_T:
    case XK_t:
      return VKEY_T;
    case XK_U:
    case XK_u:
      return VKEY_U;
    case XK_V:
    case XK_v:
      return VKEY_V;
    case XK_W:
    case XK_w:
      return VKEY_W;
    case XK_X:
    case XK_x:
      return VKEY_X;
    case XK_Y:
    case XK_y:
      return VKEY_Y;
    case XK_Z:
    case XK_z:
      return VKEY_Z;

    case XK_0:
    case XK_1:
    case XK_2:
    case XK_3:
    case XK_4:
    case XK_5:
    case XK_6:
    case XK_7:
    case XK_8:
    case XK_9:
      return static_cast<KeyboardCode>(VKEY_0 + (keysym - XK_0));

    case XK_parenright:
      return VKEY_0;
    case XK_exclam:
      return VKEY_1;
    case XK_at:
      return VKEY_2;
    case XK_numbersign:
      return VKEY_3;
    case XK_dollar:
      return VKEY_4;
    case XK_percent:
      return VKEY_5;
    case XK_asciicircum:
      return VKEY_6;
    case XK_ampersand:
      return VKEY_7;
    case XK_asterisk:
      return VKEY_8;
    case XK_parenleft:
      return VKEY_9;

    case XK_KP_0:
    case XK_KP_1:
    case XK_KP_2:
    case XK_KP_3:
    case XK_KP_4:
    case XK_KP_5:
    case XK_KP_6:
    case XK_KP_7:
    case XK_KP_8:
    case XK_KP_9:
      return static_cast<KeyboardCode>(VKEY_NUMPAD0 + (keysym - XK_KP_0));

    case XK_multiply:
    case XK_KP_Multiply:
      return VKEY_MULTIPLY;
    case XK_KP_Add:
      return VKEY_ADD;
    case XK_KP_Separator:
      return VKEY_SEPARATOR;
    case XK_KP_Subtract:
      return VKEY_SUBTRACT;
    case XK_KP_Decimal:
      return VKEY_DECIMAL;
    case XK_KP_Divide:
      return VKEY_DIVIDE;
    case XK_KP_Equal:
    case XK_equal:
    case XK_plus:
      return VKEY_OEM_PLUS;
    case XK_comma:
    case XK_less:
      return VKEY_OEM_COMMA;
    case XK_minus:
    case XK_underscore:
      return VKEY_OEM_MINUS;
    case XK_greater:
    case XK_period:
      return VKEY_OEM_PERIOD;
    case XK_colon:
    case XK_semicolon:
      return VKEY_OEM_1;
    case XK_question:
    case XK_slash:
      return VKEY_OEM_2;
    case XK_asciitilde:
    case XK_quoteleft:
      return VKEY_OEM_3;
    case XK_bracketleft:
    case XK_braceleft:
      return VKEY_OEM_4;
    case XK_backslash:
    case XK_bar:
      return VKEY_OEM_5;
    case XK_bracketright:
    case XK_braceright:
      return VKEY_OEM_6;
    case XK_quoteright:
    case XK_quotedbl:
      return VKEY_OEM_7;
    case XK_ISO_Level5_Shift:
      return VKEY_OEM_8;
    case XK_Shift_L:
    case XK_Shift_R:
      return VKEY_SHIFT;
    case XK_Control_L:
    case XK_Control_R:
      return VKEY_CONTROL;
    case XK_Meta_L:
    case XK_Meta_R:
    case XK_Alt_L:
    case XK_Alt_R:
      return VKEY_MENU;
    case XK_ISO_Level3_Shift:
      return VKEY_ALTGR;
    case XK_Multi_key:
      return VKEY_COMPOSE;
    case XK_Pause:
      return VKEY_PAUSE;
    case XK_Caps_Lock:
      return VKEY_CAPITAL;
    case XK_Num_Lock:
      return VKEY_NUMLOCK;
    case XK_Scroll_Lock:
      return VKEY_SCROLL;
    case XK_Select:
      return VKEY_SELECT;
    case XK_Print:
      return VKEY_PRINT;
    case XK_Execute:
      return VKEY_EXECUTE;
    case XK_Insert:
    case XK_KP_Insert:
      return VKEY_INSERT;
    case XK_Help:
      return VKEY_HELP;
    case XK_Super_L:
      return VKEY_LWIN;
    case XK_Super_R:
      return VKEY_RWIN;
    case XK_Menu:
      return VKEY_APPS;
    case XK_F1:
    case XK_F2:
    case XK_F3:
    case XK_F4:
    case XK_F5:
    case XK_F6:
    case XK_F7:
    case XK_F8:
    case XK_F9:
    case XK_F10:
    case XK_F11:
    case XK_F12:
    case XK_F13:
    case XK_F14:
    case XK_F15:
    case XK_F16:
    case XK_F17:
    case XK_F18:
    case XK_F19:
    case XK_F20:
    case XK_F21:
    case XK_F22:
    case XK_F23:
    case XK_F24:
      return static_cast<KeyboardCode>(VKEY_F1 + (keysym - XK_F1));
    case XK_KP_F1:
    case XK_KP_F2:
    case XK_KP_F3:
    case XK_KP_F4:
      return static_cast<KeyboardCode>(VKEY_F1 + (keysym - XK_KP_F1));

    case XK_guillemotleft:
    case XK_guillemotright:
    case XK_degree:
    // In the case of canadian multilingual keyboard layout, VKEY_OEM_102 is
    // assigned to ugrave key.
    case XK_ugrave:
    case XK_Ugrave:
    case XK_brokenbar:
      return VKEY_OEM_102;  // international backslash key in 102 keyboard.

    // When evdev is in use, /usr/share/X11/xkb/symbols/inet maps F13-18 keys
    // to the special XF86XK symbols to support Microsoft Ergonomic keyboards:
    // https://bugs.freedesktop.org/show_bug.cgi?id=5783
    // In Chrome, we map these X key symbols back to F13-18 since we don't have
    // VKEYs for these XF86XK symbols.
    case XF86XK_Tools:
      return VKEY_F13;
    case XF86XK_Launch5:
      return VKEY_F14;
    case XF86XK_Launch6:
      return VKEY_F15;
    case XF86XK_Launch7:
      return VKEY_F16;
    case XF86XK_Launch8:
      return VKEY_F17;
    case XF86XK_Launch9:
      return VKEY_F18;
    case XF86XK_Refresh:
    case XF86XK_History:
    case XF86XK_OpenURL:
    case XF86XK_AddFavorite:
    case XF86XK_Go:
    case XF86XK_ZoomIn:
    case XF86XK_ZoomOut:
      // ui::AcceleratorGtk tries to convert the XF86XK_ keysyms on Chrome
      // startup. It's safe to return VKEY_UNKNOWN here since ui::AcceleratorGtk
      // also checks a Gdk keysym. http://crbug.com/109843
      return VKEY_UNKNOWN;
    // For supporting multimedia buttons on a USB keyboard.
    case XF86XK_Back:
      return VKEY_BROWSER_BACK;
    case XF86XK_Forward:
      return VKEY_BROWSER_FORWARD;
    case XF86XK_Reload:
      return VKEY_BROWSER_REFRESH;
    case XF86XK_Stop:
      return VKEY_BROWSER_STOP;
    case XF86XK_Search:
      return VKEY_BROWSER_SEARCH;
    case XF86XK_Favorites:
      return VKEY_BROWSER_FAVORITES;
    case XF86XK_HomePage:
      return VKEY_BROWSER_HOME;
    case XF86XK_AudioMute:
      return VKEY_VOLUME_MUTE;
    case XF86XK_AudioLowerVolume:
      return VKEY_VOLUME_DOWN;
    case XF86XK_AudioRaiseVolume:
      return VKEY_VOLUME_UP;
    case XF86XK_AudioNext:
      return VKEY_MEDIA_NEXT_TRACK;
    case XF86XK_AudioPrev:
      return VKEY_MEDIA_PREV_TRACK;
    case XF86XK_AudioStop:
      return VKEY_MEDIA_STOP;
    case XF86XK_AudioPlay:
      return VKEY_MEDIA_PLAY_PAUSE;
    case XF86XK_Mail:
      return VKEY_MEDIA_LAUNCH_MAIL;
    case XF86XK_LaunchA:  // F3 on an Apple keyboard.
      return VKEY_MEDIA_LAUNCH_APP1;
    case XF86XK_LaunchB:  // F4 on an Apple keyboard.
    case XF86XK_Calculator:
      return VKEY_MEDIA_LAUNCH_APP2;
    case XF86XK_WLAN:
      return VKEY_WLAN;
    case XF86XK_PowerOff:
      return VKEY_POWER;
    case XF86XK_MonBrightnessDown:
      return VKEY_BRIGHTNESS_DOWN;
    case XF86XK_MonBrightnessUp:
      return VKEY_BRIGHTNESS_UP;
    case XF86XK_KbdBrightnessDown:
      return VKEY_KBD_BRIGHTNESS_DOWN;
    case XF86XK_KbdBrightnessUp:
      return VKEY_KBD_BRIGHTNESS_UP;

      // TODO(sad): some keycodes are still missing.
  }
  return VKEY_UNKNOWN;
}

// From content/browser/renderer_host/input/web_input_event_util_posix.cc.
KeyboardCode GetWindowsKeyCodeWithoutLocation(KeyboardCode key_code) {
  switch (key_code) {
    case VKEY_LCONTROL:
    case VKEY_RCONTROL:
      return VKEY_CONTROL;
    case VKEY_LSHIFT:
    case VKEY_RSHIFT:
      return VKEY_SHIFT;
    case VKEY_LMENU:
    case VKEY_RMENU:
      return VKEY_MENU;
    default:
      return key_code;
  }
}

// From content/browser/renderer_host/input/web_input_event_builders_gtk.cc.
// Gets the corresponding control character of a specified key code. See:
// http://en.wikipedia.org/wiki/Control_characters
// We emulate Windows behavior here.
int GetControlCharacter(KeyboardCode windows_key_code, bool shift) {
  if (windows_key_code >= VKEY_A && windows_key_code <= VKEY_Z) {
    // ctrl-A ~ ctrl-Z map to \x01 ~ \x1A
    return windows_key_code - VKEY_A + 1;
  }
  if (shift) {
    // following graphics chars require shift key to input.
    switch (windows_key_code) {
      // ctrl-@ maps to \x00 (Null byte)
      case VKEY_2:
        return 0;
      // ctrl-^ maps to \x1E (Record separator, Information separator two)
      case VKEY_6:
        return 0x1E;
      // ctrl-_ maps to \x1F (Unit separator, Information separator one)
      case VKEY_OEM_MINUS:
        return 0x1F;
      // Returns 0 for all other keys to avoid inputting unexpected chars.
      default:
        return 0;
    }
  } else {
    switch (windows_key_code) {
      // ctrl-[ maps to \x1B (Escape)
      case VKEY_OEM_4:
        return 0x1B;
      // ctrl-\ maps to \x1C (File separator, Information separator four)
      case VKEY_OEM_5:
        return 0x1C;
      // ctrl-] maps to \x1D (Group separator, Information separator three)
      case VKEY_OEM_6:
        return 0x1D;
      // ctrl-Enter maps to \x0A (Line feed)
      case VKEY_RETURN:
        return 0x0A;
      // Returns 0 for all other keys to avoid inputting unexpected chars.
      default:
        return 0;
    }
  }
}

#endif  // defined(OS_LINUX)

#if defined(OS_MACOSX)
// A convenient array for getting symbol characters on the number keys.
const char kShiftCharsForNumberKeys[] = ")!@#$%^&*(";

// Convert an ANSI character to a Mac key code.
int GetMacKeyCodeFromChar(int key_char) {
  switch (key_char) {
    case ' ':
      return kVK_Space;

    case '0':
    case ')':
      return kVK_ANSI_0;
    case '1':
    case '!':
      return kVK_ANSI_1;
    case '2':
    case '@':
      return kVK_ANSI_2;
    case '3':
    case '#':
      return kVK_ANSI_3;
    case '4':
    case '$':
      return kVK_ANSI_4;
    case '5':
    case '%':
      return kVK_ANSI_5;
    case '6':
    case '^':
      return kVK_ANSI_6;
    case '7':
    case '&':
      return kVK_ANSI_7;
    case '8':
    case '*':
      return kVK_ANSI_8;
    case '9':
    case '(':
      return kVK_ANSI_9;

    case 'a':
    case 'A':
      return kVK_ANSI_A;
    case 'b':
    case 'B':
      return kVK_ANSI_B;
    case 'c':
    case 'C':
      return kVK_ANSI_C;
    case 'd':
    case 'D':
      return kVK_ANSI_D;
    case 'e':
    case 'E':
      return kVK_ANSI_E;
    case 'f':
    case 'F':
      return kVK_ANSI_F;
    case 'g':
    case 'G':
      return kVK_ANSI_G;
    case 'h':
    case 'H':
      return kVK_ANSI_H;
    case 'i':
    case 'I':
      return kVK_ANSI_I;
    case 'j':
    case 'J':
      return kVK_ANSI_J;
    case 'k':
    case 'K':
      return kVK_ANSI_K;
    case 'l':
    case 'L':
      return kVK_ANSI_L;
    case 'm':
    case 'M':
      return kVK_ANSI_M;
    case 'n':
    case 'N':
      return kVK_ANSI_N;
    case 'o':
    case 'O':
      return kVK_ANSI_O;
    case 'p':
    case 'P':
      return kVK_ANSI_P;
    case 'q':
    case 'Q':
      return kVK_ANSI_Q;
    case 'r':
    case 'R':
      return kVK_ANSI_R;
    case 's':
    case 'S':
      return kVK_ANSI_S;
    case 't':
    case 'T':
      return kVK_ANSI_T;
    case 'u':
    case 'U':
      return kVK_ANSI_U;
    case 'v':
    case 'V':
      return kVK_ANSI_V;
    case 'w':
    case 'W':
      return kVK_ANSI_W;
    case 'x':
    case 'X':
      return kVK_ANSI_X;
    case 'y':
    case 'Y':
      return kVK_ANSI_Y;
    case 'z':
    case 'Z':
      return kVK_ANSI_Z;

    // U.S. Specific mappings.  Mileage may vary.
    case ';':
    case ':':
      return kVK_ANSI_Semicolon;
    case '=':
    case '+':
      return kVK_ANSI_Equal;
    case ',':
    case '<':
      return kVK_ANSI_Comma;
    case '-':
    case '_':
      return kVK_ANSI_Minus;
    case '.':
    case '>':
      return kVK_ANSI_Period;
    case '/':
    case '?':
      return kVK_ANSI_Slash;
    case '`':
    case '~':
      return kVK_ANSI_Grave;
    case '[':
    case '{':
      return kVK_ANSI_LeftBracket;
    case '\\':
    case '|':
      return kVK_ANSI_Backslash;
    case ']':
    case '}':
      return kVK_ANSI_RightBracket;
    case '\'':
    case '"':
      return kVK_ANSI_Quote;
  }

  return -1;
}
#endif  // defined(OS_MACOSX)

struct JNIObjectsForCreate {
 public:
  ScopedJNIObjectGlobal jbrowser;
  ScopedJNIObjectGlobal jparentBrowser;
  ScopedJNIObjectGlobal jclientHandler;
  ScopedJNIObjectGlobal url;
  ScopedJNIObjectGlobal jcontext;
  ScopedJNIObjectGlobal jinspectAt;
  ScopedJNIObjectGlobal jbrowserSettings;

  JNIObjectsForCreate(JNIEnv* env,
                      jobject _jbrowser,
                      jobject _jparentBrowser,
                      jobject _jclientHandler,
                      jstring _url,
                      jobject _jcontext,
                      jobject _jinspectAt,
                      jobject _browserSettings)
      :

        jbrowser(env, _jbrowser),
        jparentBrowser(env, _jparentBrowser),
        jclientHandler(env, _jclientHandler),
        url(env, _url),
        jcontext(env, _jcontext),
        jinspectAt(env, _jinspectAt),
        jbrowserSettings(env, _browserSettings) {}
};

void create(std::shared_ptr<JNIObjectsForCreate> objs,
            jlong windowHandle,
            jboolean osr,
            jboolean transparent) {
  ScopedJNIEnv env;
  CefRefPtr<ClientHandler> clientHandler = GetCefFromJNIObject<ClientHandler>(
      env, objs->jclientHandler, "CefClientHandler");
  if (!clientHandler.get())
    return;

  CefRefPtr<LifeSpanHandler> lifeSpanHandler =
      (LifeSpanHandler*)clientHandler->GetLifeSpanHandler().get();
  if (!lifeSpanHandler.get())
    return;

  CefWindowInfo windowInfo;
  windowInfo.SetAsWindowless((CefWindowHandle)windowHandle);

  CefBrowserSettings settings;

  if (transparent == JNI_FALSE) {
    // Specify an opaque background color (white) to disable transparency.
    settings.background_color = CefColorSetARGB(255, 255, 255, 255);
  }

  ScopedJNIClass cefBrowserSettings(env, "org/cef/CefBrowserSettings");
  if (cefBrowserSettings != nullptr &&
      objs->jbrowserSettings != nullptr) {  // Dev-tools settings are null
    GetJNIFieldInt(env, cefBrowserSettings, objs->jbrowserSettings,
                   "windowless_frame_rate", &settings.windowless_frame_rate);
  }

  CefRefPtr<CefBrowser> browserObj;
  CefString strUrl = GetJNIString(env, static_cast<jstring>(objs->url.get()));

  CefRefPtr<CefRequestContext> context = GetCefFromJNIObject<CefRequestContext>(
      env, objs->jcontext, "CefRequestContext");

  CefRefPtr<CefBrowser> parentBrowser =
      GetCefFromJNIObject<CefBrowser>(env, objs->jparentBrowser, "CefBrowser");

  // Add a global ref that will be released in LifeSpanHandler::OnAfterCreated.
  jobject globalRef = env->NewGlobalRef(objs->jbrowser);
  lifeSpanHandler->registerJBrowser(globalRef);

  // If parentBrowser is set, we want to show the DEV-Tools for that browser
  if (parentBrowser.get() != nullptr) {
    CefPoint inspectAt;
    if (objs->jinspectAt != nullptr) {
      int x, y;
      GetJNIPoint(env, objs->jinspectAt, &x, &y);
      inspectAt.Set(x, y);
    }
    parentBrowser->GetHost()->ShowDevTools(windowInfo, clientHandler.get(),
                                           settings, inspectAt);
    JNI_CALL_VOID_METHOD(env, objs->jbrowser, "notifyBrowserCreated", "()V");
    return;
  }

  CefRefPtr<CefDictionaryValue> extra_info;
  auto router_configs = BrowserProcessHandler::GetMessageRouterConfigs();
  if (router_configs) {
    // Send the message router config to CefHelperApp::OnBrowserCreated.
    extra_info = CefDictionaryValue::Create();
    extra_info->SetList("router_configs", router_configs);
  }

  bool result = CefBrowserHost::CreateBrowser(
      windowInfo, clientHandler.get(), strUrl, settings, extra_info, context);
  if (!result) {
    lifeSpanHandler->unregisterJBrowser(globalRef);
    env->DeleteGlobalRef(globalRef);
    return;
  }
  JNI_CALL_VOID_METHOD(env, objs->jbrowser, "notifyBrowserCreated", "()V");
}

void getZoomLevel(CefRefPtr<CefBrowserHost> host,
                  CriticalWait* waitCond,
                  double* result) {
  if (waitCond && result) {
    waitCond->lock()->Lock();
    *result = host->GetZoomLevel();
    waitCond->WakeUp();
    waitCond->lock()->Unlock();
  }
}

void executeDevToolsMethod(CefRefPtr<CefBrowserHost> host,
                           const CefString& method,
                           const CefString& parametersAsJson,
                           CefRefPtr<IntCallback> callback) {
  CefRefPtr<CefDictionaryValue> parameters = nullptr;
  if (!parametersAsJson.empty()) {
    CefRefPtr<CefValue> value = CefParseJSON(
        parametersAsJson, cef_json_parser_options_t::JSON_PARSER_RFC);

    if (!value || value->GetType() != VTYPE_DICTIONARY) {
      callback->onComplete(0);
      return;
    }

    parameters = value->GetDictionary();
  }

  callback->onComplete(host->ExecuteDevToolsMethod(0, method, parameters));
}

// Removed because we don't care about when the native parent window changes.
// This fixes a compile issue on macOS - ds58
//void OnAfterParentChanged(CefRefPtr<CefBrowser> browser) {
//  if (!CefCurrentlyOn(TID_UI)) {
//    CefPostTask(TID_UI, base::BindOnce(&OnAfterParentChanged, browser));
//    return;
//  }
//
//  if (browser->GetHost()->GetClient()) {
//    CefRefPtr<LifeSpanHandler> lifeSpanHandler =
//        (LifeSpanHandler*)browser->GetHost()
//            ->GetClient()
//            ->GetLifeSpanHandler()
//            .get();
//    if (lifeSpanHandler) {
//      lifeSpanHandler->OnAfterParentChanged(browser);
//    }
//  }
//}

CefPdfPrintSettings GetJNIPdfPrintSettings(JNIEnv* env, jobject obj) {
  CefString tmp;
  CefPdfPrintSettings settings;
  if (!obj)
    return settings;

  ScopedJNIClass cls(env, "org/cef/misc/CefPdfPrintSettings");
  if (!cls)
    return settings;

  GetJNIFieldBoolean(env, cls, obj, "landscape", &settings.landscape);

  GetJNIFieldBoolean(env, cls, obj, "print_background",
                     &settings.print_background);

  GetJNIFieldDouble(env, cls, obj, "scale", &settings.scale);

  GetJNIFieldDouble(env, cls, obj, "paper_width", &settings.paper_width);
  GetJNIFieldDouble(env, cls, obj, "paper_height", &settings.paper_height);

  GetJNIFieldBoolean(env, cls, obj, "prefer_css_page_size",
                     &settings.prefer_css_page_size);

  jobject obj_margin_type = nullptr;
  if (GetJNIFieldObject(env, cls, obj, "margin_type", &obj_margin_type,
                        "Lorg/cef/misc/CefPdfPrintSettings$MarginType;")) {
    ScopedJNIObjectLocal margin_type(env, obj_margin_type);
    if (IsJNIEnumValue(env, margin_type,
                       "org/cef/misc/CefPdfPrintSettings$MarginType",
                       "DEFAULT")) {
      settings.margin_type = PDF_PRINT_MARGIN_DEFAULT;
    } else if (IsJNIEnumValue(env, margin_type,
                              "org/cef/misc/CefPdfPrintSettings$MarginType",
                              "NONE")) {
      settings.margin_type = PDF_PRINT_MARGIN_NONE;
    } else if (IsJNIEnumValue(env, margin_type,
                              "org/cef/misc/CefPdfPrintSettings$MarginType",
                              "CUSTOM")) {
      settings.margin_type = PDF_PRINT_MARGIN_CUSTOM;
    }
  }

  GetJNIFieldDouble(env, cls, obj, "margin_top", &settings.margin_top);
  GetJNIFieldDouble(env, cls, obj, "margin_bottom", &settings.margin_bottom);
  GetJNIFieldDouble(env, cls, obj, "margin_right", &settings.margin_right);
  GetJNIFieldDouble(env, cls, obj, "margin_left", &settings.margin_left);

  if (GetJNIFieldString(env, cls, obj, "page_ranges", &tmp) && !tmp.empty()) {
    CefString(&settings.page_ranges) = tmp;
    tmp.clear();
  }

  GetJNIFieldBoolean(env, cls, obj, "display_header_footer",
                     &settings.display_header_footer);

  if (GetJNIFieldString(env, cls, obj, "header_template", &tmp) &&
      !tmp.empty()) {
    CefString(&settings.header_template) = tmp;
    tmp.clear();
  }

  if (GetJNIFieldString(env, cls, obj, "footer_template", &tmp) &&
      !tmp.empty()) {
    CefString(&settings.footer_template) = tmp;
    tmp.clear();
  }

  GetJNIFieldBoolean(env, cls, obj, "generate_tagged_pdf",
                     &settings.generate_tagged_pdf);

  GetJNIFieldBoolean(env, cls, obj, "generate_document_outline",
                     &settings.generate_document_outline);

  return settings;
}

// JNI CefRegistration object.
class ScopedJNIRegistration : public ScopedJNIObject<CefRegistration> {
 public:
  ScopedJNIRegistration(JNIEnv* env, CefRefPtr<CefRegistration> obj)
      : ScopedJNIObject<CefRegistration>(env,
                                         obj,
                                         "org/cef/browser/CefRegistration_N",
                                         "CefRegistration") {}
};

}  // namespace

JNIEXPORT jboolean JNICALL
Java_org_cef_browser_CefBrowser_1N_N_1CreateBrowser(JNIEnv* env,
                                                    jobject jbrowser,
                                                    jobject jclientHandler,
                                                    jlong windowHandle,
                                                    jstring url,
                                                    jboolean osr,
                                                    jboolean transparent,
                                                    jobject jcontext,
                                                    jobject browserSettings) {
  std::shared_ptr<JNIObjectsForCreate> objs(new JNIObjectsForCreate(
      env, jbrowser, nullptr, jclientHandler, url, jcontext, nullptr, browserSettings));
  if (CefCurrentlyOn(TID_UI)) {
    create(objs, windowHandle, osr, transparent);
  } else {
    CefPostTask(TID_UI,
                base::BindOnce(&create, objs, windowHandle, osr, transparent));
  }
  return JNI_FALSE;  // set asynchronously
}

JNIEXPORT jboolean JNICALL
Java_org_cef_browser_CefBrowser_1N_N_1CreateDevTools(JNIEnv* env,
                                                     jobject jbrowser,
                                                     jobject jparent,
                                                     jobject jclientHandler,
                                                     jlong windowHandle,
                                                     jboolean osr,
                                                     jboolean transparent,
                                                     jobject inspect) {
  std::shared_ptr<JNIObjectsForCreate> objs(
      new JNIObjectsForCreate(env, jbrowser, jparent, jclientHandler, nullptr,
                              nullptr, inspect, nullptr));
  if (CefCurrentlyOn(TID_UI)) {
    create(objs, windowHandle, osr, transparent);
  } else {
    CefPostTask(TID_UI,
                base::BindOnce(&create, objs, windowHandle, osr, transparent));
  }
  return JNI_FALSE;  // set asynchronously
}

JNIEXPORT void JNICALL
Java_org_cef_browser_CefBrowser_1N_N_1ExecuteDevToolsMethod(
    JNIEnv* env,
    jobject jbrowser,
    jstring method,
    jstring parametersAsJson,
    jobject jcallback) {
  CefRefPtr<IntCallback> callback = new IntCallback(env, jcallback);

  CefRefPtr<CefBrowser> browser = GetJNIBrowser(env, jbrowser);
  if (!browser.get()) {
    callback->onComplete(0);
    return;
  }

  CefString strMethod = GetJNIString(env, method);
  CefString strParametersAsJson = GetJNIString(env, parametersAsJson);

  if (CefCurrentlyOn(TID_UI)) {
    executeDevToolsMethod(browser->GetHost(), strMethod, strParametersAsJson,
                          callback);
  } else {
    CefPostTask(TID_UI,
                base::BindOnce(executeDevToolsMethod, browser->GetHost(),
                               strMethod, strParametersAsJson, callback));
  }
}

JNIEXPORT jobject JNICALL
Java_org_cef_browser_CefBrowser_1N_N_1AddDevToolsMessageObserver(
    JNIEnv* env,
    jobject jbrowser,
    jobject jobserver) {
  CefRefPtr<CefBrowser> browser =
      JNI_GET_BROWSER_OR_RETURN(env, jbrowser, NULL);

  CefRefPtr<DevToolsMessageObserver> observer =
      new DevToolsMessageObserver(env, jobserver);

  CefRefPtr<CefRegistration> registration =
      browser->GetHost()->AddDevToolsMessageObserver(observer);

  ScopedJNIRegistration jregistration(env, registration);
  return jregistration.Release();
}

JNIEXPORT jlong JNICALL
Java_org_cef_browser_CefBrowser_1N_N_1GetWindowHandle(JNIEnv* env,
                                                      jobject obj,
                                                      jlong displayHandle) {
  CefWindowHandle windowHandle = kNullWindowHandle;
#if defined(OS_WIN)
  windowHandle = ::WindowFromDC((HDC)displayHandle);
#elif defined(OS_LINUX)
  return displayHandle;
#elif defined(OS_MACOSX)
  ASSERT(util_mac::IsNSView((void*)displayHandle));
#endif
  return (jlong)windowHandle;
}

JNIEXPORT jboolean JNICALL
Java_org_cef_browser_CefBrowser_1N_N_1CanGoBack(JNIEnv* env, jobject obj) {
  CefRefPtr<CefBrowser> browser =
      JNI_GET_BROWSER_OR_RETURN(env, obj, JNI_FALSE);
  return browser->CanGoBack() ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT void JNICALL
Java_org_cef_browser_CefBrowser_1N_N_1GoBack(JNIEnv* env, jobject obj) {
  CefRefPtr<CefBrowser> browser = JNI_GET_BROWSER_OR_RETURN(env, obj);
  browser->GoBack();
}

JNIEXPORT jboolean JNICALL
Java_org_cef_browser_CefBrowser_1N_N_1CanGoForward(JNIEnv* env, jobject obj) {
  CefRefPtr<CefBrowser> browser =
      JNI_GET_BROWSER_OR_RETURN(env, obj, JNI_FALSE);
  return browser->CanGoForward() ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT void JNICALL
Java_org_cef_browser_CefBrowser_1N_N_1GoForward(JNIEnv* env, jobject obj) {
  CefRefPtr<CefBrowser> browser = JNI_GET_BROWSER_OR_RETURN(env, obj);
  browser->GoForward();
}

JNIEXPORT jboolean JNICALL
Java_org_cef_browser_CefBrowser_1N_N_1IsLoading(JNIEnv* env, jobject obj) {
  CefRefPtr<CefBrowser> browser =
      JNI_GET_BROWSER_OR_RETURN(env, obj, JNI_FALSE);
  return browser->IsLoading() ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT void JNICALL
Java_org_cef_browser_CefBrowser_1N_N_1Reload(JNIEnv* env, jobject obj) {
  CefRefPtr<CefBrowser> browser = JNI_GET_BROWSER_OR_RETURN(env, obj);
  browser->Reload();
}

JNIEXPORT void JNICALL
Java_org_cef_browser_CefBrowser_1N_N_1ReloadIgnoreCache(JNIEnv* env,
                                                        jobject obj) {
  CefRefPtr<CefBrowser> browser = JNI_GET_BROWSER_OR_RETURN(env, obj);
  browser->ReloadIgnoreCache();
}

JNIEXPORT void JNICALL
Java_org_cef_browser_CefBrowser_1N_N_1StopLoad(JNIEnv* env, jobject obj) {
  CefRefPtr<CefBrowser> browser = JNI_GET_BROWSER_OR_RETURN(env, obj);
  browser->StopLoad();
}

JNIEXPORT jint JNICALL
Java_org_cef_browser_CefBrowser_1N_N_1GetIdentifier(JNIEnv* env, jobject obj) {
  CefRefPtr<CefBrowser> browser = JNI_GET_BROWSER_OR_RETURN(env, obj, -1);
  return browser->GetIdentifier();
}

JNIEXPORT jobject JNICALL
Java_org_cef_browser_CefBrowser_1N_N_1GetMainFrame(JNIEnv* env, jobject obj) {
  CefRefPtr<CefBrowser> browser = JNI_GET_BROWSER_OR_RETURN(env, obj, nullptr);
  CefRefPtr<CefFrame> frame = browser->GetMainFrame();
  if (!frame)
    return nullptr;
  ScopedJNIFrame jframe(env, frame);
  return jframe.Release();
}

JNIEXPORT jobject JNICALL
Java_org_cef_browser_CefBrowser_1N_N_1GetFocusedFrame(JNIEnv* env,
                                                      jobject obj) {
  CefRefPtr<CefBrowser> browser = JNI_GET_BROWSER_OR_RETURN(env, obj, nullptr);
  CefRefPtr<CefFrame> frame = browser->GetFocusedFrame();
  if (!frame)
    return nullptr;
  ScopedJNIFrame jframe(env, frame);
  return jframe.Release();
}

JNIEXPORT jobject JNICALL
Java_org_cef_browser_CefBrowser_1N_N_1GetFrameByIdentifier(JNIEnv* env,
                                                           jobject obj,
                                                           jstring identifier) {
  CefRefPtr<CefBrowser> browser = JNI_GET_BROWSER_OR_RETURN(env, obj, nullptr);
  CefRefPtr<CefFrame> frame =
      browser->GetFrameByIdentifier(GetJNIString(env, identifier));
  if (!frame)
    return nullptr;
  ScopedJNIFrame jframe(env, frame);
  return jframe.Release();
}

JNIEXPORT jobject JNICALL
Java_org_cef_browser_CefBrowser_1N_N_1GetFrameByName(JNIEnv* env,
                                                     jobject obj,
                                                     jstring name) {
  CefRefPtr<CefBrowser> browser = JNI_GET_BROWSER_OR_RETURN(env, obj, nullptr);
  CefRefPtr<CefFrame> frame = browser->GetFrameByName(GetJNIString(env, name));
  if (!frame)
    return nullptr;
  ScopedJNIFrame jframe(env, frame);
  return jframe.Release();
}

JNIEXPORT jint JNICALL
Java_org_cef_browser_CefBrowser_1N_N_1GetFrameCount(JNIEnv* env, jobject obj) {
  CefRefPtr<CefBrowser> browser = JNI_GET_BROWSER_OR_RETURN(env, obj, -1);
  return (jint)browser->GetFrameCount();
}

JNIEXPORT jobject JNICALL
Java_org_cef_browser_CefBrowser_1N_N_1GetFrameIdentifiers(JNIEnv* env,
                                                          jobject obj) {
  CefRefPtr<CefBrowser> browser = JNI_GET_BROWSER_OR_RETURN(env, obj, nullptr);
  std::vector<CefString> identifiers;
  browser->GetFrameIdentifiers(identifiers);
  return NewJNIStringVector(env, identifiers);
}

JNIEXPORT jobject JNICALL
Java_org_cef_browser_CefBrowser_1N_N_1GetFrameNames(JNIEnv* env, jobject obj) {
  CefRefPtr<CefBrowser> browser = JNI_GET_BROWSER_OR_RETURN(env, obj, nullptr);
  std::vector<CefString> names;
  browser->GetFrameNames(names);
  return NewJNIStringVector(env, names);
}

JNIEXPORT jboolean JNICALL
Java_org_cef_browser_CefBrowser_1N_N_1IsPopup(JNIEnv* env, jobject obj) {
  CefRefPtr<CefBrowser> browser =
      JNI_GET_BROWSER_OR_RETURN(env, obj, JNI_FALSE);
  return browser->IsPopup() ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL
Java_org_cef_browser_CefBrowser_1N_N_1HasDocument(JNIEnv* env, jobject obj) {
  CefRefPtr<CefBrowser> browser =
      JNI_GET_BROWSER_OR_RETURN(env, obj, JNI_FALSE);
  return browser->HasDocument() ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT void JNICALL
Java_org_cef_browser_CefBrowser_1N_N_1ViewSource(JNIEnv* env, jobject obj) {
  CefRefPtr<CefBrowser> browser = JNI_GET_BROWSER_OR_RETURN(env, obj);
  CefRefPtr<CefFrame> mainFrame = browser->GetMainFrame();
  CefPostTask(TID_UI, base::BindOnce(&CefFrame::ViewSource, mainFrame.get()));
}

JNIEXPORT void JNICALL
Java_org_cef_browser_CefBrowser_1N_N_1GetSource(JNIEnv* env,
                                                jobject obj,
                                                jobject jvisitor) {
  CefRefPtr<CefBrowser> browser = JNI_GET_BROWSER_OR_RETURN(env, obj);
  browser->GetMainFrame()->GetSource(new StringVisitor(env, jvisitor));
}

JNIEXPORT void JNICALL
Java_org_cef_browser_CefBrowser_1N_N_1GetText(JNIEnv* env,
                                              jobject obj,
                                              jobject jvisitor) {
  CefRefPtr<CefBrowser> browser = JNI_GET_BROWSER_OR_RETURN(env, obj);
  browser->GetMainFrame()->GetText(new StringVisitor(env, jvisitor));
}

JNIEXPORT void JNICALL
Java_org_cef_browser_CefBrowser_1N_N_1LoadRequest(JNIEnv* env,
                                                  jobject obj,
                                                  jobject jrequest) {
  CefRefPtr<CefBrowser> browser = JNI_GET_BROWSER_OR_RETURN(env, obj);
  ScopedJNIRequest requestObj(env);
  requestObj.SetHandle(jrequest, false /* should_delete */);
  CefRefPtr<CefRequest> request = requestObj.GetCefObject();
  if (!request)
    return;
  browser->GetMainFrame()->LoadRequest(request);
}

JNIEXPORT void JNICALL
Java_org_cef_browser_CefBrowser_1N_N_1LoadURL(JNIEnv* env,
                                              jobject obj,
                                              jstring url) {
  CefRefPtr<CefBrowser> browser = JNI_GET_BROWSER_OR_RETURN(env, obj);
  browser->GetMainFrame()->LoadURL(GetJNIString(env, url));
}

JNIEXPORT void JNICALL
Java_org_cef_browser_CefBrowser_1N_N_1ExecuteJavaScript(JNIEnv* env,
                                                        jobject obj,
                                                        jstring code,
                                                        jstring url,
                                                        jint line) {
  CefRefPtr<CefBrowser> browser = JNI_GET_BROWSER_OR_RETURN(env, obj);
  browser->GetMainFrame()->ExecuteJavaScript(GetJNIString(env, code),
                                             GetJNIString(env, url), line);
}

JNIEXPORT jstring JNICALL
Java_org_cef_browser_CefBrowser_1N_N_1GetURL(JNIEnv* env, jobject obj) {
  jstring tmp = NewJNIString(env, "");
  CefRefPtr<CefBrowser> browser = JNI_GET_BROWSER_OR_RETURN(env, obj, tmp);
  return NewJNIString(env, browser->GetMainFrame()->GetURL());
}

JNIEXPORT void JNICALL
Java_org_cef_browser_CefBrowser_1N_N_1Close(JNIEnv* env,
                                            jobject obj,
                                            jboolean force) {
  CefRefPtr<CefBrowser> browser = JNI_GET_BROWSER_OR_RETURN(env, obj);
  if (force != JNI_FALSE) {
    if (browser->GetHost()->IsWindowRenderingDisabled()) {
      browser->GetHost()->CloseBrowser(true);
    } else {
      // Destroy the native window representation.
      if (CefCurrentlyOn(TID_UI))
        util::DestroyCefBrowser(browser);
      else
        CefPostTask(TID_UI, base::BindOnce(&util::DestroyCefBrowser, browser));
    }
  } else {
    browser->GetHost()->CloseBrowser(false);
  }
}

JNIEXPORT void JNICALL
Java_org_cef_browser_CefBrowser_1N_N_1SetFocus(JNIEnv* env,
                                               jobject obj,
                                               jboolean enable) {
  CefRefPtr<CefBrowser> browser = JNI_GET_BROWSER_OR_RETURN(env, obj);
  browser->GetHost()->SetFocus(enable != JNI_FALSE);
}

JNIEXPORT void JNICALL
Java_org_cef_browser_CefBrowser_1N_N_1SetWindowVisibility(JNIEnv* env,
                                                          jobject obj,
                                                          jboolean visible) {
  CefRefPtr<CefBrowser> browser = JNI_GET_BROWSER_OR_RETURN(env, obj);

#if defined(OS_MACOSX)
  if (!browser->GetHost()->IsWindowRenderingDisabled()) {
    util_mac::SetVisibility(browser->GetHost()->GetWindowHandle(),
                            visible != JNI_FALSE);
  }
#endif
}

JNIEXPORT jdouble JNICALL
Java_org_cef_browser_CefBrowser_1N_N_1GetZoomLevel(JNIEnv* env, jobject obj) {
  CefRefPtr<CefBrowser> browser = JNI_GET_BROWSER_OR_RETURN(env, obj, 0.0);
  CefRefPtr<CefBrowserHost> host = browser->GetHost();
  double result = 0.0;
  if (CefCurrentlyOn(TID_UI))
    result = host->GetZoomLevel();
  else {
    CriticalLock lock;
    CriticalWait waitCond(&lock);
    lock.Lock();
    CefPostTask(TID_UI, base::BindOnce(getZoomLevel, host, &waitCond, &result));
    waitCond.Wait(1000);
    lock.Unlock();
  }
  return result;
}

JNIEXPORT void JNICALL
Java_org_cef_browser_CefBrowser_1N_N_1SetZoomLevel(JNIEnv* env,
                                                   jobject obj,
                                                   jdouble zoom) {
  CefRefPtr<CefBrowser> browser = JNI_GET_BROWSER_OR_RETURN(env, obj);
  browser->GetHost()->SetZoomLevel(zoom);
}

JNIEXPORT void JNICALL
Java_org_cef_browser_CefBrowser_1N_N_1RunFileDialog(JNIEnv* env,
                                                    jobject obj,
                                                    jobject jmode,
                                                    jstring jtitle,
                                                    jstring jdefaultFilePath,
                                                    jobject jacceptFilters,
                                                    jint selectedAcceptFilter,
                                                    jobject jcallback) {
  CefRefPtr<CefBrowser> browser = JNI_GET_BROWSER_OR_RETURN(env, obj);

  std::vector<CefString> accept_types;
  GetJNIStringVector(env, jacceptFilters, accept_types);

  CefBrowserHost::FileDialogMode mode;
  if (IsJNIEnumValue(env, jmode,
                     "org/cef/handler/CefDialogHandler$FileDialogMode",
                     "FILE_DIALOG_OPEN")) {
    mode = FILE_DIALOG_OPEN;
  } else if (IsJNIEnumValue(env, jmode,
                            "org/cef/handler/CefDialogHandler$FileDialogMode",
                            "FILE_DIALOG_OPEN_MULTIPLE")) {
    mode = FILE_DIALOG_OPEN_MULTIPLE;
  } else if (IsJNIEnumValue(env, jmode,
                            "org/cef/handler/CefDialogHandler$FileDialogMode",
                            "FILE_DIALOG_SAVE")) {
    mode = FILE_DIALOG_SAVE;
  } else {
    mode = FILE_DIALOG_OPEN;
  }

  browser->GetHost()->RunFileDialog(
      mode, GetJNIString(env, jtitle), GetJNIString(env, jdefaultFilePath),
      accept_types, new RunFileDialogCallback(env, jcallback));
}

JNIEXPORT void JNICALL
Java_org_cef_browser_CefBrowser_1N_N_1StartDownload(JNIEnv* env,
                                                    jobject obj,
                                                    jstring url) {
  CefRefPtr<CefBrowser> browser = JNI_GET_BROWSER_OR_RETURN(env, obj);
  browser->GetHost()->StartDownload(GetJNIString(env, url));
}

JNIEXPORT void JNICALL
Java_org_cef_browser_CefBrowser_1N_N_1Print(JNIEnv* env, jobject obj) {
  CefRefPtr<CefBrowser> browser = JNI_GET_BROWSER_OR_RETURN(env, obj);
  browser->GetHost()->Print();
}

JNIEXPORT void JNICALL
Java_org_cef_browser_CefBrowser_1N_N_1PrintToPDF(JNIEnv* env,
                                                 jobject obj,
                                                 jstring jpath,
                                                 jobject jsettings,
                                                 jobject jcallback) {
  CefRefPtr<CefBrowser> browser = JNI_GET_BROWSER_OR_RETURN(env, obj);

  CefPdfPrintSettings settings = GetJNIPdfPrintSettings(env, jsettings);

  browser->GetHost()->PrintToPDF(GetJNIString(env, jpath), settings,
                                 new PdfPrintCallback(env, jcallback));
}

JNIEXPORT void JNICALL
Java_org_cef_browser_CefBrowser_1N_N_1Find(JNIEnv* env,
                                           jobject obj,
                                           jstring searchText,
                                           jboolean forward,
                                           jboolean matchCase,
                                           jboolean findNext) {
  CefRefPtr<CefBrowser> browser = JNI_GET_BROWSER_OR_RETURN(env, obj);
  browser->GetHost()->Find(GetJNIString(env, searchText),
                           (forward != JNI_FALSE), (matchCase != JNI_FALSE),
                           (findNext != JNI_FALSE));
}

JNIEXPORT void JNICALL
Java_org_cef_browser_CefBrowser_1N_N_1StopFinding(JNIEnv* env,
                                                  jobject obj,
                                                  jboolean clearSelection) {
  CefRefPtr<CefBrowser> browser = JNI_GET_BROWSER_OR_RETURN(env, obj);
  browser->GetHost()->StopFinding(clearSelection != JNI_FALSE);
}

JNIEXPORT void JNICALL
Java_org_cef_browser_CefBrowser_1N_N_1CloseDevTools(JNIEnv* env, jobject obj) {
  CefRefPtr<CefBrowser> browser = JNI_GET_BROWSER_OR_RETURN(env, obj);
  browser->GetHost()->CloseDevTools();
}

JNIEXPORT void JNICALL
Java_org_cef_browser_CefBrowser_1N_N_1ReplaceMisspelling(JNIEnv* env,
                                                         jobject obj,
                                                         jstring jword) {
  CefRefPtr<CefBrowser> browser = JNI_GET_BROWSER_OR_RETURN(env, obj);
  browser->GetHost()->ReplaceMisspelling(GetJNIString(env, jword));
}

JNIEXPORT void JNICALL
Java_org_cef_browser_CefBrowser_1N_N_1WasResized(JNIEnv* env,
                                                 jobject obj,
                                                 jint width,
                                                 jint height) {
  CefRefPtr<CefBrowser> browser = JNI_GET_BROWSER_OR_RETURN(env, obj);
  if (browser->GetHost()->IsWindowRenderingDisabled()) {
    browser->GetHost()->WasResized();
  }
#if (defined(OS_WIN) || defined(OS_LINUX))
  else {
    CefWindowHandle browserHandle = browser->GetHost()->GetWindowHandle();
    if (CefCurrentlyOn(TID_UI)) {
      util::SetWindowSize(browserHandle, width, height);
    } else {
      CefPostTask(TID_UI, base::BindOnce(util::SetWindowSize, browserHandle,
                                         (int)width, (int)height));
    }
  }
#endif
}

JNIEXPORT void JNICALL
Java_org_cef_browser_CefBrowser_1N_N_1Invalidate(JNIEnv* env, jobject obj) {
  CefRefPtr<CefBrowser> browser = JNI_GET_BROWSER_OR_RETURN(env, obj);
  browser->GetHost()->Invalidate(PET_VIEW);
}

JNIEXPORT void JNICALL
Java_org_cef_browser_CefBrowser_1N_N_1SendKeyEvent(JNIEnv* env,
                                                   jobject obj,
                                                   jobject key_event) {
  CefRefPtr<CefBrowser> browser = JNI_GET_BROWSER_OR_RETURN(env, obj);
  ScopedJNIClass cls(env, "org/lwjgl/glfw/GLFW");
  ScopedJNIClass objClass = ScopedJNIClass(env, env->GetObjectClass(key_event));
  if (!cls || !objClass)
    return;

  JNI_STATIC_DEFINE_INT(env, cls, GLFW_PRESS);
  JNI_STATIC_DEFINE_INT(env, cls, GLFW_RELEASE);
  JNI_STATIC_DEFINE_INT(env, cls, GLFW_REPEAT);

  int event_type, modifiers;
  char16_t key_char;
  if (!CallJNIMethodI_V(env, objClass, key_event, "getID", &event_type) ||
      !CallJNIMethodC_V(env, objClass, key_event, "getKeyChar", &key_char) ||
      !CallJNIMethodI_V(env, objClass, key_event, "getModifiers", &modifiers)) {
    return;
  }

  CefKeyEvent cef_event;
  cef_event.modifiers = GetCefModifiersGlfw(env, cls, modifiers);

#if defined(OS_WIN)
  jlong scanCode = 0;
  GetJNIFieldLong(env, objClass, key_event, "scancode", &scanCode);
  BYTE VkCode = LOBYTE(MapVirtualKey(scanCode, MAPVK_VSC_TO_VK));
  cef_event.native_key_code = (scanCode << 16) |  // key scan code
                              1;                  // key repeat count
#elif defined(OS_LINUX) || defined(OS_MACOSX)
  int key_code;
  if (!CallJNIMethodI_V(env, objClass, key_event, "getKeyCode", &key_code)) {
    return;
  }

  JNI_STATIC_DEFINE_INT(env, cls, GLFW_KEY_BACKSPACE);
  JNI_STATIC_DEFINE_INT(env, cls, GLFW_KEY_DELETE);
  JNI_STATIC_DEFINE_INT(env, cls, GLFW_KEY_DOWN);
  JNI_STATIC_DEFINE_INT(env, cls, GLFW_KEY_ENTER);
  JNI_STATIC_DEFINE_INT(env, cls, GLFW_KEY_ESCAPE);
  JNI_STATIC_DEFINE_INT(env, cls, GLFW_KEY_LEFT);
  JNI_STATIC_DEFINE_INT(env, cls, GLFW_KEY_RIGHT);
  JNI_STATIC_DEFINE_INT(env, cls, GLFW_KEY_TAB);
  JNI_STATIC_DEFINE_INT(env, cls, GLFW_KEY_UP);

#if defined(OS_LINUX)
  if (key_code == JNI_STATIC(GLFW_KEY_BACKSPACE))
    cef_event.native_key_code = XK_BackSpace;
  else if (key_code == JNI_STATIC(GLFW_KEY_DELETE))
    cef_event.native_key_code = XK_Delete;
  else if (key_code == JNI_STATIC(GLFW_KEY_DOWN))
    cef_event.native_key_code = XK_Down;
  else if (key_code == JNI_STATIC(GLFW_KEY_ENTER))
    cef_event.native_key_code = XK_Return;
  else if (key_code == JNI_STATIC(GLFW_KEY_ESCAPE))
    cef_event.native_key_code = XK_Escape;
  else if (key_code == JNI_STATIC(GLFW_KEY_LEFT))
    cef_event.native_key_code = XK_Left;
  else if (key_code == JNI_STATIC(GLFW_KEY_RIGHT))
    cef_event.native_key_code = XK_Right;
  else if (key_code == JNI_STATIC(GLFW_KEY_TAB))
    cef_event.native_key_code = XK_Tab;
  else if (key_code == JNI_STATIC(GLFW_KEY_UP))
    cef_event.native_key_code = XK_Up;
  else
    cef_event.native_key_code = key_char;

  KeyboardCode windows_key_code =
      KeyboardCodeFromXKeysym(cef_event.native_key_code);
  cef_event.windows_key_code =
      GetWindowsKeyCodeWithoutLocation(windows_key_code);

  if (cef_event.modifiers & EVENTFLAG_ALT_DOWN)
    cef_event.is_system_key = true;

  if (windows_key_code == VKEY_RETURN) {
    // We need to treat the enter key as a key press of character \r.  This
    // is apparently just how webkit handles it and what it expects.
    cef_event.unmodified_character = '\r';
  } else {
    cef_event.unmodified_character = cef_event.native_key_code;
  }

  // If ctrl key is pressed down, then control character shall be input.
  if (cef_event.modifiers & EVENTFLAG_CONTROL_DOWN) {
    cef_event.character = GetControlCharacter(
        windows_key_code, cef_event.modifiers & EVENTFLAG_SHIFT_DOWN);
  } else {
    cef_event.character = cef_event.unmodified_character;
  }
#elif defined(OS_MACOSX)
  if (key_code == JNI_STATIC(GLFW_KEY_BACKSPACE)) {
    cef_event.native_key_code = kVK_Delete;
    cef_event.unmodified_character = kBackspaceCharCode;
  } else if (key_code == JNI_STATIC(GLFW_KEY_DELETE)) {
    cef_event.native_key_code = kVK_ForwardDelete;
    cef_event.unmodified_character = kDeleteCharCode;
  } else if (key_code == JNI_STATIC(GLFW_KEY_DOWN)) {
    cef_event.native_key_code = kVK_DownArrow;
    cef_event.unmodified_character = /* NSDownArrowFunctionKey */ 0xF701;
  } else if (key_code == JNI_STATIC(GLFW_KEY_ENTER)) {
    cef_event.native_key_code = kVK_Return;
    cef_event.unmodified_character = kReturnCharCode;
  } else if (key_code == JNI_STATIC(GLFW_KEY_ESCAPE)) {
    cef_event.native_key_code = kVK_Escape;
    cef_event.unmodified_character = kEscapeCharCode;
  } else if (key_code == JNI_STATIC(GLFW_KEY_LEFT)) {
    cef_event.native_key_code = kVK_LeftArrow;
    cef_event.unmodified_character = /* NSLeftArrowFunctionKey */ 0xF702;
  } else if (key_code == JNI_STATIC(GLFW_KEY_RIGHT)) {
    cef_event.native_key_code = kVK_RightArrow;
    cef_event.unmodified_character = /* NSRightArrowFunctionKey */ 0xF703;
  } else if (key_code == JNI_STATIC(GLFW_KEY_TAB)) {
    cef_event.native_key_code = kVK_Tab;
    cef_event.unmodified_character = kTabCharCode;
  } else if (key_code == JNI_STATIC(GLFW_KEY_UP)) {
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

  if (event_type == JNI_STATIC(GLFW_PRESS)) {
#if defined(OS_WIN)
    cef_event.windows_key_code = VkCode;
#endif
    cef_event.type = KEYEVENT_RAWKEYDOWN;
  } else if (event_type == JNI_STATIC(GLFW_RELEASE)) {
#if defined(OS_WIN)
    cef_event.windows_key_code = VkCode;
    // bits 30 and 31 should always be 1 for WM_KEYUP
    cef_event.native_key_code |= 0xC0000000;
#endif
    cef_event.type = KEYEVENT_KEYUP;
  } else if (event_type == JNI_STATIC(GLFW_REPEAT)) {
#if defined(OS_WIN)
    cef_event.windows_key_code = key_char;
#endif
    cef_event.type = KEYEVENT_CHAR;
  } else {
    return;
  }

  browser->GetHost()->SendKeyEvent(cef_event);
}

JNIEXPORT void JNICALL
Java_org_cef_browser_CefBrowser_1N_N_1SendMouseEvent(JNIEnv* env,
                                                     jobject obj,
                                                     jobject mouse_event) {
  CefRefPtr<CefBrowser> browser = JNI_GET_BROWSER_OR_RETURN(env, obj);
  ScopedJNIClass cls(env, "org/lwjgl/glfw/GLFW");
  ScopedJNIClass objClass = ScopedJNIClass(env, env->GetObjectClass(mouse_event));
  if (!cls || !objClass)
    return;

  JNI_STATIC_DEFINE_INT(env, cls, GLFW_MOUSE_BUTTON_1);
  JNI_STATIC_DEFINE_INT(env, cls, GLFW_MOUSE_BUTTON_2);
  JNI_STATIC_DEFINE_INT(env, cls, GLFW_MOUSE_BUTTON_3);
  JNI_STATIC_DEFINE_INT(env, cls, GLFW_PRESS);
  JNI_STATIC_DEFINE_INT(env, cls, GLFW_RELEASE);

  int event_type, x, y, modifiers;
  if (!CallJNIMethodI_V(env, objClass, mouse_event, "getID", &event_type) ||
      !CallJNIMethodI_V(env, objClass, mouse_event, "getX", &x) ||
      !CallJNIMethodI_V(env, objClass, mouse_event, "getY", &y) ||
      !CallJNIMethodI_V(env, objClass, mouse_event, "getModifiers", &modifiers)) {
    return;
  }

  CefMouseEvent cef_event;
  cef_event.x = x;
  cef_event.y = y;

  cef_event.modifiers = GetCefModifiersGlfw(env, cls, modifiers);

  if (event_type == JNI_STATIC(GLFW_PRESS) ||
      event_type == JNI_STATIC(GLFW_RELEASE)) {
    int click_count, button;
    if (!CallJNIMethodI_V(env, objClass, mouse_event, "getClickCount",
                          &click_count) ||
        !CallJNIMethodI_V(env, objClass, mouse_event, "getButton", &button)) {
      return;
    }

    CefBrowserHost::MouseButtonType cef_mbt;
    if (button == JNI_STATIC(GLFW_MOUSE_BUTTON_1))
      cef_mbt = MBT_LEFT;
    else if (button == JNI_STATIC(GLFW_MOUSE_BUTTON_2))
      cef_mbt = MBT_MIDDLE;
    else if (button == JNI_STATIC(GLFW_MOUSE_BUTTON_3))
      cef_mbt = MBT_RIGHT;
    else
      return;

    browser->GetHost()->SendMouseClickEvent(
        cef_event, cef_mbt, (event_type == JNI_STATIC(GLFW_RELEASE)),
        click_count);
  } else if (event_type == 503 || // MOUSE_MOVED
             event_type == 506 || // MOUSE_DRAGGED
             event_type == 504 || // MOUSE_ENTERED
             event_type == 505) { // MOUSE_EXITED
    browser->GetHost()->SendMouseMoveEvent(
        cef_event, (event_type == 505)); // MOUSE_EXITED
  }
}

JNIEXPORT void JNICALL
Java_org_cef_browser_CefBrowser_1N_N_1SendMouseWheelEvent(
    JNIEnv* env,
    jobject obj,
    jobject mouse_wheel_event) {
  CefRefPtr<CefBrowser> browser = JNI_GET_BROWSER_OR_RETURN(env, obj);
  ScopedJNIClass cls(env, "org/lwjgl/glfw/GLFW");
  ScopedJNIClass objClass = ScopedJNIClass(env, env->GetObjectClass(mouse_wheel_event));
  if (!cls || !objClass)
    return;

  JNI_STATIC_DEFINE_INT(env, objClass, WHEEL_UNIT_SCROLL);

  int scroll_type, x, y, modifiers;
  double delta;
  if (!CallJNIMethodI_V(env, objClass, mouse_wheel_event, "getScrollType",
                        &scroll_type) ||
      !CallJNIMethodD_V(env, objClass, mouse_wheel_event, "getWheelRotation",
                        &delta) ||
      !CallJNIMethodI_V(env, objClass, mouse_wheel_event, "getX", &x) ||
      !CallJNIMethodI_V(env, objClass, mouse_wheel_event, "getY", &y) ||
      !CallJNIMethodI_V(env, objClass, mouse_wheel_event, "getModifiers",
                        &modifiers)) {
    return;
  }

  CefMouseEvent cef_event;
  cef_event.x = x;
  cef_event.y = y;

  cef_event.modifiers = GetCefModifiersGlfw(env, cls, modifiers);

  if (scroll_type == 0) { // WHEEL_UNIT_SCROLL
    // Use the smarter version that considers platform settings.
    CallJNIMethodD_V(env, objClass, mouse_wheel_event, "getUnitsToScroll", &delta);
  }

  double deltaX = 0, deltaY = 0;
  if (cef_event.modifiers & EVENTFLAG_SHIFT_DOWN)
    deltaX = delta;
  else
    deltaY = delta;

  browser->GetHost()->SendMouseWheelEvent(cef_event, deltaX, deltaY);
}

JNIEXPORT void JNICALL
Java_org_cef_browser_CefBrowser_1N_N_1DragTargetDragEnter(JNIEnv* env,
                                                          jobject obj,
                                                          jobject jdragData,
                                                          jobject pos,
                                                          jint jmodifiers,
                                                          jint allowedOps) {
  CefRefPtr<CefDragData> drag_data =
      GetCefFromJNIObject<CefDragData>(env, jdragData, "CefDragData");
  if (!drag_data.get())
    return;
  ScopedJNIClass cls(env, "org/lwjgl/glfw/GLFW");
  if (!cls)
    return;

  CefMouseEvent cef_event;
  GetJNIPoint(env, pos, &cef_event.x, &cef_event.y);
  cef_event.modifiers = GetCefModifiersGlfw(env, cls, jmodifiers);

  CefRefPtr<CefBrowser> browser = JNI_GET_BROWSER_OR_RETURN(env, obj);
  browser->GetHost()->DragTargetDragEnter(
      drag_data, cef_event, (CefBrowserHost::DragOperationsMask)allowedOps);
}

JNIEXPORT void JNICALL
Java_org_cef_browser_CefBrowser_1N_N_1DragTargetDragOver(JNIEnv* env,
                                                         jobject obj,
                                                         jobject pos,
                                                         jint jmodifiers,
                                                         jint allowedOps) {
  ScopedJNIClass cls(env, "org/lwjgl/glfw/GLFW");
  if (!cls)
    return;

  CefMouseEvent cef_event;
  GetJNIPoint(env, pos, &cef_event.x, &cef_event.y);
  cef_event.modifiers = GetCefModifiersGlfw(env, cls, jmodifiers);

  CefRefPtr<CefBrowser> browser = JNI_GET_BROWSER_OR_RETURN(env, obj);
  browser->GetHost()->DragTargetDragOver(
      cef_event, (CefBrowserHost::DragOperationsMask)allowedOps);
}

JNIEXPORT void JNICALL
Java_org_cef_browser_CefBrowser_1N_N_1DragTargetDragLeave(JNIEnv* env,
                                                          jobject obj) {
  CefRefPtr<CefBrowser> browser = JNI_GET_BROWSER_OR_RETURN(env, obj);
  browser->GetHost()->DragTargetDragLeave();
}

JNIEXPORT void JNICALL
Java_org_cef_browser_CefBrowser_1N_N_1DragTargetDrop(JNIEnv* env,
                                                     jobject obj,
                                                     jobject pos,
                                                     jint jmodifiers) {
  ScopedJNIClass cls(env, "org/lwjgl/glfw/GLFW");
  if (!cls)
    return;

  CefMouseEvent cef_event;
  GetJNIPoint(env, pos, &cef_event.x, &cef_event.y);
  cef_event.modifiers = GetCefModifiersGlfw(env, cls, jmodifiers);

  CefRefPtr<CefBrowser> browser = JNI_GET_BROWSER_OR_RETURN(env, obj);
  browser->GetHost()->DragTargetDrop(cef_event);
}

JNIEXPORT void JNICALL
Java_org_cef_browser_CefBrowser_1N_N_1DragSourceEndedAt(JNIEnv* env,
                                                        jobject obj,
                                                        jobject pos,
                                                        jint operation) {
  CefRefPtr<CefBrowser> browser = JNI_GET_BROWSER_OR_RETURN(env, obj);
  int x, y;
  GetJNIPoint(env, pos, &x, &y);
  browser->GetHost()->DragSourceEndedAt(
      x, y, (CefBrowserHost::DragOperationsMask)operation);
}

JNIEXPORT void JNICALL
Java_org_cef_browser_CefBrowser_1N_N_1DragSourceSystemDragEnded(JNIEnv* env,
                                                                jobject obj) {
  CefRefPtr<CefBrowser> browser = JNI_GET_BROWSER_OR_RETURN(env, obj);
  browser->GetHost()->DragSourceSystemDragEnded();
}

JNIEXPORT void JNICALL
Java_org_cef_browser_CefBrowser_1N_N_1UpdateUI(JNIEnv* env,
                                               jobject obj,
                                               jobject jcontentRect,
                                               jobject jbrowserRect) {
  CefRefPtr<CefBrowser> browser = JNI_GET_BROWSER_OR_RETURN(env, obj);
  CefRect contentRect = GetJNIRect(env, jcontentRect);
#if defined(OS_MACOSX)
  CefRect browserRect = GetJNIRect(env, jbrowserRect);
  util_mac::UpdateView(browser->GetHost()->GetWindowHandle(), contentRect,
                       browserRect);
#else
  CefWindowHandle windowHandle = browser->GetHost()->GetWindowHandle();
  if (CefCurrentlyOn(TID_UI)) {
    util::SetWindowBounds(windowHandle, contentRect);
  } else {
    CefPostTask(TID_UI, base::BindOnce(util::SetWindowBounds, windowHandle,
                                       contentRect));
  }
#endif
}

JNIEXPORT void JNICALL
Java_org_cef_browser_CefBrowser_1N_N_1NotifyMoveOrResizeStarted(JNIEnv* env,
                                                                jobject obj) {
#if (defined(OS_WIN) || defined(OS_LINUX))
  CefRefPtr<CefBrowser> browser = JNI_GET_BROWSER_OR_RETURN(env, obj);
  if (!browser->GetHost()->IsWindowRenderingDisabled()) {
    browser->GetHost()->NotifyMoveOrResizeStarted();
  }
#endif
}

JNIEXPORT void JNICALL
Java_org_cef_browser_CefBrowser_1N_N_1SetWindowlessFrameRate(JNIEnv* env,
                                                             jobject jbrowser,
                                                             jint frameRate) {
  CefRefPtr<CefBrowser> browser = JNI_GET_BROWSER_OR_RETURN(env, jbrowser);
  CefRefPtr<CefBrowserHost> host = browser->GetHost();
  host->SetWindowlessFrameRate(frameRate);
}

void getWindowlessFrameRate(CefRefPtr<CefBrowserHost> host,
                            CefRefPtr<IntCallback> callback) {
  callback->onComplete((jint)host->GetWindowlessFrameRate());
}

JNIEXPORT void JNICALL
Java_org_cef_browser_CefBrowser_1N_N_1GetWindowlessFrameRate(
    JNIEnv* env,
    jobject jbrowser,
    jobject jintCallback) {
  CefRefPtr<IntCallback> callback = new IntCallback(env, jintCallback);

  CefRefPtr<CefBrowser> browser = GetJNIBrowser(env, jbrowser);
  if (!browser.get()) {
    callback->onComplete(0);
    return;
  }

  CefRefPtr<CefBrowserHost> host = browser->GetHost();
  if (CefCurrentlyOn(TID_UI)) {
    getWindowlessFrameRate(host, callback);
  } else {
    CefPostTask(TID_UI, base::BindOnce(getWindowlessFrameRate, host, callback));
  }
}
