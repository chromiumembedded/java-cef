// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package tests;

import java.awt.BorderLayout;
import java.awt.Component;
import java.awt.Cursor;
import java.awt.FocusTraversalPolicy;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
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
    // Timer used to pump the CEF message loop in osr mode.
    final Timer timer = new Timer(33, new ActionListener() {
      public void actionPerformed(ActionEvent evt) {
        SwingUtilities.invokeLater(new Runnable() {
          public void run() {
            CefContext.doMessageLoopWork();
          }
        });
      }
    });

    // OSR mode is enabled by default on Windows and Linux.
    boolean osrEnabledArg = true;
    if (CefContext.isWindows()) {
      // TODO(jcef): Enable OSR mode by default once issue #32 is resolved.
      osrEnabledArg = true;
      for (String arg : args) {
        if (arg.toLowerCase().equals("--off-screen-rendering-disabled")) {
          osrEnabledArg = false;
          break;
        }
      }
    } else if (CefContext.isMacintosh()) {
      // OSR mode is disabled by default on Mac OS X.
      osrEnabledArg = false;
      for (String arg : args) {
        if (arg.toLowerCase().equals("--off-screen-rendering-enabled")) {
          osrEnabledArg = true;
          break;
        }
      }
    }

    final boolean osrEnabled = osrEnabledArg;
    System.out.println("Offscreen rendering " + (osrEnabled ? "enabled" : "disabled"));

    final String[] argsForCef = args;
    final MainFrame frame = new MainFrame(osrEnabled); 
    frame.addWindowListener(new WindowAdapter() {
      @Override
      public void windowOpened(WindowEvent e) {
        if (osrEnabled || !CefContext.isMacintosh()) {
          CefContext.initialize("", osrEnabled, argsForCef);
          timer.start();
        }
        frame.createBrowser();
      }

      @Override
      public void windowClosing(WindowEvent e) {
        frame.destroyBrowser();
        frame.dispose();
        if (osrEnabled || !CefContext.isMacintosh()) {
          timer.stop();
        }
        CefContext.shutdown();
        System.out.println("shutdown complete");
        if (osrEnabled || !CefContext.isMacintosh()) {
          System.exit(0);
        }
      }
    });

    frame.setSize(800, 600);
    frame.setVisible(true);

    // On Mac OS X the message loop is performed by calling initialize, which will block the main
    // process until the message loop will be quit. See
    // http://stackoverflow.com/questions/5642802/termination-of-program-on-main-thread-exit
    // for further information about the Java threading system.
    if (!osrEnabled && CefContext.isMacintosh()) {
      CefContext.initialize("", osrEnabled, argsForCef);
    }
  }

  private JTextField address_field_;
  private String last_selected_file_ = "";
  private CefClient client_;
  private JMenu bookmarkMenu_;

  public MainFrame(boolean osrEnabled) {
    client_ = new CefClient(this, false, osrEnabled);
    getContentPane().add(createContentPanel(), BorderLayout.CENTER);

    JMenuBar menuBar = createMenuBar();
    addBookmark("javachromiumembedded", "https://code.google.com/p/javachromiumembedded/");
    addBookmark("chromiumembedded", "https://code.google.com/p/chromiumembedded/");
    setJMenuBar(menuBar);
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
    address_field_.addMouseListener(new MouseAdapter() {
      @Override
      public void mouseClicked(MouseEvent arg0) {
        client_.getBrowser().setFocus(false);
        address_field_.requestFocus();
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
        // Show open dialog; this method does not return until the dialog is closed .
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

    bookmarkMenu_ = new JMenu("Bookmarks");

    JMenuItem addBookmarkItem = new JMenuItem("Add bookmark");
    addBookmarkItem.addActionListener(new ActionListener() {
      @Override
      public void actionPerformed(ActionEvent e) {
        addBookmark(getTitle(), address_field_.getText());
      }
    });
    bookmarkMenu_.add(addBookmarkItem);
    bookmarkMenu_.addSeparator();

    menuBar.add(fileMenu);
    menuBar.add(bookmarkMenu_);

    return menuBar;
  }

  void addBookmark(String name, String URL) {
    if (bookmarkMenu_ == null)
      return;

    // Test if the bookmark already exists. If yes, update URL
    Component[] entries = bookmarkMenu_.getMenuComponents();
    for (Component itemEntry : entries) {
      if( !(itemEntry instanceof JMenuItem) )
        continue;

      JMenuItem item = (JMenuItem)itemEntry;
      if (item.getText().equals(name)) {
         item.setActionCommand(URL);
         return;
      }
    }

    JMenuItem menuItem = new JMenuItem(name);
    menuItem.setActionCommand(URL);
    menuItem.addActionListener(new ActionListener() {
      @Override
      public void actionPerformed(ActionEvent e) {
        client_.getBrowser().loadURL(e.getActionCommand());
      }
    });
    bookmarkMenu_.add(menuItem);
    validate();
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

  @Override
  public void onTakeFocus(CefClient client, boolean next) {
    FocusTraversalPolicy policy = getFocusTraversalPolicy();
    if (policy == null)
      return;
    if (next) {
      policy.getFirstComponent(this).requestFocus();
    } else {
      policy.getLastComponent(this).requestFocus();
    }
  }

  @Override
  public boolean onSetFocus(CefClient client, FocusSource source) {
    // Currently nothing to do.
    return false;
  }

  @Override
  public void onGotFocus(CefClient client) {
    // Currently nothing to do.
  }
}
