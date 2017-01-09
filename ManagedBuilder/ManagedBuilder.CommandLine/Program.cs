// -----------------------------------------------------------------------
// <copyright file="Program.cs" company="GFI Software Development Ltd.">
//      Copyright (c) GFI Software All rights reserved.
// </copyright>
// -----------------------------------------------------------------------
﻿using System;
using System.Collections.Generic;
using System.IO;
using System.Reflection;
using System.Text;
using log4net;
using log4net.Config;
using ManagedBuilder.Core;
using ManagedBuilder.Core.Configuration;
using ManagedBuilder.Core.IO;
using ManagedBuilder.Core.Jobs;
using ManagedBuilder.Core.Model;

namespace ManagedBuilder.CommandLine
{
    class Program
    {
        private static readonly log4net.ILog Log = log4net.LogManager.GetLogger(
            System.Reflection.MethodBase.GetCurrentMethod().DeclaringType);

        private readonly ArgumentParser _argParser;

        static Program()
        {                        
        }

        static int Main(string[] args)
        {
            int returnValue = new Program().Run(args);

            //var bf = BuildFileHandler.Load(@"E:\Code\MailEssentials\MainTrunk\mailessentials - Copy.mbuild");
            //var bf = BuildFileHandler.Load(@"E:\Code\MailEssentials\MainTrunk\test.mbuild");
            //BuildFileHandler.Save(@"E:\Code\MailEssentials\MainTrunk\test.mbuild", bf);
#if DEBUG
            Console.WriteLine("Press any key to exit ...");
            Console.ReadKey();
#endif

            return returnValue;
        }

        public Program()
        {
            _argParser = new ArgumentParser(
                '-',
                "f",                // file name of builds file
                "q",                // quiet
                "hidelogo",         
                "t",                // build a particular target
                "nodeps",           // do not build dependencies (used with -t)
                "c"                 // use a custom configuration file (instead of app.config)
                );
        }

        private int Run(string[] args)
        {
            #region Arguments check
            try
            {
                _argParser.Parse(args);
            }
            catch 
            {
                DisplayUsage();
                return 1;
            }

            var filename = string.Empty;

            if (!_argParser.IsSwitchSet("f"))
            {
                // check if there is an mbuild file in the current directory
                var files = Directory.GetFiles(Directory.GetCurrentDirectory(), "*.mbuild");
                if (files == null || files.Length == 0)
                {
                    DisplayUsage("No Build File Specified");
                    return 1;
                }
                
                if (files.Length > 1)
                {
                    DisplayUsage("More than one mbuild file found in current directory");
                    return 1;
                }

                filename = files[0];
            }
            else
            {
                if (_argParser["f"] == null)
                {
                    DisplayUsage("No Build File Specified");
                    return 1;
                }

                filename = _argParser["f"];
                if (!File.Exists(filename))
                {
                    DisplayUsage("Could not find file: {0}", filename);
                    return 1;
                }
            }

            //
            if (_argParser.IsSwitchSet("t"))
            {
                if (string.IsNullOrEmpty(_argParser["t"]))
                {
                    DisplayUsage("Target specified is empty or invalid");
                    return 1;
                }                
            }

            //
            if (_argParser.IsSwitchSet("c"))
            {
                if (_argParser["c"] == null)
                {
                    DisplayUsage("Configuration file not specified");
                    return 1;
                }
            }

            //
            if (!_argParser.IsSwitchSet("hidelogo"))
                ShowLogo();
            
            #endregion            

            if (_argParser.IsSwitchSet("c"))
            {
                ManagedBuilderSettings.LoadFromFile(_argParser["c"]);
            }
            else
            {
                ManagedBuilderSettings.LoadFromApplicationConfig();
            }

            if (!Path.IsPathRooted(filename))
            {
                filename = Path.GetFullPath(filename);
            }

            // setup common properties
            PropertyManager.Common.Add("build.file", filename);
            PropertyManager.Common.AddDirectoryProperty("build.directory", Path.GetDirectoryName(filename));

            OutputString("Building from <{0}>", filename);
            OutputString();

            bool bSuccess = false;
            if (_argParser.IsSwitchSet("t"))
                bSuccess = this.MultiTargetMode(filename);
            else
                bSuccess = this.AllTargetsMode(filename);

            return (bSuccess) ? 0 : 1;
            
        }

        private bool AllTargetsMode(string filename)
        {
            var pFile = BuildFileHandler.Load(filename);
            var m = new JobManager();

            foreach (var p in pFile.Projects)
            {
                m.AddWorker(p.Name, p);                
            }
                      
            try
            {
                m.Execute();

                return true;
            }
            catch (Exception e)
            {
                Log.Error("Building failed", e);

                return false;
            }
        }

        private bool RecursiveDependencyEval(JobManager jobManager, BuildFile bFile, string targetName)
        {
            var targetInst = bFile.Projects.Find(x => x.Name.Equals(targetName));
            if (targetInst == null)
            {
                DisplayUsage("Target [{0}] does not exist", targetName);
                return false;
            }

            // check if we have already added an instance of the project to the worker list
            if (jobManager.ContainsWorkerData(targetInst))
                return true;

            Log.InfoFormat("Adding {0} to worker list", targetInst.Name);
            jobManager.AddWorker(targetInst.Name, targetInst);

            if (_argParser.IsSwitchSet("nodeps"))
                return true;

            foreach (var depName in targetInst.Dependencies)
            {
                var b = RecursiveDependencyEval(jobManager, bFile, depName);
                if (!b)
                    return false;
            }

            return true;
        }

        private bool MultiTargetMode(string filename)
        {
            var pFile = BuildFileHandler.Load(filename);
            var m = new JobManager();

            var targetName = _argParser["t"];

            if (targetName.Contains(","))
            {
                // more than one target specified
                var targets = targetName.Split(
                    new char[] {','}, 
                    StringSplitOptions.RemoveEmptyEntries);

                if (targets.Length == 0)
                {
                    DisplayUsage("Invalid target list specified");
                    return false;
                }

                foreach (var tname in targets)
                {
                    if (!RecursiveDependencyEval(m, pFile, tname))
                        return false;
                }
            }
            else
            {
                if (!RecursiveDependencyEval(m, pFile, targetName))
                    return false;
            }

            if (_argParser.IsSwitchSet("nodeps"))
                m.ResolveDependencies = false;            

            try
            {
                m.Execute();
                return true;
            }
            catch (Exception e)
            {
                Log.Error("Building failed", e);

                return false;
            }
            
        }

        private void OutputString()
        {
            OutputString("");
        }

        private void OutputString(string fmt, params object[] args)
        {
            if (_argParser.IsSwitchSet("q"))
                return;

            var msg = string.Format(fmt, args);
            Console.WriteLine(msg);            
        }

        private static void DisplayUsage()
        {
            DisplayUsage("");
        }
        private static void DisplayUsage(string message, params object[] args)
        {
            ShowLogo();
            if (message.Length > 0)
            {
                Console.WriteLine(message, args);
                Console.WriteLine();
            }            

            Console.WriteLine("Usage: -f buildfile <options>");
            Console.WriteLine();
            Console.WriteLine("Available options:");
            Console.WriteLine("\t-f <filename> - Data file containing project information (required)");
            Console.WriteLine("\t-t <target>   - Name of project to build (optional)");
            Console.WriteLine("\t-nodeps       - When used with -t, the target's dependencies won't be built");
            Console.WriteLine("\t-q            - Quite mode");
            Console.WriteLine("\t-hidelogo     - Hide tool logo");
            Console.WriteLine("\t-c <filename> - configuration file to use");
        }

        private static void ShowLogo()
        {
            var asm = Assembly.GetEntryAssembly();
            Console.WriteLine("ManagedBuilder Command Line Utility [{0}]", asm.GetName().Version);
            Console.WriteLine("(c) GFI 2009");
            Console.WriteLine();

        }
    }
}
