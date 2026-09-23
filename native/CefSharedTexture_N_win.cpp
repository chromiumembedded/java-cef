// Copyright (c) 2026 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "CefSharedTexture_N.h"

#include <windows.h>

#include <GL/gl.h>
#include <stdint.h>

namespace {

typedef uint64_t GLuint64;

constexpr GLenum GL_HANDLE_TYPE_D3D11_IMAGE_EXT = 0x958B;

typedef void(APIENTRY* CreateMemoryObjects)(GLsizei, GLuint*);
typedef void(APIENTRY* DeleteMemoryObjects)(GLsizei, const GLuint*);
typedef void(APIENTRY* ImportMemoryWin32Handle)(GLuint,
                                                GLuint64,
                                                GLenum,
                                                void*);
typedef void(APIENTRY* TexStorageMem2D)(GLenum,
                                        GLsizei,
                                        GLenum,
                                        GLsizei,
                                        GLsizei,
                                        GLuint,
                                        GLuint64);

struct Api {
  HGLRC context = nullptr;
  CreateMemoryObjects create_memory_objects = nullptr;
  DeleteMemoryObjects delete_memory_objects = nullptr;
  ImportMemoryWin32Handle import_memory_win32_handle = nullptr;
  TexStorageMem2D tex_storage_mem_2d = nullptr;
};

// wglGetProcAddress results are only valid for the pixel format of the context
// they were resolved on.
const Api* LoadApi() {
  static Api api;
  HGLRC context = wglGetCurrentContext();
  if (!context)
    return nullptr;
  if (api.context != context) {
    api.context = context;
    api.create_memory_objects = reinterpret_cast<CreateMemoryObjects>(
        wglGetProcAddress("glCreateMemoryObjectsEXT"));
    api.delete_memory_objects = reinterpret_cast<DeleteMemoryObjects>(
        wglGetProcAddress("glDeleteMemoryObjectsEXT"));
    api.import_memory_win32_handle = reinterpret_cast<ImportMemoryWin32Handle>(
        wglGetProcAddress("glImportMemoryWin32HandleEXT"));
    api.tex_storage_mem_2d = reinterpret_cast<TexStorageMem2D>(
        wglGetProcAddress("glTexStorageMem2DEXT"));
  }
  if (!api.create_memory_objects || !api.delete_memory_objects ||
      !api.import_memory_win32_handle || !api.tex_storage_mem_2d) {
    return nullptr;
  }
  return &api;
}

}  // namespace

JNIEXPORT jint JNICALL
Java_org_cef_browser_CefSharedTexture_1N_N_1BindD3D11Texture(JNIEnv*,
                                                             jclass,
                                                             jlong handle,
                                                             jint width,
                                                             jint height) {
  const Api* api = LoadApi();
  if (!api)
    return -1;

  glGetError();

  GLuint memory = 0;
  api->create_memory_objects(1, &memory);
  // Importing a Win32 handle does not transfer ownership, and the size is
  // ignored for D3D11 images.
  api->import_memory_win32_handle(memory, 0, GL_HANDLE_TYPE_D3D11_IMAGE_EXT,
                                  reinterpret_cast<void*>(handle));
  GLenum error = glGetError();
  if (error == GL_NO_ERROR) {
    api->tex_storage_mem_2d(GL_TEXTURE_2D, 1, GL_RGBA8, width, height, memory,
                            0);
    error = glGetError();
  }
  // The texture keeps the memory alive.
  api->delete_memory_objects(1, &memory);
  return static_cast<jint>(error);
}
