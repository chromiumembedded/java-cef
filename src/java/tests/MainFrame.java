// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package tests;

import java.awt.BorderLayout;
import java.awt.Component;
import java.awt.Dimension;
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
import javax.swing.JProgressBar;
import javax.swing.JTextField;
import javax.swing.SwingUtilities;
import javax.swing.Timer;

import org.cef.CefBrowser;
import org.cef.CefClient;
import org.cef.CefDisplayHandler;
import org.cef.CefContext;
import org.cef.CefFocusHandlerAdapter;
import org.cef.CefLoadHandlerAdapter;
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
  private JButton backButton_;
  private JButton forwardButton_;
  private JButton reloadButton_;
  private JProgressBar progressBar_;
  private JLabel status_field_;
  private double zoomLevel_ = 0.0;
  private JLabel zoom_label_;
  private String errorMsg_ = "";

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
    client_.addLoadHandler(new CefLoadHandlerAdapter() {
      @Override
      public void onLoadingStateChange(CefBrowser browser,
                                       boolean isLoading,
                                       boolean canGoBack,
                                       boolean canGoForward) {
        backButton_.setEnabled(canGoBack);
        forwardButton_.setEnabled(canGoForward);
        reloadButton_.setText( isLoading ? "Abort" : "Reload");
        progressBar_.setIndeterminate(isLoading);

        if (!isLoading && !errorMsg_.isEmpty()) {
          browser.loadString(errorMsg_, address_field_.getText());
          errorMsg_ = "";
        }
      }

      @Override
      public void onLoadError(CefBrowser browser,
                              int frameIdentifer,
                              ErrorCode errorCode,
                              String errorText,
                              String failedUrl) {
        if (errorCode != ErrorCode.ERR_NONE && errorCode != ErrorCode.ERR_ABORTED) {
          errorMsg_  = "<html><head>";
          errorMsg_ += "<title>Error while loading</title>";
          errorMsg_ += "</head><body>";
          errorMsg_ += "<h1>" + errorCode + "</h1>";
          errorMsg_ += "<h3>Failed to load " + failedUrl + "</h3>";
          errorMsg_ += "<p>" + errorText + "</p>";
          errorMsg_ += "</body></html>";
          browser.stopLoad();
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
    contentPanel.add(createBottomPanel(), BorderLayout.SOUTH);
    return contentPanel;
  }

  private JPanel createControlPanel() {
    JPanel controlPanel = new JPanel();
    controlPanel.setLayout(new BoxLayout(controlPanel, BoxLayout.X_AXIS));

    controlPanel.add(Box.createHorizontalStrut(5));
    controlPanel.add(Box.createHorizontalStrut(5));

    backButton_ = new JButton("Back");
    backButton_.setAlignmentX(LEFT_ALIGNMENT);
    backButton_.addActionListener(new ActionListener() {
      @Override
      public void actionPerformed(ActionEvent e) {
        client_.getBrowser().goBack();
      }
    });
    controlPanel.add(backButton_);
    controlPanel.add(Box.createHorizontalStrut(5));

    forwardButton_ = new JButton("Forward");
    forwardButton_.setAlignmentX(LEFT_ALIGNMENT);
    forwardButton_.addActionListener(new ActionListener() {
      @Override
      public void actionPerformed(ActionEvent e) {
        client_.getBrowser().goForward();
      }
    });
    controlPanel.add(forwardButton_);
    controlPanel.add(Box.createHorizontalStrut(5));

    reloadButton_ = new JButton("Reload");
    reloadButton_.setAlignmentX(LEFT_ALIGNMENT);
    reloadButton_.addActionListener(new ActionListener() {
      @Override
      public void actionPerformed(ActionEvent e) {
        if (reloadButton_.getText().equalsIgnoreCase("reload")) {
          int mask = CefContext.isMacintosh()
                     ? ActionEvent.META_MASK
                     : ActionEvent.CTRL_MASK;
          if ((e.getModifiers() & mask) != 0) {
            System.out.println("Reloading - ignoring cached values");
            client_.getBrowser().reloadIgnoreCache();
          } else {
            System.out.println("Reloading - using cached values");
            client_.getBrowser().reload();
          }
        } else {
          client_.getBrowser().stopLoad();
        }
      }
    });
    controlPanel.add(reloadButton_);
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

    JButton minusButton = new JButton("-");
    minusButton.setAlignmentX(CENTER_ALIGNMENT);
    minusButton.addActionListener(new ActionListener() {
      @Override
      public void actionPerformed(ActionEvent e) {
        client_.getBrowser().setZoomLevel(--zoomLevel_);
        zoom_label_.setText(new Double(zoomLevel_).toString());
      }
    });
    controlPanel.add(minusButton);

    zoom_label_ = new JLabel("0.0");
    controlPanel.add(zoom_label_);

    JButton plusButton = new JButton("+");
    plusButton.setAlignmentX(CENTER_ALIGNMENT);
    plusButton.addActionListener(new ActionListener() {
      @Override
      public void actionPerformed(ActionEvent e) {
        client_.getBrowser().setZoomLevel(++zoomLevel_);
        zoom_label_.setText(new Double(zoomLevel_).toString());
      }
    });
    controlPanel.add(plusButton);

    return controlPanel;
  }

  private JPanel createBottomPanel() {
    JPanel bottomPanel = new JPanel();
    bottomPanel.setLayout(new BoxLayout(bottomPanel, BoxLayout.X_AXIS));

    bottomPanel.add(Box.createHorizontalStrut(5));
    bottomPanel.add(Box.createHorizontalStrut(5));

    progressBar_ = new JProgressBar();
    Dimension progressBarSize = progressBar_.getMaximumSize();
    progressBarSize.width = 100;
    progressBar_.setMinimumSize(progressBarSize);
    progressBar_.setMaximumSize(progressBarSize);

    bottomPanel.add(progressBar_);
    bottomPanel.add(Box.createHorizontalStrut(5));

    status_field_ = new JLabel("Info");
    status_field_.setAlignmentX(LEFT_ALIGNMENT);
    bottomPanel.add(status_field_);
    bottomPanel.add(Box.createHorizontalStrut(5));

    return bottomPanel;
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

    JMenu testMenu = new JMenu("Tests");

    JMenuItem testJSItem = new JMenuItem("JavaScript alert");
    testJSItem.addActionListener(new ActionListener() {
      @Override
      public void actionPerformed(ActionEvent e) {
        client_.getBrowser().executeJavaScript("alert('Hello World');", address_field_.getText(), 1);
      }
    });
    testMenu.add(testJSItem);

    JMenuItem testShowText = new JMenuItem("Show Text");
    testShowText.addActionListener(new ActionListener() {
      @Override
      public void actionPerformed(ActionEvent e) {
        client_.getBrowser().loadString("<html><body><h1>Hello World</h1></body></html>", address_field_.getText());
      }
    });
    testMenu.add(testShowText);

    JMenuItem showInfo = new JMenuItem("Show Info");
    showInfo.addActionListener(new ActionListener() {
      @Override
      public void actionPerformed(ActionEvent e) {
        CefBrowser browser = client_.getBrowser();
        String info =  "<html><head><title>Browser status</title></head>";
               info += "<body><h1>Browser status</h1><table border=\"0\">";
               info += "<tr><td>CanGoBack</td><td>" + browser.canGoBack() + "</td></tr>";
               info += "<tr><td>CanGoForward</td><td>" + browser.canGoForward() + "</td></tr>";
               info += "<tr><td>IsLoading</td><td>" + browser.isLoading() + "</td></tr>";
               info += "<tr><td>isPopup</td><td>" + browser.isPopup() + "</td></tr>";
               info += "<tr><td>hasDocument</td><td>" + browser.hasDocument() + "</td></tr>";
               info += "<tr><td>Url</td><td>" + browser.getURL() + "</td></tr>";
               info += "<tr><td>Zoom-Level</td><td>" + browser.getZoomLevel() + "</td></tr>";
               info += "</table></body></html>";
        String js = "var x=window.open(); x.document.open(); x.document.write('" + info + "'); x.document.close();";
        browser.executeJavaScript(js, "", 0);
      }
    });
    testMenu.add(showInfo);

    JMenuItem showDevTools = new JMenuItem("Show DevTools");
    showDevTools.addActionListener(new ActionListener() {
      @Override
      public void actionPerformed(ActionEvent e) {
        client_.getBrowser().showDevTools(client_);
      }
    });
    testMenu.add(showDevTools);

    JMenuItem closeDevTools = new JMenuItem("Close DevTools");
    closeDevTools.addActionListener(new ActionListener() {
      @Override
      public void actionPerformed(ActionEvent e) {
        client_.getBrowser().closeDevTools();
      }
    });
    testMenu.add(closeDevTools);

    menuBar.add(fileMenu);
    menuBar.add(bookmarkMenu_);
    menuBar.add(testMenu);

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
  public boolean onTooltip(CefBrowser browser, String text) {
    return false;
  }

  @Override
  public void onStatusMessage(CefBrowser browser, String value) {
    status_field_.setText(value);
  }

  @Override
  public boolean onConsoleMessage(CefBrowser browser,
                                  String message,
                                  String source,
                                  int line) {
    return false;
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
