# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project overview

JCEF is a Java wrapper around the C++ Chromium Embedded Framework (CEF). It is a hybrid Java + native (C++/Obj-C) project bound together by JNI. JCEF tracks a specific CEF release branch — the active version is set by `CEF_VERSION` in `CMakeLists.txt`, and the matching CEF binary distribution is downloaded by `cmake/DownloadCEF.cmake` into `third_party/cef/` at configure time.

## Build

The build is driven by CMake and **must** run inside a directory literally named `jcef_build/` at the repo root — `tools/run.sh`, `tools/make_distrib.sh`, etc. hard-code that path.

```sh
mkdir jcef_build && cd jcef_build

# macOS (arm64 host):
cmake -G "Ninja" -DPROJECT_ARCH="arm64" -DCMAKE_BUILD_TYPE=Release ..
ninja
# Or generate Xcode: cmake -G "Xcode" -DPROJECT_ARCH="arm64" ..

# Linux:
cmake -G "Ninja" -DCMAKE_BUILD_TYPE=Release ..
ninja

# Windows (x64):
cmake -G "Visual Studio 17" -A x64 ..
# build via VS or ninja with vcvars64.bat
```

Configure-time side effects to be aware of:
- Downloads + extracts the CEF binary distribution.
- Generates `native/jcef_version.h` from CEF headers via `tools/make_version_header.py`.
- Downloads `clang-format` from Google Storage into `tools/buildtools/<platform>/`.

Java compilation differs by platform:
- **macOS:** the CMake build invokes Ant (`build.xml`) as a post-build step. It produces `jcef.jar` + `jcef-tests.jar`, bundles everything into `jcef_app.app` via `third_party/appbundler`, and copies the CEF framework + helper apps into the bundle. Requires `ant` (`brew install ant`).
- **Linux/Windows:** Java is compiled separately with `tools/compile.{sh,bat} <platform>` (e.g. `linux64`, `win64`); the CMake build only handles native code.

## Run / test / package

```sh
# Launch the test app (Linux/Windows). On macOS run jcef_build/native/Release/jcef_app.app.
tools/run.sh    <platform> <Debug|Release> <simple|detailed> [args...]

# JUnit tests under java/tests/junittests via junit-platform-console-standalone.
tools/run_tests.sh <platform> <Debug|Release> [extra junit args...]

# Build a redistributable in binary_distrib/<platform>/.
tools/make_distrib.sh <platform>
```

`<platform>` is `linux32`, `linux64`, `macosx64`, `win32`, or `win64`.

## Code style

Chromium coding style for both C++ and Java. Run `tools/fix_style.sh [path...]` (which calls `tools/fix_style.py`) to apply clang-format. clang-format is downloaded by CMake into `tools/buildtools/`, so a configure step must have run first.

## Architecture

The project is organized as **Java API ↔ JNI bridge ↔ CEF C++**.

### Java side (`java/org/cef/`)
- `CefApp` — global singleton; owns CEF process lifecycle and state machine (`CefAppState`).
- `CefClient` — per-browser client object that fans handler callbacks out to user-registered `Cef*Handler` interfaces.
- `browser/` — `CefBrowser` interface plus two implementations: `CefBrowserOsr` (off-screen rendering, OpenGL via JOGL) and `CefBrowserWr` (windowed rendering); `CefBrowserFactory` picks one. macOS uses the extra `browser/mac/CefBrowserWindowMac.java`.
- `handler/` — observer-style callback interfaces (`CefLifeSpanHandler`, `CefLoadHandler`, `CefRequestHandler`, …) each paired with an `*Adapter` no-op base class.
- `callback/`, `misc/`, `network/` — value/callback types mirrored to native.

### Native side (`native/`)
- Files ending in `_N.cpp/.h` (e.g. `CefBrowser_N.cpp`) implement the JNI methods of the matching Java class with the `_N` suffix (e.g. `org.cef.browser.CefBrowser_N`). Headers are machine-generated via `javah`/`javac -h` — regenerate with `tools/make_all_jni_headers.{sh,bat} <platform>` (or `make_jni_header.{sh,bat} <platform> <fully.qualified.Class>` for one class). When you add a new `_N` Java class, also add it to `make_all_jni_headers.*`.
- Non-`_N.cpp` files (`client_handler.cpp`, `*_handler.cpp`, `client_app.cpp`, `context.cpp`, …) implement the CEF C++ side: they receive callbacks from CEF and dispatch them up into Java via the helpers in `jni_util.{h,cpp}` and `jni_scoped_helpers.{h,cpp}`.
- Platform-specific code is split into `*_linux.cpp`, `*_mac.mm`, `*_win.cpp`, `*_posix.cpp`, plus `temp_window_{x11,mac,win}` and `critical_wait_{posix,win}`. CMake's `APPEND_PLATFORM_SOURCES` selects the right set per OS.
- `jcef_helper.cpp` builds a separate `jcef_helper`/`jcef Helper` executable — Chromium's multi-process model requires a helper binary for renderer/GPU/etc. processes.

### Build outputs
- `jcef_build/native/<config>/` — `libjcef.{so,dylib}` / `jcef.dll`, the helper executable(s), and (on macOS) `jcef_app.app` containing the framework + Helper bundles.
- `out/` — Ant-produced Java class files / jars on macOS; mirrored layout used by `tools/compile.sh` on other platforms.

## Other notes

- macOS app launches require these JVM flags (already wired into `build.xml` and `run.sh`): `--add-opens=java.desktop/sun.{awt,lwawt,lwawt.macosx}=ALL-UNNAMED`, `--add-opens=java.desktop/java.awt=ALL-UNNAMED`, `--enable-native-access=ALL-UNNAMED`. Anything that calls JCEF off-bundle needs the same flags.
