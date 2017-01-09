// -----------------------------------------------------------------------
// <copyright file="BuildFileAdapter.cs" company="GFI Software Development Ltd.">
//      Copyright (c) GFI Software All rights reserved.
// </copyright>
// -----------------------------------------------------------------------
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.IO;
using System.Windows.Forms;
using ManagedBuilder.Core.IO;
using ManagedBuilder.Core.Model;
using ManagedBuilder.UI.Collections;
using ManagedBuilder.UI.Utils.VisualStudioHelpers;

namespace ManagedBuilder.UI.Adapters
{
    public class BuildFileAdapter : INotifyPropertyChanged, IEditableObject
    {
        
        /// <summary>
        /// The _hash dictionary associates a name with one or more projects
        /// </summary>
        private readonly Dictionary<string, List<ProjectAdapter>> _hash = new Dictionary<string, List<ProjectAdapter>>();
        private readonly Dictionary<ProjectAdapter, string> _reverseHash = new Dictionary<ProjectAdapter, string>();                
        private bool _isPopulating = false;

        private readonly MonitoredCollection<ProjectAdapter> _originalProjectsList = new MonitoredCollection<ProjectAdapter>();        

        #region Properties
        public MonitoredCollection<ProjectAdapter> Projects
        {
            get { return _originalProjectsList; }
        }

        public bool SameNameProjects
        {
            get
            {
                foreach (var keyVal in _hash)
                    if (keyVal.Value.Count > 1)
                        return true;

                return false;
            }
        }
        
        public bool IsNew { get; private set; }

        private bool _isDirty = false;        

        public bool IsDirty
        {
            get
            {
                if (_isDirty) return true;
                foreach (var item in Projects)
                {
                    if (item.IsDirty)
                        return true;
                }

                return false;
            }
            protected set
            {
                _isDirty = value;
                foreach (var item in Projects)
                    item.IsDirty = value;

                DoPropertyChangedEvent("IsDirty");
            }
        }        
        
        public string FileName { get; set; }
        #endregion

        #region c'tor
        /// <summary>
        /// Creates a new adapter for ProjectFile.  A new empty ProjectFile is created.
        /// This constructor is best used when creating a new build file
        /// </summary>
        public BuildFileAdapter()
            : this(new BuildFile(), "Untitled.mbuild", true)
        {            
        }


        public BuildFileAdapter(string filename)
            : this(BuildFileHandler.Load(filename), filename, false)
        {            
        }

        protected BuildFileAdapter(BuildFile pFile, string filename, bool isNew)
        {            
            if (pFile == null)
                throw new FileLoadException("Project file supplied cannot be loaded", filename);
            
            LoadProjectFile(pFile);

            FileName = filename;
            IsNew = isNew;

            _originalProjectsList.ListChanged += Projects_CollectionChanged;
            _originalProjectsList.BeforeRemoveItem += Projects_BeforeRemoveItem;
        }


        #endregion

        #region Event handlers
        void Projects_BeforeRemoveItem(object sender, RemoveItemEventArgs<ProjectAdapter> args)
        {
            RemoveHashes(args.Item);            
        }

        void Projects_CollectionChanged(object sender, ListChangedEventArgs e)
        {
            if (_isPopulating) return;

            _isDirty = true;
                        
            switch (e.ListChangedType)
            {
                case ListChangedType.ItemAdded:
                    {
                        var pa = _originalProjectsList[e.NewIndex];                       
                        AddHashes(pa);
                    }
                    break;
                case ListChangedType.ItemChanged:
                    {
                        var pa = _originalProjectsList[e.NewIndex];
                        UpdateHashes(pa);
                    }
                    break;
            }            

            DoPropertyChangedEvent("Projects");
        }        

        #endregion

        #region Public Methods
        public ProjectAdapter GetProjectByName(string name)
        {
            return _hash.ContainsKey(name) && _hash[name].Count > 0 ? _hash[name][0] : null;
        }

        private BuildFile PrepBuildFile()
        {
            var bFile = new BuildFile();

            foreach (var projectAdapter in Projects)
            {
                bFile.Projects.Add(projectAdapter.Target);
            }

            return bFile;
        }

        public void Save()
        {
            var bFile = PrepBuildFile();
            BuildFileHandler.Save(FileName, bFile);
            
            IsNew = false;
            IsDirty = false;
        }

        public void SaveAs(string filename)
        {
            var tmp = FileName;

            // save new filename
            FileName = filename;

            // we need to go through all the projects and updated their 
            // paths so that they are rooted to the place where the builds file is
            // located

            var tmpBuildFileDir = Path.GetDirectoryName(tmp);
            var pathBackup = new Dictionary<ProjectAdapter, string>();
            

            foreach (var proj in Projects)
            {
                var actualPath = proj.Path;
                if (string.IsNullOrEmpty(actualPath)) continue;

                if (!Path.IsPathRooted(actualPath))
                    actualPath = Path.Combine(tmpBuildFileDir, actualPath);

                pathBackup[proj] = proj.Path;
                proj.Path = GetPathRootedAtBuildsFile(actualPath);

                foreach (var pOut in proj.OutputInformation)
                {
                    actualPath = pOut.ActualOutput.DestPath;
                    if (!string.IsNullOrEmpty(actualPath))
                    {
                        if (!Path.IsPathRooted(actualPath))
                            actualPath = Path.Combine(tmpBuildFileDir, actualPath);

                        pOut.DestPath = GetPathRootedAtBuildsFile(actualPath);
                    }

                    for (int i = 0; i < pOut.PathList.Count; i++ )
                    {
                        actualPath = pOut.PathList[i];
                        if (string.IsNullOrEmpty(actualPath)) continue;

                        if (!Path.IsPathRooted(actualPath))
                            actualPath = Path.Combine(tmpBuildFileDir, actualPath);

                        pOut.PathList[i] = GetPathRootedAtBuildsFile(actualPath);
                    }
                }
            }

            try
            {
                var bFile = PrepBuildFile();
                BuildFileHandler.Save(filename, bFile);                
            }
            catch (Exception e)
            {
                // restore paths
                foreach (var val in pathBackup)
                    val.Key.Path = val.Value;

                FileName = filename;

                // rethrow exception
                throw;
            }

            IsDirty = false;
            IsNew = false;
        }
        #endregion

        void RemoveHashes(ProjectAdapter pa)
        {            
            if (_reverseHash.ContainsKey(pa))
            {
                var revHashStr = _reverseHash[pa];

                if (_hash.ContainsKey(revHashStr))
                {
                    _hash[revHashStr].Remove(pa);
                }

                if (_hash[revHashStr].Count == 0)
                    _hash.Remove(revHashStr);

                _reverseHash.Remove(pa);
            }            
        }

        void AddHashes(ProjectAdapter pa)
        {
            if (!_hash.ContainsKey(pa.Name))
            {
                var lst = new List<ProjectAdapter>() { pa };
                _hash.Add(pa.Name, lst);
            }
            else
            {
                _hash[pa.Name].Add(pa);
            }

            _reverseHash.Add(pa, pa.Name);
        }

        void UpdateHashes(ProjectAdapter pa)
        {
            // get the reverse hash so that we can
            // go through the actual hash and remove
            // the changed entry
            RemoveHashes(pa);
            AddHashes(pa);

        }

        protected void LoadProjectFile(BuildFile projectsFile)
        {            
            _isPopulating = true;

            foreach (var p in projectsFile.Projects)
            {
                var pa = new ProjectAdapter(p) {BuildFile = this};
                Projects.Add(pa);

                UpdateHashes(pa);
            }
            _isPopulating = false;

            foreach (var p in Projects)
            {
                foreach(var depName in p.Dependencies)
                {
                    var dep = GetProjectByName(depName);
                    if (dep == null)
                        continue;

                    dep.ActualDependants.GetInternalList().Add(p);
                    p.ActualDependencies.GetInternalList().Add(dep);
                }
            }
        }

        #region INotifyPropertyChanged
        public event PropertyChangedEventHandler PropertyChanged;

        protected void DoPropertyChangedEvent(string propertyName)
        {
            var handler = PropertyChanged;

            if (handler == null) return;

            handler(this, new PropertyChangedEventArgs(propertyName));
        }
        #endregion

        public MonitoredCollection<ProjectAdapter> FilterProjects(string text)
        {
            var lst = new MonitoredCollection<ProjectAdapter>();

            lock (this)
                foreach (var p in Projects)
                    if (p.Name.ToLower().Contains(text.ToLower()))
                        lst.Add(p);
            return lst;
        }

        public MonitoredCollection<ProjectAdapter> FilterDuplicateProjects()
        {
            var lst = new MonitoredCollection<ProjectAdapter>();

            lock (this)
                foreach (var keyVal in _hash)
                    if (keyVal.Value.Count > 1)
                        foreach (var val in keyVal.Value)
                            lst.Add(val);

            return lst;
        }

        public ProjectAdapter CreateProject()
        {
            return CreateProject("");
        }

        public ProjectAdapter CreateProject(string projectName)
        {
            var pa = new ProjectAdapter(projectName)
                         {                             
                             BuildFile = this
                         };
            return pa;
        }

        public ProjectAdapter ImportProject(string fileName)
        {
            return ImportProject(fileName, String.Empty);
        }

        public ProjectAdapter ImportProject(string fileName, string sccRoot)
        {
            var filename = fileName;
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
                    return null;
            }

            if (vsProject == null)
            {
                return null;
            }

            if(!sccRoot.EndsWith("/"))
                sccRoot = sccRoot + "/";

            var project = new ProjectAdapter(new Project(), false)
                              {
                                  BuildFile = this,
                                  Name = vsProject.Name,
                                  Path = GetPathRootedAtBuildsFile(vsProject.Filepath),
                                  SccPath = sccRoot + GetPathRootedAtBuildsFile(Path.GetDirectoryName(vsProject.Filepath)).Replace("\\", "/")
                              };

            // check if project path is relative to mproj file

            switch (vsProject.Framework)
            {
                case ProjectFramework.VisualCpp2003:
                    project.Framework = "vc71";
                    break;
                case ProjectFramework.VisualCpp2005:
                    project.Framework = "vc8";
                    break;
                case ProjectFramework.VisualCpp6:
                    project.Framework = "vc6";
                    break;
                case ProjectFramework.VisualCs2005:
                    project.Framework = "cs";
                    break;
                case ProjectFramework.VisualCs2003:
                    project.Framework = "vc71";
                    break;
            }            

            bool bConfigurationFound = false;
            var projectOutput = new ProjectOutputAdapter();

            foreach (var conf in vsProject.Configurations)
            {
                if (conf.Name.ToLower().Contains("release"))
                {
                    project.BuildOpt = "\"" + conf.Name + "\" " + conf.Platform;
                    projectOutput.PathList.Add(Path.Combine(GetPathRootedAtBuildsFile(conf.OutputPath), "*.*"));
                    bConfigurationFound = true;
                    project.Target.PlatformName = conf.Platform;
                    project.Target.ConfigurationName = conf.Name;
                    break;
                }
            }

            if (!bConfigurationFound)
            {
                if (vsProject.Framework == ProjectFramework.VisualCpp6)
                    projectOutput.PathList.Add(
                        Path.Combine(GetPathRootedAtBuildsFile(Path.GetDirectoryName(vsProject.Filepath)),
                                     @"release\*.*"));
                else
                    projectOutput.PathList.Add(
                        Path.Combine(GetPathRootedAtBuildsFile(Path.GetDirectoryName(vsProject.Filepath)), "*.*"));
            }

            project.OutputInformation.Add(projectOutput);

            return project;
        }

        public string GetPathRootedAtBuildsFile(string path)
        {
            var buildFilePath = Path.GetDirectoryName(FileName);

            if (path.StartsWith(buildFilePath, StringComparison.OrdinalIgnoreCase))
            {
                path = path.Remove(0, buildFilePath.Length);
                if (path.StartsWith("\\", StringComparison.OrdinalIgnoreCase))
                    path = path.Remove(0, 1);
            }

            return path;
        }

        public static BuildFileAdapter CreateNewBuildFile()
        {
            var bf = new BuildFileAdapter();
            bf.Projects.Add(bf.CreateProject("New Project"));

            return bf;
        }
    }
}
