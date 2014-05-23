# Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
# reserved. Use of this source code is governed by a BSD-style license that
# can be found in the LICENSE file.

{
  'variables': {
    'chromium_code': 1,
    'cef_directory': '<(DEPTH)/third_party/cef/<(jcef_platform)',
    'cef_directory_win': '<(DEPTH)\\third_party\\cef\\<(jcef_platform)',
    'jcef_launch_name' : 'jcef_app',
    'jcef_launch_id' : 'org.jcef.jcef_app',
    'jcef_launch_class' : 'tests.detailed.MainFrame',
    'jcef_simple_launch_name' : 'jcef_simple_app',
    'jcef_simple_launch_id' : 'org.jcef.jcef_simple_app',
    'jcef_simple_launch_class' : 'tests.simple.MainFrame',
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
        'native/CefAllowCertificateErrorCallback_N.cpp',
        'native/CefAllowCertificateErrorCallback_N.h',
        'native/CefApp.cpp',
        'native/CefApp.h',
        'native/CefAuthCallback_N.cpp',
        'native/CefAuthCallback_N.h',
        'native/CefBeforeDownloadCallback_N.cpp',
        'native/CefBeforeDownloadCallback_N.h',
        'native/CefBrowser_N.cpp',
        'native/CefBrowser_N.h',
        'native/CefClientHandler.cpp',
        'native/CefClientHandler.h',
        'native/CefCommandLine_N.cpp',
        'native/CefCommandLine_N.h',
        'native/CefContextMenuParams_N.cpp',
        'native/CefContextMenuParams_N.h',
        'native/CefDownloadItemCallback_N.cpp',
        'native/CefDownloadItemCallback_N.h',
        'native/CefDownloadItem_N.cpp',
        'native/CefDownloadItem_N.h',
        'native/CefDragData_N.cpp',
        'native/CefDragData_N.h',
        'native/CefFileDialogCallback_N.cpp',
        'native/CefFileDialogCallback_N.h',
        'native/CefGeolocationCallback_N.h',
        'native/CefGeolocationCallback_N.cpp',
        'native/CefJSDialogCallback_N.h',
        'native/CefJSDialogCallback_N.cpp',
        'native/CefMenuModel_N.cpp',
        'native/CefMenuModel_N.h',
        'native/CefPostData_N.cpp',
        'native/CefPostData_N.h',
        'native/CefPostDataElement_N.cpp',
        'native/CefPostDataElement_N.h',
        'native/CefQueryCallback_N.cpp',
        'native/CefQueryCallback_N.h',
        'native/CefQuotaCallback_N.cpp',
        'native/CefQuotaCallback_N.h',
        'native/CefRequest_N.cpp',
        'native/CefRequest_N.h',
        'native/client_handler.cpp',
        'native/client_handler.h',
        'native/context_menu_handler.cpp',
        'native/context_menu_handler.h',
        'native/dialog_handler.cpp',
        'native/dialog_handler.h',
        'native/display_handler.cpp',
        'native/display_handler.h',
        'native/download_handler.cpp',
        'native/download_handler.h',
        'native/drag_handler.cpp',
        'native/drag_handler.h',
        'native/focus_handler.cpp',
        'native/focus_handler.h',
        'native/geolocation_handler.cpp',
        'native/geolocation_handler.h',
        'native/jni_util.h',
        'native/jni_util.cpp',
        'native/jsdialog_handler.cpp',
        'native/jsdialog_handler.h',
        'native/keyboard_handler.cpp',
        'native/keyboard_handler.h',
        'native/life_span_handler.cpp',
        'native/life_span_handler.h',
        'native/load_handler.cpp',
        'native/load_handler.h',
        'native/message_router_handler.cpp',
        'native/message_router_handler.h',
        'native/render_handler.cpp',
        'native/render_handler.h',
        'native/request_handler.cpp',
        'native/request_handler.h',
        'native/run_file_dialog_callback.cpp',
        'native/run_file_dialog_callback.h',
        'native/string_visitor.cpp',
        'native/string_visitor.h',
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
            'native/render_handler_win.cpp',
            'native/jni_util_win.cpp',
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
              '-l\"<(jdk_directory)/lib/jvm.lib\"',
              '-l\"<(jdk_directory)/lib/jawt.lib\"',
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
            'native/render_handler_gtk.cpp',
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
            'native/render_handler_mac.mm',
            'native/jni_util_mac.mm',
            'native/util_mac.h',
            'native/util_mac.mm',
          ],
          'include_dirs': [
            '<(jdk_directory)/include',
            '<(jdk_directory)/include/darwin',
          ],
          'postbuilds': [
            {
              'postbuild_name': 'Fix Framework Link',
              'action': [
                'install_name_tool',
                '-change',
                '@executable_path/Chromium Embedded Framework',
                '@loader_path/../Frameworks/Chromium Embedded Framework.framework/Chromium Embedded Framework',
                '${BUILT_PRODUCTS_DIR}/${EXECUTABLE_PATH}'
              ],
            },
          ],
          'link_settings': {
            'libraries': [
              '<(jdk_directory)/jre/lib/libjawt.dylib',
              '$(SDKROOT)/System/Library/Frameworks/AppKit.framework',
              '<(cef_directory)/$(CONFIGURATION)/Chromium Embedded Framework.framework/Chromium Embedded Framework',
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
                '@executable_path/Chromium Embedded Framework',
                '@executable_path/../../../../Frameworks/Chromium Embedded Framework.framework/Chromium Embedded Framework',
                '${BUILT_PRODUCTS_DIR}/${EXECUTABLE_PATH}'
              ],
            },
          ],
          'link_settings': {
            'libraries': [
              '$(SDKROOT)/System/Library/Frameworks/AppKit.framework',
              '<(cef_directory)/$(CONFIGURATION)/Chromium Embedded Framework.framework/Chromium Embedded Framework',
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
          'type': 'none',
          'dependencies': [
            'jcef',
            'jcef_helper',
          ],
          'xcode_settings': {
            'CONTENTS_FOLDER_PATH' : '<(jcef_launch_name).app/Contents',
          },
          'actions': [
            {
              'action_name': 'Build and bundle Java',
              'inputs': [],
              'outputs': ['<(PRODUCT_DIR)/make_java_code.stamp'],
              'action': [
                'ant',
                '-Djdk7.path=<(jdk_directory)',
                '-Dout.path=<(PRODUCT_DIR)',
                '-Dout.name=<(jcef_launch_name)',
                '-Dout.id=<(jcef_launch_id)',
                '-Dout.mainclass=<(jcef_launch_class)',
                'bundle',
              ],
            },
          ],
          'copies': [
            {
              # Add libraries and helper app.
              'destination': '<(PRODUCT_DIR)/<(jcef_launch_name).app/Contents/Frameworks',
              'files': [
                '<(PRODUCT_DIR)/jcef Helper.app',
                '<(cef_directory)/Release/libplugin_carbon_interpose.dylib',
              ],
            },
            {
              # Add the JCEF library.
              'destination': '<(PRODUCT_DIR)/<(jcef_launch_name).app/Contents/Java',
              'files': [
                '<(PRODUCT_DIR)/libjcef.dylib',
              ],
            },
          ],
          'postbuilds': [   
            {
              'postbuild_name': 'Add framework',
              'action': [
                'cp',
                '-Rf',
                '<(cef_directory)/Release/Chromium Embedded Framework.framework',
                '${BUILT_PRODUCTS_DIR}/<(jcef_launch_name).app/Contents/Frameworks/'
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
        {
          'target_name': 'jcef_simple_app',
          'type': 'none',
          'dependencies': [
            'jcef',
            'jcef_helper',
          ],
          'xcode_settings': {
            'CONTENTS_FOLDER_PATH' : '<(jcef_simple_launch_name).app/Contents',
          },
          'actions': [
            {
              'action_name': 'Build and bundle Java',
              'inputs': [],
              'outputs': ['<(PRODUCT_DIR)/make_java_code.stamp'],
              'action': [
                'ant',
                '-Djdk7.path=<(jdk_directory)',
                '-Dout.path=<(PRODUCT_DIR)',
                '-Dout.name=<(jcef_simple_launch_name)',
                '-Dout.id=<(jcef_simple_launch_id)',
                '-Dout.mainclass=<(jcef_simple_launch_class)',
                'bundle',
              ],
            },
          ],
          'copies': [
            {
              # Add libraries and helper app.
              'destination': '<(PRODUCT_DIR)/<(jcef_simple_launch_name).app/Contents/Frameworks',
              'files': [
                '<(PRODUCT_DIR)/jcef Helper.app',
                '<(cef_directory)/Release/libplugin_carbon_interpose.dylib',
              ],
            },
            {
              # Add the JCEF library.
              'destination': '<(PRODUCT_DIR)/<(jcef_simple_launch_name).app/Contents/Java',
              'files': [
                '<(PRODUCT_DIR)/libjcef.dylib',
              ],
            },
          ],
          'postbuilds': [   
            {
              'postbuild_name': 'Add framework',
              'action': [
                'cp',
                '-Rf',
                '<(cef_directory)/Release/Chromium Embedded Framework.framework',
                '${BUILT_PRODUCTS_DIR}/<(jcef_simple_launch_name).app/Contents/Frameworks/'
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
