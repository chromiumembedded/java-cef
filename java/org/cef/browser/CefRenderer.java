// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.browser;

import java.awt.Rectangle;
import java.nio.ByteBuffer;
import java.nio.FloatBuffer;
import javax.media.opengl.GL2;

class CefRenderer {
  private boolean transparent_;
  private GL2 initialized_context_ = null;
  private int[] texture_id_ = new int[1];
  private int view_width_ = 0;
  private int view_height_ = 0;
  private float spin_x_ = 0f;
  private float spin_y_ = 0f;
  private Rectangle popup_rect_ = new Rectangle(0, 0, 0, 0);
  private Rectangle original_popup_rect_ = new Rectangle(0, 0, 0, 0);
  private boolean use_draw_pixels_ = false;

  protected CefRenderer(boolean transparent) {
    transparent_ = transparent;
  }

  protected boolean isTransparent() {
    return transparent_;
  }

  @SuppressWarnings("static-access")
  protected void initialize(GL2 gl2) {
    if (initialized_context_ == gl2)
      return;

    initialized_context_ = gl2;

    if (!gl2.getContext().isHardwareRasterizer()) {
      // Workaround for Windows Remote Desktop which requires pot textures.
      System.out.println("opengl rendering may be slow as hardware rendering isn't available");
      use_draw_pixels_ = true;
      return;
    }

    gl2.glHint(gl2.GL_POLYGON_SMOOTH_HINT, gl2.GL_NICEST);

    gl2.glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Necessary for non-power-of-2 textures to render correctly.
    gl2.glPixelStorei(gl2.GL_UNPACK_ALIGNMENT, 1);

    // Create the texture.
    gl2.glGenTextures(1, texture_id_, 0);
    assert(texture_id_[0] != 0);

    gl2.glBindTexture(gl2.GL_TEXTURE_2D, texture_id_[0]);
    gl2.glTexParameteri(gl2.GL_TEXTURE_2D, gl2.GL_TEXTURE_MIN_FILTER, gl2.GL_NEAREST);
    gl2.glTexParameteri(gl2.GL_TEXTURE_2D, gl2.GL_TEXTURE_MAG_FILTER, gl2.GL_NEAREST);
    gl2.glTexEnvf(gl2.GL_TEXTURE_ENV, gl2.GL_TEXTURE_ENV_MODE, gl2.GL_MODULATE);
  }
  
  protected void cleanup(GL2 gl2) {
    if (texture_id_[0] != 0)
      gl2.glDeleteTextures(1, texture_id_, 0);
  }

  @SuppressWarnings("static-access")
  protected void render(GL2 gl2) {
    if (use_draw_pixels_ || view_width_ == 0 || view_height_ == 0)
      return;

    assert(initialized_context_ != null);

    final float[] vertex_data = {
        //tu,   tv,     x,     y,    z
        0.0f, 1.0f, -1.0f, -1.0f, 0.0f,
        1.0f, 1.0f,  1.0f, -1.0f, 0.0f,
        1.0f, 0.0f,  1.0f,  1.0f, 0.0f,
        0.0f, 0.0f, -1.0f,  1.0f, 0.0f
    };
    FloatBuffer vertices = FloatBuffer.wrap(vertex_data);

    gl2.glClear(gl2.GL_COLOR_BUFFER_BIT | gl2.GL_DEPTH_BUFFER_BIT);

    gl2.glMatrixMode(gl2.GL_MODELVIEW);
    gl2.glLoadIdentity();

    // Match GL units to screen coordinates.
    gl2.glViewport(0, 0, view_width_, view_height_);
    gl2.glMatrixMode(gl2.GL_PROJECTION);
    gl2.glLoadIdentity();
    gl2.glOrtho(0, 0, view_width_, view_height_, 0.1, 100.0);

    // Draw the background gradient.
    gl2.glPushAttrib(gl2.GL_ALL_ATTRIB_BITS);
    gl2.glBegin(gl2.GL_QUADS);
    gl2.glColor4f(1.0f, 0.0f, 0.0f, 1.0f);  // red
    gl2.glVertex2f(-1.0f, -1.0f);
    gl2.glVertex2f(1.0f, -1.0f);
    gl2.glColor4f(0.0f, 0.0f, 1.0f, 1.0f);  // blue
    gl2.glVertex2f(1.0f, 1.0f);
    gl2.glVertex2f(-1.0f, 1.0f);
    gl2.glEnd();
    gl2.glPopAttrib();

    // Rotate the view based on the mouse spin.
    if (spin_x_ != 0)
      gl2.glRotatef(-spin_x_, 1.0f, 0.0f, 0.0f);
    if (spin_y_ != 0)
      gl2.glRotatef(-spin_y_, 0.0f, 1.0f, 0.0f);

    if (transparent_) {
      // Alpha blending style. Texture values have premultiplied alpha.
      gl2.glBlendFunc(gl2.GL_ONE, gl2.GL_ONE_MINUS_SRC_ALPHA);

      // Enable alpha blending.
      gl2.glEnable(gl2.GL_BLEND);
    }

    // Enable 2D textures.
    gl2.glEnable(gl2.GL_TEXTURE_2D);

    // Draw the facets with the texture.
    assert(texture_id_[0] != 0);
    gl2.glBindTexture(gl2.GL_TEXTURE_2D, texture_id_[0]);
    gl2.glInterleavedArrays(gl2.GL_T2F_V3F, 0, vertices);
    gl2.glDrawArrays(gl2.GL_QUADS, 0, 4);

    // Disable 2D textures.
    gl2.glDisable(gl2.GL_TEXTURE_2D);

    if (transparent_) {
      // Disable alpha blending.
      gl2.glDisable(gl2.GL_BLEND);
    }
  }

  protected void onPopupSize(Rectangle rect) {
    if (rect.width <= 0 || rect.height <= 0)
      return;
    original_popup_rect_ = rect;
    popup_rect_ = getPopupRectInWebView(original_popup_rect_);
  }

  protected Rectangle getPopupRect() {
    return (Rectangle)popup_rect_.clone();
  }

  protected Rectangle getPopupRectInWebView(Rectangle original_rect) {
    Rectangle rc = original_rect;
    // if x or y are negative, move them to 0.
    if (rc.x < 0)
      rc.x = 0;
    if (rc.y < 0)
      rc.y = 0;
    // if popup goes outside the view, try to reposition origin
    if (rc.x + rc.width > view_width_)
      rc.x = view_width_ - rc.width;
    if (rc.y + rc.height > view_height_)
      rc.y = view_height_ - rc.height;
    // if x or y became negative, move them to 0 again.
    if (rc.x < 0)
      rc.x = 0;
    if (rc.y < 0)
      rc.y = 0;
    return rc;
  }

  protected void clearPopupRects() {
    popup_rect_.setBounds(0, 0, 0, 0);
    original_popup_rect_.setBounds(0, 0, 0, 0);
  }

  @SuppressWarnings("static-access")
  protected void onPaint(GL2 gl2,
                         boolean popup,
                         Rectangle[] dirtyRects,
                         ByteBuffer buffer,
                         int width,
                         int height) {
    initialize(gl2);

    if (use_draw_pixels_) {
      gl2.glRasterPos2f(-1, 1);
      gl2.glPixelZoom(1, -1);
      gl2.glDrawPixels(width, height, GL2.GL_BGRA, GL2.GL_UNSIGNED_BYTE, buffer);
      return;
    }

    if (transparent_) {
      // Enable alpha blending.
      gl2.glEnable(gl2.GL_BLEND);
    }

    // Enable 2D textures.
    gl2.glEnable(gl2.GL_TEXTURE_2D);

    assert(texture_id_[0] != 0);
    gl2.glBindTexture(gl2.GL_TEXTURE_2D, texture_id_[0]);

    if (!popup) {
      int old_width = view_width_;
      int old_height = view_height_;

      view_width_ = width;
      view_height_ = height;

      gl2.glPixelStorei(gl2.GL_UNPACK_ROW_LENGTH, view_width_);

      if (old_width != view_width_ || old_height != view_height_) {
        // Update/resize the whole texture.
        gl2.glPixelStorei(gl2.GL_UNPACK_SKIP_PIXELS, 0);
        gl2.glPixelStorei(gl2.GL_UNPACK_SKIP_ROWS, 0);
        gl2.glTexImage2D(gl2.GL_TEXTURE_2D, 0, gl2.GL_RGBA, view_width_, view_height_, 0,
                         gl2.GL_BGRA, gl2.GL_UNSIGNED_INT_8_8_8_8_REV, buffer);
      } else {
        // Update just the dirty rectangles.
        for (int i = 0; i < dirtyRects.length; ++i) {
          Rectangle rect = dirtyRects[i];
          gl2.glPixelStorei(gl2.GL_UNPACK_SKIP_PIXELS, rect.x);
          gl2.glPixelStorei(gl2.GL_UNPACK_SKIP_ROWS, rect.y);
          gl2.glTexSubImage2D(gl2.GL_TEXTURE_2D, 0, rect.x, rect.y, rect.width,
                              rect.height, gl2.GL_BGRA, gl2.GL_UNSIGNED_INT_8_8_8_8_REV,
                              buffer);
        }
      }
    } else if (popup && popup_rect_.width > 0 && popup_rect_.height > 0) {
      int skip_pixels = 0, x = popup_rect_.x;
      int skip_rows = 0, y = popup_rect_.y;
      int w = width;
      int h = height;

      // Adjust the popup to fit inside the view.
      if (x < 0) {
        skip_pixels = -x;
        x = 0;
      }
      if (y < 0) {
        skip_rows = -y;
        y = 0;
      }
      if (x + w > view_width_)
        w -= x + w - view_width_;
      if (y + h > view_height_)
        h -= y + h - view_height_;

      // Update the popup rectangle.
      gl2.glPixelStorei(gl2.GL_UNPACK_ROW_LENGTH, width);
      gl2.glPixelStorei(gl2.GL_UNPACK_SKIP_PIXELS, skip_pixels);
      gl2.glPixelStorei(gl2.GL_UNPACK_SKIP_ROWS, skip_rows);
      gl2.glTexSubImage2D(gl2.GL_TEXTURE_2D, 0, x, y, w, h, gl2.GL_BGRA,
                          gl2.GL_UNSIGNED_INT_8_8_8_8_REV, buffer);
    }

    // Disable 2D textures.
    gl2.glDisable(gl2.GL_TEXTURE_2D);

    if (transparent_) {
      // Disable alpha blending.
      gl2.glDisable(gl2.GL_BLEND);
    }
  }

  protected void setSpin(float spinX, float spinY) {
    spin_x_ = spinX;
    spin_y_ = spinY;
  }

  protected void incrementSpin(float spinDX, float spinDY) {
    spin_x_ -= spinDX;
    spin_y_ -= spinDY;
  }
}
