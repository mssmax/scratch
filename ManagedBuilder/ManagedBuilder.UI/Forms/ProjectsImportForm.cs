// -----------------------------------------------------------------------
// <copyright file="ProjectsImportForm.cs" company="GFI Software Development Ltd.">
//      Copyright (c) GFI Software All rights reserved.
// </copyright>
// -----------------------------------------------------------------------
﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using ManagedBuilder.UI.Adapters;

namespace ManagedBuilder.UI.Forms
{
    public partial class ProjectsImportForm : Form
    {
        public BuildFileAdapter BuildFile
        {
            get; set;
        }

        public ProjectsImportForm()
        {
            InitializeComponent();
        }

        private void btnBrowse_Click(object sender, EventArgs e)
        {
            using (var d = new FolderBrowserDialog())
            {
                var dr = d.ShowDialog(this);
                if (dr != DialogResult.OK)
                    return;


            }
        }

        private bool bSkipAllCheckedUpdate = false;
        private void cbAll_CheckedChanged(object sender, EventArgs e)
        {
            if (bSkipAllCheckedUpdate)
                return;

            cbVcProj.Checked = cbDspProj.Checked = cbCsProj.Checked = cbAll.Checked;
        }

        private void cbCsProj_CheckedChanged(object sender, EventArgs e)
        {
            if (((CheckBox) sender).Checked != false) return;

            bSkipAllCheckedUpdate = true;
            cbAll.Checked = false;
            bSkipAllCheckedUpdate = false;
        }

        private void panel1_Paint(object sender, PaintEventArgs e)
        {
            var g = e.Graphics;
            
            g.FillRectangle(new SolidBrush(Color.FromArgb(240,240,240)),
                ((Control)sender).ClientRectangle);
            ControlPaint.DrawBorder3D(g, ((Control)sender).ClientRectangle, Border3DStyle.Raised, Border3DSide.Top);

        }

        private void btnStart_Click(object sender, EventArgs e)
        {
            var arg = new ProjectScanSettings(txtImportRoot.Text,
                                              txtBaseSccPath.Text,
                                              cbCsProj.Checked,
                                              cbVcProj.Checked,
                                              cbDspProj.Checked,
                                              BuildFile);
            ProjectScanProgressForm.Start(arg);
            this.Close();
        }
    }
}
