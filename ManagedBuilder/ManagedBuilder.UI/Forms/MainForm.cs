// -----------------------------------------------------------------------
// <copyright file="MainForm.cs" company="GFI Software Development Ltd.">
//      Copyright (c) GFI Software All rights reserved.
// </copyright>
// -----------------------------------------------------------------------
using System;
using System.ComponentModel;
using System.IO;
using System.Text;
using System.Windows.Forms;
using ManagedBuilder.Core;
using ManagedBuilder.UI.Adapters;
using ManagedBuilder.UI.Collections;

namespace ManagedBuilder.UI.Forms
{
    public partial class MainForm : Form, INotifyPropertyChanged
    {
        private BuildFileAdapter _pFileAdapter;
        //private ProjectAdapter CurrentItem = new ProjectAdapter();
        //private ProjectAdapter CurrentItem = new ProjectAdapter("");

        private static int _projectCounter;        

        #region c'tor
        public MainForm()
        {
            InitializeComponent();

            // setup binding for checkbox listboxes
            // for some reason some brain child decided that they 
            // don't need the datasource property visible in the Designer
            lstDependants.DataSource = projectChildDepsSource;
            lstDependencies.DataSource = projectDependencySource;

            //toolStrip.Renderer = new ToolStripProfessionalRenderer(new VsColorTable());

            //txtName.DataBindings.Add(
            //    "Text", this, "CurrentItem.Name",
            //    true, DataSourceUpdateMode.OnPropertyChanged);
            //txtPath.DataBindings.Add(
            //    "Text", this, "CurrentItem.Path",
            //    true, DataSourceUpdateMode.OnPropertyChanged);
            //txtSccPath.DataBindings.Add(
            //    "Text", this, "CurrentItem.SccPath",
            //    true, DataSourceUpdateMode.OnPropertyChanged);
            //txtBuildOpt.DataBindings.Add(
            //    "Text", this, "CurrentItem.BuildOpt",
            //    true, DataSourceUpdateMode.OnPropertyChanged);
            //txtFramework.DataBindings.Add(
            //    "Text", this, "CurrentItem.Framework",
            //    true, DataSourceUpdateMode.OnPropertyChanged);
            //txtPlatform.DataBindings.Add(
            //    "Text", this, "CurrentItem.PlatformName",
            //    true, DataSourceUpdateMode.OnPropertyChanged);
            //txtConfigName.DataBindings.Add(
            //    "Text", this, "CurrentItem.ConfigurationName",
            //    true, DataSourceUpdateMode.OnPropertyChanged);

            SetupUiForBuildFile(BuildFileAdapter.CreateNewBuildFile());

            // lets register some application hotkeys
            //HotkeyManager.RegisterLocalHotkey(Keys.J, Keys.Alt, MoveNextProjectEvent, this);
            //HotkeyManager.RegisterLocalHotkey(Keys.K, Keys.Alt, MovePrevProjectEvent, this);
            //HotkeyManager.RegisterLocalHotkey(Keys.D1, Keys.Alt, SwitchDetailsView, this);
            //HotkeyManager.RegisterLocalHotkey(Keys.D2, Keys.Alt, SwitchDependenciesView, this);
            //HotkeyManager.RegisterLocalHotkey(Keys.D3, Keys.Alt, SwitchChildDependsView, this);
        }

        #endregion                

        #region Databinding Helper Property
        public ProjectAdapter CurrentItem
        {
            get
            {
                var pa = projectsSource.Current as ProjectAdapter;
                return pa ?? _pFileAdapter.CreateProject();
            }
        }

        public BuildFileAdapter CurrentBuildFile
        {
            get { return _pFileAdapter; }
        }
        #endregion

        #region Utility functions

        private void DoSave()
        {
            lblSaving.Visible = true;

            _pFileAdapter.Save();

            lblSaving.Visible = false;
            lblSaved.Visible = true;
            lblModifying.Visible = false;

            savedLabelTimer.Start();
        }

        private void DoSaveAs(string newFilename)
        {
            lblSaving.Visible = true;

            _pFileAdapter.SaveAs(newFilename);

            lblSaving.Visible = false;
            lblSaved.Visible = true;
            lblModifying.Visible = false;
            savedLabelTimer.Start();
        }

        private void SetupUiForBuildFile(BuildFileAdapter adapter)
        {
            if (_pFileAdapter != null) _pFileAdapter.PropertyChanged -= PFileAdapterPropertyChanged;

            _pFileAdapter = adapter;

            _pFileAdapter.PropertyChanged += PFileAdapterPropertyChanged;

            // check if we need to show any warnings
            tslblDuplicateEntries.Visible = _pFileAdapter.SameNameProjects;            

            _pFileAdapter.Projects.Sort();

            //buildFileSource.SuspendBinding();
            //buildFileSource.DataSource = new MonitoredCollection<BuildFileAdapter>() { _pFileAdapter };
            //buildFileSource.ResumeBinding();
            //buildFileSource.ResetBindings(true);

            SwitchToPrimarySource();

            projectDependencySource.DataSource = _pFileAdapter;
            projectChildDepsSource.DataSource = _pFileAdapter;
            
        }

        void PFileAdapterPropertyChanged(object sender, PropertyChangedEventArgs e)
        {            
            if (!e.PropertyName.Equals("Projects", StringComparison.CurrentCultureIgnoreCase))
                return;

            if (projectsSource.IsBindingSuspended)
                return;

            tslblDuplicateEntries.Visible = _pFileAdapter.SameNameProjects;
        }


        private bool HandleDirtyFile()
        {
            var dr = MessageBox.Show(
                "Project file has been modified.  Do you want to save?",
                "Unsaved Work",
                MessageBoxButtons.YesNoCancel,
                MessageBoxIcon.Information);

            if (dr == DialogResult.Cancel)
            {
                return false;
            }

            if (dr != DialogResult.Yes) return true;

            if (_pFileAdapter.IsNew)
            {
                using (var fd = CreateSaveProjectFileDialog())
                {
                    if (fd.ShowDialog() == DialogResult.OK)
                    {
                        DoSaveAs(fd.FileName);
                        return true;
                    }

                    // cancel'd 
                    return false;
                }
            }

            DoSave();
            return true;
        }

        private void RefreshView()
        {
            if (projectsSource.IsBindingSuspended)
                return;

            lblModifying.Visible = _pFileAdapter.IsDirty;

            _populatingDeps = true;
            
            // set the items that are checked
            lstDependencies.BeginUpdate();
            for (var i = 0; i < lstDependencies.Items.Count; i++)
            {
                var p = lstDependencies.Items[i] as ProjectAdapter;
                lstDependencies.SetItemChecked(
                    i,
                    CurrentItem.ActualDependencies.Contains(p));
            }
            lstDependencies.EndUpdate();

            lstDependants.BeginUpdate();
            for (var i = 0; i < lstDependants.Items.Count; i++)
            {
                var p = lstDependants.Items[i] as ProjectAdapter;
                lstDependants.SetItemChecked(
                    i,
                    CurrentItem.ActualDependants.Contains(p));
            }
            lstDependants.EndUpdate();
            _populatingDeps = false;
            DoPropertyChangedEvent("CurrentItem");
        }        

        #endregion

        #region File Dialog Handlers

        private static SaveFileDialog CreateSaveFileDialog()
        {
            return new SaveFileDialog();
        }

        private static SaveFileDialog CreateSaveProjectFileDialog()
        {
            var d = CreateSaveFileDialog();
            AddFilterToDialog(d, "Managed Builder File", "*.mbuild");

            return d;
        }

        private static OpenFileDialog CreateOpenFileDialog()
        {
            var fd = new OpenFileDialog();            
            return fd;
        }

        private static OpenFileDialog CreateOpenProjectFileDialog()
        {
            var d = CreateOpenFileDialog();
            AddFilterToDialog(d, "Managed Builder File", "*.mbuild");

            return d;
        }

        private static OpenFileDialog CreateSelectOutputFileDialog()
        {
            var d = CreateOpenFileDialog();
            

            return d;
        }

        private static void AddFilterToDialog(FileDialog fd, string filterName, params string[] extensions)
        {
            var sb = new StringBuilder();
            foreach (var ext in extensions)
                if (sb.Length == 0)
                    sb.Append(ext);
                else
                {
                    sb.AppendFormat(";{0}", ext);
                }

            fd.Filter += string.Format("{2}{0} ({1})|{1}", 
                filterName, 
                sb,
                (fd.Filter.Length > 0) ? "|" : "");
        }

        #endregion

        #region INotifyPropertyChanged
        public event PropertyChangedEventHandler PropertyChanged;

        protected void DoPropertyChangedEvent(string propertyName)
        {
            var handler = PropertyChanged;

            if (handler == null)
                return;

            handler(this, new PropertyChangedEventArgs(propertyName));
        }
        #endregion

        #region Event Handlers

        private void btnLoad_Click(object sender, EventArgs e)
        {
            using (var fd = CreateOpenProjectFileDialog())
            {
                if (fd.ShowDialog() != DialogResult.OK)
                    return;

                SetupUiForBuildFile(new BuildFileAdapter(fd.FileName));
                
            }
        }

        
        private void bntSave_Click(object sender, EventArgs e)
        {
            if (!_pFileAdapter.IsDirty) return;

            if (!_pFileAdapter.IsNew)
            {
                DoSave();
                return;
            }

            using (var fd = CreateSaveProjectFileDialog())
            {
                if (fd.ShowDialog() == DialogResult.OK)
                    DoSaveAs(fd.FileName);
            }


        }        

        private bool _populatingDeps;
        private bool _bFilteredMode;

        private void MainForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            var fileModified = _pFileAdapter != null && _pFileAdapter.IsDirty;
            if (!fileModified && !CurrentItem.IsDirty)
            {
                e.Cancel = false;
                return;
            }

            if (fileModified)
            {
                e.Cancel = !HandleDirtyFile();
            }
        }
        
        private void btnAddOutputFile_Click(object sender, EventArgs e)
        {
            var buildFilePath = Path.GetDirectoryName(_pFileAdapter.FileName);
            var path = CurrentItem.Path;
            if (string.IsNullOrEmpty(path))
                path = buildFilePath;
            else
            {
                path = Path.GetDirectoryName(path);

                if (!Path.IsPathRooted(path))
                    path = Path.Combine(buildFilePath, path);
            }

            using (var fd = CreateSelectOutputFileDialog())
            {
                fd.InitialDirectory = path;
                if (fd.ShowDialog() != DialogResult.OK) return;                

                pathListBindingSource.Add(_pFileAdapter.GetPathRootedAtBuildsFile(fd.FileName));
            }
        }

        private void btnAddOutputPath_Click(object sender, EventArgs e)
        {
            using (var fd = new FolderBrowserDialog())
            {
                if (fd.ShowDialog() != DialogResult.OK) return;
                pathListBindingSource.Add(_pFileAdapter.GetPathRootedAtBuildsFile(fd.SelectedPath));
            }
        }

        private void btnDeleteOutputEntry_Click(object sender, EventArgs e)
        {
            if (pathListBindingSource.Current != null)
                pathListBindingSource.RemoveCurrent();
        }

        private void btnAddProject_Click(object sender, EventArgs e)
        {
            projectsSource.Add(_pFileAdapter.CreateProject(string.Format("New Project {0}", ++_projectCounter)));
        }

        private void tabControl1_SizeChanged(object sender, EventArgs e)
        {
            ((Control)sender).Refresh();
        }        

        private void lstDependencies_ItemCheck(object sender, ItemCheckEventArgs e)
        {
            if (_populatingDeps) return;

            var dep = projectDependencySource[e.Index] as ProjectAdapter;
            if (e.NewValue == CheckState.Checked)
                CurrentItem.ActualDependencies.Add(dep);
            else
                CurrentItem.ActualDependencies.Remove(dep);
        }

        private void lstDependants_ItemCheck(object sender, ItemCheckEventArgs e)
        {
            if (_populatingDeps) return;

            var dep = lstDependants.Items[e.Index] as ProjectAdapter;
            if (dep == null) return;

            if (e.NewValue == CheckState.Checked)
                CurrentItem.ActualDependants.Add(dep);
            else
                CurrentItem.ActualDependants.Remove(dep);
        }

        private void txtFilter_KeyUp(object sender, KeyEventArgs e)
        {
            if (e.KeyValue == 27)
            {
                txtFilter.Text = "";
            }
        }

        private void txtFilter_TextChanged(object sender, EventArgs e)
        {
            if (txtFilter.Text.Length == 0)
            {
                filterTimer.Enabled = false;
                SwitchToPrimarySource();
            }
            else
            {                
                filterTimer.Stop();
                filterTimer.Start();                
            }
        }

        private void btnChooseProjectFile_Click(object sender, EventArgs e)
        {            
            var buildFilePath = Path.GetDirectoryName(_pFileAdapter.FileName);
            var path = CurrentItem.Path;
            if (string.IsNullOrEmpty(path))
                path = buildFilePath;
            else
            {
                path = Path.GetDirectoryName(path);

                if (!Path.IsPathRooted(path))
                    path = Path.Combine(buildFilePath, path);
            }

            using (var fd = CreateOpenFileDialog())
            {
                AddFilterToDialog(fd, "Project Files", "*.vcproj", "*.csproj", "*.dsp", "*.dsw", "*.sln");
                AddFilterToDialog(fd, "All Files", "*.*");

                fd.InitialDirectory = path;

                if (fd.ShowDialog() != DialogResult.OK) return;

                CurrentItem.Path = _pFileAdapter.GetPathRootedAtBuildsFile(fd.FileName);

                //DoPropertyChangedEvent("CurrentItem");
            }

        }        

        private void txtDirectOutputPath_KeyUp(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                if (e.Alt)
                {
                    // alt key is down, force add new path
                }
                else
                {
                    // if we have a selected item in the list than we updated that item
                    // otherwise we add a new item
                }

            }
        }

        private void btnAddDirectPath_Click(object sender, EventArgs e)
        {
            pathListBindingSource.Add(txtDirectOutputPath.Text);
        }

        private void projectsSource_ListChanged(object sender, ListChangedEventArgs e)
        {
            if (_pFileAdapter != null) 
                lblModifying.Visible = _pFileAdapter.IsDirty;

            //if (e.ListChangedType == ListChangedType.)

            //projectDependencySource.SuspendBinding();
            //projectChildDepsSource.SuspendBinding();

            //projectChildDepsSource.ResetBindings(true);
            //projectDependencySource.ResetBindings(true);

            //projectDependencySource.ResumeBinding();
            //projectChildDepsSource.ResumeBinding();


            //projectDependencySource.DataSource = null;
            //projectDependencySource.DataSource = buildFileSource;
            //projectDependencySource.DataMember = "Projects";

            //projectChildDepsSource.DataSource = null;
            //projectChildDepsSource.DataSource = buildFileSource;
            //projectChildDepsSource.DataMember = "Projects";
        }

        private void projectsSource_CurrentItemChanged(object sender, EventArgs e)
        {
            if (_pFileAdapter == null) return;
            
            lblModifying.Visible = _pFileAdapter.IsDirty;
            RefreshView();
        }

        private void importOneToolStripMenuItem_Click(object sender, EventArgs e)
        {
            using (var fd = CreateOpenFileDialog())
            {
                AddFilterToDialog(fd, "Project Files", "*.vcproj", "*.csproj", "*.dsp");
                AddFilterToDialog(fd, "Visual C++ 2005/2003 Project", "*.vcproj");
                AddFilterToDialog(fd, "Visual C# 2005/2003 Project", "*.csproj");
                AddFilterToDialog(fd, "Visual C++ 6 Project", "*.dsp");
                AddFilterToDialog(fd, "All Files", "*.*");
                if (fd.ShowDialog() != DialogResult.OK)
                    return;

                var project = _pFileAdapter.ImportProject(fd.FileName);

                if (project == null)
                {
                    MessageBox.Show("Failed to import project", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return;
                }

                projectsSource.Add(project);
                projectsSource.Position = projectsSource.IndexOf(project);

                tslblDuplicateEntries.Visible = _pFileAdapter.SameNameProjects;
            }
        }        

        private void saveAsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            using (var fd = CreateSaveProjectFileDialog())
            {
                if (fd.ShowDialog() == DialogResult.OK)
                    DoSaveAs(fd.FileName);
            }
        }        

        private void btnRemoveProject_Click(object sender, EventArgs e)
        {
            if (CurrentItem.IsDirty)
            {
                var dr = MessageBox.Show("Selected project is modified.  Are you sure you want to delete?", "",
                                         MessageBoxButtons.YesNo);
                if (dr == DialogResult.No)
                    return;
            }

            if (_bFilteredMode)
            {
                var obj = projectsSource.Current as ProjectAdapter;
                _pFileAdapter.Projects.Remove(obj);
            }
            
            projectsSource.RemoveCurrent();
            
        }        

        private void btnEditDirectPath_Click(object sender, EventArgs e)
        {
            if (pathListBindingSource.Current == null)
            {
                btnAddDirectPath_Click(sender, e);
                return;
            }

            var str = txtDirectOutputPath.Text;
            var idx = pathListBindingSource.Position;
            pathListBindingSource.RemoveCurrent();
            pathListBindingSource.Insert(idx, str);
        }

        

        private void btnNew_Click(object sender, EventArgs e)
        {
            if (_pFileAdapter.IsDirty)
            {
                var dr = MessageBox.Show("You have unsaved work.  Do you want to save?", "Unsaved Work",
                                         MessageBoxButtons.YesNoCancel, MessageBoxIcon.Warning);

                switch (dr)
                {
                    case DialogResult.Yes:
                        if (_pFileAdapter.IsNew)
                        {
                            using (var fd = new SaveFileDialog())
                            {
                                if (fd.ShowDialog() != DialogResult.OK)
                                    return;

                                DoSaveAs(fd.FileName);
                            }
                        }
                        else
                        {
                            DoSave();
                        }
                        break;                    
                    case DialogResult.Cancel:
                        return;
                }
            }

            SetupUiForBuildFile(BuildFileAdapter.CreateNewBuildFile());
        }

        private void MainForm_FormClosed(object sender, FormClosedEventArgs e)
        {

        }

        private void lstOutputs_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Delete)
            {
                pathListBindingSource.RemoveCurrent();
            }
        }


        private void SwitchChildDependsView(object sender, EventArgs e)
        {
            tabControl.SelectedTab = tabChildDeps;
            lstDependants.Focus();
        }

        private void SwitchDependenciesView(object sender, EventArgs e)
        {
            tabControl.SelectedTab = tabDependencies;
            lstDependencies.Focus();
        }

        private void SwitchDetailsView(object sender, EventArgs e)
        {
            tabControl.SelectedTab = tabDetails;
            tabDetails.Focus();
        }

        private void MovePrevProjectEvent(object sender, EventArgs e)
        {
            projectsSource.MovePrevious();
        }

        private void MoveNextProjectEvent(object sender, EventArgs e)
        {
            projectsSource.MoveNext();
        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void label8_Click(object sender, EventArgs e)
        {
            txtDirectOutputPath.Focus();
        }

        private void JumpToProjectList(object sender, EventArgs e)
        {
            lstProjects.Focus();
        }

        private void JumpToFilter(object sender, EventArgs e)
        {
            txtFilter.Focus();
        }

        private void lstProjects_KeyUp(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Delete)
            {
                btnRemoveProject_Click(sender, e);
                e.Handled = true;
            }
        }

        private void importManyToolStripMenuItem_Click(object sender, EventArgs e)
        {
            projectsSource.SuspendBinding();            
            var f = new ProjectsImportForm {BuildFile = _pFileAdapter};
            f.ShowDialog();
            projectsSource.ResumeBinding();
            projectsSource.ResetBindings(true);

            tslblDuplicateEntries.Visible = _pFileAdapter.SameNameProjects;
        }

        

        private void btnShowDuplicates_Click(object sender, EventArgs e)
        {
            if (!((ToolStripButton)sender).Checked)
            {
                SwitchToPrimarySource();
            }
            else
            {
                SwitchToFilteredSource(_pFileAdapter.FilterDuplicateProjects());
            }
        }

        private class BuildFileCompatibleCollection
        {
// ReSharper disable MemberCanBePrivate.Local
// ReSharper disable UnusedAutoPropertyAccessor.Local
            public MonitoredCollection<ProjectAdapter> Projects { get; set; }
// ReSharper restore UnusedAutoPropertyAccessor.Local
// ReSharper restore MemberCanBePrivate.Local

            public BuildFileCompatibleCollection(MonitoredCollection<ProjectAdapter> dataSource)
            {
                Projects = dataSource;
            }
        }

        private void SwitchToFilteredSource(MonitoredCollection<ProjectAdapter> dataSource)
        {
            if (dataSource == null)
                return;

            var obj = new BuildFileCompatibleCollection(dataSource);
            
            projectsSource.EndEdit();

            _bFilteredMode = true;

            //lstProjects.BeginUpdate();
            projectsSource.SuspendBinding();

            projectsSource.DataSource = obj;            

            projectsSource.ResetBindings(true);
            projectsSource.ResumeBinding();
            //lstProjects.EndUpdate();
        }

        private void SwitchToPrimarySource()
        {
            projectsSource.EndEdit();

            _bFilteredMode = false;

            //lstProjects.BeginUpdate();
            projectsSource.SuspendBinding();
                        
            //projectsSource.DataSource = buildFileSource;
            projectsSource.DataSource = _pFileAdapter;

            projectsSource.ResetBindings(true);
            projectsSource.ResumeBinding();
            //lstProjects.EndUpdate();
        }

        private void btnResetFilter_Click(object sender, EventArgs e)
        {
            txtFilter.Text = string.Empty;
        }

        private void btnAddBinPlaceOpt_Click(object sender, EventArgs e)
        {
            var newBinPlaceOpt = new ProjectOutputAdapter();
            outputInformationBindingSource.Add(newBinPlaceOpt);
            outputInformationBindingSource.Position = outputInformationBindingSource.IndexOf(newBinPlaceOpt);
        }

        private void btnDeleteBinPlaceOptions_Click(object sender, EventArgs e)
        {
            var itm = outputInformationBindingSource.Current as ProjectOutputAdapter;
            if (itm == null)
                return;

            var msg = string.Format(
                "Are you sure you want to delete the selected binplacement item ({0})", 
                itm.DestPath);
            var dr = MessageBox.Show(msg, "Warning", MessageBoxButtons.YesNo, MessageBoxIcon.Warning);

            switch (dr)
            {
                case DialogResult.Yes:
                    outputInformationBindingSource.RemoveCurrent();
                    break;
            }
        }

        private void filterTimer_Tick(object sender, EventArgs e)
        {
            SwitchToFilteredSource(_pFileAdapter.FilterProjects(txtFilter.Text));
            filterTimer.Stop();
        }
        #endregion

        private void buildFileSource_CurrentItemChanged(object sender, EventArgs e)
        {
            lblModifying.Visible = _pFileAdapter.IsDirty;

            RefreshView();
        }

        private void savedLabelTimer_Tick(object sender, EventArgs e)
        {
            lblSaved.Visible = false;
            savedLabelTimer.Stop();
        }

        private void projectDependencySource_ListChanged(object sender, ListChangedEventArgs e)
        {
            RefreshView();
        }

        private void buildWithoutDependenciesToolStripMenuItem_Click(object sender, EventArgs e)
        {

        }

        private void buildProject_Click(object sender, EventArgs e)
        {
            
        }

        private void binPlaceFileEditorToolStripMenuItem_Click(object sender, EventArgs e)
        {
            var f = new BinplaceFileEditorForm();
            var binplaceFilename = "binplace_fm.text";
            var path = Path.GetDirectoryName(_pFileAdapter.FileName);
            if (File.Exists(Path.Combine(path,binplaceFilename)))
            {
                f.OpenBinplaceFile(Path.Combine(path,binplaceFilename));
            }
            f.ShowDialog();            
        }

        private void btnChooseConfig_Click(object sender, EventArgs e)
        {
            var path = CurrentItem.Path;

            if (!Path.IsPathRooted(path))
                path = Path.Combine(
                    Path.GetDirectoryName(this.CurrentBuildFile.FileName),
                    path);

            var conf = ProjectConfigurationsForm.Show(path);

            CurrentItem.PlatformName = conf.Platform;
            CurrentItem.ConfigurationName = conf.Name;
        }
    }
    
}