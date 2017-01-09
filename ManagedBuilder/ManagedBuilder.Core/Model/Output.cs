// -----------------------------------------------------------------------
// <copyright file="Output.cs" company="GFI Software Development Ltd.">
//      Copyright (c) GFI Software All rights reserved.
// </copyright>
// -----------------------------------------------------------------------
using System;
using System.Collections.Generic;
using System.Text;
using System.Xml.Serialization;

namespace ManagedBuilder.Core.Model
{
    [Serializable]
    public class Output
    {
        public Output()
        {
            PathList = new List<string>();
            DestPath = string.Empty;
        }

        public string DestPath { get; set; } 

        public List<string> PathList
        {
            get; private set;
        }
    }

}