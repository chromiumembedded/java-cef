# Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
# reserved. Use of this source code is governed by a BSD-style license that
# can be found in the LICENSE file.

{
  'variables': {
    'chromium_code': 1,
    'cef_directory': '<(DEPTH)/third_party/cef/<(jcef_platform)',
    'cef_directory_win': '<(DEPTH)\\third_party\\cef\\<(jcef_platform)',
  },
  'targets': [
    {
      'target_name': 'jcef',
      'type': 'shared_library',
      'msvs_guid': '6CA3406B-DAE0-4D3B-AEDC-F5A113702C07',
      'defines': [
        'USING_CEF_SHARED',
      ],
      'include_dirs': [
        '.',
        '<(cef_directory)',
      ],
      'dependencies': [
        '<(cef_directory)/cefclient.gyp:libcef_dll_wrapper',
        'jcef_helper',
      ],
      'sources': [
        'native/client_handler.cpp',
        'native/client_handler.h',
      ],
      'conditions': [
        ['OS=="win"', {
          'sources': [
            'native/CefBrowser_N.cpp',
            'native/CefBrowser_N.h',
            'native/CefContext.cpp',
            'native/CefContext.h',
            'native/client_handler_win.cpp',
            'native/jcef_dll.rc',
            'native/jni_util.h',
            'native/jni_util.cpp',
            'native/util.h',
          ],
          'include_dirs': [
            '<(jdk_directory)\include',
            '<(jdk_directory)\include\win32',
          ],
          'actions': [
            {
              'action_name': 'copy_resources',
              'msvs_cygwin_shell': 0,
              'inputs': [],
              'outputs': [
                '<(PRODUCT_DIR)/copy_resources.stamp',
              ],
              'action': [
                'xcopy /efy',
                '<(cef_directory_win)\Resources\*',
                '$(OutDir)',
              ],
            },
            {
              'action_name': 'copy_libraries',
              'msvs_cygwin_shell': 0,
              'inputs': [],
              'outputs': [
                '<(PRODUCT_DIR)/copy_resources.stamp',
              ],
              'action': [
                'xcopy /efy',
                '<(cef_directory_win)\$(ConfigurationName)\*.dll',
                '$(OutDir)',
              ],
            },
          ],
          'link_settings': {
            'libraries': [
              '-lcomctl32.lib',
              '-l<(cef_directory)/$(ConfigurationName)/libcef.lib',
              '-l<(jdk_directory)/lib/jvm.lib',
            ],
          },
          'msvs_settings': {
            'VCLinkerTool': {
              # Generate a PDB symbol file for both Debug and Release builds.
              'GenerateDebugInformation': 'true',
            },
            'VCManifestTool': {
              'AdditionalManifestFiles': [
                'native/jcef.dll.manifest',
              ],
            },
          },
        }],
      ],
    },
    {
      'target_name': 'jcef_helper',
      'type': 'executable',
      'msvs_guid': '739F13CA-9706-4075-B64F-2B875F075570',
      'defines': [
        'USING_CEF_SHARED',
      ],
      'include_dirs': [
        '.',
        '<(cef_directory)',
      ],
      'dependencies': [
        '<(cef_directory)/cefclient.gyp:libcef_dll_wrapper',
      ],
      'sources': [
        'native/jcef_helper.cpp',
      ],
      'conditions': [
        ['OS=="win"', {
          'sources': [
            'native/jcef_helper.rc',
          ],
          'variables': {
            'win_exe_compatibility_manifest': '<(cef_directory)/cefclient/compatibility.manifest',
          },
          'link_settings': {
            'libraries': [
              '-lcomctl32.lib',
              '-l<(cef_directory)/$(ConfigurationName)/libcef.lib'
            ],
          },
          'msvs_settings': {
            'VCLinkerTool': {
              # Set /SUBSYSTEM:WINDOWS.
              'SubSystem': '2',
            },
            'VCManifestTool': {
              'AdditionalManifestFiles': [
                '<(cef_directory)/cefclient/cefclient.exe.manifest',
              ],
            },
          },
        }],
      ],
    },
  ],
}
