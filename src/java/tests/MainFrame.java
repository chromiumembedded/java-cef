// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package tests;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Component;
import java.awt.Dimension;
import java.awt.GridLayout;
import java.awt.Frame;
import java.awt.KeyboardFocusManager;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.PrintWriter;
import java.io.UnsupportedEncodingException;
import java.util.HashMap;
import java.util.Map;
import java.util.Vector;

import javax.swing.Box;
import javax.swing.BoxLayout;
import javax.swing.JButton;
import javax.swing.JCheckBox;
import javax.swing.JDialog;
import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JPasswordField;
import javax.swing.JProgressBar;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.JTextField;
import javax.swing.SwingUtilities;
import javax.swing.Timer;

import org.cef.CefClient;
import org.cef.CefApp;
import org.cef.OS;
import org.cef.browser.CefBrowser;
import org.cef.callback.CefAllowCertificateErrorCallback;
import org.cef.callback.CefAuthCallback;
import org.cef.callback.CefBeforeDownloadCallback;
import org.cef.callback.CefDownloadItem;
import org.cef.callback.CefDownloadItemCallback;
import org.cef.callback.CefContextMenuParams;
import org.cef.callback.CefDragData;
import org.cef.callback.CefGeolocationCallback;
import org.cef.callback.CefJSDialogCallback;
import org.cef.callback.CefMenuModel;
import org.cef.callback.CefQueryCallback;
import org.cef.callback.CefRunFileDialogCallback;
import org.cef.callback.CefStringVisitor;
import org.cef.callback.CefMenuModel.MenuId;
import org.cef.handler.CefContextMenuHandlerAdapter;
import org.cef.handler.CefDialogHandler.FileDialogMode;
import org.cef.handler.CefDisplayHandler;
import org.cef.handler.CefDownloadHandler;
import org.cef.handler.CefDragHandler;
import org.cef.handler.CefGeolocationHandlerAdapter;
import org.cef.handler.CefJSDialogHandlerAdapter;
import org.cef.handler.CefKeyboardHandlerAdapter;
import org.cef.handler.CefRequestHandlerAdapter;
import org.cef.handler.CefLoadHandler.ErrorCode;
import org.cef.handler.CefLoadHandlerAdapter;
import org.cef.handler.CefMessageRouterHandler;
import org.cef.misc.BoolRef;

public class MainFrame extends JFrame implements CefDisplayHandler, CefMessageRouterHandler {
  private static final long serialVersionUID = -2295538706810864538L;
  public static void main(String [] args) {
    // OSR mode is enabled by default on Linux.
    boolean osrEnabledArg = true;
    if (OS.isWindows() || OS.isMacintosh()) {
      // OSR mode is disabled by default on Windows and Mac OS X.
      osrEnabledArg = false;
      for (String arg : args) {
        if (arg.toLowerCase().equals("--off-screen-rendering-enabled")) {
          osrEnabledArg = true;
          break;
        }
      }
    }

    System.out.println("Offscreen rendering " + (osrEnabledArg ? "enabled" : "disabled"));

    frame_ = new MainFrame(osrEnabledArg, args); 
    frame_.addWindowListener(new WindowAdapter() {
      @Override
      public void windowClosing(WindowEvent e) {
        frame_.dispose();
        CefApp.getInstance().dispose();

        new Timer(3000, new ActionListener() {
          @Override
          public void actionPerformed(ActionEvent e) {
            System.exit(0);
          }
        }).start();
      }
    });

    frame_.setSize(800, 600);
    frame_.setVisible(true);
  }

  private static MainFrame frame_;
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
  private CefBrowser browser_;
  private DownloadDialog downloadDialog_;

  public MainFrame(boolean osrEnabled, String [] args) {
    client_ = CefApp.getInstance(args).createClient();
    client_.addDisplayHandler(this);
    client_.addMessageRouterHandler(this);
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
    downloadDialog_ = new DownloadDialog(this);
    client_.addDownloadHandler(downloadDialog_);
    client_.addContextMenuHandler(new CefContextMenuHandlerAdapter() {
      @Override
      public void onBeforeContextMenu(CefBrowser browser,
                                      CefContextMenuParams params,
                                      CefMenuModel model) {
        model.addItem(CefMenuModel.MenuId.MENU_ID_FIND, "Find...");
        if (params.hasImageContents())
          model.addItem(CefMenuModel.MenuId.MENU_ID_USER_FIRST, "Download Image...");
      }
      @Override
      public boolean onContextMenuCommand(CefBrowser browser,
                                          CefContextMenuParams params,
                                          int commandId,
                                          int eventFlags) {
        switch (commandId) {
          case MenuId.MENU_ID_VIEW_SOURCE:
            ShowText visitor = new ShowText("Source of \"" + address_field_.getText() + "\"");
            browser_.getSource(visitor);
            return true;
          case MenuId.MENU_ID_FIND:
            SearchDialog search = new SearchDialog();
            search.setVisible(true);
            return true;
          case MenuId.MENU_ID_USER_FIRST:
            browser_.startDownload(params.getSourceUrl());
            return true;
          default:
            return false;
        }
      }
    });
    client_.addDragHandler(new CefDragHandler() {
      @Override
      public boolean onDragEnter(CefBrowser browser, CefDragData dragData, int mask) {
        System.out.println("DRAG:");
        System.out.print("  flags:");
        if ((mask & CefDragHandler.DragOperationMask.DRAG_OPERATION_COPY) != 0)
          System.out.print(" COPY");
        if ((mask & CefDragHandler.DragOperationMask.DRAG_OPERATION_LINK) != 0)
          System.out.print(" LINK");
        if ((mask & CefDragHandler.DragOperationMask.DRAG_OPERATION_GENERIC) != 0)
          System.out.print(" GENERIC");
        if ((mask & CefDragHandler.DragOperationMask.DRAG_OPERATION_PRIVATE) != 0)
          System.out.print(" PRIVATE");
        if ((mask & CefDragHandler.DragOperationMask.DRAG_OPERATION_MOVE) != 0)
          System.out.print(" MOVE");
        if ((mask & CefDragHandler.DragOperationMask.DRAG_OPERATION_DELETE) != 0)
          System.out.print(" DELETE");
        System.out.println("\n  " + dragData);
        return false;
      }
    });
    client_.addGeolocationHandler(new CefGeolocationHandlerAdapter() {
      @Override
      public void onRequestGeolocationPermission(CefBrowser browser,
                                                 String requesting_url,
                                                 int request_id,
                                                 CefGeolocationCallback callback) {
        final CefGeolocationCallback cb = callback;
        final String reqUrl = requesting_url;
        SwingUtilities.invokeLater(new Runnable() {
          @Override
          public void run() {
            int dialogResult = JOptionPane.showConfirmDialog(frame_,
                "The URL \n" + reqUrl +
                "\nwants to request your geolocation." +
                "\nDo you want to proceed?",
                "Geolocation requested",
                JOptionPane.YES_NO_OPTION,
                JOptionPane.QUESTION_MESSAGE);
            cb.Continue(dialogResult == JOptionPane.YES_OPTION);
          }
        });
      }
    });
    client_.addJSDialogHandler(new CefJSDialogHandlerAdapter() {
      @Override
      public boolean onJSDialog(CefBrowser browser,
                                String origin_url,
                                String accept_lang,
                                JSDialogType dialog_type,
                                String message_text,
                                String default_prompt_text,
                                CefJSDialogCallback callback,
                                BoolRef suppress_message) {
        if (message_text.equalsIgnoreCase("Never displayed")) {
          suppress_message.set(true);
          System.out.println("The " + dialog_type + " from origin \"" + origin_url + "\" was suppressed.");
          System.out.println("   The content of the suppressed dialog was: \"" + message_text + "\"");
        }
        return false;
      }
    });
    client_.addKeyboardHandler(new CefKeyboardHandlerAdapter() {
      @Override
      public boolean onKeyEvent(CefBrowser browser, CefKeyEvent event) {
        if (!event.focus_on_editable_field && event.windows_key_code == 0x20) {
          // Special handling for the space character when an input element does not
          // have focus. Handling the event in OnPreKeyEvent() keeps the event from
          // being processed in the renderer. If we instead handled the event in the
          // OnKeyEvent() method the space key would cause the window to scroll in
          // addition to showing the alert box.
          if (event.type == CefKeyEvent.EventType.KEYEVENT_RAWKEYDOWN) {
            browser_.executeJavaScript("alert('You pressed the space bar!');", "", 0);
          }
          return true;
        }
        return false;
      }
    });
    client_.addRequestHandler(new CefRequestHandlerAdapter() {
      @Override
      public boolean getAuthCredentials(CefBrowser browser,
                                        boolean isProxy,
                                        String host,
                                        int port,
                                        String realm,
                                        String scheme,
                                        CefAuthCallback callback) {
        SwingUtilities.invokeLater(new PasswordDialog(callback));
        return true;
      }
      @Override
      public boolean onCertificateError(ErrorCode cert_error,
                                        String request_url,
                                        CefAllowCertificateErrorCallback callback) {
        SwingUtilities.invokeLater(new CertError(cert_error, request_url, callback));
        return true;
      }
    });

    browser_ = client_.createBrowser("http://www.google.com", osrEnabled, false);
    getContentPane().add(createContentPanel(), BorderLayout.CENTER);

    JMenuBar menuBar = createMenuBar();

    // Binding Test resource is cefclient/res/binding.html from the CEF binary distribution.
    addBookmark("Binding Test", "http://www.magpcss.org/pub/jcef_binding_1750.html");
    addBookmark("Download Test", "http://cefbuilds.com");
    addBookmark("Geolocation Test","http://slides.html5rocks.com/#geolocation");
    addBookmark("Login Test (username:pumpkin, password:pie)","http://www.colostate.edu/~ric/protect/your.html");
    addBookmark("Certificate-error Test","https://www.k2go.de");
    bookmarkMenu_.addSeparator();
    addBookmark("javachromiumembedded", "https://code.google.com/p/javachromiumembedded/");
    addBookmark("chromiumembedded", "https://code.google.com/p/chromiumembedded/");
    setJMenuBar(menuBar);
  }

  @SuppressWarnings("serial")
  private class PasswordDialog extends JDialog implements Runnable {
    private JTextField username_ = new JTextField(20);
    private JPasswordField password_ = new JPasswordField(20);
    private CefAuthCallback callback_;

    public PasswordDialog(CefAuthCallback callback) {
      super(frame_, "Authentication required", true);
      callback_ = callback;
      setSize(400,100);
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
          if (username_.getText().isEmpty())
            return;
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

  private class CertError implements Runnable {
    private ErrorCode cert_error_;
    private String request_url_;
    private CefAllowCertificateErrorCallback callback_;

    CertError(ErrorCode cert_error, String request_url,CefAllowCertificateErrorCallback callback) {
      cert_error_ = cert_error;
      request_url_ = request_url;
      callback_ = callback;
    }

    @Override
    public void run() {
      int dialogResult = JOptionPane.showConfirmDialog(frame_,
              "An certificate error (" +
              cert_error_ + ") occurreed " +
              "while requesting\n" + request_url_ +
              "\nDo you want to proceed anyway?",
              "Certificate error",
              JOptionPane.YES_NO_OPTION,
              JOptionPane.ERROR_MESSAGE);
      callback_.Continue(dialogResult == JOptionPane.YES_OPTION);
    }
  }

  @SuppressWarnings("serial")
  private class DownloadDialog extends JDialog implements CefDownloadHandler {
    private Map<Integer,DownloadObject> downloadObjects_ = new HashMap<Integer, DownloadObject>();
    private JPanel downloadPanel_ = new JPanel();
    private DownloadDialog dialog_ = null;

    public DownloadDialog(Frame owner) {
      super(owner, "Downloads", false);
      setVisible(false);
      setSize(400,300);

      dialog_ = this;
      downloadPanel_.setLayout(new BoxLayout(downloadPanel_, BoxLayout.Y_AXIS));
      add(downloadPanel_);
    }

    private class DownloadObject extends JPanel {
      private boolean isHidden_ = true;
      private final int identifier_;
      private JLabel fileName_ = new JLabel();
      private JLabel status_ = new JLabel();
      private JButton dlAbort_ = new JButton();
      private JButton dlRemoveEntry_ = new JButton("x");
      private CefDownloadItemCallback callback_;
      private Color bgColor_;

      DownloadObject(CefDownloadItem downloadItem, String suggestedName) {
        super();
        setOpaque(true);
        setLayout(new BorderLayout());
        setMaximumSize(new Dimension(dialog_.getWidth()-10, 80));
        identifier_ = downloadItem.getId();
        bgColor_ = identifier_%2 == 0 ? Color.WHITE : Color.YELLOW;
        setBackground(bgColor_); 

        fileName_.setText(suggestedName);
        add(fileName_, BorderLayout.NORTH);

        status_.setAlignmentX(LEFT_ALIGNMENT);
        add(status_, BorderLayout.CENTER);

        JPanel controlPane = new JPanel();
        controlPane.setLayout(new BoxLayout(controlPane, BoxLayout.X_AXIS));
        controlPane.setOpaque(true);
        controlPane.setBackground(bgColor_);
        dlAbort_.setText("Abort");
        dlAbort_.setEnabled(false);
        dlAbort_.addActionListener(new ActionListener() {
          @Override
          public void actionPerformed(ActionEvent e) {
            if (callback_ != null) {
              fileName_.setText("ABORTED - " + fileName_.getText());
              callback_.cancel();
            }
          }
        });
        controlPane.add(dlAbort_);

        dlRemoveEntry_.setEnabled(false);
        dlRemoveEntry_.addActionListener(new ActionListener() {
          @Override
          public void actionPerformed(ActionEvent e) {
            DownloadObject removed = downloadObjects_.remove(identifier_);
            if (removed != null) {
              downloadPanel_.remove(removed);
              dialog_.repaint();
            }
          }
        });
        controlPane.add(dlRemoveEntry_);
        add(controlPane, BorderLayout.SOUTH);

        update(downloadItem, null);
      }

      // The method humanReadableByteCount() is based on
      // http://stackoverflow.com/questions/3758606/how-to-convert-byte-size-into-human-readable-format-in-java
      String humanReadableByteCount(long bytes) {
        int unit = 1024;
        if (bytes < unit)
          return bytes + " B";

        int exp = (int) (Math.log(bytes) / Math.log(unit));
        String pre = "" + ("kMGTPE").charAt(exp-1);
        return String.format("%.1f %sB", bytes / Math.pow(unit, exp), pre);
      }

      void update(CefDownloadItem downloadItem, CefDownloadItemCallback callback) {
        int percentComplete = downloadItem.getPercentComplete();
        String rcvBytes = humanReadableByteCount(downloadItem.getReceivedBytes());
        String totalBytes = humanReadableByteCount(downloadItem.getTotalBytes());
        String speed = humanReadableByteCount(downloadItem.getCurrentSpeed()) + "it/s";

        if (downloadItem.getReceivedBytes() >= 5 && isHidden_) {
          downloadDialog_.setVisible(true);
          downloadDialog_.toFront();
          isHidden_ = false;
        }
        Runtime.getRuntime().runFinalization();

        callback_ = callback;
        status_.setText(rcvBytes + " of " + totalBytes + " - " + percentComplete + "%" + " - " + speed);
        dlAbort_.setEnabled(downloadItem.isInProgress());
        dlRemoveEntry_.setEnabled(!downloadItem.isInProgress() || downloadItem.isCanceled() || downloadItem.isComplete());
        if (!downloadItem.isInProgress() && !downloadItem.isCanceled() && !downloadItem.isComplete()) {
          fileName_.setText("FAILED - " + fileName_.getText());
          callback.cancel();
        }
      }
    }

    @Override
    public void onBeforeDownload(CefBrowser browser,
                                 CefDownloadItem downloadItem,
                                 String suggestedName,
                                 CefBeforeDownloadCallback callback) {
      callback.Continue(suggestedName, true);

      DownloadObject dlObject = new DownloadObject(downloadItem, suggestedName);
      downloadObjects_.put(downloadItem.getId(), dlObject);
      downloadPanel_.add(dlObject);
    }

    @Override
    public void onDownloadUpdated(CefBrowser browser,
                                  CefDownloadItem downloadItem,
                                  CefDownloadItemCallback callback) {
      DownloadObject dlObject = downloadObjects_.get(downloadItem.getId());
      if (dlObject == null)
        return;
      dlObject.update(downloadItem, callback);
    }
  }

  private JPanel createContentPanel() {
    JPanel contentPanel = new JPanel(new BorderLayout());
    contentPanel.add(createControlPanel(), BorderLayout.NORTH);
    contentPanel.add(browser_.getUIComponent(), BorderLayout.CENTER);
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
        browser_.goBack();
      }
    });
    controlPanel.add(backButton_);
    controlPanel.add(Box.createHorizontalStrut(5));

    forwardButton_ = new JButton("Forward");
    forwardButton_.setAlignmentX(LEFT_ALIGNMENT);
    forwardButton_.addActionListener(new ActionListener() {
      @Override
      public void actionPerformed(ActionEvent e) {
        browser_.goForward();
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
          int mask = OS.isMacintosh()
                     ? ActionEvent.META_MASK
                     : ActionEvent.CTRL_MASK;
          if ((e.getModifiers() & mask) != 0) {
            System.out.println("Reloading - ignoring cached values");
            browser_.reloadIgnoreCache();
          } else {
            System.out.println("Reloading - using cached values");
            browser_.reload();
          }
        } else {
          browser_.stopLoad();
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
        browser_.loadURL(address_field_.getText());
      }
    });
    address_field_.addMouseListener(new MouseAdapter() {
      @Override
      public void mouseClicked(MouseEvent arg0) {
        KeyboardFocusManager.getCurrentKeyboardFocusManager()
            .clearGlobalFocusOwner();
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
        browser_.loadURL(address_field_.getText());
      }
    });
    controlPanel.add(goButton);
    controlPanel.add(Box.createHorizontalStrut(5));

    JButton minusButton = new JButton("-");
    minusButton.setAlignmentX(CENTER_ALIGNMENT);
    minusButton.addActionListener(new ActionListener() {
      @Override
      public void actionPerformed(ActionEvent e) {
        browser_.setZoomLevel(--zoomLevel_);
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
        browser_.setZoomLevel(++zoomLevel_);
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
    bottomPanel.add(Box.createVerticalStrut(21));

    return bottomPanel;
  }

  @SuppressWarnings("serial")
  private class SearchDialog extends JDialog {
    private JTextField searchField_  = new JTextField(30);
    private JCheckBox caseCheckBox_ = new JCheckBox("Case sensitive");
    private int identifier_ = (int)Math.random();
    private JButton prevButton_ = new JButton("Prev");
    private JButton nextButton_ = new JButton("Next");

    public SearchDialog() {
      super(frame_, "Find...", false);

      setLayout(new BorderLayout());
      setSize(400, 100);

      JPanel searchPanel = new JPanel();
      searchPanel.setLayout(new BoxLayout(searchPanel, BoxLayout.X_AXIS));
      searchPanel.add(Box.createHorizontalStrut(5));
      searchPanel.add(new JLabel("Search:"));
      searchPanel.add(searchField_);

      JPanel controlPanel = new JPanel();
      controlPanel.setLayout(new BoxLayout(controlPanel, BoxLayout.X_AXIS));
      controlPanel.add(Box.createHorizontalStrut(5));

      JButton searchButton = new JButton("Search");
      searchButton.addActionListener(new ActionListener() {
        @Override
        public void actionPerformed(ActionEvent e) {
          if (searchField_.getText() == null || searchField_.getText().isEmpty())
            return;

          setTitle("Find \"" + searchField_.getText() + "\"");
          boolean matchCase = caseCheckBox_.isSelected();
          browser_.find(identifier_, searchField_.getText(), true, matchCase, false);
          prevButton_.setEnabled(true);
          nextButton_.setEnabled(true);
        }
      });
      controlPanel.add(searchButton);
  
      prevButton_.addActionListener(new ActionListener() {
        @Override
        public void actionPerformed(ActionEvent e) {
          boolean matchCase = caseCheckBox_.isSelected();
          setTitle("Find \"" + searchField_.getText() + "\"");
          browser_.find(identifier_, searchField_.getText(), false, matchCase, true);
        }
      });
      prevButton_.setEnabled(false);
      controlPanel.add(prevButton_);

      nextButton_.addActionListener(new ActionListener() {
        @Override
        public void actionPerformed(ActionEvent e) {
          boolean matchCase = caseCheckBox_.isSelected();
          setTitle("Find \"" + searchField_.getText() + "\"");
          browser_.find(identifier_, searchField_.getText(), true, matchCase, true);
        }
      });
      nextButton_.setEnabled(false);
      controlPanel.add(nextButton_);

      controlPanel.add(Box.createHorizontalStrut(50));

      JButton doneButton = new JButton("Done");
      doneButton.addActionListener(new ActionListener() {
        @Override
        public void actionPerformed(ActionEvent e) {
          setVisible(false);
          dispose();
        }
      });
      controlPanel.add(doneButton);

      add(searchPanel, BorderLayout.NORTH);
      add(caseCheckBox_);
      add(controlPanel,BorderLayout.SOUTH);
    }

    @Override
    public void dispose() {
      browser_.stopFinding(true);
      super.dispose();
    }
  }

  @SuppressWarnings("serial")
  private class ShowText extends JDialog implements CefStringVisitor {
    private JTextArea textArea_ = new JTextArea();

    public ShowText(String title) {
      super(frame_, title, false);
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

  private class SaveAs implements CefStringVisitor {
    private PrintWriter fileWriter_;

    public SaveAs(String fName) throws FileNotFoundException, UnsupportedEncodingException {
      fileWriter_ = new PrintWriter(fName, "UTF-8");
    }

    @Override
    public void visit(String string) {
      fileWriter_.write(string);
    }
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
          browser_.loadURL("file:///" + selectedFile.getAbsolutePath());
        }
      }
    });
    fileMenu.add(openFileItem);

    JMenuItem openFileDialog = new JMenuItem("Save as...");
    openFileDialog.addActionListener(new ActionListener() {
      @Override
      public void actionPerformed(ActionEvent e) {
        CefRunFileDialogCallback callback = new CefRunFileDialogCallback() {
          @Override
          public void onFileDialogDismissed(CefBrowser browser, Vector<String> filePaths) {
            if (!filePaths.isEmpty()) {
              try {
                SaveAs saveContent = new SaveAs(filePaths.get(0));
                browser_.getSource(saveContent);
              } catch (FileNotFoundException | UnsupportedEncodingException e) {
                browser_.executeJavaScript("alert(\"Can't save file\");", address_field_.getText(), 0);
              }
            }
          }
        };
        browser_.runFileDialog(FileDialogMode.FILE_DIALOG_SAVE,
                               getTitle(),
                               "index.html",
                               null,
                               callback);
      }
    });
    fileMenu.add(openFileDialog);

    JMenuItem printItem = new JMenuItem("Print...");
    printItem.addActionListener(new ActionListener() {
      @Override
      public void actionPerformed(ActionEvent e) {
        browser_.print();
      }
    });
    fileMenu.add(printItem);

    JMenuItem searchItem = new JMenuItem("Search...");
    searchItem.addActionListener(new ActionListener() {
      @Override
      public void actionPerformed(ActionEvent e) {
        new SearchDialog().setVisible(true);
      }
    });
    fileMenu.add(searchItem);

    fileMenu.addSeparator();

    JMenuItem viewSource = new JMenuItem("View source");
    viewSource.addActionListener(new ActionListener() {
      @Override
      public void actionPerformed(ActionEvent e) {
        browser_.viewSource();
      }
    });
    fileMenu.add(viewSource);

    JMenuItem getSource = new JMenuItem("Get source...");
    getSource.addActionListener(new ActionListener() {
      @Override
      public void actionPerformed(ActionEvent e) {
        ShowText visitor = new ShowText("Source of \"" + address_field_.getText() + "\"");
        browser_.getSource(visitor);
      }
    });
    fileMenu.add(getSource);

    JMenuItem getText = new JMenuItem("Get text...");
    getText.addActionListener(new ActionListener() {
      @Override
      public void actionPerformed(ActionEvent e) {
        ShowText visitor = new ShowText("Content of \"" + address_field_.getText() + "\"");
        browser_.getText(visitor);
      }
    });
    fileMenu.add(getText);

    fileMenu.addSeparator();

    JMenuItem showDownloads = new JMenuItem("Show Downloads");
    showDownloads.addActionListener(new ActionListener() {
      @Override
      public void actionPerformed(ActionEvent e) {
        downloadDialog_.setVisible(true);
      }
    });
    fileMenu.add(showDownloads);

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
        browser_.executeJavaScript("alert('Hello World');", address_field_.getText(), 1);
      }
    });
    testMenu.add(testJSItem);

    JMenuItem jsAlertItem = new JMenuItem("JavaScript alert (will be suppressed)");
    jsAlertItem.addActionListener(new ActionListener() {
      @Override
      public void actionPerformed(ActionEvent e) {
        browser_.executeJavaScript("alert('Never displayed');", "http://dontshow.me", 1);
      }
    });
    testMenu.add(jsAlertItem);

    JMenuItem testShowText = new JMenuItem("Show Text");
    testShowText.addActionListener(new ActionListener() {
      @Override
      public void actionPerformed(ActionEvent e) {
        browser_.loadString("<html><body><h1>Hello World</h1></body></html>", address_field_.getText());
      }
    });
    testMenu.add(testShowText);

    JMenuItem showInfo = new JMenuItem("Show Info");
    showInfo.addActionListener(new ActionListener() {
      @Override
      public void actionPerformed(ActionEvent e) {
        String info =  "<html><head><title>Browser status</title></head>";
               info += "<body><h1>Browser status</h1><table border=\"0\">";
               info += "<tr><td>CanGoBack</td><td>" + browser_.canGoBack() + "</td></tr>";
               info += "<tr><td>CanGoForward</td><td>" + browser_.canGoForward() + "</td></tr>";
               info += "<tr><td>IsLoading</td><td>" + browser_.isLoading() + "</td></tr>";
               info += "<tr><td>isPopup</td><td>" + browser_.isPopup() + "</td></tr>";
               info += "<tr><td>hasDocument</td><td>" + browser_.hasDocument() + "</td></tr>";
               info += "<tr><td>Url</td><td>" + browser_.getURL() + "</td></tr>";
               info += "<tr><td>Zoom-Level</td><td>" + browser_.getZoomLevel() + "</td></tr>";
               info += "</table></body></html>";
        String js = "var x=window.open(); x.document.open(); x.document.write('" + info + "'); x.document.close();";
        browser_.executeJavaScript(js, "", 0);
      }
    });
    testMenu.add(showInfo);

    JMenuItem showDevTools = new JMenuItem("Show DevTools");
    showDevTools.addActionListener(new ActionListener() {
      @Override
      public void actionPerformed(ActionEvent e) {
        browser_.showDevTools(client_);
      }
    });
    testMenu.add(showDevTools);

    JMenuItem closeDevTools = new JMenuItem("Close DevTools");
    closeDevTools.addActionListener(new ActionListener() {
      @Override
      public void actionPerformed(ActionEvent e) {
        browser_.closeDevTools();
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
        browser_.loadURL(e.getActionCommand());
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
