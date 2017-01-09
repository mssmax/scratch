// -----------------------------------------------------------------------
// <copyright file="ProjectScanProgressForm.cs" company="GFI Software Development Ltd.">
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
using ManagedBuilder.UI.Adapters;

namespace ManagedBuilder.UI.Forms
{
    public partial class ProjectScanProgressForm : Form
    {
        public ProjectScanProgressForm()
        {
            _uiUpdateTaskInfo = (ss => UpdateTaskInfo(ss));
            InitializeComponent();
        }

        private readonly BackgroundWorker _worker = new BackgroundWorker();

        public static void Start(ProjectScanSettings conf)
        {
            if (conf == null)
                return;

            var f = new ProjectScanProgressForm();        
            f._worker.DoWork += f.DoScanProjects;
            f._worker.RunWorkerAsync(conf);
            f._worker.RunWorkerCompleted += f.WorkerRunWorkerCompleted;
            f._worker.ProgressChanged += f.WorkerProgressChanged;            
            f._worker.WorkerSupportsCancellation = true;
            f._worker.WorkerReportsProgress = true;
            f.ShowDialog();
        }

        private void WorkerProgressChanged(object sender, ProgressChangedEventArgs e)
        {
            progressBar1.Value = e.ProgressPercentage;
        }

        void WorkerRunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            Close();
        }

        private void DoScanProjects(object sender, DoWorkEventArgs e)
        {
            var arg = e.Argument as ProjectScanSettings;            
            if (arg == null)
                return;

            UpdateTaskInfo("Scanning for projects ...");

            FindProjects(arg.Root, arg);

            UpdateTaskInfo("Starting import ...");

            var i = 0;

            foreach (var path in arg.Filenames)
            {                
                UpdateTaskInfo(string.Format("Loading {0}", path));
                var pa = arg.BuildFile.ImportProject(path, arg.SccRoot);
                arg.BuildFile.Projects.Add(pa);
                i++;

                var perc = (i / (float)arg.Filenames.Count) * 100f;
                _worker.ReportProgress((int)perc);
            }
        }

        private delegate void UpdateTaskInfoDelegate(string s);
        private readonly UpdateTaskInfoDelegate _uiUpdateTaskInfo;

        private void UpdateTaskInfo(string s)
        {
            if (this.InvokeRequired)
            {                
                this.BeginInvoke(_uiUpdateTaskInfo, s);
            }
            else
            {                
                this.lblTask.Text = s;
            }
            
        }

        private static void FindProjects(string path, ProjectScanSettings settings)
        {            
            if (settings.CppProjects)
            {
                var arr = Directory.GetFiles(path, "*.vcproj", SearchOption.AllDirectories);
                lock (settings)
                    settings.Filenames.AddRange(arr);
            }

            if (settings.CsProjects)
            {
                var arr = Directory.GetFiles(path, "*.csproj", SearchOption.AllDirectories);
                lock (settings)
                    settings.Filenames.AddRange(arr);
            }

            if (settings.DspProjects)
            {
                var arr = Directory.GetFiles(path, "*.dsp", SearchOption.AllDirectories);
                lock (settings)
                    settings.Filenames.AddRange(arr);
            }           
        }

        private void panel1_Paint(object sender, PaintEventArgs e)
        {
            var g = e.Graphics;
            g.FillRectangle(new SolidBrush(Color.FromArgb(240, 240, 240)),
                ((Control)sender).ClientRectangle);
            ControlPaint.DrawBorder3D(g, ((Control)sender).ClientRectangle, Border3DStyle.Raised, Border3DSide.Top);
        }

        private void btnCancel_Click(object sender, EventArgs e)
        {
            _worker.CancelAsync();
        }
    }

    public class ProjectScanSettings
    {
        public string Root { get; set; }
        public string SccRoot { get; set; }

        public bool CsProjects { get; set; }
        public bool CppProjects { get; set; }
        public bool DspProjects { get; set; }

        public BuildFileAdapter BuildFile { get; set; }


        public ProjectScanSettings(string root, string sccroot, bool csProjects, bool cppProjects, bool dspProjects, BuildFileAdapter buildFile)
        {
            Root = root;
            SccRoot = sccroot;
            CsProjects = csProjects;
            CppProjects = cppProjects;
            DspProjects = dspProjects;
            BuildFile = buildFile;

            Filenames = new List<string>();
        }

        public List<string> Filenames { get; private set; }
    }
}
