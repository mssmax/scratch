// -----------------------------------------------------------------------
// <copyright file="IBuilder.cs" company="GFI Software Development Ltd.">
//      Copyright (c) GFI Software All rights reserved.
// </copyright>
// -----------------------------------------------------------------------
using ManagedBuilder.Core.Model;

namespace ManagedBuilder.Core.Builders
{
    public interface IBuilder
    {
        void Execute(Project p);
    }
}