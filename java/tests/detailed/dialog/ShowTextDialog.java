// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package tests.detailed.dialog;

import org.cef.callback.CefStringVisitor;

import java.awt.BorderLayout;
import java.awt.Frame;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.BoxLayout;
import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;

@SuppressWarnings("serial")
public class ShowTextDialog extends JDialog implements CefStringVisitor {
    private final JTextArea textArea_ = new JTextArea();

    public ShowTextDialog(Frame owner, String title) {
        super(owner, title, false);
        setLayout(new BorderLayout());
        setSize(800, 600);

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

        add(new JScrollPane(textArea_));
        add(controlPanel, BorderLayout.SOUTH);
    }

    @Override
    public void visit(String string) {
        if (!isVisible()) {
            setVisible(true);
        }
        textArea_.append(string);
    }
}
