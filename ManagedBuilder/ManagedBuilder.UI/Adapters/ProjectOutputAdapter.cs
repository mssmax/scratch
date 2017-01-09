// -----------------------------------------------------------------------
// <copyright file="ProjectOutputAdapter.cs" company="GFI Software Development Ltd.">
//      Copyright (c) GFI Software All rights reserved.
// </copyright>
// -----------------------------------------------------------------------
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Collections.Specialized;
using System.ComponentModel;
using ManagedBuilder.Core.Model;
using ManagedBuilder.UI.Collections;

namespace ManagedBuilder.UI.Adapters
{
    public class ProjectOutputAdapter : INotifyPropertyChanged, IEditableObject
    {
        public bool IsDirty { get; set; }

        public ProjectOutputAdapter(Output pOut)
        {
            ActualOutput = pOut;

            PathList = new MonitoredCollection<string>(ActualOutput.PathList);
            PathList.ListChanged += ObservablePathListCollectionChanged;
        }

        public ProjectOutputAdapter() : this(new Output())
        {
            
        }

        void ObservablePathListCollectionChanged(object sender, ListChangedEventArgs e)
        {
            IsDirty = true;
        }

        public string DestPath
        {
            get
            {
                if (string.IsNullOrEmpty(ActualOutput.DestPath))
                    return "<root>";

                return ActualOutput.DestPath;
            }
            set
            {
                if (value == "<root>" && string.IsNullOrEmpty(ActualOutput.DestPath))
                    return;

                ActualOutput.DestPath = value;                 
                IsDirty = true;

                DoPropertyChangedEvent("DestPath");
            }
        }

        public MonitoredCollection<string> PathList { get; private set; }
        public Output ActualOutput { get; private set; }

        #region INotifyPropertyChanged
        public event PropertyChangedEventHandler PropertyChanged;

        protected void DoPropertyChangedEvent(string propertyName)
        {
            var handler = PropertyChanged;

            if (handler == null) return;

            handler(this, new PropertyChangedEventArgs(propertyName));
        }
        #endregion
    }

    
}