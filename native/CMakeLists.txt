# Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
# reserved. Use of this source code is governed by a BSD-style license that
# can be found in the LICENSE file.

#
# Source files.
#

# jcef sources.
set(JCEF_SRCS
  CefApp.cpp
  CefApp.h
  CefAuthCallback_N.cpp
  CefAuthCallback_N.h
  CefBeforeDownloadCallback_N.cpp
  CefBeforeDownloadCallback_N.h
  CefBrowser_N.cpp
  CefBrowser_N.h
  CefCallback_N.cpp
  CefCallback_N.h
  CefClientHandler.cpp
  CefClientHandler.h
  CefCommandLine_N.cpp
  CefCommandLine_N.h
  CefContextMenuParams_N.cpp
  CefContextMenuParams_N.h
  CefCookieManager_N.cpp
  CefCookieManager_N.h
  CefDownloadItemCallback_N.cpp
  CefDownloadItemCallback_N.h
  CefDownloadItem_N.cpp
  CefDownloadItem_N.h
  CefDragData_N.cpp
  CefDragData_N.h
  CefFileDialogCallback_N.cpp
  CefFileDialogCallback_N.h
  CefFrame_N.cpp
  CefFrame_N.h
  CefJSDialogCallback_N.h
  CefJSDialogCallback_N.cpp
  CefMenuModel_N.cpp
  CefMenuModel_N.h
  CefMessageRouter_N.cpp
  CefMessageRouter_N.h
  CefPostData_N.cpp
  CefPostData_N.h
  CefPostDataElement_N.cpp
  CefPostDataElement_N.h
  CefPrintDialogCallback_N.cpp
  CefPrintDialogCallback_N.h
  CefPrintJobCallback_N.cpp
  CefPrintJobCallback_N.h
  CefPrintSettings_N.cpp
  CefPrintSettings_N.h
  CefQueryCallback_N.cpp
  CefQueryCallback_N.h
  CefRequest_N.cpp
  CefRequest_N.h
  CefRequestCallback_N.cpp
  CefRequestCallback_N.h
  CefRequestContext_N.cpp
  CefRequestContext_N.h
  CefResponse_N.cpp
  CefResponse_N.h
  CefSchemeRegistrar_N.cpp
  CefSchemeRegistrar_N.h
  CefURLRequest_N.cpp
  CefURLRequest_N.h
  CefWebPluginInfo_N.cpp
  CefWebPluginInfo_N.h
  CefWebPluginManager_N.cpp
  CefWebPluginManager_N.h
  browser_process_handler.cpp
  browser_process_handler.h
  client_app.cpp
  client_app.h
  client_handler.cpp
  client_handler.h
  completion_callback.cpp
  completion_callback.h
  context.cpp
  context.h
  context_menu_handler.cpp
  context_menu_handler.h
  cookie_visitor.cpp
  cookie_visitor.h
  critical_wait.h
  dialog_handler.cpp
  dialog_handler.h
  display_handler.cpp
  display_handler.h
  download_handler.cpp
  download_handler.h
  drag_handler.cpp
  drag_handler.h
  focus_handler.cpp
  focus_handler.h
  jcef_version.h
  jni_util.h
  jni_util.cpp
  jsdialog_handler.cpp
  jsdialog_handler.h
  keyboard_handler.cpp
  keyboard_handler.h
  life_span_handler.cpp
  life_span_handler.h
  load_handler.cpp
  load_handler.h
  message_router_handler.cpp
  message_router_handler.h
  pdf_print_callback.cpp
  pdf_print_callback.h
  print_handler.cpp
  print_handler.h
  render_handler.cpp
  render_handler.h
  request_context_handler.cpp
  request_context_handler.h
  request_handler.cpp
  request_handler.h
  resource_handler.cpp
  resource_handler.h
  run_file_dialog_callback.cpp
  run_file_dialog_callback.h
  scheme_handler_factory.cpp
  scheme_handler_factory.h
  string_visitor.cpp
  string_visitor.h
  temp_window.h
  url_request_client.cpp
  url_request_client.h
  util.h
  web_plugin_unstable_callback.cpp
  web_plugin_unstable_callback.h
  web_plugin_visitor.cpp
  web_plugin_visitor.h
  window_handler.cpp
  window_handler.h
  write_handler.cpp
  write_handler.h
  )
set(JCEF_SRCS_LINUX
  critical_wait_posix.cpp
  jni_util_linux.cpp
  signal_restore_posix.cpp
  signal_restore_posix.h
  temp_window_x11.cc
  temp_window_x11.h
  util_linux.cpp
  util_posix.cpp
  )
set(JCEF_SRCS_MACOSX
  critical_wait_posix.cpp
  signal_restore_posix.cpp
  signal_restore_posix.h
  temp_window_mac.h
  temp_window_mac.mm
  util_mac.h
  util_mac.mm
  util_posix.cpp
  )
set(JCEF_SRCS_WINDOWS
  critical_wait_win.cpp
  jni_util_win.cpp
  jcef_dll.rc
  temp_window_win.cc
  temp_window_win.h
  util_win.cpp
  )
APPEND_PLATFORM_SOURCES(JCEF_SRCS)
source_group(jcef FILES ${JCEF_SRCS})

# jcef_helper sources.
set(JCEF_HELPER_SRCS
  jcef_helper.cpp
  util.h
  )
set(JCEF_HELPER_SRCS_LINUX
  util_posix.cpp
  )
set(JCEF_HELPER_SRCS_MACOSX
  util_posix.cpp
  )
set(JCEF_HELPER_SRCS_WINDOWS
  jcef_helper.rc
  util_win.cpp
  )
APPEND_PLATFORM_SOURCES(JCEF_HELPER_SRCS)
source_group(jcef FILES ${JCEF_HELPER_SRCS})


#
# Shared configuration.
#

# Target binary names.
set(JCEF_TARGET "jcef")
if(OS_MACOSX)
  set(JCEF_APP_NAME "jcef_app")
  set(JCEF_HELPER_TARGET "jcef_Helper")
  set(JCEF_HELPER_OUTPUT_NAME "jcef Helper")
else()
  set(JCEF_HELPER_TARGET "jcef_helper")
endif()

if(OS_LINUX OR OS_WINDOWS)
  # Logical target used to link the libcef library on Linux and Windows.
  # On macOS the CEF framework is loaded dynamically at startup.
  ADD_LOGICAL_TARGET("libcef_lib" "${CEF_LIB_DEBUG}" "${CEF_LIB_RELEASE}")
endif()

# Determine the target output directory.
SET_CEF_TARGET_OUT_DIR()


#
# Linux configuration.
#

if(OS_LINUX)
  # Find required libraries and update compiler/linker variables.
  FIND_LINUX_LIBRARIES("gmodule-2.0 gtk+-2.0 gthread-2.0")

  # Helper executable target.
  add_executable(${JCEF_HELPER_TARGET} ${JCEF_HELPER_SRCS})
  SET_EXECUTABLE_TARGET_PROPERTIES(${JCEF_HELPER_TARGET})
  add_dependencies(${JCEF_HELPER_TARGET} libcef_dll_wrapper)
  target_link_libraries(${JCEF_HELPER_TARGET} libcef_lib libcef_dll_wrapper ${CEF_STANDARD_LIBS})

  # Set rpath so that libraries can be placed next to the executable.
  set_target_properties(${JCEF_HELPER_TARGET} PROPERTIES INSTALL_RPATH "$ORIGIN")
  set_target_properties(${JCEF_HELPER_TARGET} PROPERTIES BUILD_WITH_INSTALL_RPATH TRUE)

  # JCEF library target.
  add_library(${JCEF_TARGET} SHARED ${JCEF_SRCS})
  SET_LIBRARY_TARGET_PROPERTIES(${JCEF_TARGET})
  add_dependencies(${JCEF_TARGET} libcef_dll_wrapper ${JCEF_HELPER_TARGET})
  target_link_libraries(${JCEF_TARGET} libcef_lib libcef_dll_wrapper ${CEF_STANDARD_LIBS} ${JNI_LIBRARIES})
  target_include_directories(${JCEF_TARGET} PUBLIC ${JNI_INCLUDE_DIRS})

  # Compile flags specific to the JCEF library target.
  # USING_JAVA          = Add the USING_JAVA define.
  target_compile_definitions(${JCEF_TARGET} PRIVATE "USING_JAVA")

  # -fvisibility=default  = Give default visibility to declarations that are not explicitly marked as visible.
  #                         Necessary so that JNI symbols are properly exported when building with GCC.
  #                         Related discussion: http://mail.openjdk.java.net/pipermail/core-libs-dev/2013-February/014446.html
  #                         Test symbol export with: nm -D --defined-only libjcef.so | grep Java
  set_target_properties(${JCEF_TARGET} PROPERTIES
    COMPILE_FLAGS -fvisibility=default
    )

  # Set rpath so that libraries can be placed next to the library.
  set_target_properties(${JCEF_TARGET} PROPERTIES INSTALL_RPATH "$ORIGIN")
  set_target_properties(${JCEF_TARGET} PROPERTIES BUILD_WITH_INSTALL_RPATH TRUE)

  # Copy binary and resource files to the target output directory.
  COPY_FILES("${JCEF_TARGET}" "${CEF_BINARY_FILES}" "${CEF_BINARY_DIR}" "${CEF_TARGET_OUT_DIR}")
  COPY_FILES("${JCEF_TARGET}" "${CEF_RESOURCE_FILES}" "${CEF_RESOURCE_DIR}" "${CEF_TARGET_OUT_DIR}")

  # TODO(jcef): Eliminate the symlink requirement once we figure out how.
  # See https://bitbucket.org/chromiumembedded/java-cef/issues/137#comment-20535941
  add_custom_command(
    TARGET ${JCEF_TARGET}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo ""
    COMMAND ${CMAKE_COMMAND} -E echo "*** Run the following commands manually to create necessary symlinks ***"
    COMMAND ${CMAKE_COMMAND} -E echo "sudo ln -s ${CEF_RESOURCE_DIR}/icudtl.dat ${JAVA_DIR}/jre/bin/icudtl.dat"
    COMMAND ${CMAKE_COMMAND} -E echo "sudo ln -s ${CEF_BINARY_DIR}/natives_blob.bin ${JAVA_DIR}/jre/bin/natives_blob.bin"
    COMMAND ${CMAKE_COMMAND} -E echo "sudo ln -s ${CEF_BINARY_DIR}/snapshot_blob.bin ${JAVA_DIR}/jre/bin/snapshot_blob.bin"
    COMMAND ${CMAKE_COMMAND} -E echo "sudo ln -s ${CEF_BINARY_DIR}/v8_context_snapshot.bin ${JAVA_DIR}/jre/bin/v8_context_snapshot.bin"
    COMMAND ${CMAKE_COMMAND} -E echo ""
    VERBATIM
    )
endif()


#
# Mac OS X configuration.
#

if(OS_MACOSX)
  # Avoid CMP0042 policy errors.
  set(CMAKE_MACOSX_RPATH 1)

  # Avoid CMP0068 policy errors.
  if(POLICY CMP0068)
    cmake_policy(SET CMP0068 NEW)
  endif()

  # Output paths for the app bundles.
  set(JCEF_APP "${CEF_TARGET_OUT_DIR}/${JCEF_APP_NAME}.app")
  set(JCEF_HELPER_APP "${CEF_TARGET_OUT_DIR}/${JCEF_HELPER_OUTPUT_NAME}.app")

  # Variable referenced from Info.plist files.
  set(PRODUCT_NAME "${JCEF_APP_NAME}")

  # Helper executable target.
  add_executable(${JCEF_HELPER_TARGET} MACOSX_BUNDLE ${JCEF_HELPER_SRCS})
  SET_EXECUTABLE_TARGET_PROPERTIES(${JCEF_HELPER_TARGET})
  add_dependencies(${JCEF_HELPER_TARGET} libcef_dll_wrapper)
  target_link_libraries(${JCEF_HELPER_TARGET} libcef_dll_wrapper ${CEF_STANDARD_LIBS})
  set_target_properties(${JCEF_HELPER_TARGET} PROPERTIES
    MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/resources/jcef-Info.plist
    OUTPUT_NAME ${JCEF_HELPER_OUTPUT_NAME}
    )

  if(USE_SANDBOX)
    # Logical target used to link the cef_sandbox library.
    ADD_LOGICAL_TARGET("cef_sandbox_lib" "${CEF_SANDBOX_LIB_DEBUG}" "${CEF_SANDBOX_LIB_RELEASE}")
    target_link_libraries(${JCEF_HELPER_TARGET} cef_sandbox_lib)
  endif()

  # JCEF library target.
  add_library(${JCEF_TARGET} SHARED ${JCEF_RESOURCES_SRCS} ${JCEF_SRCS})
  SET_EXECUTABLE_TARGET_PROPERTIES(${JCEF_TARGET})
  add_dependencies(${JCEF_TARGET} libcef_dll_wrapper "${JCEF_HELPER_TARGET}")
  target_link_libraries(${JCEF_TARGET} libcef_dll_wrapper ${CEF_STANDARD_LIBS}  ${JNI_LIBRARIES})
  target_include_directories(${JCEF_TARGET} PUBLIC ${JNI_INCLUDE_DIRS})

  # Compile flags specific to the JCEF library target.
  # USING_JAVA          = Add the USING_JAVA define.
  target_compile_definitions(${JCEF_TARGET} PRIVATE "USING_JAVA")

  # Name and location of the JCEF library in the main app bundle.
  set(JCEF_TARGET_LIBRARY "lib${JCEF_TARGET}.dylib")
  set(JCEF_TARGET_LIBRARY_APP_PATH "${JCEF_APP}/Contents/Java/${JCEF_TARGET_LIBRARY}")

  add_custom_command(
    TARGET ${JCEF_TARGET}
    POST_BUILD
    # Build the main app bundle.
    COMMAND ant
            -buildfile ${CMAKE_SOURCE_DIR}
            -Djdk7.path=${JAVA_DIR}
            -Dout.path=${CEF_TARGET_OUT_DIR}
            -Dout.name=${JCEF_APP_NAME}
            -Dout.id=org.jcef.jcef
            -Dout.mainclass=tests.detailed.MainFrame
            bundle
    # Copy the helper app bundle into the main app bundle.
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${JCEF_HELPER_APP}"
            "${JCEF_APP}/Contents/Frameworks/${JCEF_HELPER_OUTPUT_NAME}.app"
    # Copy the CEF framework into the main app bundle.
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CEF_BINARY_DIR}/Chromium Embedded Framework.framework"
            "${JCEF_APP}/Contents/Frameworks/Chromium Embedded Framework.framework"
    # Copy the JCEF library into the main app bindle.
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CEF_TARGET_OUT_DIR}/${JCEF_TARGET_LIBRARY}"
            "${JCEF_TARGET_LIBRARY_APP_PATH}"
    VERBATIM
    )
endif()


#
# Windows configuration.
#

if(OS_WINDOWS)
  # Helper executable target.
  add_executable(${JCEF_HELPER_TARGET} WIN32 ${JCEF_HELPER_SRCS})
  SET_EXECUTABLE_TARGET_PROPERTIES(${JCEF_HELPER_TARGET})
  add_dependencies(${JCEF_HELPER_TARGET} libcef_dll_wrapper)
  target_link_libraries(${JCEF_HELPER_TARGET} libcef_lib libcef_dll_wrapper ${CEF_STANDARD_LIBS})

  # JCEF library target.
  add_library(${JCEF_TARGET} SHARED ${JCEF_SRCS})
  SET_LIBRARY_TARGET_PROPERTIES(${JCEF_TARGET})
  add_dependencies(${JCEF_TARGET} libcef_dll_wrapper ${JCEF_HELPER_TARGET})
  target_link_libraries(${JCEF_TARGET} libcef_lib libcef_dll_wrapper ${CEF_STANDARD_LIBS} ${JNI_LIBRARIES})
  target_include_directories(${JCEF_TARGET} PUBLIC ${JNI_INCLUDE_DIRS})

  # Compile flags specific to the JCEF library target.
  # USING_JAVA          = Add the USING_JAVA define.
  target_compile_definitions(${JCEF_TARGET} PRIVATE "USING_JAVA")

  # Add the custom manifest files to the DLL and helper EXE.
  ADD_WINDOWS_MANIFEST("${CMAKE_CURRENT_SOURCE_DIR}" "${JCEF_TARGET}" "dll")
  ADD_WINDOWS_MANIFEST("${CMAKE_CURRENT_SOURCE_DIR}" "${JCEF_HELPER_TARGET}" "exe")

  # Copy binary and resource files to the target output directory.
  COPY_FILES("${JCEF_TARGET}" "${CEF_BINARY_FILES}" "${CEF_BINARY_DIR}" "${CEF_TARGET_OUT_DIR}")
  COPY_FILES("${JCEF_TARGET}" "${CEF_RESOURCE_FILES}" "${CEF_RESOURCE_DIR}" "${CEF_TARGET_OUT_DIR}")
endif()

