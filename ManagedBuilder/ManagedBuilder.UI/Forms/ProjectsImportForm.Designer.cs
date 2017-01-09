// -----------------------------------------------------------------------
// <copyright file="ProjectsImportForm.Designer.cs" company="GFI Software Development Ltd.">
//      Copyright (c) GFI Software All rights reserved.
// </copyright>
// -----------------------------------------------------------------------
// -----------------------------------------------------------------------
// <copyright file="ProjectsImportForm.Designer.cs" company="GFI Software Development Ltd.">
//      Copyright (c) GFI Software All rights reserved.
// </copyright>
// -----------------------------------------------------------------------
﻿namespace ManagedBuilder.UI.Forms
{
    partial class ProjectsImportForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.panel1 = new System.Windows.Forms.Panel();
            this.btnStart = new System.Windows.Forms.Button();
            this.btnCancel = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.txtImportRoot = new System.Windows.Forms.TextBox();
            this.btnBrowse = new System.Windows.Forms.Button();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.cbDspProj = new System.Windows.Forms.CheckBox();
            this.cbVcProj = new System.Windows.Forms.CheckBox();
            this.cbCsProj = new System.Windows.Forms.CheckBox();
            this.cbAll = new System.Windows.Forms.CheckBox();
            this.label3 = new System.Windows.Forms.Label();
            this.txtBaseSccPath = new System.Windows.Forms.TextBox();
            this.panel1.SuspendLayout();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // panel1
            // 
            this.panel1.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(238)))), ((int)(((byte)(238)))), ((int)(((byte)(238)))));
            this.panel1.Controls.Add(this.btnStart);
            this.panel1.Controls.Add(this.btnCancel);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.panel1.Location = new System.Drawing.Point(0, 335);
            this.panel1.Margin = new System.Windows.Forms.Padding(10);
            this.panel1.Name = "panel1";
            this.panel1.Padding = new System.Windows.Forms.Padding(5);
            this.panel1.Size = new System.Drawing.Size(419, 37);
            this.panel1.TabIndex = 7;
            this.panel1.Paint += new System.Windows.Forms.PaintEventHandler(this.panel1_Paint);
            // 
            // btnStart
            // 
            this.btnStart.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.btnStart.Location = new System.Drawing.Point(275, 8);
            this.btnStart.Name = "btnStart";
            this.btnStart.Size = new System.Drawing.Size(65, 21);
            this.btnStart.TabIndex = 1;
            this.btnStart.Text = "&Start";
            this.btnStart.UseVisualStyleBackColor = true;
            this.btnStart.Click += new System.EventHandler(this.btnStart_Click);
            // 
            // btnCancel
            // 
            this.btnCancel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.btnCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.btnCancel.Location = new System.Drawing.Point(346, 8);
            this.btnCancel.Name = "btnCancel";
            this.btnCancel.Size = new System.Drawing.Size(65, 21);
            this.btnCancel.TabIndex = 2;
            this.btnCancel.Text = "&Cancel";
            this.btnCancel.UseVisualStyleBackColor = true;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Verdana", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.ForeColor = System.Drawing.Color.RoyalBlue;
            this.label1.Location = new System.Drawing.Point(12, 9);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(208, 18);
            this.label1.TabIndex = 0;
            this.label1.Text = "Import multiple projects";
            // 
            // label2
            // 
            this.label2.Location = new System.Drawing.Point(35, 47);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(348, 37);
            this.label2.TabIndex = 1;
            this.label2.Text = "Specify a root folder from where the Import Wizard will start searching for proje" +
                "cts";
            // 
            // txtImportRoot
            // 
            this.txtImportRoot.Location = new System.Drawing.Point(37, 87);
            this.txtImportRoot.Name = "txtImportRoot";
            this.txtImportRoot.Size = new System.Drawing.Size(345, 20);
            this.txtImportRoot.TabIndex = 2;
            // 
            // btnBrowse
            // 
            this.btnBrowse.Location = new System.Drawing.Point(308, 113);
            this.btnBrowse.Name = "btnBrowse";
            this.btnBrowse.Size = new System.Drawing.Size(75, 21);
            this.btnBrowse.TabIndex = 3;
            this.btnBrowse.Text = "&Browse";
            this.btnBrowse.UseVisualStyleBackColor = true;
            this.btnBrowse.Click += new System.EventHandler(this.btnBrowse_Click);
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.cbDspProj);
            this.groupBox1.Controls.Add(this.cbVcProj);
            this.groupBox1.Controls.Add(this.cbCsProj);
            this.groupBox1.Controls.Add(this.cbAll);
            this.groupBox1.Location = new System.Drawing.Point(37, 203);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Padding = new System.Windows.Forms.Padding(5, 10, 5, 3);
            this.groupBox1.Size = new System.Drawing.Size(345, 123);
            this.groupBox1.TabIndex = 6;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Project types to import";
            // 
            // cbDspProj
            // 
            this.cbDspProj.AutoSize = true;
            this.cbDspProj.Location = new System.Drawing.Point(8, 95);
            this.cbDspProj.Name = "cbDspProj";
            this.cbDspProj.Size = new System.Drawing.Size(126, 17);
            this.cbDspProj.TabIndex = 4;
            this.cbDspProj.Text = "Visual C++ 6 Projects";
            this.cbDspProj.UseVisualStyleBackColor = true;
            this.cbDspProj.CheckedChanged += new System.EventHandler(this.cbCsProj_CheckedChanged);
            // 
            // cbVcProj
            // 
            this.cbVcProj.AutoSize = true;
            this.cbVcProj.Location = new System.Drawing.Point(8, 72);
            this.cbVcProj.Name = "cbVcProj";
            this.cbVcProj.Size = new System.Drawing.Size(173, 17);
            this.cbVcProj.TabIndex = 3;
            this.cbVcProj.Text = "Visual C++ 2005/2003 Projects";
            this.cbVcProj.UseVisualStyleBackColor = true;
            this.cbVcProj.CheckedChanged += new System.EventHandler(this.cbCsProj_CheckedChanged);
            // 
            // cbCsProj
            // 
            this.cbCsProj.AutoSize = true;
            this.cbCsProj.Location = new System.Drawing.Point(8, 49);
            this.cbCsProj.Name = "cbCsProj";
            this.cbCsProj.Size = new System.Drawing.Size(168, 17);
            this.cbCsProj.TabIndex = 2;
            this.cbCsProj.Text = "Visual C# 2005/2003 Projects";
            this.cbCsProj.UseVisualStyleBackColor = true;
            this.cbCsProj.CheckedChanged += new System.EventHandler(this.cbCsProj_CheckedChanged);
            // 
            // cbAll
            // 
            this.cbAll.AutoSize = true;
            this.cbAll.Location = new System.Drawing.Point(8, 26);
            this.cbAll.Name = "cbAll";
            this.cbAll.Size = new System.Drawing.Size(130, 17);
            this.cbAll.TabIndex = 1;
            this.cbAll.Text = "All Supported Projects";
            this.cbAll.UseVisualStyleBackColor = true;
            this.cbAll.CheckedChanged += new System.EventHandler(this.cbAll_CheckedChanged);
            // 
            // label3
            // 
            this.label3.Location = new System.Drawing.Point(35, 137);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(348, 37);
            this.label3.TabIndex = 4;
            this.label3.Text = "Enter the root folder in your source code control (projects on disk will be mappe" +
                "d to this path)";
            // 
            // txtBaseSccPath
            // 
            this.txtBaseSccPath.Location = new System.Drawing.Point(38, 177);
            this.txtBaseSccPath.Name = "txtBaseSccPath";
            this.txtBaseSccPath.Size = new System.Drawing.Size(345, 20);
            this.txtBaseSccPath.TabIndex = 5;
            // 
            // ProjectsImportForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.SystemColors.Window;
            this.CancelButton = this.btnCancel;
            this.ClientSize = new System.Drawing.Size(419, 372);
            this.Controls.Add(this.txtBaseSccPath);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.btnBrowse);
            this.Controls.Add(this.txtImportRoot);
            this.Controls.Add(this.txtBaseSccPath);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.panel1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.Name = "ProjectsImportForm";
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Import Projects";
            this.panel1.ResumeLayout(false);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Button btnCancel;
        private System.Windows.Forms.Button btnStart;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox txtImportRoot;
        private System.Windows.Forms.Button btnBrowse;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.CheckBox cbDspProj;
        private System.Windows.Forms.CheckBox cbVcProj;
        private System.Windows.Forms.CheckBox cbCsProj;
        private System.Windows.Forms.CheckBox cbAll;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox txtBaseSccPath;
    }
}