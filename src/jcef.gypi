# Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
# reserved. Use of this source code is governed by a BSD-style license that
# can be found in the LICENSE file.

{
  'variables': {
    # Don't use the chrome style plugin with CEF.
    'clang_use_chrome_plugins': 0,
    'conditions': [
      [ 'OS=="win"', {
        'build_dir_prefix': '../out/<(jcef_platform)/',
      }],
    ]
  }, 'conditions': [
    ['os_posix==1 and OS!="mac" and OS!="android"', {
      'target_defaults': {
        'cflags_cc': ['-Wno-deprecated-declarations'],
      },
    }]
  ],
}
