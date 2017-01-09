// -----------------------------------------------------------------------
// <copyright file="ProjectConfigurationsForm.cs" company="GFI Software Development Ltd.">
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
using ManagedBuilder.UI.Utils.VisualStudioHelpers;

namespace ManagedBuilder.UI.Forms
{
    public partial class ProjectConfigurationsForm : Form
    {
        public ProjectConfigurationsForm()
        {
            InitializeComponent();
        }

        public static ProjectConfiguration Show (string filename)
        {
            var ext = Path.GetExtension(filename);

            BaseVsProject vsProject = null;
            switch (ext.ToLower())
            {
                case ".vcproj":
                    vsProject = VcProject.Parse(filename);
                    break;
                case ".csproj":
                    vsProject = CSharpProject.Parse(filename);
                    break;
                case ".dsp":
                    vsProject = VisualCpp6Project.Parse(filename);
                    break;
                default:
                    MessageBox.Show("Project file not recognized");
                    return null;
            }

            var f = new ProjectConfigurationsForm();

            foreach (var cfg in vsProject.Configurations)
            {
                var item = f.listView.Items.Add(cfg.Name);
                item.SubItems.Add(cfg.Platform);
                item.Tag = cfg;
            }
            
            if (f.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                return f.listView.SelectedItems[0].Tag as ProjectConfiguration;
            }

            return null;
        }

        private void listView_SelectedIndexChanged(object sender, EventArgs e)
        {
            btnOK.Enabled = listView.SelectedItems.Count > 0;
        }

        private void listView_MouseDoubleClick(object sender, MouseEventArgs e)
        {

            btnOK_Click(sender, e);
        }

        private void btnOK_Click(object sender, EventArgs e)
        {
            if (listView.SelectedItems.Count == 0)
                return;

            this.DialogResult = DialogResult.OK;
            Close();
        }
    }
}
