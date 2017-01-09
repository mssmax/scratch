// -----------------------------------------------------------------------
// <copyright file="BuildFile.cs" company="GFI Software Development Ltd.">
//      Copyright (c) GFI Software All rights reserved.
// </copyright>
// -----------------------------------------------------------------------
using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Xml;
using System.Xml.Schema;
using System.Xml.Serialization;

namespace ManagedBuilder.Core.Model
{
    [Serializable]
    [XmlRoot("ProjectsFile", Namespace = "http://www.gfi.com/schemas/projectsfile")]    
    public class BuildFile
    {
        private readonly List<Project> _projects = new List<Project>();
        public List<Project> Projects
        {
            get { return _projects; }            
        }        
    }

}