// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "CefMenuModel_N.h"

#include "include/cef_menu_model.h"

#include "jni_scoped_helpers.h"
#include "jni_util.h"

namespace {

CefRefPtr<CefMenuModel> GetSelf(jlong self) {
  return reinterpret_cast<CefMenuModel*>(self);
}

}  // namespace

JNIEXPORT jboolean JNICALL
Java_org_cef_callback_CefMenuModel_1N_N_1Clear(JNIEnv* env,
                                               jobject obj,
                                               jlong self) {
  CefRefPtr<CefMenuModel> menuModel = GetSelf(self);
  if (!menuModel)
    return JNI_FALSE;
  return menuModel->Clear() ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jint JNICALL
Java_org_cef_callback_CefMenuModel_1N_N_1GetCount(JNIEnv* env,
                                                  jobject obj,
                                                  jlong self) {
  CefRefPtr<CefMenuModel> menuModel = GetSelf(self);
  if (!menuModel)
    return 0;
  return (jint)menuModel->GetCount();
}

JNIEXPORT jboolean JNICALL
Java_org_cef_callback_CefMenuModel_1N_N_1AddSeparator(JNIEnv* env,
                                                      jobject obj,
                                                      jlong self) {
  CefRefPtr<CefMenuModel> menuModel = GetSelf(self);
  if (!menuModel)
    return JNI_FALSE;
  return menuModel->AddSeparator() ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL
Java_org_cef_callback_CefMenuModel_1N_N_1AddItem(JNIEnv* env,
                                                 jobject obj,
                                                 jlong self,
                                                 jint jcommand_id,
                                                 jstring jlabel) {
  CefRefPtr<CefMenuModel> menuModel = GetSelf(self);
  if (!menuModel)
    return JNI_FALSE;
  return menuModel->AddItem(jcommand_id, GetJNIString(env, jlabel)) ? JNI_TRUE
                                                                    : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL
Java_org_cef_callback_CefMenuModel_1N_N_1AddCheckItem(JNIEnv* env,
                                                      jobject obj,
                                                      jlong self,
                                                      jint jcommand_id,
                                                      jstring jlabel) {
  CefRefPtr<CefMenuModel> menuModel = GetSelf(self);
  if (!menuModel)
    return JNI_FALSE;
  return menuModel->AddCheckItem(jcommand_id, GetJNIString(env, jlabel))
             ? JNI_TRUE
             : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL
Java_org_cef_callback_CefMenuModel_1N_N_1AddRadioItem(JNIEnv* env,
                                                      jobject obj,
                                                      jlong self,
                                                      jint jcommand_id,
                                                      jstring jlabel,
                                                      jint jgroup_id) {
  CefRefPtr<CefMenuModel> menuModel = GetSelf(self);
  if (!menuModel)
    return JNI_FALSE;
  return menuModel->AddRadioItem(jcommand_id, GetJNIString(env, jlabel),
                                 jgroup_id)
             ? JNI_TRUE
             : JNI_FALSE;
}

JNIEXPORT jobject JNICALL
Java_org_cef_callback_CefMenuModel_1N_N_1AddSubMenu(JNIEnv* env,
                                                    jobject obj,
                                                    jlong self,
                                                    jint jcommand_id,
                                                    jstring jlabel) {
  CefRefPtr<CefMenuModel> menuModel = GetSelf(self);
  if (!menuModel)
    return nullptr;
  CefRefPtr<CefMenuModel> subMenu =
      menuModel->AddSubMenu(jcommand_id, GetJNIString(env, jlabel));
  ScopedJNIMenuModel jmenuModel(env, subMenu);
  return jmenuModel.Release();
}

JNIEXPORT jboolean JNICALL
Java_org_cef_callback_CefMenuModel_1N_N_1InsertSeparatorAt(JNIEnv* env,
                                                           jobject obj,
                                                           jlong self,
                                                           jint jindex) {
  CefRefPtr<CefMenuModel> menuModel = GetSelf(self);
  if (!menuModel)
    return JNI_FALSE;
  return menuModel->InsertSeparatorAt(jindex) ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL
Java_org_cef_callback_CefMenuModel_1N_N_1InsertItemAt(JNIEnv* env,
                                                      jobject obj,
                                                      jlong self,
                                                      jint jindex,
                                                      jint jcommand_id,
                                                      jstring jlabel) {
  CefRefPtr<CefMenuModel> menuModel = GetSelf(self);
  if (!menuModel)
    return JNI_FALSE;
  return menuModel->InsertItemAt(jindex, jcommand_id, GetJNIString(env, jlabel))
             ? JNI_TRUE
             : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL
Java_org_cef_callback_CefMenuModel_1N_N_1InsertCheckItemAt(JNIEnv* env,
                                                           jobject obj,
                                                           jlong self,
                                                           jint jindex,
                                                           jint jcommand_id,
                                                           jstring jlabel) {
  CefRefPtr<CefMenuModel> menuModel = GetSelf(self);
  if (!menuModel)
    return JNI_FALSE;
  return menuModel->InsertCheckItemAt(jindex, jcommand_id,
                                      GetJNIString(env, jlabel))
             ? JNI_TRUE
             : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL
Java_org_cef_callback_CefMenuModel_1N_N_1InsertRadioItemAt(JNIEnv* env,
                                                           jobject obj,
                                                           jlong self,
                                                           jint jindex,
                                                           jint jcommand_id,
                                                           jstring jlabel,
                                                           jint jgroup_id) {
  CefRefPtr<CefMenuModel> menuModel = GetSelf(self);
  if (!menuModel)
    return JNI_FALSE;
  return menuModel->InsertRadioItemAt(jindex, jcommand_id,
                                      GetJNIString(env, jlabel), jgroup_id)
             ? JNI_TRUE
             : JNI_FALSE;
}

JNIEXPORT jobject JNICALL
Java_org_cef_callback_CefMenuModel_1N_N_1InsertSubMenuAt(JNIEnv* env,
                                                         jobject obj,
                                                         jlong self,
                                                         jint jindex,
                                                         jint jcommand_id,
                                                         jstring jlabel) {
  CefRefPtr<CefMenuModel> menuModel = GetSelf(self);
  if (!menuModel)
    return nullptr;
  CefRefPtr<CefMenuModel> subMenu = menuModel->InsertSubMenuAt(
      jindex, jcommand_id, GetJNIString(env, jlabel));
  ScopedJNIMenuModel jmenuModel(env, subMenu);
  return jmenuModel.Release();
}

JNIEXPORT jboolean JNICALL
Java_org_cef_callback_CefMenuModel_1N_N_1Remove(JNIEnv* env,
                                                jobject obj,
                                                jlong self,
                                                jint jcommand_id) {
  CefRefPtr<CefMenuModel> menuModel = GetSelf(self);
  if (!menuModel)
    return JNI_FALSE;
  return menuModel->Remove(jcommand_id) ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL
Java_org_cef_callback_CefMenuModel_1N_N_1RemoveAt(JNIEnv* env,
                                                  jobject obj,
                                                  jlong self,
                                                  jint jindex) {
  CefRefPtr<CefMenuModel> menuModel = GetSelf(self);
  if (!menuModel)
    return JNI_FALSE;
  return menuModel->RemoveAt(jindex) ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jint JNICALL
Java_org_cef_callback_CefMenuModel_1N_N_1GetIndexOf(JNIEnv* env,
                                                    jobject obj,
                                                    jlong self,
                                                    jint jcommand_id) {
  CefRefPtr<CefMenuModel> menuModel = GetSelf(self);
  if (!menuModel)
    return -1;
  return menuModel->GetIndexOf(jcommand_id);
}

JNIEXPORT jint JNICALL
Java_org_cef_callback_CefMenuModel_1N_N_1GetCommandIdAt(JNIEnv* env,
                                                        jobject obj,
                                                        jlong self,
                                                        jint jindex) {
  CefRefPtr<CefMenuModel> menuModel = GetSelf(self);
  if (!menuModel)
    return -1;
  return menuModel->GetCommandIdAt(jindex);
}

JNIEXPORT jboolean JNICALL
Java_org_cef_callback_CefMenuModel_1N_N_1SetCommandIdAt(JNIEnv* env,
                                                        jobject obj,
                                                        jlong self,
                                                        jint jindex,
                                                        jint jcommand_id) {
  CefRefPtr<CefMenuModel> menuModel = GetSelf(self);
  if (!menuModel)
    return JNI_FALSE;
  return menuModel->SetCommandIdAt(jindex, jcommand_id) ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jstring JNICALL
Java_org_cef_callback_CefMenuModel_1N_N_1GetLabel(JNIEnv* env,
                                                  jobject obj,
                                                  jlong self,
                                                  jint jcommand_id) {
  CefRefPtr<CefMenuModel> menuModel = GetSelf(self);
  if (!menuModel)
    return nullptr;
  return NewJNIString(env, menuModel->GetLabel(jcommand_id));
}

JNIEXPORT jstring JNICALL
Java_org_cef_callback_CefMenuModel_1N_N_1GetLabelAt(JNIEnv* env,
                                                    jobject obj,
                                                    jlong self,
                                                    jint jindex) {
  CefRefPtr<CefMenuModel> menuModel = GetSelf(self);
  if (!menuModel)
    return nullptr;
  return NewJNIString(env, menuModel->GetLabelAt(jindex));
}

JNIEXPORT jboolean JNICALL
Java_org_cef_callback_CefMenuModel_1N_N_1SetLabel(JNIEnv* env,
                                                  jobject obj,
                                                  jlong self,
                                                  jint jcommand_id,
                                                  jstring jlabel) {
  CefRefPtr<CefMenuModel> menuModel = GetSelf(self);
  if (!menuModel)
    return JNI_FALSE;
  return menuModel->SetLabel(jcommand_id, GetJNIString(env, jlabel))
             ? JNI_TRUE
             : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL
Java_org_cef_callback_CefMenuModel_1N_N_1SetLabelAt(JNIEnv* env,
                                                    jobject obj,
                                                    jlong self,
                                                    jint jindex,
                                                    jstring jlabel) {
  CefRefPtr<CefMenuModel> menuModel = GetSelf(self);
  if (!menuModel)
    return JNI_FALSE;
  return menuModel->SetLabelAt(jindex, GetJNIString(env, jlabel)) ? JNI_TRUE
                                                                  : JNI_FALSE;
}

jobject toJType(JNIEnv* env, CefMenuModel::MenuItemType type) {
  jobject result = nullptr;
  switch (type) {
    JNI_CASE(env, "org/cef/callback/CefMenuModel$MenuItemType",
             MENUITEMTYPE_COMMAND, result);
    JNI_CASE(env, "org/cef/callback/CefMenuModel$MenuItemType",
             MENUITEMTYPE_CHECK, result);
    JNI_CASE(env, "org/cef/callback/CefMenuModel$MenuItemType",
             MENUITEMTYPE_RADIO, result);
    JNI_CASE(env, "org/cef/callback/CefMenuModel$MenuItemType",
             MENUITEMTYPE_SEPARATOR, result);
    JNI_CASE(env, "org/cef/callback/CefMenuModel$MenuItemType",
             MENUITEMTYPE_SUBMENU, result);
    default:
      JNI_CASE(env, "org/cef/callback/CefMenuModel$MenuItemType",
               MENUITEMTYPE_NONE, result);
  }
  return result;
}

JNIEXPORT jobject JNICALL
Java_org_cef_callback_CefMenuModel_1N_N_1GetType(JNIEnv* env,
                                                 jobject obj,
                                                 jlong self,
                                                 jint jcommand_id) {
  CefRefPtr<CefMenuModel> menuModel = GetSelf(self);
  if (!menuModel)
    return toJType(env, MENUITEMTYPE_NONE);
  return toJType(env, menuModel->GetType(jcommand_id));
}

JNIEXPORT jobject JNICALL
Java_org_cef_callback_CefMenuModel_1N_N_1GetTypeAt(JNIEnv* env,
                                                   jobject obj,
                                                   jlong self,
                                                   jint jindex) {
  CefRefPtr<CefMenuModel> menuModel = GetSelf(self);
  if (!menuModel)
    return toJType(env, MENUITEMTYPE_NONE);
  return toJType(env, menuModel->GetTypeAt(jindex));
}

JNIEXPORT jint JNICALL
Java_org_cef_callback_CefMenuModel_1N_N_1GetGroupId(JNIEnv* env,
                                                    jobject obj,
                                                    jlong self,
                                                    jint jcommand_id) {
  CefRefPtr<CefMenuModel> menuModel = GetSelf(self);
  if (!menuModel)
    return -1;
  return menuModel->GetGroupId(jcommand_id);
}

JNIEXPORT jint JNICALL
Java_org_cef_callback_CefMenuModel_1N_N_1GetGroupIdAt(JNIEnv* env,
                                                      jobject obj,
                                                      jlong self,
                                                      jint jindex) {
  CefRefPtr<CefMenuModel> menuModel = GetSelf(self);
  if (!menuModel)
    return -1;
  return menuModel->GetGroupIdAt(jindex);
}

JNIEXPORT jboolean JNICALL
Java_org_cef_callback_CefMenuModel_1N_N_1SetGroupId(JNIEnv* env,
                                                    jobject obj,
                                                    jlong self,
                                                    jint jcommand_id,
                                                    jint jgroup_id) {
  CefRefPtr<CefMenuModel> menuModel = GetSelf(self);
  if (!menuModel)
    return JNI_FALSE;
  return menuModel->SetGroupId(jcommand_id, jgroup_id) ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL
Java_org_cef_callback_CefMenuModel_1N_N_1SetGroupIdAt(JNIEnv* env,
                                                      jobject obj,
                                                      jlong self,
                                                      jint jindex,
                                                      jint jgroup_id) {
  CefRefPtr<CefMenuModel> menuModel = GetSelf(self);
  if (!menuModel)
    return JNI_FALSE;
  return menuModel->SetGroupIdAt(jindex, jgroup_id) ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jobject JNICALL
Java_org_cef_callback_CefMenuModel_1N_N_1GetSubMenu(JNIEnv* env,
                                                    jobject obj,
                                                    jlong self,
                                                    jint jcommand_id) {
  CefRefPtr<CefMenuModel> menuModel = GetSelf(self);
  if (!menuModel)
    return nullptr;

  CefRefPtr<CefMenuModel> subMenu = menuModel->GetSubMenu(jcommand_id);
  ScopedJNIMenuModel jmenuModel(env, subMenu);
  return jmenuModel.Release();
}

JNIEXPORT jobject JNICALL
Java_org_cef_callback_CefMenuModel_1N_N_1GetSubMenuAt(JNIEnv* env,
                                                      jobject obj,
                                                      jlong self,
                                                      jint jindex) {
  CefRefPtr<CefMenuModel> menuModel = GetSelf(self);
  if (!menuModel)
    return nullptr;

  CefRefPtr<CefMenuModel> subMenu = menuModel->GetSubMenuAt(jindex);
  ScopedJNIMenuModel jmenuModel(env, subMenu);
  return jmenuModel.Release();
}

JNIEXPORT jboolean JNICALL
Java_org_cef_callback_CefMenuModel_1N_N_1IsVisible(JNIEnv* env,
                                                   jobject obj,
                                                   jlong self,
                                                   jint jcommand_id) {
  CefRefPtr<CefMenuModel> menuModel = GetSelf(self);
  if (!menuModel)
    return JNI_FALSE;
  return menuModel->IsVisible(jcommand_id) ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL
Java_org_cef_callback_CefMenuModel_1N_N_1IsVisibleAt(JNIEnv* env,
                                                     jobject obj,
                                                     jlong self,
                                                     jint jindex) {
  CefRefPtr<CefMenuModel> menuModel = GetSelf(self);
  if (!menuModel)
    return JNI_FALSE;
  return menuModel->IsVisibleAt(jindex) ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL
Java_org_cef_callback_CefMenuModel_1N_N_1SetVisible(JNIEnv* env,
                                                    jobject obj,
                                                    jlong self,
                                                    jint jcommand_id,
                                                    jboolean jvisible) {
  CefRefPtr<CefMenuModel> menuModel = GetSelf(self);
  if (!menuModel)
    return JNI_FALSE;
  return menuModel->SetVisible(jcommand_id, (jvisible != JNI_FALSE))
             ? JNI_TRUE
             : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL
Java_org_cef_callback_CefMenuModel_1N_N_1SetVisibleAt(JNIEnv* env,
                                                      jobject obj,
                                                      jlong self,
                                                      jint jindex,
                                                      jboolean jvisible) {
  CefRefPtr<CefMenuModel> menuModel = GetSelf(self);
  if (!menuModel)
    return JNI_FALSE;
  return menuModel->SetVisibleAt(jindex, (jvisible != JNI_FALSE)) ? JNI_TRUE
                                                                  : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL
Java_org_cef_callback_CefMenuModel_1N_N_1IsEnabled(JNIEnv* env,
                                                   jobject obj,
                                                   jlong self,
                                                   jint jcommand_id) {
  CefRefPtr<CefMenuModel> menuModel = GetSelf(self);
  if (!menuModel)
    return JNI_FALSE;
  return menuModel->IsEnabled(jcommand_id) ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL
Java_org_cef_callback_CefMenuModel_1N_N_1IsEnabledAt(JNIEnv* env,
                                                     jobject obj,
                                                     jlong self,
                                                     jint jindex) {
  CefRefPtr<CefMenuModel> menuModel = GetSelf(self);
  if (!menuModel)
    return JNI_FALSE;
  return menuModel->IsEnabledAt(jindex) ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL
Java_org_cef_callback_CefMenuModel_1N_N_1SetEnabled(JNIEnv* env,
                                                    jobject obj,
                                                    jlong self,
                                                    jint jcommand_id,
                                                    jboolean jenabled) {
  CefRefPtr<CefMenuModel> menuModel = GetSelf(self);
  if (!menuModel)
    return JNI_FALSE;
  return menuModel->SetEnabled(jcommand_id, jenabled != JNI_FALSE) ? JNI_TRUE
                                                                   : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL
Java_org_cef_callback_CefMenuModel_1N_N_1SetEnabledAt(JNIEnv* env,
                                                      jobject obj,
                                                      jlong self,
                                                      jint jindex,
                                                      jboolean jenabled) {
  CefRefPtr<CefMenuModel> menuModel = GetSelf(self);
  if (!menuModel)
    return JNI_FALSE;
  return menuModel->SetEnabledAt(jindex, jenabled != JNI_FALSE) ? JNI_TRUE
                                                                : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL
Java_org_cef_callback_CefMenuModel_1N_N_1IsChecked(JNIEnv* env,
                                                   jobject obj,
                                                   jlong self,
                                                   jint jcommand_id) {
  CefRefPtr<CefMenuModel> menuModel = GetSelf(self);
  if (!menuModel)
    return JNI_FALSE;
  return menuModel->IsChecked(jcommand_id) ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL
Java_org_cef_callback_CefMenuModel_1N_N_1IsCheckedAt(JNIEnv* env,
                                                     jobject obj,
                                                     jlong self,
                                                     jint jindex) {
  CefRefPtr<CefMenuModel> menuModel = GetSelf(self);
  if (!menuModel)
    return JNI_FALSE;
  return menuModel->IsCheckedAt(jindex) ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL
Java_org_cef_callback_CefMenuModel_1N_N_1SetChecked(JNIEnv* env,
                                                    jobject obj,
                                                    jlong self,
                                                    jint jcommand_id,
                                                    jboolean jchecked) {
  CefRefPtr<CefMenuModel> menuModel = GetSelf(self);
  if (!menuModel)
    return JNI_FALSE;
  return menuModel->SetChecked(jcommand_id, (jchecked != JNI_FALSE))
             ? JNI_TRUE
             : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL
Java_org_cef_callback_CefMenuModel_1N_N_1SetCheckedAt(JNIEnv* env,
                                                      jobject obj,
                                                      jlong self,
                                                      jint jindex,
                                                      jboolean jchecked) {
  CefRefPtr<CefMenuModel> menuModel = GetSelf(self);
  if (!menuModel)
    return JNI_FALSE;
  return menuModel->SetCheckedAt(jindex, (jchecked != JNI_FALSE)) ? JNI_TRUE
                                                                  : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL
Java_org_cef_callback_CefMenuModel_1N_N_1HasAccelerator(JNIEnv* env,
                                                        jobject obj,
                                                        jlong self,
                                                        jint jcommand_id) {
  CefRefPtr<CefMenuModel> menuModel = GetSelf(self);
  if (!menuModel)
    return JNI_FALSE;
  return menuModel->HasAccelerator(jcommand_id) ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL
Java_org_cef_callback_CefMenuModel_1N_N_1HasAcceleratorAt(JNIEnv* env,
                                                          jobject obj,
                                                          jlong self,
                                                          jint jindex) {
  CefRefPtr<CefMenuModel> menuModel = GetSelf(self);
  if (!menuModel)
    return JNI_FALSE;
  return menuModel->HasAcceleratorAt(jindex) ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL
Java_org_cef_callback_CefMenuModel_1N_N_1SetAccelerator(JNIEnv* env,
                                                        jobject obj,
                                                        jlong self,
                                                        jint jcommand_id,
                                                        jint jkey_code,
                                                        jboolean jshift_pressed,
                                                        jboolean jctrl_pressed,
                                                        jboolean jalt_pressed) {
  CefRefPtr<CefMenuModel> menuModel = GetSelf(self);
  if (!menuModel)
    return JNI_FALSE;
  return menuModel->SetAccelerator(
             jcommand_id, jkey_code, (jshift_pressed != JNI_FALSE),
             (jctrl_pressed != JNI_FALSE), (jalt_pressed != JNI_FALSE))
             ? JNI_TRUE
             : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL
Java_org_cef_callback_CefMenuModel_1N_N_1SetAcceleratorAt(
    JNIEnv* env,
    jobject obj,
    jlong self,
    jint jindex,
    jint jkey_code,
    jboolean jshift_pressed,
    jboolean jctrl_pressed,
    jboolean jalt_pressed) {
  CefRefPtr<CefMenuModel> menuModel = GetSelf(self);
  if (!menuModel)
    return JNI_FALSE;
  return menuModel->SetAcceleratorAt(
             jindex, jkey_code, (jshift_pressed != JNI_FALSE),
             (jctrl_pressed != JNI_FALSE), (jalt_pressed != JNI_FALSE))
             ? JNI_TRUE
             : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL
Java_org_cef_callback_CefMenuModel_1N_N_1RemoveAccelerator(JNIEnv* env,
                                                           jobject obj,
                                                           jlong self,
                                                           jint jcommand_id) {
  CefRefPtr<CefMenuModel> menuModel = GetSelf(self);
  if (!menuModel)
    return JNI_FALSE;
  return menuModel->RemoveAccelerator(jcommand_id) ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL
Java_org_cef_callback_CefMenuModel_1N_N_1RemoveAcceleratorAt(JNIEnv* env,
                                                             jobject obj,
                                                             jlong self,
                                                             jint jindex) {
  CefRefPtr<CefMenuModel> menuModel = GetSelf(self);
  if (!menuModel)
    return JNI_FALSE;
  return menuModel->RemoveAcceleratorAt(jindex) ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL
Java_org_cef_callback_CefMenuModel_1N_N_1GetAccelerator(JNIEnv* env,
                                                        jobject obj,
                                                        jlong self,
                                                        jint jcommand_id,
                                                        jobject jkey_code,
                                                        jobject jshift_pressed,
                                                        jobject jctrl_pressed,
                                                        jobject jalt_pressed) {
  CefRefPtr<CefMenuModel> menuModel = GetSelf(self);
  if (!menuModel)
    return JNI_FALSE;
  int key_code = 0;
  bool shift_pressed = false;
  bool ctrl_pressed = false;
  bool alt_pressed = false;
  bool success = menuModel->GetAccelerator(jcommand_id, key_code, shift_pressed,
                                           ctrl_pressed, alt_pressed);
  if (!success)
    return JNI_FALSE;

  SetJNIIntRef(env, jkey_code, key_code);
  SetJNIBoolRef(env, jshift_pressed, shift_pressed);
  SetJNIBoolRef(env, jctrl_pressed, ctrl_pressed);
  SetJNIBoolRef(env, jalt_pressed, alt_pressed);
  return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_org_cef_callback_CefMenuModel_1N_N_1GetAcceleratorAt(
    JNIEnv* env,
    jobject obj,
    jlong self,
    jint jindex,
    jobject jkey_code,
    jobject jshift_pressed,
    jobject jctrl_pressed,
    jobject jalt_pressed) {
  CefRefPtr<CefMenuModel> menuModel = GetSelf(self);
  if (!menuModel)
    return JNI_FALSE;
  int key_code = 0;
  bool shift_pressed = false;
  bool ctrl_pressed = false;
  bool alt_pressed = false;
  bool success = menuModel->GetAcceleratorAt(jindex, key_code, shift_pressed,
                                             ctrl_pressed, alt_pressed);
  if (!success)
    return JNI_FALSE;

  SetJNIIntRef(env, jkey_code, key_code);
  SetJNIBoolRef(env, jshift_pressed, shift_pressed);
  SetJNIBoolRef(env, jctrl_pressed, ctrl_pressed);
  SetJNIBoolRef(env, jalt_pressed, alt_pressed);
  return JNI_TRUE;
}
