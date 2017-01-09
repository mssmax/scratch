// -----------------------------------------------------------------------
// <copyright file="Project.cs" company="GFI Software Development Ltd.">
//      Copyright (c) GFI Software All rights reserved.
// </copyright>
// -----------------------------------------------------------------------
using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using System.Xml.Schema;
using System.Xml.Serialization;

namespace ManagedBuilder.Core.Model
{
    public class Project
    {   
        public Project() : this(true)
        {

        }

        internal Project(bool initOutputs)
        {
            Dependencies = new List<string>();            
            Outputs = new List<Output>();

            PlatformName = Framework = BuildOpt = Path = SccPath = Name = string.Empty;
        }

        public string Name { get; set; }
        public string Framework { get; set; }
        public string BuildOpt { get; set; }
        public string Path { get; set; }
        public string SccPath { get; set; }        
        public string PlatformName { get; set; }
        public string ConfigurationName { get; set; }

        public List<Output> Outputs { get; private set; }
       
        public List<string> Dependencies { get; private set; }

        public override string ToString()
        {
            return Name;
        }
    }

}