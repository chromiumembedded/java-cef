// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package tests.detailed.dialog;

import java.awt.BorderLayout;
import java.awt.Frame;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.Vector;

import javax.swing.BoxLayout;
import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTable;
import javax.swing.table.AbstractTableModel;

import org.cef.callback.CefWebPluginInfoVisitor;
import org.cef.network.CefWebPluginInfo;
import org.cef.network.CefWebPluginManager;

@SuppressWarnings("serial")
public class WebPluginManagerDialog extends JDialog {
    private final CefWebPluginManager manager = CefWebPluginManager.getGlobalManager();
    private final PluginTableModel tblModel = new PluginTableModel();

    public WebPluginManagerDialog(Frame owner, String title) {
        super(owner, title, false);
        setLayout(new BorderLayout());
        setSize(800, 600);

        JTable pluginTable = new JTable(tblModel);
        pluginTable.setFillsViewportHeight(true);

        JPanel controlPanel = new JPanel();
        controlPanel.setLayout(new BoxLayout(controlPanel, BoxLayout.X_AXIS));
        JButton delButton = new JButton("Remove selected plugins");
        delButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                tblModel.removeSelected();
            }
        });
        controlPanel.add(delButton);

        JButton doneButton = new JButton("Done");
        doneButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                setVisible(false);
            }
        });
        controlPanel.add(doneButton);

        add(new JScrollPane(pluginTable));
        add(controlPanel, BorderLayout.SOUTH);

        if (manager == null) throw new NullPointerException("Plugin manager is null");
        manager.visitPlugins(tblModel);
    }

    private class PluginTableModel extends AbstractTableModel implements CefWebPluginInfoVisitor {
        private final String[] columnNames;
        private Vector<Object[]> rowData = new Vector<>();

        public PluginTableModel() {
            super();
            columnNames = new String[] {"Name", "Path", "Version", "Description", ""};
        }

        // add an entry to the table
        @Override
        public boolean visit(CefWebPluginInfo info, int count, int total) {
            Object[] entry = {info.getName(), info.getPath(), info.getVersion(),
                    info.getDescription(), new Boolean(false)};
            int row = rowData.size();
            rowData.addElement(entry);
            fireTableRowsInserted(row, row);

            return true;
        }

        public void removeSelected() {
            for (int i = 0; i < rowData.size(); ++i) {
                if ((Boolean) rowData.get(i)[4]) {
                    String path = (String) rowData.get(i)[1];
                    rowData.remove(i);
                    fireTableRowsDeleted(i, i);
                    i--;
                }
            }
            manager.refreshPlugins();
        }

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
            return (columnIndex == 4);
        }

        @Override
        public Object getValueAt(int rowIndex, int columnIndex) {
            return rowData.get(rowIndex)[columnIndex];
        }

        @Override
        public void setValueAt(Object aValue, int rowIndex, int columnIndex) {
            rowData.get(rowIndex)[columnIndex] = aValue;
            fireTableCellUpdated(rowIndex, columnIndex);
        }
    }
}
