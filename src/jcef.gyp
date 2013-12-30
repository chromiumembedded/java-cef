# Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
# reserved. Use of this source code is governed by a BSD-style license that
# can be found in the LICENSE file.

{
  'variables': {
    'chromium_code': 1,
    'cef_directory': '<(DEPTH)/third_party/cef/<(jcef_platform)',
    'cef_directory_win': '<(DEPTH)\\third_party\\cef\\<(jcef_platform)',
    'version_mac_dylib': '1.0.0',
    'conditions': [
      ['OS=="mac"', {
        # Don't use clang with CEF binary releases due to Chromium tree structure dependency.
        'clang': 0,
      }],
    ]
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
        'native/CefBrowser_N.cpp',
        'native/CefBrowser_N.h',
        'native/CefContext.cpp',
        'native/CefContext.h',
        'native/client_handler.cpp',
        'native/client_handler.h',
        'native/jni_util.h',
        'native/jni_util.cpp',
        'native/util.h',
      ],
      'xcode_settings': {
        # Default path that will be changed by install_name_tool in dependent targets.
        'INSTALL_PATH': '@executable_path',
        'DYLIB_INSTALL_NAME_BASE': '@executable_path',
        'LD_DYLIB_INSTALL_NAME': '@executable_path/libjcef.dylib',
        'DYLIB_COMPATIBILITY_VERSION': '<(version_mac_dylib)',
        'DYLIB_CURRENT_VERSION': '<(version_mac_dylib)',
      },
      'conditions': [
        ['OS=="win"', {
          'sources': [
            'native/client_handler_win.cpp',
            'native/jcef_dll.rc',
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
                '<(PRODUCT_DIR)/copy_libraries.stamp',
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
        ['OS=="linux"', {
          'dependencies': [
            '<(cef_directory)/cefclient.gyp:gtk',
          ],
          'sources': [
            'native/client_handler_gtk.cpp',
          ],
          'include_dirs': [
            '<(jdk_directory)/include',
            '<(jdk_directory)/include/linux',
          ],
          'actions': [
            {
              'action_name': 'copy_resources',
              'inputs': [],
              'outputs': [
                '<(PRODUCT_DIR)/copy_resources.stamp',
              ],
              'action': [
                'cp',
                '-rf',
                '<(cef_directory)/Resources/cef.pak',
                '<(cef_directory)/Resources/devtools_resources.pak',
                '<(cef_directory)/Resources/locales',
                '<(PRODUCT_DIR)',
              ],
            },
            {
              'action_name': 'copy_libraries',
              'inputs': [],
              'outputs': [
                '<(PRODUCT_DIR)/copy_libraries.stamp',
              ],
              'action': [
                'cp',
                '-rf',
                '<(cef_directory)/$(BUILDTYPE)/libcef.so',
                '<(cef_directory)/$(BUILDTYPE)/libffmpegsumo.so',
                '<(PRODUCT_DIR)',
              ],
            },
          ],
          'link_settings': {
            'cflags': [
              # Chromium adds "-fvisibility=hidden" by default. Override it here
              # so that JNI symbols are properly exported when building with GCC.
              # Related discussion: http://mail.openjdk.java.net/pipermail/core-libs-dev/2013-February/014446.html
              # Test symbol export with: nm -D --defined-only libjcef.so | grep Java
              '-fvisibility=default',
            ],
            'ldflags': [
              # Look for libcef.so in the current directory. Path can also be
              # specified using the LD_LIBRARY_PATH environment variable.
              '-Wl,-rpath,.',
            ],
            'libraries': [
              "<(cef_directory)/$(BUILDTYPE)/libcef.so",
            ],
          },
        }],
        ['OS=="mac"', {
          'product_name': 'jcef',
          'sources': [
            'native/client_handler_mac.mm',
            'native/util_mac.h',
            'native/util_mac.mm',
          ],
          'include_dirs': [
            '<(jdk_directory)/Headers',
          ],
          'postbuilds': [
            {
              'postbuild_name': 'Fix Framework Link',
              'action': [
                'install_name_tool',
                '-change',
                '@executable_path/libcef.dylib',
                '@loader_path/../Frameworks/Chromium Embedded Framework.framework/Libraries/libcef.dylib',
                '${BUILT_PRODUCTS_DIR}/${EXECUTABLE_PATH}'
              ],
            },
          ],
          'link_settings': {
            'libraries': [
              '$(SDKROOT)/System/Library/Frameworks/AppKit.framework',
              '$(SDKROOT)/System/Library/Frameworks/JavaVM.framework',
              '$(SDKROOT)/System/Library/Frameworks/OpenGL.framework',
              '<(cef_directory)/$(CONFIGURATION)/libcef.dylib',
            ],
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
      'mac_bundle': 1,
      'mac_bundle_resources!': [
        '<(cef_directory)/cefclient/mac/helper-Info.plist',
      ],
      'mac_bundle_resources/': [
        ['exclude', '.*'],
      ],
      'xcode_settings': {
        'INFOPLIST_FILE': '<(cef_directory)/cefclient/mac/helper-Info.plist',
      },
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
        ['OS=="linux"', {
          'dependencies': [
            '<(cef_directory)/cefclient.gyp:gtk',
          ],
          'link_settings': {
            'ldflags': [
              # Look for libcef.so in the current directory. Path can also be
              # specified using the LD_LIBRARY_PATH environment variable.
              '-Wl,-rpath,.',
            ],
            'libraries': [
              '<(cef_directory)/$(BUILDTYPE)/libcef.so',
            ],
          },
        }],
        ['OS=="mac"', {
          'product_name': 'jcef Helper',
          'variables': {
            'enable_wexit_time_destructors': 1,
          },
          'postbuilds': [
            {
              # The framework defines its load-time path
              # (DYLIB_INSTALL_NAME_BASE) relative to the main executable
              # (chrome).  A different relative path needs to be used in
              # jcef_helper_app.
              'postbuild_name': 'Fix Framework Link',
              'action': [
                'install_name_tool',
                '-change',
                '@executable_path/libcef.dylib',
                '@executable_path/../../../../Frameworks/Chromium Embedded Framework.framework/Libraries/libcef.dylib',
                '${BUILT_PRODUCTS_DIR}/${EXECUTABLE_PATH}'
              ],
            },
          ],
          'link_settings': {
            'libraries': [
              '$(SDKROOT)/System/Library/Frameworks/AppKit.framework',
              '<(cef_directory)/$(CONFIGURATION)/libcef.dylib',
            ],
          },
        }],
      ],
    },
  ],
  'conditions': [
    ['OS=="mac"', {
      'targets': [
        {
          'target_name': 'jcef_app',
          'type': 'executable',
          'product_name': 'jcef_app',
          'mac_bundle': 1,
          'dependencies': [
            'jcef',
          ],
          'defines': [
            'USING_CEF_SHARED',
          ],
          'link_settings': {
            'libraries': [
              '$(SDKROOT)/System/Library/Frameworks/AppKit.framework',
              '<(cef_directory)/$(CONFIGURATION)/libcef.dylib',
            ],
          },
          'mac_bundle_resources': [
            'native/resources/jcef-Info.plist',
          ],
          'mac_bundle_resources!': [
            'native/resources/jcef-Info.plist',
          ],
          'xcode_settings': {
            'INFOPLIST_FILE': 'native/resources/jcef-Info.plist',
          },
          'actions': [
            {
              'action_name': 'generate_stub_main',
              'process_outputs_as_sources': 1,
              'inputs': [],
              'outputs': [ '<(INTERMEDIATE_DIR)/dummy_main.c' ],
              'action': [
                'bash', '-c',
                'echo "int main() { return 0; }" > <(INTERMEDIATE_DIR)/dummy_main.c'
              ],
            },
          ],
          'copies': [
            {
              # Add library dependencies to the bundle.
              'destination': '<(PRODUCT_DIR)/jcef_app.app/Contents/Frameworks/Chromium Embedded Framework.framework/Libraries/',
              'files': [
                '<(cef_directory)/Release/libcef.dylib',
                '<(cef_directory)/Release/ffmpegsumo.so',
              ],
            },
            {
              # Add other resources to the bundle.
              'destination': '<(PRODUCT_DIR)/jcef_app.app/Contents/Frameworks/Chromium Embedded Framework.framework/',
              'files': [
                '<(cef_directory)/Resources/',
              ],
            },
            {
              # Add the helper app.
              'destination': '<(PRODUCT_DIR)/jcef_app.app/Contents/Frameworks',
              'files': [
                '<(PRODUCT_DIR)/jcef Helper.app',
                '<(cef_directory)/Release/libplugin_carbon_interpose.dylib',
              ],
            },
            {
              # Add the JCEF library.
              'destination': '<(PRODUCT_DIR)/jcef_app.app/Contents/MacOS',
              'files': [
                '<(PRODUCT_DIR)/libjcef.dylib',
              ],
            },
          ],
          'postbuilds': [
            {
              'postbuild_name': 'Fix Framework Link',
              'action': [
                'install_name_tool',
                '-change',
                '@executable_path/libcef.dylib',
                '@executable_path/../Frameworks/Chromium Embedded Framework.framework/Libraries/libcef.dylib',
                '${BUILT_PRODUCTS_DIR}/${EXECUTABLE_PATH}'
              ],
            },     
            {
              # This postbuid step is responsible for creating the following
              # helpers:
              #
              # jcef Helper EH.app and jcef Helper NP.app are created
              # from jcef Helper.app.
              #
              # The EH helper is marked for an executable heap. The NP helper
              # is marked for no PIE (ASLR).
              'postbuild_name': 'Make More Helpers',
              'action': [
                '<(cef_directory)/tools/make_more_helpers.sh',
                'Frameworks',
                'jcef',
              ],
            },
          ],
        },  # target jcef_app
      ],
    }],  # OS=="mac"
  ],
}
