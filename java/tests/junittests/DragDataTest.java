// Copyright (c) 2019 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package tests.junittests;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertNotNull;
import static org.junit.jupiter.api.Assertions.assertTrue;

import org.cef.callback.CefDragData;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.extension.ExtendWith;

import java.util.Vector;

// Test the TestFrame implementation.
@ExtendWith(TestSetupExtension.class)
class DragDataTest {
    @Test
    void createEmpty() {
        CefDragData dragData = CefDragData.create();
        assertNotNull(dragData);
        assertFalse(dragData.isReadOnly());

        assertFalse(dragData.isLink());
        assertFalse(dragData.isFile());
        assertTrue(dragData.isFragment()); // Default state.

        assertEquals("", dragData.getLinkURL());
        assertEquals("", dragData.getLinkTitle());
        assertEquals("", dragData.getLinkMetadata());
        assertEquals("", dragData.getFragmentText());
        assertEquals("", dragData.getFragmentHtml());
        assertEquals("", dragData.getFragmentBaseURL());
        assertEquals(0, dragData.getFileContents(null));
        assertEquals("", dragData.getFileName());
        assertFalse(dragData.getFileNames(null));

        // Explicit cleanup to avoid memory leaks.
        dragData.dispose();
    }

    @Test
    void createLink() {
        CefDragData dragData = CefDragData.create();
        assertNotNull(dragData);
        assertFalse(dragData.isReadOnly());

        final String linkURL = "http://test.com/test.html";
        dragData.setLinkURL(linkURL);
        assertEquals(linkURL, dragData.getLinkURL());

        final String linkTitle = "Test Title";
        dragData.setLinkTitle(linkTitle);
        assertEquals(linkTitle, dragData.getLinkTitle());

        final String linkMetadata = "something";
        dragData.setLinkMetadata(linkMetadata);
        assertEquals(linkMetadata, dragData.getLinkMetadata());

        assertTrue(dragData.isLink());
        assertFalse(dragData.isFile());
        assertFalse(dragData.isFragment());

        // Explicit cleanup to avoid memory leaks.
        dragData.dispose();
    }

    @Test
    void createFile() {
        CefDragData dragData = CefDragData.create();
        assertNotNull(dragData);
        assertFalse(dragData.isReadOnly());

        final String path1 = "/path/to/file1.txt";
        final String path2 = "/path/to/file2.txt";

        dragData.addFile(path1, "File 1");
        dragData.addFile(path2, "File 2");

        Vector<String> fileNames = new Vector<>();
        assertTrue(dragData.getFileNames(fileNames));

        assertEquals(2, fileNames.size());
        assertEquals(path1, fileNames.get(0));
        assertEquals(path2, fileNames.get(1));

        assertFalse(dragData.isLink());
        assertTrue(dragData.isFile());
        assertFalse(dragData.isFragment());

        // Explicit cleanup to avoid memory leaks.
        dragData.dispose();
    }

    @Test
    void createFragment() {
        CefDragData dragData = CefDragData.create();
        assertNotNull(dragData);
        assertFalse(dragData.isReadOnly());

        final String fragmentText = "something";
        dragData.setFragmentText(fragmentText);
        assertEquals(fragmentText, dragData.getFragmentText());

        final String fragmentHtml = "<span>something</span>";
        dragData.setFragmentHtml(fragmentHtml);
        assertEquals(fragmentHtml, dragData.getFragmentHtml());

        final String fragmentBaseURL = "http://test.com/test.html";
        dragData.setFragmentBaseURL(fragmentBaseURL);
        assertEquals(fragmentBaseURL, dragData.getFragmentBaseURL());

        assertFalse(dragData.isLink());
        assertFalse(dragData.isFile());
        assertTrue(dragData.isFragment());

        // Explicit cleanup to avoid memory leaks.
        dragData.dispose();
    }

    @Test
    void cloneObject() {
        CefDragData dragData = CefDragData.create();
        assertNotNull(dragData);
        assertFalse(dragData.isReadOnly());

        final String fragmentText = "something";
        final String fragmentBaseURL = "http://test.com/test.html";
        dragData.setFragmentText(fragmentText);
        dragData.setFragmentBaseURL(fragmentBaseURL);

        CefDragData dragData2 = dragData.clone();
        assertFalse(dragData2.isReadOnly());
        assertEquals(fragmentText, dragData2.getFragmentText());
        assertEquals(fragmentBaseURL, dragData2.getFragmentBaseURL());

        assertFalse(dragData2.isLink());
        assertFalse(dragData2.isFile());
        assertTrue(dragData2.isFragment());

        // Explicit cleanup to avoid memory leaks.
        dragData.dispose();
        dragData2.dispose();
    }

    @Test
    void disposeObject() {
        CefDragData dragData = CefDragData.create();
        assertNotNull(dragData);
        assertFalse(dragData.isReadOnly());

        final String fragmentText = "something";
        dragData.setFragmentText(fragmentText);
        assertEquals(fragmentText, dragData.getFragmentText());

        // Explicit cleanup to avoid memory leaks.
        // After dispose() the Java object is no longer backed by a CEF object.
        dragData.dispose();

        assertEquals(null, dragData.getFragmentText());
    }
}
