// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef;

public class OS {
  private static enum OSType {
    OSUndefined,
    OSLinux,
    OSWindows,
    OSMacintosh,
    OSUnknown,
  };
  private static OSType osType = OSType.OSUndefined;

  public static final boolean isWindows() {
    return getOSType() == OSType.OSWindows;
  }

  public static final boolean isMacintosh() {
    return getOSType() == OSType.OSMacintosh;
  }

  public static final boolean isLinux() {
    return getOSType() == OSType.OSLinux;
  }

  private static final OSType getOSType() {
    if(osType == OSType.OSUndefined) {
      String os = System.getProperty("os.name").toLowerCase();
      if (os.startsWith("windows"))
        osType = OSType.OSWindows;
      else if (os.startsWith("linux"))
        osType = OSType.OSLinux;
      else if (os.startsWith("mac"))
        osType = OSType.OSMacintosh;
      else
        osType = OSType.OSUnknown;
    }
    return osType;
  }
}
