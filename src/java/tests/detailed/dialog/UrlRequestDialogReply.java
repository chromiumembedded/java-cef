// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package tests.detailed.dialog;

import java.awt.BorderLayout;
import java.awt.Frame;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.BorderFactory;
import javax.swing.BoxLayout;
import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.SwingUtilities;

import org.cef.callback.CefAuthCallback;
import org.cef.callback.CefURLRequestClient;
import org.cef.network.CefRequest;
import org.cef.network.CefURLRequest;
import org.cef.network.CefURLRequest.Status;

@SuppressWarnings("serial")
public class UrlRequestDialogReply extends JDialog implements CefURLRequestClient {
  private long nativeRef_ = 0;
  private final JLabel statusLabel_ = new JLabel("HTTP-Request status: ");
  private final JTextArea sentRequest_ = new JTextArea();
  private final JTextArea repliedResult_ = new JTextArea();
  private final JButton cancelButton_ = new JButton("Cancel");
  private CefURLRequest urlRequest_ = null;
  private final Frame owner_;

  public UrlRequestDialogReply(Frame owner, String title) {
    super(owner, title, false);
    setLayout(new BorderLayout());
    setSize(800, 600);

    owner_ = owner;

    JPanel controlPanel = new JPanel();
    controlPanel.setLayout(new BoxLayout(controlPanel, BoxLayout.X_AXIS));
    JButton doneButton = new JButton("Done");
    doneButton.addActionListener(new ActionListener() {
      @Override
      public void actionPerformed(ActionEvent e) {
        setVisible(false);
        dispose();
      }
    });
    controlPanel.add(doneButton);

    cancelButton_.addActionListener(new ActionListener() {
      @Override
      public void actionPerformed(ActionEvent e) {
        if (urlRequest_ != null) {
          urlRequest_.cancel();
        }
      }
    });
    cancelButton_.setEnabled(false);
    controlPanel.add(cancelButton_);

    JPanel requestPane = createPanelWithTitle("Sent HTTP-Request",1,0);
    requestPane.add(new JScrollPane(sentRequest_));
    
    JPanel replyPane = createPanelWithTitle("Reply from the server",1,0);
    replyPane.add(new JScrollPane(repliedResult_));

    JPanel contentPane = new JPanel(new GridLayout(2,0));
    contentPane.add(requestPane);
    contentPane.add(replyPane);

    add(statusLabel_, BorderLayout.PAGE_START);
    add(contentPane, BorderLayout.CENTER);
    add(controlPanel, BorderLayout.PAGE_END);
  }

  private JPanel createPanelWithTitle(String title, int rows, int cols) {
    JPanel result = new JPanel(new GridLayout(rows, cols));
    result.setBorder(BorderFactory.createCompoundBorder(
        BorderFactory.createTitledBorder(title),
        BorderFactory.createEmptyBorder(10, 10, 10, 10)));
    return result;
  }

  public void send(CefRequest request) {
    if (request == null) {
      statusLabel_.setText("HTTP-Request status: FAILED");
      sentRequest_.append("Can't send CefRequest because it is NULL");
      cancelButton_.setEnabled(false);
      return;
    }

    urlRequest_ = CefURLRequest.create(request, this);
    if (urlRequest_ == null) {
      statusLabel_.setText("HTTP-Request status: FAILED");
      sentRequest_.append("Can't send CefRequest because creation of CefURLRequest failed.");
      repliedResult_.append("The native code (CEF) returned a NULL-Pointer for CefURLRequest.");
      cancelButton_.setEnabled(false);
    } else {
      sentRequest_.append(request.toString());
      cancelButton_.setEnabled(true);
      updateStatus();
    }
  }

  private void updateStatus() {
    Status status = urlRequest_.getRequestStatus();
    statusLabel_.setText("HTTP-Request status: " + status);
    if (status == Status.UR_FAILED || status == Status.UR_CANCELED || status == Status.UR_SUCCESS){
      cancelButton_.setEnabled(false);
    }
  }


  // CefURLRequestClient

  @Override
  public void setNativeRef(String identifer, long nativeRef) {
    nativeRef_ = nativeRef;
  }

  @Override
  public long getNativeRef(String identifer) {
    return nativeRef_;
  }

  @Override
  public void onRequestComplete(CefURLRequest request) {
    repliedResult_.append("onRequestCompleted\n\n");
    repliedResult_.append(request.getResponse().toString());
    updateStatus();
  }

  @Override
  public void onUploadProgress(CefURLRequest request, int current, int total) {
    repliedResult_.append("onUploadProgress: " + current + "/" + total + " bytes\n");
    updateStatus();
  }

  @Override
  public void onDownloadProgress(CefURLRequest request, int current, int total) {
    repliedResult_.append("onDownloadProgress: " + current + "/" + total + " bytes\n");
    updateStatus();
  }

  @Override
  public void onDownloadData(CefURLRequest request, byte[] data, int data_length) {
    repliedResult_.append("onDownloadData: " + data_length + " bytes:");
    String byteStr = "";
    for (int i=0; i < data_length; i++) {
      if (i%40 == 0)
        byteStr += "\n    ";
      byteStr += String.format("%02X", data[i]) + " ";
    }
    byteStr += "\n";
    repliedResult_.append(byteStr);
    updateStatus();
  }

  @Override
  public boolean getAuthCredentials(boolean isProxy, String host, int port,
      String realm, String scheme, CefAuthCallback callback) {
    SwingUtilities.invokeLater(new PasswordDialog(owner_, callback));
    return true;
  }
}
