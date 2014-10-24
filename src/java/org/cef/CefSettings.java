// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef;

/**
 * Initialization settings. Specify NULL or 0 to get the recommended default
 * values. Many of these and other settings can also configured using command-
 * line switches.
 */
public class CefSettings {

  /**
   * Log severity levels.
   */
  public enum LogSeverity {
    /**
     * Default logging (currently INFO logging).
     */
    LOGSEVERITY_DEFAULT,

    /**
     * Verbose logging.
     */
    LOGSEVERITY_VERBOSE,

    /**
     * INFO logging.
     */
    LOGSEVERITY_INFO,

    /**
     * WARNING logging.
    */
    LOGSEVERITY_WARNING,

    /**
     * ERROR logging.
     */
    LOGSEVERITY_ERROR,

    /**
     * Completely disable logging.
     */
    LOGSEVERITY_DISABLE
  }

  /**
   * 32-bit ARGB color value, not premultiplied. The color components are always
   * in a known order. Equivalent to the SkColor type.
   */
  public class ColorType {
    private long color_value = 0;

    private ColorType() {}

    public ColorType(int alpha, int red, int green, int blue) {
      color_value =  (alpha << 24) | (red << 16) | (green << 8) | (blue << 0);
    }

    public long getColor() {
      return color_value;
    }

    @Override
    public ColorType clone() {
      ColorType res = new ColorType();
      res.color_value = this.color_value;
      return res;
    }
  }

  /**
   * The path to a separate executable that will be launched for sub-processes.
   * By default the browser process executable is used. See the comments on
   * CefExecuteProcess() for details. Also configurable using the
   * "browser-subprocess-path" command-line switch.
   */
  public String browser_subprocess_path = null;

  /**
   * Set to true to enable windowless (off-screen) rendering support. Do not
   * enable this value if the application does not use windowless rendering as
   * it may reduce rendering performance on some systems.
   */
  public boolean windowless_rendering_enabled = true;

  /**
   * Set to true to disable configuration of browser process features using
   * standard CEF and Chromium command-line arguments. Configuration can still
   * be specified using CEF data structures or via the
   * CefApp::OnBeforeCommandLineProcessing() method.
   */
  public boolean command_line_args_disabled = false;

  /**
   * The location where cache data will be stored on disk. If empty an in-memory
   * cache will be used for some features and a temporary disk cache for others.
   * HTML5 databases such as localStorage will only persist across sessions if a
   * cache path is specified.
   */
  public String cache_path = null;

  /**
   * To persist session cookies (cookies without an expiry date or validity
   * interval) by default when using the global cookie manager set this value to
   * true. Session cookies are generally intended to be transient and most Web
   * browsers do not persist them. A |cache_path| value must also be specified to
   * enable this feature. Also configurable using the "persist-session-cookies"
   * command-line switch.
   */
  public boolean persist_session_cookies = false;

  /**
   * Value that will be returned as the User-Agent HTTP header. If empty the
   * default User-Agent string will be used. Also configurable using the
   * "user-agent" command-line switch.
   */
  public String user_agent = null;

  /**
   * Value that will be inserted as the product portion of the default
   * User-Agent string. If empty the Chromium product version will be used. If
   * |userAgent| is specified this value will be ignored. Also configurable
   * using the "product-version" command-line switch.
   */
  public String product_version = null;

  /**
   * The locale string that will be passed to Blink. If empty the default
   * locale of "en-US" will be used. This value is ignored on Linux where locale
   * is determined using environment variable parsing with the precedence order:
   * LANGUAGE, LC_ALL, LC_MESSAGES and LANG. Also configurable using the "lang"
   * command-line switch.
   */
  public String locale = null;

  /**
   * The directory and file name to use for the debug log. If empty, the
   * default name of "debug.log" will be used and the file will be written
   * to the application directory. Also configurable using the "log-file"
   * command-line switch.
   */
  public String log_file = null;

  /**
   * The log severity. Only messages of this severity level or higher will be
   * logged. Also configurable using the "log-severity" command-line switch with
   * a value of "verbose", "info", "warning", "error", "error-report" or
   * "disable".
   */
  public LogSeverity log_severity = LogSeverity.LOGSEVERITY_DEFAULT;

  /**
   * Custom flags that will be used when initializing the V8 JavaScript engine.
   * The consequences of using custom flags may not be well tested. Also
   * configurable using the "js-flags" command-line switch.
   */
  public String javascript_flags = null;

  /**
   * The fully qualified path for the resources directory. If this value is
   * empty the cef.pak and/or devtools_resources.pak files must be located in
   * the module directory on Windows/Linux or the app bundle Resources directory
   * on Mac OS X. Also configurable using the "resources-dir-path" command-line
   * switch.
   */
  public String resources_dir_path = null;

  /**
   * The fully qualified path for the locales directory. If this value is empty
   * the locales directory must be located in the module directory. This value
   * is ignored on Mac OS X where pack files are always loaded from the app
   * bundle Resources directory. Also configurable using the "locales-dir-path"
   * command-line switch.
   */
  public String locales_dir_path = null;

  /**
   * Set to true to disable loading of pack files for resources and locales.
   * A resource bundle handler must be provided for the browser and render
   * processes via CefApp::GetResourceBundleHandler() if loading of pack files
   * is disabled. Also configurable using the "disable-pack-loading" command-
   * line switch.
   */
  public boolean pack_loading_disabled = false;

  /**
   * Set to a value between 1024 and 65535 to enable remote debugging on the
   * specified port. For example, if 8080 is specified the remote debugging URL
   * will be http: *localhost:8080. CEF can be remotely debugged from any CEF or
   * Chrome browser window. Also configurable using the "remote-debugging-port"
   * command-line switch.
   */
  public int remote_debugging_port = 0;

  /**
   * The number of stack trace frames to capture for uncaught exceptions.
   * Specify a positive value to enable the CefV8ContextHandler::
   * OnUncaughtException() callback. Specify 0 (default value) and
   * OnUncaughtException() will not be called. Also configurable using the
   * "uncaught-exception-stack-size" command-line switch.
   */
  public int uncaught_exception_stack_size = 0;

  /**
   * By default CEF V8 references will be invalidated (the IsValid() method will
   * return false) after the owning context has been released. This reduces the
   * need for external record keeping and avoids crashes due to the use of V8
   * references after the associated context has been released.
   *
   * CEF currently offers two context safety implementations with different
   * performance characteristics. The default implementation (value of 0) uses a
   * map of hash values and should provide better performance in situations with
   * a small number contexts. The alternate implementation (value of 1) uses a
   * hidden value attached to each context and should provide better performance
   * in situations with a large number of contexts.
   *
   * If you need better performance in the creation of V8 references and you
   * plan to manually track context lifespan you can disable context safety by
   * specifying a value of -1.
   *
   * Also configurable using the "context-safety-implementation" command-line
   * switch.
   */
  public int context_safety_implementation = 0;

  /**
   * Set to true to ignore errors related to invalid SSL certificates.
   * Enabling this setting can lead to potential security vulnerabilities like
   * "man in the middle" attacks. Applications that load content from the
   * internet should not enable this setting. Also configurable using the
   * "ignore-certificate-errors" command-line switch.
   */
  public boolean ignore_certificate_errors = false;

  /**
   * Opaque background color used for accelerated content. By default the
   * background color will be white. Only the RGB compontents of the specified
   * value will be used. The alpha component must greater than 0 to enable use
   * of the background color but will be otherwise ignored.
   */
  public ColorType background_color = null;

  public CefSettings() { }

  @Override
  public CefSettings clone() {
    CefSettings tmp = new CefSettings();
    tmp.browser_subprocess_path = browser_subprocess_path;
    tmp.windowless_rendering_enabled = windowless_rendering_enabled;
    tmp.command_line_args_disabled = command_line_args_disabled;
    tmp.cache_path = cache_path;
    tmp.persist_session_cookies = persist_session_cookies;
    tmp.user_agent = user_agent;
    tmp.product_version = product_version;
    tmp.locale = locale;
    tmp.log_file = log_file;
    tmp.log_severity = log_severity;
    tmp.javascript_flags = javascript_flags;
    tmp.resources_dir_path = resources_dir_path;
    tmp.locales_dir_path = locales_dir_path;
    tmp.pack_loading_disabled = pack_loading_disabled;
    tmp.remote_debugging_port = remote_debugging_port;
    tmp.uncaught_exception_stack_size = uncaught_exception_stack_size;
    tmp.context_safety_implementation = context_safety_implementation;
    tmp.ignore_certificate_errors = ignore_certificate_errors;
    if (background_color != null)
      tmp.background_color = background_color.clone();
    return tmp;
  }
}
