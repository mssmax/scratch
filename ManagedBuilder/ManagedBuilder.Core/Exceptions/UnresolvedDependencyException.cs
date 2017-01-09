// -----------------------------------------------------------------------
// <copyright file="UnresolvedDependencyException.cs" company="GFI Software Development Ltd.">
//      Copyright (c) GFI Software All rights reserved.
// </copyright>
// -----------------------------------------------------------------------
using System;

namespace ManagedBuilder.Core.Exceptions
{
    public class UnresolvedDependencyException : Exception
    {
        public UnresolvedDependencyException(string projectName, string dependencies)
        {
            Name = projectName;
            Dependencies = dependencies;
        }

        public string Name
        {
            get; private set;
        }

        public string Dependencies
        {
            get; private set;
        }
    }
}