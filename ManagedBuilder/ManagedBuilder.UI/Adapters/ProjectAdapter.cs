// -----------------------------------------------------------------------
// <copyright file="ProjectAdapter.cs" company="GFI Software Development Ltd.">
//      Copyright (c) GFI Software All rights reserved.
// </copyright>
// -----------------------------------------------------------------------
using System;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.ComponentModel;
using ManagedBuilder.Core.Model;
using ManagedBuilder.UI.Collections;

namespace ManagedBuilder.UI.Adapters
{
    [Sortable("Name")]
    public class ProjectAdapter : INotifyPropertyChanged, IComparable<ProjectAdapter>, IEditableObject
    {
        #region Properties
        internal Project Target { get; private set; }

        public BuildFileAdapter BuildFile
        {
            get; set;
        }

        private bool _isDirty;
        public bool IsDirty
        {
            get
            {
                foreach (var itm in OutputInformation)
                    if (itm.IsDirty)
                        return true;

                return _isDirty;
            }
            set 
            { 
                _isDirty = value;
                foreach (var itm in OutputInformation)
                    itm.IsDirty = value;
            }
        }

        public MonitoredCollection<ProjectAdapter> ActualDependants { get; private set; }

        public MonitoredCollection<ProjectAdapter> ActualDependencies { get; private set; }

        public MonitoredCollection<ProjectOutputAdapter> OutputInformation
        {
            get;
            private set;
        }

        public string Name
        {
            get
            {
                return Target.Name;
            }
            set
            {
                var prevName = Target.Name;

                Target.Name = value;
                IsDirty = true;

                foreach (var pa in ActualDependants)
                {
                    pa.RenameDependency(prevName, value);
                }

                NotifyPropertyChanged("Name");
            }
        }

        private void RenameDependency(string prevName, string newName)
        {
            var idx = Target.Dependencies.IndexOf(prevName);
            if (idx >= 0 && idx < Target.Dependencies.Count)
                Target.Dependencies[idx] = newName;
        }

        public string Framework
        {
            get
            {
                return Target.Framework;
            }
            set
            {
                Target.Framework = value;
                IsDirty = true;
                NotifyPropertyChanged("Framework");
            }
        }

        public List<string> Dependencies
        {
            get
            {
                return Target.Dependencies;
            }            
        }

        public string BuildOpt
        {
            get
            {
                return Target.BuildOpt;
            }
            set
            {
                Target.BuildOpt = value;
                NotifyPropertyChanged("BuildOpt");
                IsDirty = true;
            }
        }

        public string Path
        {
            get { return Target.Path; }
            set { Target.Path = value; IsDirty = true; NotifyPropertyChanged("Path"); }
        }

        public string AbsolutePath
        {
            get
            {
                if (!System.IO.Path.IsPathRooted(Target.Path))
                {
                    return BuildFile.GetPathRootedAtBuildsFile(Target.Path);
                }

                return Target.Path;
            }
        }

        public string SccPath
        {
            get { return Target.SccPath; }
            set
            {
                Target.SccPath = value;
                IsDirty = true;
                NotifyPropertyChanged("SccPath");
            }
        }

        public string PlatformName
        {
            get { return Target.PlatformName; }
            set
            {
                Target.PlatformName = value;
                IsDirty = true;
                NotifyPropertyChanged("PlatformName");
            }
        }

        public string ConfigurationName
        {
            get { return Target.ConfigurationName; }
            set
            {
                Target.ConfigurationName = value;
                IsDirty = true;
                NotifyPropertyChanged("ConfigurationName");
            }
        }

        #endregion

        #region c'tor

        public ProjectAdapter() : this("New Project")
        {            
        }

        public ProjectAdapter(string name)
            : this(new Project(), true)
        {
            Name = name;
        }

        public ProjectAdapter(Project target) : this(target, false)
        {            
        }

        internal ProjectAdapter(Project target, bool isNewProject)
        {
            Target = target;
            OutputInformation = new MonitoredCollection<ProjectOutputAdapter>();
            ActualDependants = new MonitoredCollection<ProjectAdapter>();
            ActualDependencies = new MonitoredCollection<ProjectAdapter>();

            if (isNewProject && Target.Outputs.Count == 0)
                Target.Outputs.Add(new Output());

            foreach (var pOut in Target.Outputs)
                OutputInformation.Add(new ProjectOutputAdapter(pOut));

            IsDirty = false;

            OutputInformation.ListChanged += OutputInformation_ListChanged;
            ActualDependants.BeforeRemoveItem += ActualDependants_BeforeRemoveItem;
            ActualDependants.ListChanged += ActualDependants_CollectionChanged;
            ActualDependencies.BeforeRemoveItem += ActualDependencies_BeforeRemoveItem;
            ActualDependencies.ListChanged += ActualDependencies_CollectionChanged;
        }        
        
        #endregion

        #region Collection Handlers

        

        void OutputInformation_ListChanged(object sender, ListChangedEventArgs e)
        {
            IsDirty = true;

            switch (e.ListChangedType)
            {
                case ListChangedType.ItemAdded:
                    Target.Outputs.Insert(e.NewIndex, OutputInformation[e.NewIndex].ActualOutput);
                    break;
                case ListChangedType.ItemDeleted:
                    Target.Outputs.RemoveAt(e.NewIndex);
                    break;
            }
        }

        void ActualDependencies_BeforeRemoveItem(object sender, RemoveItemEventArgs<ProjectAdapter> args)
        {
            var dep = args.Item;
            // remove reference of dep from target
            Target.Dependencies.Remove(dep.Target.Name);

            // update dependants
            dep.ActualDependants.GetInternalList().Remove(this);
        }

        void ActualDependencies_CollectionChanged(object sender, ListChangedEventArgs e)
        {            
            switch (e.ListChangedType)
            {
                case ListChangedType.ItemAdded:
                    var dep = ActualDependencies[e.NewIndex];
                    this.DoAddDependency(dep);
                    dep.ActualDependants.GetInternalList().Add(this);
                    break;                
            }
        }

        void ActualDependants_BeforeRemoveItem(object sender, RemoveItemEventArgs<ProjectAdapter> args)
        {
            var dep = args.Item;
            dep.DoRemoveDependency(this);
        }

        void ActualDependants_CollectionChanged(object sender, ListChangedEventArgs e)
        {            
            switch (e.ListChangedType)
            {
                case ListChangedType.ItemAdded:
                    var dep = ActualDependants[e.NewIndex];
                    dep.DoAddDependency(this);
                    break;
                
            }
        }

        private void DoRemoveDependency(ProjectAdapter dep)
        {
            ActualDependencies.GetInternalList().Remove(dep);
            Target.Dependencies.Remove(dep.Target.Name);

            IsDirty = true;
        }

        private void DoAddDependency(ProjectAdapter dep)
        {
            ActualDependencies.GetInternalList().Add(dep);

            var depName = Target.Dependencies.Find(x => x.Equals(dep.Target.Name));
            if (depName == null)
                Target.Dependencies.Add(dep.Target.Name);

            IsDirty = true;
        }

        public int CompareTo(ProjectAdapter other)
        {
            return Name.CompareTo(other.Name);
        }

        public override string ToString()
        {
            return Target.Name;
        }

        #endregion        

        #region INotifyPropertyChanged
        public event PropertyChangedEventHandler PropertyChanged;

        internal virtual void NotifyPropertyChanged(string propertyName)
        {
            var handler = PropertyChanged;

            if (handler != null)
                handler(this, new PropertyChangedEventArgs(propertyName));
        }
        #endregion

        
    }
}