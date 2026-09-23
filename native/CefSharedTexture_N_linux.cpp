// Copyright (c) 2026 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "CefSharedTexture_N.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <dlfcn.h>
#include <unistd.h>
#include <xcb/xcb.h>

#include "include/base/cef_logging.h"

// Mesa's GLX exposes GL_OES_EGL_image but cannot resolve EGLImages created on
// a separate EGL display, and EXT_memory_object_fd only works for dmabufs when
// the tiling guess matches the modifier. DRI3 pixmaps carry the modifier, so
// the X server imports the buffer and GLX_EXT_texture_from_pixmap binds it.

namespace {

typedef struct __GLXFBConfigRec* GLXFBConfig;
typedef XID GLXPixmap;

constexpr int GLX_DOUBLEBUFFER = 5;
constexpr int GLX_ALPHA_SIZE = 11;
constexpr int GLX_DRAWABLE_TYPE = 0x8010;
constexpr int GLX_PIXMAP_BIT = 0x2;
constexpr int GLX_BIND_TO_TEXTURE_RGBA_EXT = 0x20D1;
constexpr int GLX_BIND_TO_TEXTURE_TARGETS_EXT = 0x20D3;
constexpr int GLX_TEXTURE_FORMAT_EXT = 0x20D5;
constexpr int GLX_TEXTURE_TARGET_EXT = 0x20D6;
constexpr int GLX_TEXTURE_FORMAT_RGBA_EXT = 0x20DA;
constexpr int GLX_TEXTURE_2D_EXT = 0x20DC;
constexpr int GLX_TEXTURE_2D_BIT_EXT = 0x2;
constexpr int GLX_FRONT_LEFT_EXT = 0x20DE;

constexpr int kMaxPlanes = 4;

struct xcb_dri3_query_version_cookie_t {
  unsigned int sequence;
};

struct xcb_dri3_query_version_reply_t {
  uint8_t response_type;
  uint8_t pad0;
  uint16_t sequence;
  uint32_t length;
  uint32_t major_version;
  uint32_t minor_version;
};

struct Api {
  Display* (*glXGetCurrentDisplay)();
  GLXFBConfig* (*glXChooseFBConfig)(Display*, int, const int*, int*);
  XVisualInfo* (*glXGetVisualFromFBConfig)(Display*, GLXFBConfig);
  GLXPixmap (*glXCreatePixmap)(Display*, GLXFBConfig, Pixmap, const int*);
  void (*glXDestroyPixmap)(Display*, GLXPixmap);
  void (*glXBindTexImageEXT)(Display*, GLXPixmap, int, const int*);
  void (*glXReleaseTexImageEXT)(Display*, GLXPixmap, int);

  xcb_connection_t* (*xcb_connect)(const char*, int*);
  int (*xcb_connection_has_error)(xcb_connection_t*);
  const xcb_setup_t* (*xcb_get_setup)(xcb_connection_t*);
  xcb_screen_iterator_t (*xcb_setup_roots_iterator)(const xcb_setup_t*);
  void (*xcb_screen_next)(xcb_screen_iterator_t*);
  uint32_t (*xcb_generate_id)(xcb_connection_t*);
  xcb_generic_error_t* (*xcb_request_check)(xcb_connection_t*,
                                            xcb_void_cookie_t);
  xcb_void_cookie_t (*xcb_free_pixmap)(xcb_connection_t*, xcb_pixmap_t);
  int (*xcb_flush)(xcb_connection_t*);
  void (*xcb_disconnect)(xcb_connection_t*);

  xcb_dri3_query_version_cookie_t (*xcb_dri3_query_version)(xcb_connection_t*,
                                                            uint32_t,
                                                            uint32_t);
  xcb_dri3_query_version_reply_t* (*xcb_dri3_query_version_reply)(
      xcb_connection_t*,
      xcb_dri3_query_version_cookie_t,
      xcb_generic_error_t**);
  xcb_void_cookie_t (*xcb_dri3_pixmap_from_buffers_checked)(xcb_connection_t*,
                                                            xcb_pixmap_t,
                                                            xcb_window_t,
                                                            uint8_t,
                                                            uint16_t,
                                                            uint16_t,
                                                            uint32_t,
                                                            uint32_t,
                                                            uint32_t,
                                                            uint32_t,
                                                            uint32_t,
                                                            uint32_t,
                                                            uint32_t,
                                                            uint32_t,
                                                            uint8_t,
                                                            uint8_t,
                                                            uint64_t,
                                                            const int32_t*);
};

// Per GLX display: our own xcb connection, so DRI3 requests never race JOGL's
// use of its Xlib display.
struct Connection {
  Display* display = nullptr;
  xcb_connection_t* xcb = nullptr;
  xcb_window_t root = 0;
  GLXFBConfig config = nullptr;
};

struct Binding {
  Display* display;
  xcb_connection_t* xcb;
  xcb_pixmap_t pixmap;
  GLXPixmap glx_pixmap;
};

template <typename T>
bool Resolve(void* lib, const char* name, T* out) {
  *out = reinterpret_cast<T>(dlsym(lib, name));
  if (!*out)
    LOG(ERROR) << "Accelerated paint: missing symbol " << name;
  return *out != nullptr;
}

const Api* LoadApi() {
  static Api api;
  static bool loaded = [] {
    void* gl = dlopen("libGL.so.1", RTLD_NOW | RTLD_GLOBAL);
    void* xcb = dlopen("libxcb.so.1", RTLD_NOW | RTLD_GLOBAL);
    void* dri3 = dlopen("libxcb-dri3.so.0", RTLD_NOW | RTLD_GLOBAL);
    if (!gl || !xcb || !dri3) {
      LOG(ERROR) << "Accelerated paint needs libGL.so.1, libxcb.so.1 and "
                    "libxcb-dri3.so.0";
      return false;
    }

    typedef void* (*GetProcAddress)(const unsigned char*);
    GetProcAddress get_proc = nullptr;
    if (!Resolve(gl, "glXGetProcAddressARB", &get_proc))
      return false;
    api.glXBindTexImageEXT = reinterpret_cast<decltype(api.glXBindTexImageEXT)>(
        get_proc(reinterpret_cast<const unsigned char*>("glXBindTexImageEXT")));
    api.glXReleaseTexImageEXT =
        reinterpret_cast<decltype(api.glXReleaseTexImageEXT)>(get_proc(
            reinterpret_cast<const unsigned char*>("glXReleaseTexImageEXT")));
    if (!api.glXBindTexImageEXT || !api.glXReleaseTexImageEXT) {
      LOG(ERROR) << "Accelerated paint needs GLX_EXT_texture_from_pixmap";
      return false;
    }

    return Resolve(gl, "glXGetCurrentDisplay", &api.glXGetCurrentDisplay) &&
           Resolve(gl, "glXChooseFBConfig", &api.glXChooseFBConfig) &&
           Resolve(gl, "glXGetVisualFromFBConfig",
                   &api.glXGetVisualFromFBConfig) &&
           Resolve(gl, "glXCreatePixmap", &api.glXCreatePixmap) &&
           Resolve(gl, "glXDestroyPixmap", &api.glXDestroyPixmap) &&
           Resolve(xcb, "xcb_connect", &api.xcb_connect) &&
           Resolve(xcb, "xcb_connection_has_error",
                   &api.xcb_connection_has_error) &&
           Resolve(xcb, "xcb_get_setup", &api.xcb_get_setup) &&
           Resolve(xcb, "xcb_setup_roots_iterator",
                   &api.xcb_setup_roots_iterator) &&
           Resolve(xcb, "xcb_screen_next", &api.xcb_screen_next) &&
           Resolve(xcb, "xcb_generate_id", &api.xcb_generate_id) &&
           Resolve(xcb, "xcb_request_check", &api.xcb_request_check) &&
           Resolve(xcb, "xcb_free_pixmap", &api.xcb_free_pixmap) &&
           Resolve(xcb, "xcb_flush", &api.xcb_flush) &&
           Resolve(xcb, "xcb_disconnect", &api.xcb_disconnect) &&
           Resolve(dri3, "xcb_dri3_query_version",
                   &api.xcb_dri3_query_version) &&
           Resolve(dri3, "xcb_dri3_query_version_reply",
                   &api.xcb_dri3_query_version_reply) &&
           Resolve(dri3, "xcb_dri3_pixmap_from_buffers_checked",
                   &api.xcb_dri3_pixmap_from_buffers_checked);
  }();
  return loaded ? &api : nullptr;
}

GLXFBConfig ChooseConfig(const Api* api, Display* display) {
  const int attribs[] = {GLX_DRAWABLE_TYPE,
                         GLX_PIXMAP_BIT,
                         GLX_BIND_TO_TEXTURE_RGBA_EXT,
                         True,
                         GLX_BIND_TO_TEXTURE_TARGETS_EXT,
                         GLX_TEXTURE_2D_BIT_EXT,
                         GLX_DOUBLEBUFFER,
                         False,
                         GLX_ALPHA_SIZE,
                         8,
                         None};
  int count = 0;
  GLXFBConfig* configs =
      api->glXChooseFBConfig(display, DefaultScreen(display), attribs, &count);
  GLXFBConfig result = nullptr;
  for (int i = 0; i < count && !result; ++i) {
    XVisualInfo* visual = api->glXGetVisualFromFBConfig(display, configs[i]);
    if (visual && visual->depth == 32)
      result = configs[i];
    if (visual)
      XFree(visual);
  }
  if (configs)
    XFree(configs);
  return result;
}

// Only called from the CEF UI thread.
Connection* GetConnection(const Api* api, Display* display) {
  static Connection connection;
  if (connection.display == display)
    return connection.xcb ? &connection : nullptr;

  if (connection.xcb)
    api->xcb_disconnect(connection.xcb);
  connection = Connection();
  connection.display = display;

  int screen_number = 0;
  xcb_connection_t* xcb =
      api->xcb_connect(DisplayString(display), &screen_number);
  if (api->xcb_connection_has_error(xcb)) {
    LOG(ERROR) << "Accelerated paint: cannot connect to "
               << DisplayString(display);
    api->xcb_disconnect(xcb);
    return nullptr;
  }

  xcb_dri3_query_version_reply_t* version = api->xcb_dri3_query_version_reply(
      xcb, api->xcb_dri3_query_version(xcb, 1, 2), nullptr);
  bool has_modifiers =
      version && (version->major_version > 1 || version->minor_version >= 2);
  free(version);
  if (!has_modifiers) {
    LOG(ERROR) << "Accelerated paint needs DRI3 1.2 on the X server";
    api->xcb_disconnect(xcb);
    return nullptr;
  }

  xcb_screen_iterator_t it =
      api->xcb_setup_roots_iterator(api->xcb_get_setup(xcb));
  for (int i = 0; i < screen_number; ++i)
    api->xcb_screen_next(&it);

  GLXFBConfig config = ChooseConfig(api, display);
  if (!config) {
    LOG(ERROR) << "Accelerated paint: no 32-bit GLX config can bind pixmaps";
    api->xcb_disconnect(xcb);
    return nullptr;
  }

  connection.xcb = xcb;
  connection.root = it.data->root;
  connection.config = config;
  return &connection;
}

}  // namespace

JNIEXPORT jlong JNICALL
Java_org_cef_browser_CefSharedTexture_1N_N_1BindDmaBuf(JNIEnv* env,
                                                       jclass,
                                                       jint width,
                                                       jint height,
                                                       jint plane_count,
                                                       jintArray jfds,
                                                       jintArray jstrides,
                                                       jlongArray joffsets,
                                                       jlong modifier) {
  if (plane_count <= 0 || plane_count > kMaxPlanes || width <= 0 ||
      height <= 0 || width > UINT16_MAX || height > UINT16_MAX) {
    return 0;
  }
  if (env->GetArrayLength(jfds) < plane_count ||
      env->GetArrayLength(jstrides) < plane_count ||
      env->GetArrayLength(joffsets) < plane_count) {
    return 0;
  }

  const Api* api = LoadApi();
  if (!api)
    return 0;

  Display* display = api->glXGetCurrentDisplay();
  if (!display)
    return 0;

  Connection* connection = GetConnection(api, display);
  if (!connection)
    return 0;

  jint fds[kMaxPlanes];
  jint strides[kMaxPlanes] = {};
  jlong offsets[kMaxPlanes] = {};
  env->GetIntArrayRegion(jfds, 0, plane_count, fds);
  env->GetIntArrayRegion(jstrides, 0, plane_count, strides);
  env->GetLongArrayRegion(joffsets, 0, plane_count, offsets);

  // xcb closes the fds it sends; CEF keeps ownership of the originals.
  int32_t sent_fds[kMaxPlanes];
  for (int i = 0; i < plane_count; ++i) {
    sent_fds[i] = dup(fds[i]);
    if (sent_fds[i] < 0) {
      for (int j = 0; j < i; ++j)
        close(sent_fds[j]);
      return 0;
    }
  }

  xcb_connection_t* xcb = connection->xcb;
  xcb_pixmap_t pixmap = api->xcb_generate_id(xcb);
  xcb_generic_error_t* error = api->xcb_request_check(
      xcb, api->xcb_dri3_pixmap_from_buffers_checked(
               xcb, pixmap, connection->root, static_cast<uint8_t>(plane_count),
               static_cast<uint16_t>(width), static_cast<uint16_t>(height),
               strides[0], offsets[0], strides[1], offsets[1], strides[2],
               offsets[2], strides[3], offsets[3], 32, 32,
               static_cast<uint64_t>(modifier), sent_fds));
  if (error) {
    LOG(ERROR) << "Accelerated paint: DRI3 PixmapFromBuffers failed with X "
                  "error "
               << static_cast<int>(error->error_code);
    free(error);
    return 0;
  }

  const int pixmap_attribs[] = {GLX_TEXTURE_TARGET_EXT, GLX_TEXTURE_2D_EXT,
                                GLX_TEXTURE_FORMAT_EXT,
                                GLX_TEXTURE_FORMAT_RGBA_EXT, None};
  GLXPixmap glx_pixmap =
      api->glXCreatePixmap(display, connection->config, pixmap, pixmap_attribs);
  if (!glx_pixmap) {
    api->xcb_free_pixmap(xcb, pixmap);
    api->xcb_flush(xcb);
    return 0;
  }

  api->glXBindTexImageEXT(display, glx_pixmap, GLX_FRONT_LEFT_EXT, nullptr);

  return reinterpret_cast<jlong>(new Binding{display, xcb, pixmap, glx_pixmap});
}

JNIEXPORT void JNICALL
Java_org_cef_browser_CefSharedTexture_1N_N_1ReleaseDmaBuf(JNIEnv*,
                                                          jclass,
                                                          jlong token) {
  const Api* api = LoadApi();
  Binding* binding = reinterpret_cast<Binding*>(token);
  if (!api || !binding)
    return;

  api->glXReleaseTexImageEXT(binding->display, binding->glx_pixmap,
                             GLX_FRONT_LEFT_EXT);
  api->glXDestroyPixmap(binding->display, binding->glx_pixmap);
  api->xcb_free_pixmap(binding->xcb, binding->pixmap);
  api->xcb_flush(binding->xcb);
  delete binding;
}
