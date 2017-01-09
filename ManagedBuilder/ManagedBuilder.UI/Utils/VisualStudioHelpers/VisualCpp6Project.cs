// -----------------------------------------------------------------------
// <copyright file="VisualCpp6Project.cs" company="GFI Software Development Ltd.">
//      Copyright (c) GFI Software All rights reserved.
// </copyright>
// -----------------------------------------------------------------------
using System;
using System.IO;
using System.Text.RegularExpressions;

namespace ManagedBuilder.UI.Utils.VisualStudioHelpers
{
    public class VisualCpp6Project : BaseVsProject
    {
        public static VisualCpp6Project Parse(string filename)
        {
            ////////////////////////////////////////////////////////////////////////////

            // determine project path
            var projectDir = Path.GetDirectoryName(filename);

            // determine solution path
            var solnDir = Path.GetDirectoryName(filename);

            if (Directory.GetFiles(solnDir, "*.dsw").Length == 0)
            {
                // we did not find a solution file in the same directory where
                // the project file resides, thus we need to check the parent folder
                // If solution file is in root, or parent file contains no SLN file
                // the solution path is set to the same directory where the vcproj is located
                var di = new DirectoryInfo(solnDir);

                if (di.Parent != null && (Directory.GetFiles(di.Parent.FullName, "*.sln").Length > 0))
                {
                    solnDir = di.Parent.FullName;
                }
            }

            // add ending \ to solution path
            if (!solnDir.EndsWith(Path.DirectorySeparatorChar.ToString()))
            {
                solnDir += Path.DirectorySeparatorChar;
            }
            
            //////////////////////////////////////////////////////////////////////////
            var proj = new VisualCpp6Project();
            proj.Filepath = filename;
            proj.SolutionPath = solnDir;

            proj.Framework = ProjectFramework.VisualCpp6;

            proj.Name = Path.GetFileNameWithoutExtension(filename);

            // read configurations
            try
            {
                var streamReader = new StreamReader(filename);
                var data = streamReader.ReadToEnd();
                var matches = regex.Matches(data);

                foreach (Match match in matches)
                {
                    var cfgname = match.Groups["rawcfg"].Value;
                    var platform = match.Groups["platform"].Value;

                    var cfg = new ProjectConfiguration { Name = cfgname, Platform = platform };
                    proj.InternalConfigurationList.Add(cfg);
                }
            }
            catch
            {
                
            }

            return proj;
        }

        public static Regex regex = new Regex(
            "\"\\$\\(CFG\\)\"\\ +==\\ +\\\"(?<rawcfg>(?<projectname>[\\w\\d" +
            "\\p{S}]+)\\ +-\\ +(?<platform>[\\w\\d]+)\\ (?<configname>[\\w" +
            "\\s\\d\\p{S}]+))\\\"",
            RegexOptions.Singleline
            | RegexOptions.CultureInvariant
            | RegexOptions.Compiled
            );

        public override string Version
        {
            get { throw new NotImplementedException(); }
            set { throw new NotImplementedException(); }
        }
    }
}