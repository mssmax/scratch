// -----------------------------------------------------------------------
// <copyright file="BinplaceFileEditorForm.cs" company="GFI Software Development Ltd.">
//      Copyright (c) GFI Software All rights reserved.
// </copyright>
// -----------------------------------------------------------------------
﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Text;
using System.Windows.Forms;

namespace ManagedBuilder.UI.Forms
{
    public partial class BinplaceFileEditorForm : Form
    {
        class FolderEntryWrap
        {
            private FolderEntryWrap _parent;
            public FolderEntryWrap Parent
            {
                get
                {
                    return _parent;
                }
                set
                {
                    if (value == _parent) return;
                    VisualNode.Remove();

                    _parent = value;

                    if (_parent != null)
                    {
                        _parent.VisualNode.Nodes.Add(VisualNode);
                    }
                }
            }

            public string Name
            {
                get { return VisualNode.Name; }
                set { VisualNode.Name = value; }
            }

            public bool IsRoot
            {
                get { return Name == "<root>"; }
            }

            public List<string> Modules { get; private set; }

            public TreeNode VisualNode { get; private set; }

            public FolderEntryWrap() : this("<root>")
            {
                
            }

            public FolderEntryWrap(string name)
            {
                Modules = new List<string>();
                VisualNode = new TreeNode(name);
                VisualNode.Tag = this;
                VisualNode.Expand();                
            }
        }

        public BinplaceFileEditorForm()
        {
            InitializeComponent();
        }

        public void OpenBinplaceFile(string filename)
        {
            tvDirStructure.BeginUpdate();
            try
            {
                tvDirStructure.Nodes.Clear();

                // 
                var dirHash = new Dictionary<string, FolderEntryWrap>();

                //
                var root = new FolderEntryWrap();
                tvDirStructure.Nodes.Add(root.VisualNode);
                dirHash.Add("<root>", root);

                using (var sr = new StreamReader(filename))
                {
                    var line = string.Empty;

                    while ((line = sr.ReadLine()) != null)
                    {
                        if (string.IsNullOrEmpty(line)) continue;

                        if (line[0] == ';')
                            continue;

                        var row = line.Split(new char[] {' ', '\t'}, StringSplitOptions.RemoveEmptyEntries);
                        if (row.Length < 1 || row.Length > 2)
                        {
                            // TODO: report warning
                            continue;
                        }

                        var name = row[0];
                        if (row.Length == 2)
                        {
                            var path = row[1];
                            if (path.EndsWith("\\"))
                                path = path.Remove(path.Length - 1, 1);

                            if (!dirHash.ContainsKey(path))
                            {
                                var pathParts = path.Split(new char[] {'\\'}, StringSplitOptions.RemoveEmptyEntries);
                                var tmpPath = string.Empty;
                                var parentFolder = root;
                                for (var i = 0; i < pathParts.Length; i++ )
                                {
                                    if (tmpPath.Length > 0 )
                                        tmpPath += "\\";

                                    tmpPath += pathParts[i];
                                    
                                    if (!dirHash.ContainsKey(tmpPath))
                                    {
                                        var folder = dirHash[tmpPath] = new FolderEntryWrap(pathParts[i]);
                                        folder.Parent = parentFolder;
                                    }

                                    parentFolder = dirHash[tmpPath];
                                }
                            }

                            dirHash[path].Modules.Add(name);                            
                        }
                        else
                        {
                            // add it to root
                            root.Modules.Add(name);
                        }
                    }
                }
            }
            finally
            {
                tvDirStructure.EndUpdate();
            }
        }

        private void tvDirStructure_Click(object sender, EventArgs e)
        {

        }

        private void tvDirStructure_KeyPress(object sender, KeyPressEventArgs e)
        {

        }

        private void tvDirStructure_NodeMouseClick(object sender, TreeNodeMouseClickEventArgs e)
        {

        }

        private void tvDirStructure_AfterSelect(object sender, TreeViewEventArgs e)
        {
            if (e.Node == null)
                return;

            var f = e.Node.Tag as FolderEntryWrap;
            if (f == null)
                return;

            fileList.BeginUpdate();

            fileList.Items.Clear();
            foreach (var entry in f.Modules)
                fileList.Items.Add(entry);

            fileList.EndUpdate();
        }

        private void addFolderToolStripMenuItem_Click(object sender, EventArgs e)
        {
            var folder = new FolderEntryWrap("New Folder");

            var node = tvDirStructure.SelectedNode;
            if (node == null || node.Parent == null)
            {
                tvDirStructure.Nodes.Add(folder.VisualNode);
            }
            else
            {
                folder.Parent = node.Parent.Tag as FolderEntryWrap;
            }
        }

        private void addSubfolderToolStripMenuItem_Click(object sender, EventArgs e)
        {
            var node = tvDirStructure.SelectedNode;
            if (node == null) return;

            var folder = new FolderEntryWrap("New Folder");
            folder.Parent = node.Tag as FolderEntryWrap;
        }

        private void treeViewContextMenu_Opening(object sender, CancelEventArgs e)
        {
            addFileToolStripMenuItem.Enabled = 
                addSubfolderToolStripMenuItem.Enabled =
                renameFolderToolStripMenuItem.Enabled =
                deleteToolStripMenuItem.Enabled = tvDirStructure.SelectedNode != null;
        }

        private void addFileToolStripMenuItem_Click(object sender, EventArgs e)
        {
            var node = tvDirStructure.SelectedNode;
            if (node == null) return;


            var folder = node.Tag as FolderEntryWrap;
            
        }
    }
}
