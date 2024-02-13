// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package tests.detailed.dialog;

import org.cef.callback.CefAuthCallback;

import java.awt.Frame;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JLabel;
import javax.swing.JPasswordField;
import javax.swing.JTextField;

@SuppressWarnings("serial")
public class PasswordDialog extends JDialog implements Runnable {
    private final JTextField username_ = new JTextField(20);
    private final JPasswordField password_ = new JPasswordField(20);
    private final CefAuthCallback callback_;

    public PasswordDialog(Frame owner, CefAuthCallback callback) {
        super(owner, "Authentication required", true);
        callback_ = callback;
        setSize(400, 100);
        setLayout(new GridLayout(0, 2));
        add(new JLabel("Username:"));
        add(username_);
        add(new JLabel("Password:"));
        add(password_);

        JButton abortButton = new JButton("Abort");
        abortButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                callback_.cancel();
                setVisible(false);
                dispose();
            }
        });
        add(abortButton);

        JButton okButton = new JButton("OK");
        okButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                if (username_.getText().isEmpty()) return;
                String password = new String(password_.getPassword());
                callback_.Continue(username_.getText(), password);
                setVisible(false);
                dispose();
            }
        });
        add(okButton);
    }

    @Override
    public void run() {
        setVisible(true);
    }
}
