Java Chromium Embedded Framework (JCEF) Binary Distribution for Linux 64-bit
-------------------------------------------------------------------------------

This distribution contains all components necessary to build and distribute a
Java application using JCEF on the Linux platform. Please see the LICENSING
section of this document for licensing terms and conditions.


CONTENTS
--------

bin         Contains Java archives, native library files and the MainFrame
            sample application.

docs        Contains documentation for the org.cef package.


USAGE
-----

1. Install a 64-bit version of the Oracle Java 7 runtime.
2. Execute the run.sh script to run the MainFrame sample application.
3. Optionally recompile the sample application and update jcef-tests.jar
   by running the compile.sh script.

Please visit the JCEF Website for additional usage information.

http://code.google.com/p/javachromiumembedded


REDISTRIBUTION
--------------

This binary distribution contains the below components. Components listed under
the "required" section must be redistributed with all applications using JCEF.
Components listed under the "optional" section may be excluded if the related
features will not be used.

Required components:

* Java archives
    jcef.jar
    gluegen-rt.jar
    gluegen-rt-natives-linux-amd64.jar
    jogl-all.jar
    jogl-all-natives-linux-amd64.jar

* CEF JNI library
    libjcef.so

* CEF JNI process helper
    jcef_helper

* CEF core library
    libcef.so

Optional components:

* Localized resources
    locales/
  Note: Contains localized strings for WebKit UI controls. A .pak file is loaded
  from this folder based on the value of environment variables which are read
  with the following precedence order: LANGUAGE, LC_ALL, LC_MESSAGES and LANG.
  Only configured locales need to be distributed. If no locale is configured the
  default locale of "en-US" will be used. Locale file loading can be disabled
  completely using CefSettings.pack_loading_disabled. The locales folder path
  can be customized using CefSettings.locales_dir_path.

* Other resources
    cef.pak
    devtools_resources.pak
  Note: Contains WebKit image and inspector resources. Pack file loading can be
  disabled completely using CefSettings.pack_loading_disabled. The resources
  directory path can be customized using CefSettings.resources_dir_path.

* FFmpeg audio and video support
    libffmpegsumo.so
  Note: Without this component HTML5 audio and video will not function.


LICENSING
---------

The JCEF project is BSD licensed. Please read the LICENSE.txt files included with
this binary distribution for licensing terms and conditions. Other software
included in this distribution is provided under other licenses. Please visit
"about:credits" in a CEF-based application for complete Chromium and third-party
licensing information.
