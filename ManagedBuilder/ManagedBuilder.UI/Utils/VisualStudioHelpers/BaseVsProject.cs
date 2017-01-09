// -----------------------------------------------------------------------
// <copyright file="BaseVsProject.cs" company="GFI Software Development Ltd.">
//      Copyright (c) GFI Software All rights reserved.
// </copyright>
// -----------------------------------------------------------------------
// -----------------------------------------------------------------------
// <copyright file="BaseVsProject.cs" company="GFI Software Development Ltd.">
//      Copyright (c) GFI Software All rights reserved.
// </copyright>
// -----------------------------------------------------------------------
﻿using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.IO;
using System.Text;
using System.Diagnostics;
using System.Collections;
using ManagedBuilder.UI.Utils.VisualStudioHelpers;

namespace ManagedBuilder.UI.Utils.VisualStudioHelpers 
{
    public abstract class BaseVsProject : IEquatable<BaseVsProject>
    {
        private readonly List<ProjectConfiguration> _internalConfigList = new List<ProjectConfiguration>();

        protected List<ProjectConfiguration> InternalConfigurationList
        {
            get { return _internalConfigList; }            
        }        

        public abstract string Version
        {
            get; set;
        }

        public ProjectLanguage Language { get; protected set; }

        public string Name
        {
            get; protected set;
        }

        public string Filepath
        {
            get; protected set;
        }

        public string SolutionPath
        {
            get; protected set;
        }

        public ProjectFramework Framework
        {
            get; set;
        }

        public ReadOnlyCollection<ProjectConfiguration> Configurations
        {
            get
            {
                return _internalConfigList.AsReadOnly();
            }
        }

        public bool Equals(BaseVsProject other)
        {
            if (ReferenceEquals(null, other)) return false;
            if (ReferenceEquals(this, other)) return true;
            return Equals(other.Language, Language) && Equals(other.Name, Name) && Equals(other.Filepath, Filepath) && Equals(other._internalConfigList, _internalConfigList);
        }

        public override bool Equals(object obj)
        {
            if (ReferenceEquals(null, obj)) return false;
            if (ReferenceEquals(this, obj)) return true;
            if (obj.GetType() != typeof (BaseVsProject)) return false;
            return Equals((BaseVsProject) obj);
        }

        public override int GetHashCode()
        {
            unchecked
            {
                var result = Language.GetHashCode();
                result = (result*397) ^ Name.GetHashCode();
                result = (result*397) ^ Filepath.GetHashCode();
                result = (result*397) ^ _internalConfigList.GetHashCode();
                return result;
            }
        }

        public static bool operator ==(BaseVsProject left, BaseVsProject right)
        {
            return Equals(left, right);
        }

        public static bool operator !=(BaseVsProject left, BaseVsProject right)
        {
            return !Equals(left, right);
        }
    }
}