// -----------------------------------------------------------------------
// <copyright file="BaseBuilderConfiguration.cs" company="GFI Software Development Ltd.">
//      Copyright (c) GFI Software All rights reserved.
// </copyright>
// -----------------------------------------------------------------------
using System.Xml;

namespace ManagedBuilder.Core.Configuration
{
    public abstract class BaseBuilderConfiguration
    {
        public abstract void Configure(XmlElement xmlConfig);

        protected BaseBuilderConfiguration()
        {
            IsConfigured = false;
        }

        public bool IsConfigured { get; protected set; }
    }
}