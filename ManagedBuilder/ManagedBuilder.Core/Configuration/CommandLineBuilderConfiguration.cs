// -----------------------------------------------------------------------
// <copyright file="CommandLineBuilderConfiguration.cs" company="GFI Software Development Ltd.">
//      Copyright (c) GFI Software All rights reserved.
// </copyright>
// -----------------------------------------------------------------------

using System;
using System.Collections.Generic;
using System.Xml;
using ConfigurationException=ManagedBuilder.Core.Exceptions.ConfigurationException;

namespace ManagedBuilder.Core.Configuration
{
    public class CommandLineBuilderConfiguration : BaseBuilderConfiguration
    {
        private static readonly log4net.ILog log = log4net.LogManager.GetLogger(System.Reflection.MethodBase.GetCurrentMethod().DeclaringType);

        public string BinPlaceFilename { get; set; }
        public string BinPlaceRoot { get; set; }
        public string BinPlaceLocation { get; set; }
        public bool BinPlaceEnabled { get; set; }


        public Dictionary<string, FrameworkCmdlet> FrameworkCommands
        {
            get; private set;
        }

        public CommandLineBuilderConfiguration()
        {
            BinPlaceLocation = "binplace.exe";
            BinPlaceFilename = string.Empty;
            BinPlaceRoot = "${build.directory}";
            BinPlaceEnabled = false;
            FrameworkCommands = new Dictionary<string, FrameworkCmdlet>();
        }

        public override void Configure(XmlElement element)
        {
            IsConfigured = false;

            var binplaceCfg = element.SelectSingleNode("binplace") as XmlElement;

            if (binplaceCfg == null)
            {
                log.Info("Binplace configuration was not found, disabling binplace");
                BinPlaceEnabled = false;
            }
            else
            {
                if (!binplaceCfg.HasAttribute("file") && !binplaceCfg.HasAttribute("enabled"))
                    throw new ConfigurationException("Missing file/enabled attributes from <binplace> config xml");
                                
                BinPlaceEnabled = binplaceCfg.Attributes["enabled"].Value.ToLower() == bool.TrueString.ToLower();
                BinPlaceFilename = binplaceCfg.Attributes["file"].Value;

                if (BinPlaceEnabled && BinPlaceFilename.Length == 0)
                    throw new ConfigurationException("BinPlaceFile file name is empty");

                if (binplaceCfg.HasAttribute("root"))
                    BinPlaceRoot = binplaceCfg.Attributes["root"].Value;

                if (binplaceCfg.HasAttribute("location"))
                    BinPlaceLocation = binplaceCfg.Attributes["location"].Value;

                log.DebugFormat("BinPlace.Enabled: {0}", BinPlaceEnabled);
                log.DebugFormat("BinPlace.File: [{0}]", BinPlaceFilename);
                log.DebugFormat("BinPlace.Root: [{0}]", BinPlaceRoot);
            }


            var frameworkList = element.SelectNodes("frameworks/*");
            if (frameworkList == null)
                throw new ConfigurationException("Framework configuration is missing");

            log.DebugFormat("Setting up {0} frameworks", frameworkList.Count);
            foreach (XmlElement frameworkXml in frameworkList)
            {
                if (!frameworkXml.HasAttribute("name"))
                    throw new ConfigurationException("Framework name missing");

                if (!frameworkXml.HasAttribute("cmdlet"))
                    throw new ConfigurationException("Command string missing");

                var name = frameworkXml.Attributes["name"].Value;
                var cmdlet = frameworkXml.Attributes["cmdlet"].Value;
                var args = "";
                if (frameworkXml.HasAttribute("args"))
                    args = frameworkXml.Attributes["args"].Value;

                FrameworkCommands.Add(name, new FrameworkCmdlet(cmdlet, args));
            }

            IsConfigured = true;
        }        
    }

    public class FrameworkCmdlet
    {
        public string Commandlet { get; set; }
        public string Arguments { get; set; }

        public FrameworkCmdlet()
        {
            Commandlet = string.Empty;
            Arguments = string.Empty;
        }

        public FrameworkCmdlet(string commandlet, string arguments)
        {
            Commandlet = commandlet;
            Arguments = arguments;
        }

        public override string ToString()
        {
            return string.Format("{0} {1}", Commandlet, Arguments);
        }
    }
}