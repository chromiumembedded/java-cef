// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.callback;

import java.util.Date;

class CefDownloadItem_N extends CefNativeAdapter implements CefDownloadItem {

  protected void finalize() throws Throwable {
    super.finalize();
  }

  CefDownloadItem_N() {
  }

  @Override
  public boolean isValid() {
    try {
      return N_IsValid();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return false;
  }

  @Override
  public boolean isInProgress() {
    try {
      return N_IsInProgress();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return false;
  }

  @Override
  public boolean isComplete() {
    try {
      return N_IsComplete();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return false;
  }

  @Override
  public boolean isCanceled() {
    try {
      return N_IsCanceled();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return false;
  }

  @Override
  public long getCurrentSpeed() {
    try {
      return N_GetCurrentSpeed();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return 0;
  }

  @Override
  public int getPercentComplete() {
    try {
      return N_GetPercentComplete();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return 0;
  }

  @Override
  public long getTotalBytes() {
    try {
      return N_GetTotalBytes();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return 0;
  }

  @Override
  public long getReceivedBytes() {
    try {
      return N_GetReceivedBytes();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return 0;
  }

  @Override
  public Date getStartTime() {
    try {
      return N_GetStartTime();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return null;
  }

  @Override
  public Date getEndTime() {
    try {
      return N_GetEndTime();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return null;
  }

  @Override
  public String getFullPath() {
    try {
      return N_GetFullPath();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return null;
  }

  @Override
  public int getId() {
    try {
      return N_GetId();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return 0;
  }

  @Override
  public String getURL() {
    try {
      return N_GetURL();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return null;
  }

  @Override
  public String getSuggestedFileName() {
    try {
      return N_GetSuggestedFileName();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return null;
  }

  @Override
  public String getContentDisposition() {
    try {
      return N_GetContentDisposition();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return null;
  }

  @Override
  public String getMimeType() {
    try {
      return N_GetMimeType();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return null;
  }

  private final native boolean N_IsValid();
  private final native boolean N_IsInProgress();
  private final native boolean N_IsComplete();
  private final native boolean N_IsCanceled();
  private final native long N_GetCurrentSpeed();
  private final native int N_GetPercentComplete();
  private final native long N_GetTotalBytes();
  private final native long N_GetReceivedBytes();
  private final native Date N_GetStartTime();
  private final native Date N_GetEndTime();
  private final native String N_GetFullPath();
  private final native int N_GetId();
  private final native String N_GetURL();
  private final native String N_GetSuggestedFileName();
  private final native String N_GetContentDisposition(); 
  private final native String N_GetMimeType();
}
