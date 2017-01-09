// -----------------------------------------------------------------------
// <copyright file="Enum.cs" company="GFI Software Development Ltd.">
//      Copyright (c) GFI Software All rights reserved.
// </copyright>
// -----------------------------------------------------------------------
﻿using System;

namespace ManagedBuilder.UI.Utils.VisualStudioHelpers
{
    public enum ProjectPathType : int
    {
        ProjectFile = 0,
        Documentation = 1
    }

    public enum ProjectLanguage : long
    {
        CSharp = 0,
        CPlusPlus = 1
    }

    public enum ProjectFramework
    {
        None,
        VisualCpp2005,
        VisualCpp2003,        
        VisualCpp6,
        VisualCs2005,
        VisualCs2003
    }

    public enum ProjectFileType : long
    {
        Code,
        Executable,
        Library,
        StaticLibrary,
        ProgramDebugDatabase
    }

    [Flags]
    public enum ProjectOutputType
    {
        CommandLineExe  = 1,
        WindowsExe      = 2,
        DynamicLibrary      = 4,
        StaticLibrary   = 8,
        None = 0
    }
}