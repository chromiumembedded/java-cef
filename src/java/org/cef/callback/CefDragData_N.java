package org.cef.callback;

import java.util.Vector;

class CefDragData_N implements CefDragData {
  // Used internally to store a pointer to the CEF object.
  private long N_CefHandle = 0;

  @Override
  public void setNativeRef(String identifer, long nativeRef) {
    N_CefHandle = nativeRef;
  }

  @Override
  public long getNativeRef(String identifer) {
    return N_CefHandle;
  }

  CefDragData_N() {
  }

  @Override
  public boolean isLink() {
    try {
      return N_IsLink();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return false;
  }

  @Override
  public boolean isFragment() {
    try {
      return N_IsFragment();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return false;
  }

  @Override
  public boolean isFile() {
    try {
      return N_IsFile();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return false;
  }

  @Override
  public String getLinkURL() {
    try {
      return N_GetLinkURL();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return null;
  }

  @Override
  public String getLinkTitle() {
    try {
      return N_GetLinkTitle();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return null;
  }

  @Override
  public String getLinkMetadata() {
    try {
      return N_GetLinkMetadata();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return null;
  }

  @Override
  public String getFragmentText() {
    try {
      return N_GetFragmentText();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return null;
  }

  @Override
  public String getFragmentHtml() {
    try {
      return N_GetFragmentHtml();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return null;
  }

  @Override
  public String getFragmentBaseURL() {
    try {
      return N_GetFragmentBaseURL();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return null;
  }

  @Override
  public String getFileName() {
    try {
      return N_GetFileName();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return null;
  }

  @Override
  public boolean getFileNames(Vector<String> names) {
    try {
      return N_GetFileNames(names);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return false;
  }

  private final native boolean N_IsLink();
  private final native boolean N_IsFragment();
  private final native boolean N_IsFile();
  private final native String N_GetLinkURL();
  private final native String N_GetLinkTitle();
  private final native String N_GetLinkMetadata();
  private final native String N_GetFragmentText();
  private final native String N_GetFragmentHtml();
  private final native String N_GetFragmentBaseURL();
  private final native String N_GetFileName();
  private final native boolean N_GetFileNames(Vector<String> names);

  @Override
  public String toString() {
    Vector<String> names = new Vector<>();
    getFileNames(names);
    String fileNamesStr = "{";
    for(String s : names)
      fileNamesStr += s + ",";
    fileNamesStr += "}";

    return "CefDragData_N [isLink()=" + isLink() + ", isFragment()="
        + isFragment() + ", isFile()=" + isFile() + ", getLinkURL()="
        + getLinkURL() + ", getLinkTitle()=" + getLinkTitle()
        + ", getLinkMetadata()=" + getLinkMetadata() + ", getFragmentText()="
        + getFragmentText() + ", getFragmentHtml()=" + getFragmentHtml()
        + ", getFragmentBaseURL()=" + getFragmentBaseURL() + ", getFileName()="
        + getFileName() + ", getFileNames(vector)=" + fileNamesStr + "]";
  }
}
