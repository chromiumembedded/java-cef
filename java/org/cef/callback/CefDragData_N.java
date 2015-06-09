package org.cef.callback;

import java.io.OutputStream;
import java.util.Vector;

class CefDragData_N extends CefDragData implements CefNative {
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
    super();
  }

  public static CefDragData createNative() {
    try {
      return CefDragData_N.N_Create();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
      return null;
    }
  }

  @Override
  public CefDragData clone() {
    try {
      return N_Clone();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
      return null;
    }
  }

  @Override
  public void dispose() {
    try {
      N_Dispose();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
  }

  @Override 
  public boolean isReadOnly() {
    try {
      return N_IsReadOnly();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
      return true;
    }
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
  public int getFileContents(OutputStream writer) {
    try {
      return N_GetFileContents(writer);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return 0;
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

  public void setLinkURL(String url) {
    try {
      N_SetLinkURL(url);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
  }

  public void setLinkTitle(String title) {
    try {
      N_SetLinkTitle(title);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
  }

  public void setLinkMetadata(String data) {
    try {
      N_SetLinkMetadata(data);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
  }

  public void setFragmentText(String text) {
    try {
      N_SetFragmentText(text);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
  }

  public void setFragmentHtml(String html) {
    try {
      N_SetFragmentHtml(html);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
  }

  public void setFragmentBaseURL(String baseUrl) {
    try {
      N_SetFragmentBaseURL(baseUrl);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
  }

  public void resetFileContents() {
    try {
      N_ResetFileContents();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
  }

  public void addFile(String path, String displayName) {
    try {
      N_AddFile(path, displayName);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
  }

  private final native static CefDragData_N N_Create();
  private final native CefDragData_N N_Clone();
  private final native void N_Dispose();
  private final native boolean N_IsReadOnly();
  private final native boolean N_IsLink();
  private final native boolean N_IsFragment();
  private final native boolean N_IsFile();
  private final native String N_GetLinkURL();
  private final native String N_GetLinkTitle();
  private final native String N_GetLinkMetadata();
  private final native String N_GetFragmentText();
  private final native String N_GetFragmentHtml();
  private final native String N_GetFragmentBaseURL();
  private final native int N_GetFileContents(OutputStream writer);
  private final native String N_GetFileName();
  private final native boolean N_GetFileNames(Vector<String> names);
  private final native void N_SetLinkURL(String url);
  private final native void N_SetLinkTitle(String title);
  private final native void N_SetLinkMetadata(String data);
  private final native void N_SetFragmentText(String text);
  private final native void N_SetFragmentHtml(String html);
  private final native void N_SetFragmentBaseURL(String baseUrl);
  private final native void N_ResetFileContents();
  private final native void N_AddFile(String path, String displayName);

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
