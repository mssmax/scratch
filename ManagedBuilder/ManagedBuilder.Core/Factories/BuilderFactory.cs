// -----------------------------------------------------------------------
// <copyright file="BuilderFactory.cs" company="GFI Software Development Ltd.">
//      Copyright (c) GFI Software All rights reserved.
// </copyright>
// -----------------------------------------------------------------------
using ManagedBuilder.Core.Builders;
using ManagedBuilder.Core.Model;

namespace ManagedBuilder.Core.Factories
{
    public static class BuilderFactory
    {
        public static IBuilder GetBuilderForProject(Project p)
        {
            return new CommandLineBuilder();
        }

        public static IBuilder GetBuilderByFramework(string frameworkName)
        {
            return new CommandLineBuilder();
        }
    }
}