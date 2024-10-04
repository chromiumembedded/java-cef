// Copyright (c) 2019 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package tests.junittests;

import static org.junit.jupiter.api.Assertions.assertNotNull;
import static org.junit.jupiter.api.Assertions.assertNull;

import static tests.junittests.TestSetupContext.debugPrint;

import org.cef.CefApp;
import org.cef.CefClient;
import org.cef.browser.CefBrowser;
import org.cef.browser.CefFrame;
import org.cef.callback.CefAuthCallback;
import org.cef.callback.CefCallback;
import org.cef.handler.CefCookieAccessFilter;
import org.cef.handler.CefLifeSpanHandler;
import org.cef.handler.CefLoadHandler;
import org.cef.handler.CefRequestHandler;
import org.cef.handler.CefResourceHandler;
import org.cef.handler.CefResourceRequestHandler;
import org.cef.misc.BoolRef;
import org.cef.misc.StringRef;
import org.cef.network.CefRequest;
import org.cef.network.CefRequest.TransitionType;
import org.cef.network.CefResponse;
import org.cef.network.CefURLRequest;

import java.awt.BorderLayout;
import java.awt.Component;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.util.HashMap;
import java.util.concurrent.CountDownLatch;

import javax.swing.JFrame;

// Base class for browsers that run tests.
class TestFrame extends JFrame implements CefLifeSpanHandler, CefLoadHandler, CefRequestHandler,
                                          CefResourceRequestHandler {
    private static final long serialVersionUID = -5570653778104813836L;
    private boolean isClosed_ = false;
    private CountDownLatch countdown_ = new CountDownLatch(1);

    // Resources that have been populated for the test.
    private class ResourceContent {
        String content;
        String mimeType;
        HashMap<String, String> headerMap;
    }
    private HashMap<String, ResourceContent> resourceMap_ = null;

    protected final CefClient client_;
    protected CefBrowser browser_ = null;

    TestFrame() {
        client_ = CefApp.getInstance().createClient();
        assertNotNull(client_);

        // Browser window closing works as follows:
        //   1. Sending WindowEvent.WINDOW_CLOSING calls WindowAdapter.windowClosing.
        //   2. WindowAdapter.windowClosing calls CefBrowser.close(false).
        //   3. CEF calls CefLifeSpanHandler.doClose() which calls CefBrowser.doClose()
        //      which returns true (canceling the close).
        //   4. CefBrowser.doClose() triggers another call to WindowAdapter.windowClosing.
        //   5. WindowAdapter.windowClosing calls CefBrowser.close(true).
        //   6. For windowed browsers CEF destroys the native window handle. For OSR
        //      browsers CEF calls CefLifeSpanHandler.doClose() which calls
        //      CefBrowser.doClose() again which returns false (allowing the close).
        //   7. CEF calls CefLifeSpanHandler.onBeforeClose and the browser is destroyed.
        addWindowListener(new WindowAdapter() {
            @Override
            public void windowClosing(WindowEvent e) {
                boolean isClosed = isClosed_;

                if (isClosed) {
                    // Cause browser.doClose() to return false so that OSR browsers
                    // can close.
                    browser_.setCloseAllowed();
                }

                // Results in another call to this method.
                if (debugPrint())
                    System.out.println("windowClosing CefBrowser.close(" + isClosed + ")");
                browser_.close(isClosed);
                if (!isClosed_) {
                    isClosed_ = true;
                }
                if (isClosed) {
                    // Dispose after the 2nd call to this method.
                    if (debugPrint()) System.out.println("windowClosing Frame.dispose");
                    dispose();
                }
            }
        });

        client_.addLifeSpanHandler(this);
        client_.addLoadHandler(this);
        client_.addRequestHandler(this);

        setupTest();
    }

    protected void createBrowser(String startURL) {
        assertNull(browser_);
        browser_ = client_.createBrowser(startURL, false /* isTransparent */);
        assertNotNull(browser_);

        pack();
        setSize(800, 600);
        setVisible(true);
    }

    // Override this method to perform test setup.
    protected void setupTest() {}

    // Override this method to perform test cleanup.
    protected void cleanupTest() {
        if (debugPrint()) System.out.println("cleanupTest");
        client_.dispose();
        // Allow the test to complete.
        countdown_.countDown();
    }

    // Call this method to terminate the test by dispatching a window close event.
    protected final void terminateTest() {
        if (debugPrint()) System.out.println("terminateTest");
        if (!isClosed_) {
            dispatchEvent(new WindowEvent(this, WindowEvent.WINDOW_CLOSING));
        }
    }

    // Block until the test completes.
    public final void awaitCompletion() {
        try {
            countdown_.await();
        } catch (InterruptedException e) {
        }
        if (debugPrint()) System.out.println("awaitCompletion returned");
    }

    protected void addResource(String url, String content, String mimeType) {
        addResource(url, content, mimeType, null);
    }

    // Add a resource that will be returned via getResourceHandler().
    protected void addResource(
            String url, String content, String mimeType, HashMap<String, String> headerMap) {
        if (resourceMap_ == null) resourceMap_ = new HashMap<>();

        assertNull(resourceMap_.get(url));

        ResourceContent rc = new ResourceContent();
        rc.content = content;
        rc.mimeType = mimeType;
        rc.headerMap = headerMap;

        resourceMap_.put(url, rc);
    }

    // CefLifeSpanHandler methods:

    @Override
    public void onAfterCreated(CefBrowser browser) {
        if (debugPrint()) System.out.println("onAfterCreated id=" + browser.getIdentifier());
    }

    @Override
    public boolean onBeforePopup(
            CefBrowser browser, CefFrame frame, String target_url, String target_frame_name) {
        return false;
    }

    @Override
    public void onAfterParentChanged(CefBrowser browser) {}

    @Override
    public boolean doClose(CefBrowser browser) {
        boolean result = browser.doClose();
        if (debugPrint()) {
            System.out.println(
                    "doClose id=" + browser.getIdentifier() + " CefBrowser.doClose=" + result);
        }
        return result;
    }

    @Override
    public void onBeforeClose(CefBrowser browser) {
        if (debugPrint()) System.out.println("onBeforeClose id=" + browser.getIdentifier());
        cleanupTest();
    }

    // CefLoadHandler methods:

    @Override
    public void onLoadingStateChange(
            CefBrowser browser, boolean isLoading, boolean canGoBack, boolean canGoForward) {}

    @Override
    public void onLoadStart(CefBrowser browser, CefFrame frame, TransitionType transitionType) {}

    @Override
    public void onLoadEnd(CefBrowser browser, CefFrame frame, int httpStatusCode) {}

    @Override
    public void onLoadError(CefBrowser browser, CefFrame frame, ErrorCode errorCode,
            String errorText, String failedUrl) {}

    // CefRequestHandler methods:

    @Override
    public boolean onBeforeBrowse(CefBrowser browser, CefFrame frame, CefRequest request,
            boolean user_gesture, boolean is_redirect) {
        return false;
    }

    @Override
    public boolean onOpenURLFromTab(
            CefBrowser browser, CefFrame frame, String target_url, boolean user_gesture) {
        return false;
    }

    @Override
    public CefResourceRequestHandler getResourceRequestHandler(CefBrowser browser, CefFrame frame,
            CefRequest request, boolean isNavigation, boolean isDownload, String requestInitiator,
            BoolRef disableDefaultHandling) {
        return this;
    }

    @Override
    public boolean getAuthCredentials(CefBrowser browser, String origin_url, boolean isProxy,
            String host, int port, String realm, String scheme, CefAuthCallback callback) {
        return false;
    }

    @Override
    public boolean onCertificateError(CefBrowser browser, CefLoadHandler.ErrorCode cert_error,
            String request_url, CefCallback callback) {
        return false;
    }

    @Override
    public void onRenderProcessTerminated(
            CefBrowser browser, TerminationStatus status, int error_code, String error_string) {}

    // CefResourceRequestHandler methods:

    @Override
    public CefCookieAccessFilter getCookieAccessFilter(
            CefBrowser browser, CefFrame frame, CefRequest request) {
        return null;
    }

    @Override
    public boolean onBeforeResourceLoad(CefBrowser browser, CefFrame frame, CefRequest request) {
        return false;
    }

    @Override
    public CefResourceHandler getResourceHandler(
            CefBrowser browser, CefFrame frame, CefRequest request) {
        if (resourceMap_ != null) {
            String url = request.getURL();

            // Ignore the query component, if any.
            int idx = url.indexOf('?');
            if (idx > 0) url = url.substring(0, idx);

            ResourceContent rc = resourceMap_.get(url);
            if (rc != null) {
                if (debugPrint()) System.out.println("Found resource for: " + url);
                return new TestResourceHandler(rc.content, rc.mimeType, rc.headerMap);
            }
        }

        return null;
    }

    @Override
    public void onResourceRedirect(CefBrowser browser, CefFrame frame, CefRequest request,
            CefResponse response, StringRef new_url) {}

    @Override
    public boolean onResourceResponse(
            CefBrowser browser, CefFrame frame, CefRequest request, CefResponse response) {
        return false;
    }

    @Override
    public void onResourceLoadComplete(CefBrowser browser, CefFrame frame, CefRequest request,
            CefResponse response, CefURLRequest.Status status, long receivedContentLength) {}

    @Override
    public void onProtocolExecution(
            CefBrowser browser, CefFrame frame, CefRequest request, BoolRef allowOsExecution) {}
}
