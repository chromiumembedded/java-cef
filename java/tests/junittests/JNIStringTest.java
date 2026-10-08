// Copyright (c) 2026 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package tests.junittests;

import static org.junit.jupiter.api.Assertions.assertEquals;

import org.cef.browser.CefBrowser;
import org.cef.browser.CefFrame;
import org.cef.browser.CefMessageRouter;
import org.cef.callback.CefQueryCallback;
import org.cef.handler.CefDisplayHandlerAdapter;
import org.cef.handler.CefMessageRouterHandlerAdapter;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.extension.ExtendWith;

// Test that strings containing supplementary characters (encoded as surrogate pairs in Java and as
// 4-byte sequences in UTF-8) survive the trip through JNI in both directions.
@ExtendWith(TestSetupExtension.class)
class JNIStringTest {
    private static final String TEST_URL = "http://test.com/jnistring.html";
    private static final String INITIAL_TITLE = "initial";

    // The string ends with a supplementary character because that was the one that got truncated.
    private static final String TEST_STRING = "a\uD83D\uDD31b\uD83D\uDE00";

    // The page is pure ASCII because TestResourceHandler does not encode its content as UTF-8.
    private static final String TEST_CONTENT = "<html><head><title>" + INITIAL_TITLE + "</title>"
            + "</head><body><script>"
            + "var s = String.fromCodePoint(0x61, 0x1F531, 0x62, 0x1F600);"
            + "window.cefQuery({request: s,"
            + " onSuccess: function(response) { document.title = response; },"
            + " onFailure: function(code, message) { document.title = 'failure: ' + message; }});"
            + "</script></body></html>";

    private String queryRequest_ = null;
    private String finalTitle_ = null;

    @Test
    void roundTrip() {
        TestFrame frame = new TestFrame() {
            @Override
            protected void setupTest() {
                // Native to Java: the query request. Java to native: the query response.
                CefMessageRouter router = CefMessageRouter.create();
                router.addHandler(new CefMessageRouterHandlerAdapter() {
                    @Override
                    public boolean onQuery(CefBrowser browser, CefFrame frame, long queryId,
                            String request, boolean persistent, CefQueryCallback callback) {
                        queryRequest_ = request;
                        callback.success(request);
                        return true;
                    }
                }, true);
                client_.addMessageRouter(router);

                // Native to Java again: the title that the page sets from the response.
                client_.addDisplayHandler(new CefDisplayHandlerAdapter() {
                    @Override
                    public void onTitleChange(CefBrowser browser, String title) {
                        if (!INITIAL_TITLE.equals(title)) {
                            finalTitle_ = title;
                            terminateTest();
                        }
                    }
                });

                addResource(TEST_URL, TEST_CONTENT, "text/html");

                createBrowser(TEST_URL);

                super.setupTest();
            }
        };

        frame.awaitCompletion();

        assertEquals(TEST_STRING, queryRequest_, "request received by Java");
        assertEquals(TEST_STRING, finalTitle_, "title set from the response sent by Java");
    }
}
