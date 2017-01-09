// -----------------------------------------------------------------------
// <copyright file="ProjectConfiguration.cs" company="GFI Software Development Ltd.">
//      Copyright (c) GFI Software All rights reserved.
// </copyright>
// -----------------------------------------------------------------------
// -----------------------------------------------------------------------
// <copyright file="ProjectConfiguration.cs" company="GFI Software Development Ltd.">
//      Copyright (c) GFI Software All rights reserved.
// </copyright>
// -----------------------------------------------------------------------
﻿using System;
using System.Collections.Generic;
using System.Text;

namespace ManagedBuilder.UI.Utils.VisualStudioHelpers
{
    public class ProjectConfiguration
    {
        public string Name { get; set; }
        public string Platform { get; set; }
        public string OutputPath { get; set; }
        //public ProjectOutputType OutputType { get; set; }

        public ProjectConfiguration()
        {
            Name = string.Empty;
            Platform = string.Empty;
            OutputPath = string.Empty;
            //OutputType = outType;
        }

        public override string ToString()
        {
            return Name + '|' + Platform;
        }
    }
}