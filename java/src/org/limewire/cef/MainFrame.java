// Copyright (c) 2010 Lime Wire, LLC. All rights reserved. Use of this source
// code is governed by a BSD-style license that can be found in the LICENSE file.

package org.limewire.cef;

import java.awt.BorderLayout;
import java.awt.Menu;
import java.awt.MenuBar;
import java.awt.MenuItem;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;

import javax.swing.Box;
import javax.swing.BoxLayout;
import javax.swing.JButton;
import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextField;

public class MainFrame extends JFrame {

	private CefBrowser browserComponent;
	private JTextField addressTextField;
	private String lastSelectedFile = "";
	
	public MainFrame() {
		// Initialize the CEF context.
		CefContext.initialize(null);
		
		setDefaultCloseOperation(EXIT_ON_CLOSE);
		setContentPane(createContentPanel());
		setMenuBar(createMenuBar());	
	}
	
	protected void finalize() throws Throwable
    {
    	// Shut down the CEF context.
		CefContext.shutdown();
    	super.finalize();
    }

	private MenuBar createMenuBar() {
	    MenuBar menuBar = new MenuBar();
	    
	    Menu fileMenu = new Menu("File");
	    
	    MenuItem openFile = new MenuItem("Open file...");
	    openFile.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent arg0) {
                JFileChooser fc = new JFileChooser(new File(lastSelectedFile));
                // Show open dialog; this method does not return until the dialog is closed 
                fc.showOpenDialog(MainFrame.this); 
                File selectedFile = fc.getSelectedFile(); 
                lastSelectedFile = selectedFile.getAbsolutePath();
                System.out.println("loading" + "file:///" + selectedFile.getAbsolutePath());
                browserComponent.getMainFrame().loadURL("file:///" + selectedFile.getAbsolutePath());
            }
	    });
	    fileMenu.add(openFile);
	    
	    fileMenu.addSeparator();

	    MenuItem testCallJS = new MenuItem("Test calling from Java to Javascript");
        testCallJS.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                browserComponent.getMainFrame().executeJavaScript(
                    "alert(window.test.showMessage('And it works!'));", "", 0);
            }
        });
        fileMenu.add(testCallJS);

	    menuBar.add(fileMenu);
	    
	    return menuBar;
	}
	
	private JPanel createContentPanel() {
		JPanel contentPanel = new JPanel(new BorderLayout());
		
		contentPanel.add(createControlPanel(), BorderLayout.NORTH);

		// Create a new CEF browser window.
		browserComponent = CefContext.createBrowser(new MainHandler(this),
				"http://www.google.com");
		contentPanel.add(browserComponent, BorderLayout.CENTER);
		
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
				browserComponent.goBack();
			}
		});
		controlPanel.add(backButton);
		
		controlPanel.add(Box.createHorizontalStrut(5));
		
		JButton forwardButton = new JButton("Forward");
		forwardButton.setAlignmentX(LEFT_ALIGNMENT);
		forwardButton.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				browserComponent.goForward();
			}
		});
		controlPanel.add(forwardButton);

		controlPanel.add(Box.createHorizontalStrut(5));

		JLabel addressLabel = new JLabel("Address:");
		addressLabel.setAlignmentX(LEFT_ALIGNMENT);
		controlPanel.add(addressLabel);

        controlPanel.add(Box.createHorizontalStrut(5));

		addressTextField = new JTextField(100);
		addressTextField.setText("http://www.google.com");
		addressTextField.setAlignmentX(LEFT_ALIGNMENT);
		addressTextField.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				browserComponent.getMainFrame().loadURL(addressTextField.getText());
			}
		});
		controlPanel.add(addressTextField);

        controlPanel.add(Box.createHorizontalStrut(5));

        JButton goButton = new JButton("Go");
		goButton.setAlignmentX(LEFT_ALIGNMENT);
		goButton.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				browserComponent.getMainFrame().loadURL(addressTextField.getText());
			}
		});
		controlPanel.add(goButton);

        controlPanel.add(Box.createHorizontalStrut(5));

		return controlPanel;
	}
	
	public void setAddress(String url)
	{
		addressTextField.setText(url);
	}
}
