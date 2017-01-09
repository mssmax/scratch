// -----------------------------------------------------------------------
// <copyright file="CommandLineBuilder.cs" company="GFI Software Development Ltd.">
//      Copyright (c) GFI Software All rights reserved.
// </copyright>
// -----------------------------------------------------------------------
using System;
using System.Diagnostics;
using System.IO;
using System.Threading;
using ManagedBuilder.Core.Configuration;
using ManagedBuilder.Core.Exceptions;
using ManagedBuilder.Core.Model;

namespace ManagedBuilder.Core.Builders
{
    public class CommandLineBuilder : IBuilder
    {
        private static readonly log4net.ILog log = log4net.LogManager.GetLogger(System.Reflection.MethodBase.GetCurrentMethod().DeclaringType);        

        static CommandLineBuilder()
        {
            var dbgCommandFilename = Path.Combine(Path.GetTempPath(), "dbg_commands.cmd");
            var stream = new FileStream(dbgCommandFilename, FileMode.Create, FileAccess.ReadWrite, FileShare.Read);
            DebugCommandFile = new StreamWriter(stream);
        }

        private static readonly StreamWriter DebugCommandFile;

        public void Execute(Project p)
        {
            // generate temporary filename
            string tempFileName;
            try
            {
                var protoFilename = Path.GetTempFileName();

                // build more unique filename (include timestamp)
                tempFileName = string.Format("{0}{1}{2}", DateTime.Now.ToFileTime(), Path.GetFileName(protoFilename), ".cmd");

                // prep filename with actual path
                tempFileName = Path.Combine(Path.GetDirectoryName(protoFilename), tempFileName);

                // move file to new .cmd
                File.Move(protoFilename, tempFileName);
            }
            catch (Exception e)
            {
                log.Error("Failed to generate temporary command script", e);
                throw BuilderException.CreateFailedToStartException(p, e);                
            }

            TextWriter writer = null;

            if (p == null)
                throw new BuilderException("No project provided", null);
            
            try
            {
                if (ManagedBuilderSettings.IsDebug && ManagedBuilderSettings.DebugGenerateSingleFile)
                {
                    writer = DebugCommandFile;
                    Monitor.Enter(DebugCommandFile);
                }
                else
                {
                    var fs = new FileStream(tempFileName, FileMode.Create, FileAccess.ReadWrite, FileShare.Read);
                    writer = new StreamWriter(fs);
                }

                if (!ManagedBuilderSettings.CommandLineConfig.FrameworkCommands.ContainsKey(p.Framework))
                    throw new BuilderException("Unsupported framework", p);

                log.DebugFormat("{0}:Building command line script in [{1}]", p.Name, tempFileName);

                // setup property manager for this compilation instance
                var propMan = new PropertyManager();

                var projectPath = p.Path;
                if (!Path.IsPathRooted(projectPath))
                    projectPath = Path.Combine(PropertyManager.Common["build.directory"], projectPath) ;

                log.DebugFormat("Project file: {0}", projectPath);
                log.DebugFormat("Project platform: {0}", p.PlatformName);
                log.DebugFormat("Project config: {0}", p.ConfigurationName);
                log.DebugFormat("Project build options: {0}", p.BuildOpt);
                log.DebugFormat("Project location: {0}", Path.GetDirectoryName(projectPath));
                
                propMan.Add("project.file", projectPath);
                propMan.Add("project.buildopts", p.BuildOpt);
                propMan.Add("project.sccpath", p.SccPath);
                propMan.Add("project.platform", p.PlatformName);
                propMan.Add("project.configuration", p.ConfigurationName);
                propMan.AddDirectoryProperty("project.dir", Path.GetDirectoryName(projectPath));

                var cmdlet = ManagedBuilderSettings.CommandLineConfig.FrameworkCommands[p.Framework];

                var cmdletOut = string.Format("call {0} {1}", 
                    propMan.Evaluate(cmdlet.Commandlet), propMan.Evaluate(cmdlet.Arguments));
                writer.WriteLine(cmdletOut);
                log.DebugFormat("{0}: {1}", p.Name, cmdletOut);

                // setup binplace
                if (ManagedBuilderSettings.CommandLineConfig.BinPlaceEnabled)
                {
                    // location where binplace is stored
                    var binPlaceLocation = propMan.Evaluate(
                        ManagedBuilderSettings.CommandLineConfig.BinPlaceLocation);
                    var binPlaceFile = propMan.Evaluate(
                        ManagedBuilderSettings.CommandLineConfig.BinPlaceFilename);
                    var binPlaceRoot = propMan.Evaluate(
                        ManagedBuilderSettings.CommandLineConfig.BinPlaceRoot);

                    foreach (var output in p.Outputs)
                    {
                        var actualDestPath = "";
                        if (!String.IsNullOrEmpty(output.DestPath))
                            actualDestPath = string.Format(
                                "-b {0} ",
                                output.DestPath);

                        foreach (var path in output.PathList)
                        {
                            if (string.IsNullOrEmpty(path))
                            {
                                log.Warn("Found an empty path in project outputs");
                                continue;
                            }

                            var binPlaceOut = string.Format(
                                "{4} {0} -r {1} -p {2} {3}",
                                actualDestPath,
                                binPlaceRoot,
                                // todo app.config needs to be set so that these values are evaluated 
                                Path.Combine(PropertyManager.Common["build.directory"], binPlaceFile),
                                Path.Combine(PropertyManager.Common["build.directory"], path),
                                binPlaceLocation
                                );

                            // write output to batch script
                            writer.WriteLine(binPlaceOut);
                            log.DebugFormat("{0}: {1}", p.Name, binPlaceOut);
                        }
                    }
                }

                if (ManagedBuilderSettings.IsDebug && ManagedBuilderSettings.DebugGenerateSingleFile)
                {
                    Monitor.Exit(DebugCommandFile);
                    DebugCommandFile.Flush();
                }
                else
                {
                    writer.Close();
                }

                log.DebugFormat("{0}: Executing command line script [{1}]",p.Name, tempFileName);
                var startInfo = new ProcessStartInfo();                
                startInfo.Arguments =
                    string.Format("/c \"{0}\"", tempFileName);
                startInfo.FileName = "cmd.exe";                    
                //startInfo.RedirectStandardOutput = true;
                startInfo.WindowStyle = ProcessWindowStyle.Hidden;
                startInfo.CreateNoWindow = true;
                startInfo.UseShellExecute = false;

                // todo: make it changeable via config
                startInfo.WorkingDirectory = PropertyManager.Common["build.directory"];

                try
                {
                    var process = Process.Start(startInfo);
                    if (process == null)
                        throw BuilderException.CreateFailedToStartException(p);

                    using (process)
                    {
                        process.WaitForExit();

                        if (process.ExitCode != 0)
                        {
                            throw BuilderException.CreateBuildFailedDueToExitCodeException(p, process.ExitCode);
                        }
                    }
                }
                catch (BuilderException be)
                {
                    throw;
                }
                catch (Exception e)
                {
                    throw BuilderException.CreateFailedToStartException(p, e);
                }
            }            
            finally
            {
                if (!ManagedBuilderSettings.IsDebug)
                {                    

                    File.Delete(tempFileName);
                }
            }
        }
    }
}