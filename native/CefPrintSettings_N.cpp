// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "CefPrintSettings_N.h"

#include "include/cef_print_handler.h"

#include "jni_scoped_helpers.h"
#include "jni_util.h"

namespace {

const char kCefClassName[] = "CefPrintSettings";

CefRefPtr<CefPrintSettings> GetSelf(jlong self) {
  return reinterpret_cast<CefPrintSettings*>(self);
}

// Retrieve the CefPageRange equivalent of a org.cef.misc.CefPageRange
CefRange GetJNIPageRange(JNIEnv* env, jobject obj) {
  CefRange range;

  ScopedJNIClass cls(env, "org/cef/misc/CefPageRange");
  if (!cls)
    return range;

  int from, to;
  if (GetJNIFieldInt(env, cls, obj, "from", &from) &&
      GetJNIFieldInt(env, cls, obj, "to", &to)) {
    range.Set(from, to);
  }
  return range;
}

// Create a new org.cef.misc.CefPageRange
jobject NewJNIPageRange(JNIEnv* env, const CefRange& range) {
  ScopedJNIClass cls(env, "org/cef/misc/CefPageRange");
  if (!cls)
    return nullptr;

  ScopedJNIObjectLocal obj(env, NewJNIObject(env, cls));
  if (!obj)
    return nullptr;

  if (SetJNIFieldInt(env, cls, obj, "from", range.from) &&
      SetJNIFieldInt(env, cls, obj, "to", range.to)) {
    return obj.Release();
  }

  return nullptr;
}

}  // namespace

JNIEXPORT jobject JNICALL
Java_org_cef_misc_CefPrintSettings_1N_N_1Create(JNIEnv* env, jclass cls) {
  CefRefPtr<CefPrintSettings> settings = CefPrintSettings::Create();
  ScopedJNIPrintSettings jsettings(env, settings);
  return jsettings.Release();
}

JNIEXPORT void JNICALL
Java_org_cef_misc_CefPrintSettings_1N_N_1Dispose(JNIEnv* env,
                                                 jobject obj,
                                                 jlong self) {
  SetCefForJNIObject<CefPrintSettings>(env, obj, nullptr, kCefClassName);
}

JNIEXPORT jboolean JNICALL
Java_org_cef_misc_CefPrintSettings_1N_N_1IsValid(JNIEnv* env,
                                                 jobject obj,
                                                 jlong self) {
  CefRefPtr<CefPrintSettings> settings = GetSelf(self);
  if (!settings)
    return JNI_FALSE;
  return settings->IsValid() ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL
Java_org_cef_misc_CefPrintSettings_1N_N_1IsReadOnly(JNIEnv* env,
                                                    jobject obj,
                                                    jlong self) {
  CefRefPtr<CefPrintSettings> settings = GetSelf(self);
  if (!settings)
    return JNI_FALSE;
  return settings->IsReadOnly() ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT void JNICALL
Java_org_cef_misc_CefPrintSettings_1N_N_1SetOrientation(JNIEnv* env,
                                                        jobject obj,
                                                        jlong self,
                                                        jboolean jlandscape) {
  CefRefPtr<CefPrintSettings> settings = GetSelf(self);
  if (!settings)
    return;
  settings->SetOrientation(jlandscape != JNI_FALSE);
}

JNIEXPORT jboolean JNICALL
Java_org_cef_misc_CefPrintSettings_1N_N_1IsLandscape(JNIEnv* env,
                                                     jobject obj,
                                                     jlong self) {
  CefRefPtr<CefPrintSettings> settings = GetSelf(self);
  if (!settings)
    return JNI_FALSE;
  return settings->IsLandscape() ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT void JNICALL
Java_org_cef_misc_CefPrintSettings_1N_N_1SetPrinterPrintableArea(
    JNIEnv* env,
    jobject obj,
    jlong self,
    jobject jphysical_size_device_units,
    jobject jprintable_area_device_units,
    jboolean jlandscape_needs_flip) {
  CefRefPtr<CefPrintSettings> settings = GetSelf(self);
  if (!settings)
    return;

  CefSize physical_size = GetJNISize(env, jphysical_size_device_units);
  CefRect printable_area = GetJNIRect(env, jprintable_area_device_units);
  bool needs_flip = jlandscape_needs_flip != JNI_FALSE;
  settings->SetPrinterPrintableArea(physical_size, printable_area, needs_flip);
}

JNIEXPORT void JNICALL
Java_org_cef_misc_CefPrintSettings_1N_N_1SetDeviceName(JNIEnv* env,
                                                       jobject obj,
                                                       jlong self,
                                                       jstring jname) {
  CefRefPtr<CefPrintSettings> settings = GetSelf(self);
  if (!settings)
    return;
  settings->SetDeviceName(GetJNIString(env, jname));
}

JNIEXPORT jstring JNICALL
Java_org_cef_misc_CefPrintSettings_1N_N_1GetDeviceName(JNIEnv* env,
                                                       jobject obj,
                                                       jlong self) {
  CefRefPtr<CefPrintSettings> settings = GetSelf(self);
  if (!settings)
    return nullptr;
  return NewJNIString(env, settings->GetDeviceName());
}

JNIEXPORT void JNICALL
Java_org_cef_misc_CefPrintSettings_1N_N_1SetDPI(JNIEnv* env,
                                                jobject obj,
                                                jlong self,
                                                jint jdpi) {
  CefRefPtr<CefPrintSettings> settings = GetSelf(self);
  if (!settings)
    return;
  settings->SetDPI((int)jdpi);
}

JNIEXPORT jint JNICALL
Java_org_cef_misc_CefPrintSettings_1N_N_1GetDPI(JNIEnv* env,
                                                jobject obj,
                                                jlong self) {
  CefRefPtr<CefPrintSettings> settings = GetSelf(self);
  if (!settings)
    return 0;
  return (jint)settings->GetDPI();
}

JNIEXPORT void JNICALL
Java_org_cef_misc_CefPrintSettings_1N_N_1SetPageRanges(JNIEnv* env,
                                                       jobject obj,
                                                       jlong self,
                                                       jobject jrangeVector) {
  CefRefPtr<CefPrintSettings> settings = GetSelf(self);
  if (!settings || !jrangeVector)
    return;

  CefPrintSettings::PageRangeList rangeList;
  jint jsize = 0;
  JNI_CALL_METHOD(env, jrangeVector, "size", "()I", Int, jsize);
  for (jint index = 0; index < jsize; index++) {
    ScopedJNIObjectResult jresult(env);
    JNI_CALL_METHOD(env, jrangeVector, "get", "(I)Ljava/lang/Object;", Object,
                    jresult, index);
    if (!jresult)
      continue;
    rangeList.push_back(GetJNIPageRange(env, jresult));
  }
  settings->SetPageRanges(rangeList);
}

JNIEXPORT jint JNICALL
Java_org_cef_misc_CefPrintSettings_1N_N_1GetPageRangesCount(JNIEnv* env,
                                                            jobject obj,
                                                            jlong self) {
  CefRefPtr<CefPrintSettings> settings = GetSelf(self);
  if (!settings)
    return 0;
  return (jint)settings->GetPageRangesCount();
}

JNIEXPORT void JNICALL
Java_org_cef_misc_CefPrintSettings_1N_N_1GetPageRanges(JNIEnv* env,
                                                       jobject obj,
                                                       jlong self,
                                                       jobject jrangeVector) {
  CefRefPtr<CefPrintSettings> settings = GetSelf(self);
  if (!settings || !jrangeVector)
    return;

  CefPrintSettings::PageRangeList rangeList;
  settings->GetPageRanges(rangeList);

  CefPrintSettings::PageRangeList::size_type i = 0;
  for (i = 0; i < rangeList.size(); ++i) {
    ScopedJNIObjectLocal jrange(env, NewJNIPageRange(env, rangeList.at(i)));
    JNI_CALL_VOID_METHOD(env, jrangeVector, "addElement",
                         "(Ljava/lang/Object;)V", jrange.get());
  }
}

JNIEXPORT void JNICALL
Java_org_cef_misc_CefPrintSettings_1N_N_1SetSelectionOnly(
    JNIEnv* env,
    jobject obj,
    jlong self,
    jboolean jselection_only) {
  CefRefPtr<CefPrintSettings> settings = GetSelf(self);
  if (!settings)
    return;
  settings->SetSelectionOnly(jselection_only != JNI_FALSE);
}

JNIEXPORT jboolean JNICALL
Java_org_cef_misc_CefPrintSettings_1N_N_1IsSelectionOnly(JNIEnv* env,
                                                         jobject obj,
                                                         jlong self) {
  CefRefPtr<CefPrintSettings> settings = GetSelf(self);
  if (!settings)
    return JNI_FALSE;
  return settings->IsSelectionOnly() ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT void JNICALL
Java_org_cef_misc_CefPrintSettings_1N_N_1SetCollate(JNIEnv* env,
                                                    jobject obj,
                                                    jlong self,
                                                    jboolean jcollate) {
  CefRefPtr<CefPrintSettings> settings = GetSelf(self);
  if (!settings)
    return;
  settings->SetCollate(jcollate != JNI_FALSE);
}

JNIEXPORT jboolean JNICALL
Java_org_cef_misc_CefPrintSettings_1N_N_1WillCollate(JNIEnv* env,
                                                     jobject obj,
                                                     jlong self) {
  CefRefPtr<CefPrintSettings> settings = GetSelf(self);
  if (!settings)
    return JNI_FALSE;
  return settings->WillCollate() ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT void JNICALL
Java_org_cef_misc_CefPrintSettings_1N_N_1SetColorModel(JNIEnv* env,
                                                       jobject obj,
                                                       jlong self,
                                                       jobject jcolor_model) {
  CefRefPtr<CefPrintSettings> settings = GetSelf(self);
  if (!settings)
    return;
  CefPrintSettings::ColorModel mode;
  if (IsJNIEnumValue(env, jcolor_model,
                     "org/cef/misc/CefPrintSettings$ColorModel",
                     "COLOR_MODEL_GRAY")) {
    mode = COLOR_MODEL_GRAY;
  } else if (IsJNIEnumValue(env, jcolor_model,
                            "org/cef/misc/CefPrintSettings$ColorModel",
                            "COLOR_MODEL_COLOR")) {
    mode = COLOR_MODEL_COLOR;
  } else if (IsJNIEnumValue(env, jcolor_model,
                            "org/cef/misc/CefPrintSettings$ColorModel",
                            "COLOR_MODEL_CMYK")) {
    mode = COLOR_MODEL_CMYK;
  } else if (IsJNIEnumValue(env, jcolor_model,
                            "org/cef/misc/CefPrintSettings$ColorModel",
                            "COLOR_MODEL_CMY")) {
    mode = COLOR_MODEL_CMY;
  } else if (IsJNIEnumValue(env, jcolor_model,
                            "org/cef/misc/CefPrintSettings$ColorModel",
                            "COLOR_MODEL_KCMY")) {
    mode = COLOR_MODEL_KCMY;
  } else if (IsJNIEnumValue(env, jcolor_model,
                            "org/cef/misc/CefPrintSettings$ColorModel",
                            "COLOR_MODEL_CMY_K")) {
    mode = COLOR_MODEL_CMY_K;
  } else if (IsJNIEnumValue(env, jcolor_model,
                            "org/cef/misc/CefPrintSettings$ColorModel",
                            "COLOR_MODEL_BLACK")) {
    mode = COLOR_MODEL_BLACK;
  } else if (IsJNIEnumValue(env, jcolor_model,
                            "org/cef/misc/CefPrintSettings$ColorModel",
                            "COLOR_MODEL_GRAYSCALE")) {
    mode = COLOR_MODEL_GRAYSCALE;
  } else if (IsJNIEnumValue(env, jcolor_model,
                            "org/cef/misc/CefPrintSettings$ColorModel",
                            "COLOR_MODEL_RGB")) {
    mode = COLOR_MODEL_RGB;
  } else if (IsJNIEnumValue(env, jcolor_model,
                            "org/cef/misc/CefPrintSettings$ColorModel",
                            "COLOR_MODEL_RGB16")) {
    mode = COLOR_MODEL_RGB16;
  } else if (IsJNIEnumValue(env, jcolor_model,
                            "org/cef/misc/CefPrintSettings$ColorModel",
                            "COLOR_MODEL_RGBA")) {
    mode = COLOR_MODEL_RGBA;
  } else if (IsJNIEnumValue(env, jcolor_model,
                            "org/cef/misc/CefPrintSettings$ColorModel",
                            "COLOR_MODEL_COLORMODE_COLOR")) {
    mode = COLOR_MODEL_COLORMODE_COLOR;
  } else if (IsJNIEnumValue(env, jcolor_model,
                            "org/cef/misc/CefPrintSettings$ColorModel",
                            "COLOR_MODEL_COLORMODE_MONOCHROME")) {
    mode = COLOR_MODEL_COLORMODE_MONOCHROME;
  } else if (IsJNIEnumValue(env, jcolor_model,
                            "org/cef/misc/CefPrintSettings$ColorModel",
                            "COLOR_MODEL_HP_COLOR_COLOR")) {
    mode = COLOR_MODEL_HP_COLOR_COLOR;
  } else if (IsJNIEnumValue(env, jcolor_model,
                            "org/cef/misc/CefPrintSettings$ColorModel",
                            "COLOR_MODEL_HP_COLOR_BLACK")) {
    mode = COLOR_MODEL_HP_COLOR_BLACK;
  } else if (IsJNIEnumValue(env, jcolor_model,
                            "org/cef/misc/CefPrintSettings$ColorModel",
                            "COLOR_MODEL_PRINTOUTMODE_NORMAL")) {
    mode = COLOR_MODEL_PRINTOUTMODE_NORMAL;
  } else if (IsJNIEnumValue(env, jcolor_model,
                            "org/cef/misc/CefPrintSettings$ColorModel",
                            "COLOR_MODEL_PRINTOUTMODE_NORMAL_GRAY")) {
    mode = COLOR_MODEL_PRINTOUTMODE_NORMAL_GRAY;
  } else if (IsJNIEnumValue(env, jcolor_model,
                            "org/cef/misc/CefPrintSettings$ColorModel",
                            "COLOR_MODEL_PROCESSCOLORMODEL_CMYK")) {
    mode = COLOR_MODEL_PROCESSCOLORMODEL_CMYK;
  } else if (IsJNIEnumValue(env, jcolor_model,
                            "org/cef/misc/CefPrintSettings$ColorModel",
                            "COLOR_MODEL_PROCESSCOLORMODEL_GREYSCALE")) {
    mode = COLOR_MODEL_PROCESSCOLORMODEL_GREYSCALE;
  } else if (IsJNIEnumValue(env, jcolor_model,
                            "org/cef/misc/CefPrintSettings$ColorModel",
                            "COLOR_MODEL_PROCESSCOLORMODEL_RGB")) {
    mode = COLOR_MODEL_PROCESSCOLORMODEL_RGB;
  } else {
    mode = COLOR_MODEL_UNKNOWN;
  }
  settings->SetColorModel(mode);
}

JNIEXPORT jobject JNICALL
Java_org_cef_misc_CefPrintSettings_1N_N_1GetColorModel(JNIEnv* env,
                                                       jobject obj,
                                                       jlong self) {
  jobject result = GetJNIEnumValue(
      env, "org/cef/misc/CefPrintSettings$ColorModel", "COLOR_MODEL_UNKNOWN");

  CefRefPtr<CefPrintSettings> settings = GetSelf(self);
  if (!settings)
    return result;

  switch (settings->GetColorModel()) {
    JNI_CASE(env, "org/cef/misc/CefPrintSettings$ColorModel", COLOR_MODEL_GRAY,
             result);
    JNI_CASE(env, "org/cef/misc/CefPrintSettings$ColorModel", COLOR_MODEL_COLOR,
             result);
    JNI_CASE(env, "org/cef/misc/CefPrintSettings$ColorModel", COLOR_MODEL_CMYK,
             result);
    JNI_CASE(env, "org/cef/misc/CefPrintSettings$ColorModel", COLOR_MODEL_CMY,
             result);
    JNI_CASE(env, "org/cef/misc/CefPrintSettings$ColorModel", COLOR_MODEL_KCMY,
             result);
    JNI_CASE(env, "org/cef/misc/CefPrintSettings$ColorModel", COLOR_MODEL_CMY_K,
             result);
    JNI_CASE(env, "org/cef/misc/CefPrintSettings$ColorModel", COLOR_MODEL_BLACK,
             result);
    JNI_CASE(env, "org/cef/misc/CefPrintSettings$ColorModel",
             COLOR_MODEL_GRAYSCALE, result);
    JNI_CASE(env, "org/cef/misc/CefPrintSettings$ColorModel", COLOR_MODEL_RGB,
             result);
    JNI_CASE(env, "org/cef/misc/CefPrintSettings$ColorModel", COLOR_MODEL_RGB16,
             result);
    JNI_CASE(env, "org/cef/misc/CefPrintSettings$ColorModel", COLOR_MODEL_RGBA,
             result);
    JNI_CASE(env, "org/cef/misc/CefPrintSettings$ColorModel",
             COLOR_MODEL_COLORMODE_COLOR, result);
    JNI_CASE(env, "org/cef/misc/CefPrintSettings$ColorModel",
             COLOR_MODEL_COLORMODE_MONOCHROME, result);
    JNI_CASE(env, "org/cef/misc/CefPrintSettings$ColorModel",
             COLOR_MODEL_HP_COLOR_COLOR, result);
    JNI_CASE(env, "org/cef/misc/CefPrintSettings$ColorModel",
             COLOR_MODEL_HP_COLOR_BLACK, result);
    JNI_CASE(env, "org/cef/misc/CefPrintSettings$ColorModel",
             COLOR_MODEL_PRINTOUTMODE_NORMAL, result);
    JNI_CASE(env, "org/cef/misc/CefPrintSettings$ColorModel",
             COLOR_MODEL_PRINTOUTMODE_NORMAL_GRAY, result);
    JNI_CASE(env, "org/cef/misc/CefPrintSettings$ColorModel",
             COLOR_MODEL_PROCESSCOLORMODEL_CMYK, result);
    JNI_CASE(env, "org/cef/misc/CefPrintSettings$ColorModel",
             COLOR_MODEL_PROCESSCOLORMODEL_GREYSCALE, result);
    JNI_CASE(env, "org/cef/misc/CefPrintSettings$ColorModel",
             COLOR_MODEL_PROCESSCOLORMODEL_RGB, result);
    default:
      JNI_CASE(env, "org/cef/misc/CefPrintSettings$ColorModel",
               COLOR_MODEL_UNKNOWN, result);
  }
  return result;
}

JNIEXPORT void JNICALL
Java_org_cef_misc_CefPrintSettings_1N_N_1SetCopies(JNIEnv* env,
                                                   jobject obj,
                                                   jlong self,
                                                   jint jcopies) {
  CefRefPtr<CefPrintSettings> settings = GetSelf(self);
  if (!settings)
    return;
  settings->SetCopies((int)jcopies);
}

JNIEXPORT jint JNICALL
Java_org_cef_misc_CefPrintSettings_1N_N_1GetCopies(JNIEnv* env,
                                                   jobject obj,
                                                   jlong self) {
  CefRefPtr<CefPrintSettings> settings = GetSelf(self);
  if (!settings)
    return 0;
  return (jint)settings->GetCopies();
}

JNIEXPORT void JNICALL
Java_org_cef_misc_CefPrintSettings_1N_N_1SetDuplexMode(JNIEnv* env,
                                                       jobject obj,
                                                       jlong self,
                                                       jobject jduplex_mode) {
  CefRefPtr<CefPrintSettings> settings = GetSelf(self);
  if (!settings)
    return;
  CefPrintSettings::DuplexMode mode;
  if (IsJNIEnumValue(env, jduplex_mode,
                     "org/cef/misc/CefPrintSettings$DuplexMode",
                     "DUPLEX_MODE_SIMPLEX")) {
    mode = DUPLEX_MODE_SIMPLEX;
  } else if (IsJNIEnumValue(env, jduplex_mode,
                            "org/cef/misc/CefPrintSettings$DuplexMode",
                            "DUPLEX_MODE_LONG_EDGE")) {
    mode = DUPLEX_MODE_LONG_EDGE;
  } else if (IsJNIEnumValue(env, jduplex_mode,
                            "org/cef/misc/CefPrintSettings$DuplexMode",
                            "DUPLEX_MODE_SHORT_EDGE")) {
    mode = DUPLEX_MODE_SHORT_EDGE;
  } else {
    mode = DUPLEX_MODE_UNKNOWN;
  }
  settings->SetDuplexMode(mode);
}

JNIEXPORT jobject JNICALL
Java_org_cef_misc_CefPrintSettings_1N_N_1GetDuplexMode(JNIEnv* env,
                                                       jobject obj,
                                                       jlong self) {
  jobject result = GetJNIEnumValue(
      env, "org/cef/misc/CefPrintSettings$DuplexMode", "DUPLEX_MODE_UNKNOWN");

  CefRefPtr<CefPrintSettings> settings = GetSelf(self);
  if (!settings)
    return result;

  switch (settings->GetDuplexMode()) {
    JNI_CASE(env, "org/cef/misc/CefPrintSettings$DuplexMode",
             DUPLEX_MODE_SIMPLEX, result);
    JNI_CASE(env, "org/cef/misc/CefPrintSettings$DuplexMode",
             DUPLEX_MODE_LONG_EDGE, result);
    JNI_CASE(env, "org/cef/misc/CefPrintSettings$DuplexMode",
             DUPLEX_MODE_SHORT_EDGE, result);
    default:
      JNI_CASE(env, "org/cef/misc/CefPrintSettings$DuplexMode",
               DUPLEX_MODE_UNKNOWN, result);
  }
  return result;
}
