// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package tests;

import java.awt.BorderLayout;
import java.awt.Cursor;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.io.File;
import java.net.URL;
import javax.swing.Box;
import javax.swing.BoxLayout;
import javax.swing.JButton;
import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.JPanel;
import javax.swing.JTextField;
import javax.swing.SwingUtilities;
import javax.swing.Timer;

import org.cef.CefClient;
import org.cef.CefClientDelegate;
import org.cef.CefContext;

public class MainFrame extends JFrame implements CefClientDelegate {
  public static void main(String [] args) {
    // Timer used to pump the CEF message loop.
    final Timer timer = new Timer(33, new ActionListener() {
      public void actionPerformed(ActionEvent evt) {
        SwingUtilities.invokeLater(new Runnable() {
          public void run() {
            CefContext.doMessageLoopWork();
          }
        });
      }
    });

    final MainFrame frame = new MainFrame(); 
    frame.addWindowListener(new WindowAdapter() {
      @Override
      public void windowOpened(WindowEvent e) {
        CefContext.initialize("");
        timer.start();
        frame.createBrowser();
      }

      @Override
      public void windowClosing(WindowEvent e) {
        frame.destroyBrowser();
        frame.dispose();
        timer.stop();
        CefContext.shutdown();
        System.out.println("shutdown complete");
        System.exit(0);
      }
    });

    frame.setSize(800, 600);
    frame.setVisible(true);
  }

  private JTextField address_field_;
  private String last_selected_file_ = "";
  private CefClient client_;
  
  public MainFrame() {
    client_ = new CefClient(this, false);
    getContentPane().add(createContentPanel(), BorderLayout.CENTER);
    setJMenuBar(createMenuBar());
  }
  
  private void createBrowser() {
    client_.createBrowser("http://www.google.com");
  }
  
  private void destroyBrowser() {
    client_.destroyBrowser();
    client_ = null;
  }

  private JPanel createContentPanel() {
    JPanel contentPanel = new JPanel(new BorderLayout());
    contentPanel.add(createControlPanel(), BorderLayout.NORTH);
    contentPanel.add(client_.getCanvas(), BorderLayout.CENTER);
    return contentPanel;
  }

  private JPanel createControlPanel() {
    JPanel controlPanel = new JPanel();
    controlPanel.setLayout(new BoxLayout(controlPanel, BoxLayout.X_AXIS));

    controlPanel.add(Box.createHorizontalStrut(5));

    controlPanel.add(Box.createHorizontalStrut(5));
    
    JButton backButton = new JButton("Back");
    backButton.setAlignmentX(LEFT_ALIGNMENT);
    backButton.addActionListener(new ActionListener() {
      @Override
      public void actionPerformed(ActionEvent e) {
        client_.getBrowser().goBack();
      }
    });
    controlPanel.add(backButton);
    
    controlPanel.add(Box.createHorizontalStrut(5));
    
    JButton forwardButton = new JButton("Forward");
    forwardButton.setAlignmentX(LEFT_ALIGNMENT);
    forwardButton.addActionListener(new ActionListener() {
      @Override
      public void actionPerformed(ActionEvent e) {
        client_.getBrowser().goForward();
      }
    });
    controlPanel.add(forwardButton);

    controlPanel.add(Box.createHorizontalStrut(5));

    JLabel addressLabel = new JLabel("Address:");
    addressLabel.setAlignmentX(LEFT_ALIGNMENT);
    controlPanel.add(addressLabel);

    controlPanel.add(Box.createHorizontalStrut(5));

    address_field_ = new JTextField(100);
    address_field_.setAlignmentX(LEFT_ALIGNMENT);
    address_field_.addActionListener(new ActionListener() {
      @Override
      public void actionPerformed(ActionEvent e) {
        client_.getBrowser().loadURL(address_field_.getText());
      }
    });
    controlPanel.add(address_field_);

    controlPanel.add(Box.createHorizontalStrut(5));

    JButton goButton = new JButton("Go");
    goButton.setAlignmentX(LEFT_ALIGNMENT);
    goButton.addActionListener(new ActionListener() {
      @Override
      public void actionPerformed(ActionEvent e) {
        client_.getBrowser().loadURL(address_field_.getText());
      }
    });
    controlPanel.add(goButton);

    controlPanel.add(Box.createHorizontalStrut(5));

    return controlPanel;
  }
  
  private JMenuBar createMenuBar() {
    JMenuBar menuBar = new JMenuBar();

    JMenu fileMenu = new JMenu("File");

    JMenuItem openFileItem = new JMenuItem("Open file...");
    openFileItem.addActionListener(new ActionListener() {
      @Override
      public void actionPerformed(ActionEvent arg0) {
        JFileChooser fc = new JFileChooser(new File(last_selected_file_));
        // Show open dialog; this method does not return until the dialog is closed 
        fc.showOpenDialog(MainFrame.this); 
        File selectedFile = fc.getSelectedFile(); 
        if (selectedFile != null) {
          last_selected_file_ = selectedFile.getAbsolutePath();
          client_.getBrowser().loadURL("file:///" + selectedFile.getAbsolutePath());
        }
      }
    });
    fileMenu.add(openFileItem);

    fileMenu.addSeparator();

    JMenuItem exitItem = new JMenuItem("Exit");
    final JFrame parent = this;
    exitItem.addActionListener(new ActionListener() {
      @Override
      public void actionPerformed(ActionEvent e) {
        dispatchEvent(new WindowEvent(parent, WindowEvent.WINDOW_CLOSING));
      }
    });
    fileMenu.add(exitItem);

    menuBar.add(fileMenu);

    return menuBar;
  }

  @Override
  public void onAddressChange(CefClient client, String url) {
    address_field_.setText(url);
  }

  @Override
  public void onTitleChange(CefClient client, String title) {
    setTitle(title);
  }
  
  @Override
  public void onCursorChange(CefClient client, Cursor cursor) {
    setCursor(cursor);
  }
}
