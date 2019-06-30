// Copyright (c) 2019 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package tests.junittests;

import org.cef.callback.CefCallback;
import org.cef.handler.CefResourceHandlerAdapter;
import org.cef.misc.IntRef;
import org.cef.misc.StringRef;
import org.cef.network.CefRequest;
import org.cef.network.CefResponse;

import java.nio.ByteBuffer;
import java.util.HashMap;

class TestResourceHandler extends CefResourceHandlerAdapter {
    private int offset_ = 0;
    private final String content_;
    private final String mimeType_;
    private final HashMap<String, String> headerMap_;

    TestResourceHandler(String content, String mimeType, HashMap<String, String> headerMap) {
        content_ = content;
        mimeType_ = mimeType;
        headerMap_ = headerMap;
    }

    @Override
    public boolean processRequest(CefRequest request, CefCallback callback) {
        callback.Continue();
        return true;
    }

    @Override
    public void getResponseHeaders(
            CefResponse response, IntRef response_length, StringRef redirectUrl) {
        response_length.set(content_.length());
        response.setMimeType(mimeType_);
        response.setStatus(200);

        if (headerMap_ != null) {
            HashMap<String, String> headerMap = new HashMap<>();
            response.getHeaderMap(headerMap);
            headerMap.putAll(headerMap_);
            response.setHeaderMap(headerMap);
        }
    }

    @Override
    public boolean readResponse(
            byte[] data_out, int bytes_to_read, IntRef bytes_read, CefCallback callback) {
        int length = content_.length();
        if (offset_ >= length) return false;

        // Extract up to |bytes_to_read| bytes from |content_|.
        int endPos = offset_ + bytes_to_read;
        String dataToSend = (endPos > length) ? content_.substring(offset_)
                                              : content_.substring(offset_, endPos);

        // Copy extracted bytes into |data_out| and set the read length to |bytes_read|.
        ByteBuffer result = ByteBuffer.wrap(data_out);
        result.put(dataToSend.getBytes());
        bytes_read.set(dataToSend.length());

        offset_ = endPos;
        return true;
    }
}
