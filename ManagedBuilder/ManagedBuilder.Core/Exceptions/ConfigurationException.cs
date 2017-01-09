// -----------------------------------------------------------------------
// <copyright file="ConfigurationException.cs" company="GFI Software Development Ltd.">
//      Copyright (c) GFI Software All rights reserved.
// </copyright>
// -----------------------------------------------------------------------
using System;

namespace ManagedBuilder.Core.Exceptions
{
    public class ConfigurationException : Exception
    {
        public ConfigurationException(string msg) : base(msg) { }
        public ConfigurationException(string msg, Exception innerException) : base(msg, innerException) { }

        public static ConfigurationException MultipleMBuilderSettings()
        {
            return new ConfigurationException("More than one <mbuilder> settings root found");
        }

        public static ConfigurationException NoAppConfigFound()
        {
            return new ConfigurationException("Could not find/open application config file");
        }

        public static ConfigurationException AppConfigLoadFailure(Exception innerException)
        {
            return new ConfigurationException("Failed to load app.config file", innerException);
        }

        public static ConfigurationException ConfigurationReadFailure()
        {
            return new ConfigurationException("Failed to read configuration");
        }

        public static ConfigurationException ConfigurationReadFailure(string key)
        {
            return new ConfigurationException(string.Format("Failed to read configuration [{0}]", key));
        }
    }
}