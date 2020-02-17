// Copyright (c) 2020 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.
package org.cef;

/**
 * To allow customization of System.load() calls by supplying a different
 * implementation.  You'll want to call <code>setLoader</code> with your custom
 * implementation before calling into any other CEF classes which then in turn
 * will start triggering libraries to be loaded at runtime.
 */
public class SystemBootstrap {
    /**
     * Simple interface for how a library by name should be loaded.
     */
    static public interface Loader { public void loadLibrary(String libname); }

    /**
     * Default implementation is to call System.loadLibrary
     */
    static private Loader loader_ = new Loader() {
        @Override
        public void loadLibrary(String libname) {
            System.loadLibrary(libname);
        }
    };

    static public void setLoader(Loader loader) {
        if (loader == null) {
            throw new NullPointerException("Loader cannot be null");
        }
        loader_ = loader;
    }

    static public void loadLibrary(String libname) {
        loader_.loadLibrary(libname);
    }
}