// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package tests;

import java.awt.BorderLayout;
import java.awt.Component;
import java.awt.FocusTraversalPolicy;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.io.File;
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

import org.cef.CefBrowser;
import org.cef.CefClient;
import org.cef.CefDisplayHandler;
import org.cef.CefContext;
import org.cef.CefFocusHandlerAdapter;
import org.cef.CefMessageRouterHandler;
import org.cef.CefQueryCallback;

public class MainFrame extends JFrame implements CefDisplayHandler, CefMessageRouterHandler {
  private static MainFrame frame_;
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

    // load required native libraries
    CefContext.loadLibraries();

    // OSR mode is enabled by default on Linux.
    boolean osrEnabledArg = true;
    if (CefContext.isWindows() || CefContext.isMacintosh()) {
      // OSR mode is disabled by default on Windows and Mac OS X.
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
    frame_ = new MainFrame(osrEnabled); 
    frame_.addWindowListener(new WindowAdapter() {
      @Override
      public void windowOpened(WindowEvent e) {
        if (osrEnabled || !CefContext.isMacintosh()) {
          CefContext.initialize("", osrEnabled, argsForCef);
          timer.start();
        }
        frame_.createBrowser();
      }

      @Override
      public void windowClosing(WindowEvent e) {
        frame_.destroyBrowser();
        frame_.dispose();
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

    frame_.setSize(800, 600);
    frame_.setVisible(true);

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
    client_ = new CefClient(false, osrEnabled);
    client_.addDisplayHandler(this);
    client_.addMessageRouterHandler(this);
    client_.addFocusHandler(new CefFocusHandlerAdapter() {
      @Override
      public void onTakeFocus(CefBrowser browser, boolean next) {
        FocusTraversalPolicy policy = getFocusTraversalPolicy();
        if (policy == null)
          return;
        if (next) {
          policy.getFirstComponent(frame_).requestFocus();
        } else {
          policy.getLastComponent(frame_).requestFocus();
        }
      }
    });
    getContentPane().add(createContentPanel(), BorderLayout.CENTER);

    JMenuBar menuBar = createMenuBar();

    // Binding Test resource is cefclient/res/binding.html from the CEF binary distribution.
    addBookmark("Binding Test", "http://www.magpcss.org/pub/jcef_binding_1750.html");
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
  public void onAddressChange(CefBrowser browser, String url) {
    address_field_.setText(url);
  }

  @Override
  public void onTitleChange(CefBrowser browser, String title) {
    setTitle(title);
  }

  @Override
  public boolean onQuery(CefBrowser browser,
                         long query_id,
                         String request,
                         boolean persistent,
                         CefQueryCallback callback) {
    if (request.indexOf("BindingTest:") == 0) {
      // Reverse the message and return it to the JavaScript caller.
      String msg = request.substring(12);
      callback.success(new StringBuilder(msg).reverse().toString());
      return true;
    }
    // Not handled.
    callback.failure(-1, "Request not handled");
    return false;
  }

  @Override
  public void onQueryCanceled(CefBrowser browser,
                              long query_id) {
  }
}
