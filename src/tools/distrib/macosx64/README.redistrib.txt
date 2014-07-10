REDISTRIBUTION
--------------

This binary distribution contains the below components. Components listed under
the "required" section must be redistributed with all applications using JCEF.
Components listed under the "optional" section may be excluded if the related
features will not be used.

The native components of JCEF are distributed as an app bundle (jcef_app) with
the following structure:

jcef_app.app/
  Contents/
    Frameworks/
      Chromium Embedded Framework.framework/
        Libraries/
          ffmpegsumo.so <= HTML5 audio/video support library
          libcef.dylib <= CEF core library
        Resources/
          cef.pak, devtools_resources.pak <= non-localized resources and strings
          en.lproj/, ... <= locale-specific resources and strings
      libplugin_carbon_interpose.dylib <= plugin support library
      jcef Helper.app/
        Contents/
          Info.plist
          MacOS/
            jcef Helper <= helper executable
          Pkginfo
      jcef Helper EH.app/
        Contents/
          Info.plist
          MacOS/
            jcef Helper EH <= helper executable
          Pkginfo
      jcef Helper NP.app/
        Contents/
          Info.plist
          MacOS/
            jcef Helper NP <= helper executable
          Pkginfo
    Info.plist
    Java/
      *.jar <= Required JAR files
      libjcef.dylib <= CEF JNI library
    MacOS/
      JavaAppLauncher <= Java bootstrap executable
    Pkginfo
    Resources/
      en.lproj/Localizable.strings
      GenericApp.icns

The "Chromium Embedded Framework.framework" is an unversioned framework that
contains CEF binaries and resources. The "jcef Helper" apps are used for
executing separate processes (renderer, plugin, etc) with different
characteristics.

Required components:

* Java archives
    jcef.jar
    gluegen-rt.jar
    gluegen-rt-natives-macosx-universal.jar
    jogl-all.jar
    jogl-all-natives-macosx-universal.jar

* CEF core library
    Chromium Embedded Framework.framework/Libraries/libcef.dylib

* Plugin support library
    libplugin_carbon_interpose.dylib

* Cursor resources
    Chromium Embedded Framework.framework/Resources/*.png
    Chromium Embedded Framework.framework/Resources/*.tiff

* CEF JNI library
    libjcef.dylib

Optional components:

* Localized resources
    Chromium Embedded Framework.framework/Resources/*.lproj/
  Note: Contains localized strings for WebKit UI controls. A .pak file is loaded
  from this folder based on the CefSettings.locale value. Only configured
  locales need to be distributed. If no locale is configured the default locale
  of "en" will be used. Locale file loading can be disabled completely using
  CefSettings.pack_loading_disabled.

* Other resources
    Chromium Embedded Framework.framework/Resources/cef.pak
    Chromium Embedded Framework.framework/Resources/devtools_resources.pak
  Note: Contains WebKit image and inspector resources. Pack file loading can be
  disabled completely using CefSettings.pack_loading_disabled. The resources
  directory path can be customized using CefSettings.resources_dir_path.

* FFmpeg audio and video support
    Chromium Embedded Framework.framework/Libraries/ffmpegsumo.so
  Note: Without this component HTML5 audio and video will not function.
