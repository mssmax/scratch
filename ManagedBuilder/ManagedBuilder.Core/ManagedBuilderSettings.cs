// -----------------------------------------------------------------------
// <copyright file="ManagedBuilderSettings.cs" company="GFI Software Development Ltd.">
//      Copyright (c) GFI Software All rights reserved.
// </copyright>
// -----------------------------------------------------------------------
using System;
using System.IO;
using System.Reflection;
using System.Xml;
using ManagedBuilder.Core.Configuration;
using ManagedBuilder.Core.Exceptions;

namespace ManagedBuilder.Core
{
    public static class ManagedBuilderSettings
    {
        private static readonly log4net.ILog Log = log4net.LogManager.GetLogger(
    System.Reflection.MethodBase.GetCurrentMethod().DeclaringType);

        static ManagedBuilderSettings()
        {
            IsDebug = true;
            DebugGenerateSingleFile = false;
            CommandLineConfig = new CommandLineBuilderConfiguration();
        }

        public static bool IsDebug
        {
            get; set;
        }

        public static bool DebugGenerateSingleFile
        { get; set;
        }

        public static CommandLineBuilderConfiguration CommandLineConfig
        {
            get; private set;
        }

        public static int MaximumBuilders
        {
            get; set;
        }

        public static void LoadFromFile(string filename)
        {
            #region Config file loading and verification            
            var doc = new XmlDocument();
            try
            {
                doc.Load(File.OpenRead(filename));
            }
            catch (Exception e)
            {
                throw ConfigurationException.AppConfigLoadFailure(e);
            }

            var xnList = doc.GetElementsByTagName("mbuilder");

            if (xnList.Count != 1)
            {
                throw ConfigurationException.MultipleMBuilderSettings();
            }
            #endregion

            var mbuildRoot = xnList[0] as XmlElement;
            if (mbuildRoot == null)
                throw ConfigurationException.ConfigurationReadFailure();

            var nodeList = mbuildRoot.SelectSingleNode("setup/debug") as XmlElement;
            if (nodeList != null && nodeList.HasAttribute("enabled"))
            {
                IsDebug = nodeList.Attributes["enabled"].Value.ToLower() == bool.TrueString.ToLower();
            }
            else
            {
                IsDebug = false;
            }

            var pm = new PropertyManager();

            nodeList = mbuildRoot.SelectSingleNode("setup/asyncbuilders") as XmlElement;
            if (nodeList != null && nodeList.HasAttribute("count"))
            {
                var rawBuilderCount = nodeList.Attributes["count"].Value;
                var strBuildCount = pm.Evaluate(rawBuilderCount);
                var bldCount = 0;
                if (!Int32.TryParse(strBuildCount, out bldCount))
                    throw ConfigurationException.ConfigurationReadFailure("setup/asyncbuilders");

                MaximumBuilders = bldCount;
            }
            else
            {
                MaximumBuilders = Environment.ProcessorCount;
            }

            Log.DebugFormat("Debug Mode: {0}", IsDebug);
            Log.DebugFormat("Asynchronous Builders: {0}", MaximumBuilders);

            nodeList = mbuildRoot.SelectSingleNode("builders/commandlinebuilder") as XmlElement;
            if (nodeList != null)
                CommandLineConfig.Configure(nodeList);
        }

        public static void LoadFromApplicationConfig()
        {
            #region Config file loading and verification
            var appCfgFile = System.AppDomain.CurrentDomain.SetupInformation.ConfigurationFile;            
            if (string.IsNullOrEmpty(appCfgFile) || !File.Exists(appCfgFile))
            {
                throw ConfigurationException.NoAppConfigFound();
            }

            LoadFromFile(appCfgFile);
            #endregion

            
            
            
        }
    }
}