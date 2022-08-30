// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package tests.detailed.dialog;

import org.cef.callback.CefCookieVisitor;
import org.cef.misc.BoolRef;
import org.cef.network.CefCookie;
import org.cef.network.CefCookieManager;

import java.awt.BorderLayout;
import java.awt.Frame;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.Date;
import java.util.Vector;

import javax.swing.BoxLayout;
import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTable;
import javax.swing.table.AbstractTableModel;

@SuppressWarnings("serial")
public class CookieManagerDialog extends JDialog {
    private static int testCookieId = 1;
    private final CefCookieManager manager;
    private final CookieTableModel tblModel = new CookieTableModel();

    public CookieManagerDialog(Frame owner, String title, CefCookieManager cookieManager) {
        super(owner, title, false);
        setLayout(new BorderLayout());
        setSize(800, 600);
        manager = cookieManager;

        JTable cookieTable = new JTable(tblModel);
        cookieTable.setFillsViewportHeight(true);

        JPanel controlPanel = new JPanel();
        controlPanel.setLayout(new BoxLayout(controlPanel, BoxLayout.X_AXIS));
        JButton delButton = new JButton("Delete cookies");
        delButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                tblModel.removeCookies();
            }
        });
        controlPanel.add(delButton);

        JButton testCreateCookie = new JButton("Add test cookie");
        testCreateCookie.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                Date now = new Date();
                Date expires = new Date(now.getTime() + 86400000);
                String name = "testNo" + testCookieId++;
                CefCookie cookie = new CefCookie(name, "testCookie", ".test.cookie", "/", false,
                        true, now, now, true, expires);
                if (manager.setCookie("http://my.test.cookie", cookie)) {
                    tblModel.visit(cookie, 1, 1, new BoolRef());
                }
            }
        });
        controlPanel.add(testCreateCookie);

        JButton doneButton = new JButton("Done");
        doneButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                setVisible(false);
            }
        });
        controlPanel.add(doneButton);

        add(new JScrollPane(cookieTable));
        add(controlPanel, BorderLayout.SOUTH);

        if (manager == null) throw new NullPointerException("Cookie manager is null");
        manager.visitAllCookies(tblModel);
    }

    private class CookieTableModel extends AbstractTableModel implements CefCookieVisitor {
        private final String[] columnNames;
        private Vector<Object[]> rowData = new Vector<>();

        public CookieTableModel() {
            super();
            columnNames = new String[] {"Name", "Value", "Domain", "Path", "Secure", "HTTP only",
                    "Created", "Last Access", "Expires"};
        }

        // add an entry to the table
        @Override
        public boolean visit(CefCookie cookie, int count, int total, BoolRef delete) {
            Object[] entry = {cookie.name, cookie.value, cookie.domain, cookie.path,
                    new Boolean(cookie.secure), new Boolean(cookie.httponly), cookie.creation,
                    cookie.lastAccess, cookie.expires};
            int row = rowData.size();
            rowData.addElement(entry);
            fireTableRowsInserted(row, row);

            return true;
        }

        public void removeCookies() {
            int cnt = rowData.size();
            if (cnt > 0) {
                rowData.clear();
                manager.deleteCookies("", "");
                fireTableRowsDeleted(0, cnt - 1);
            }
        }

        @Override
        public int getRowCount() {
            return rowData.size();
        }

        @Override
        public int getColumnCount() {
            return columnNames.length;
        }

        @Override
        public String getColumnName(int column) {
            return columnNames[column];
        }

        @Override
        public Class<?> getColumnClass(int columnIndex) {
            if (rowData.size() > 0) return rowData.get(0)[columnIndex].getClass();
            return Object.class;
        }

        @Override
        public boolean isCellEditable(int rowIndex, int columnIndex) {
            return false;
        }

        @Override
        public Object getValueAt(int rowIndex, int columnIndex) {
            return rowData.get(rowIndex)[columnIndex];
        }
    }
}
