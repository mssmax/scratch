// -----------------------------------------------------------------------
// <copyright file="VcProject.cs" company="GFI Software Development Ltd.">
//      Copyright (c) GFI Software All rights reserved.
// </copyright>
// -----------------------------------------------------------------------
﻿using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using System.IO;
using System.Text.RegularExpressions;

namespace ManagedBuilder.UI.Utils.VisualStudioHelpers
{
    public class VcProject : BaseVsProject
    {
        /*private string LoadVersionFromResource()
        {
            string strVersion = null;

            string strPath = Path.GetDirectoryName(ProjectFilePath);
            string[] arrFilenames = Directory.GetFiles(strPath, "*.rc");

            if (arrFilenames == null || arrFilenames.Length == 0)
                return null;

            Regex rx = new Regex(
                "FILEVERSION(?:\\ )+([\\d\\.\\,\\ ]+)(?:\\ )*(.*)",
                RegexOptions.CultureInvariant | RegexOptions.Compiled
                );
            try
            {
                FileStream fs = new FileStream(
                    arrFilenames[0], FileMode.Open, FileAccess.Read);

                StreamReader sr = new StreamReader(fs);
                string strLine = "";

                while ((strLine = sr.ReadLine()) != null)
                {
                    if (rx.IsMatch(strLine))
                    {
                        Match m = rx.Match(strLine);
                        strVersion = m.Groups[1].Value.Trim();
                        break;
                    }
                }

                fs.Close();
            }
            catch 
            {
                
            }

            return strVersion;            
        }*/

        public static VcProject Parse(string filename)
        {
            #region Contract validation
            if (string.IsNullOrEmpty(filename))
                return null;

            if (!Path.IsPathRooted(filename))
                filename = Path.GetFullPath(filename);
            #endregion

            var doc = new XmlDocument();
            try
            {
                doc.Load(filename);
            }
            catch
            {
                return null;
            }

            ////////////////////////////////////////////////////////////////////////////
            
            // determine project path
            var projectDir = Path.GetDirectoryName(filename);

            // determine solution path
            var solnDir = Path.GetDirectoryName(filename);
            
            if (Directory.GetFiles(solnDir, "*.sln").Length == 0)
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

            var proj = new VcProject();

            proj.Filepath = filename;
            proj.SolutionPath = solnDir;
            //////////////////////////////////////////////////////////////////////////
            // get project name
            var root = doc.SelectSingleNode("VisualStudioProject") as XmlElement;
            if (root == null)
                return null;

            proj.Name = root.HasAttribute("Name") 
                ? root.Attributes["Name"].Value 
                : string.Empty;

            // determine framework
            if (root.HasAttribute("Version"))
            {
                switch (root.Attributes["Version"].Value)
                {
                    case "8.00":
                        proj.Framework = ProjectFramework.VisualCpp2005;
                        break;
                    case "7.10":
                        proj.Framework = ProjectFramework.VisualCpp2003;
                        break;
                }
            }


            // get configurations
            var confNodes = doc.SelectNodes("VisualStudioProject/Configurations/*");
            if (confNodes == null)
                return null;

            foreach (XmlElement xe in confNodes)
            {
                var confNamePform = xe.HasAttribute("Name") 
                    ? xe.Attributes["Name"].Value 
                    : string.Empty;
                var confOutDir = xe.HasAttribute("OutputDirectory") 
                    ? xe.Attributes["OutputDirectory"].Value 
                    : string.Empty;
                var confType = xe.HasAttribute("ConfigurationType") 
                    ? xe.Attributes["ConfigurationType"].Value 
                    : string.Empty;

                var bMissinReqData = string.IsNullOrEmpty(confNamePform) ||
                                     string.IsNullOrEmpty(confOutDir) || 
                                     string.IsNullOrEmpty(confType);
                if (bMissinReqData)
                    continue;

                var conf = new ProjectConfiguration();                

                // prepare configuration name and platform
                if (confNamePform.Contains("|"))
                {
                    string[] parts = confNamePform.Split('|');
                    if (parts.Length < 2)
                    {
                        // cannot determine config name and pform
                    }
                    else
                    {
                        conf.Name = parts[0];
                        conf.Platform = parts[1];
                    }
                }
                else
                {
                    conf.Name = confNamePform;                    
                }

                // get output path
                
                confOutDir = confOutDir.Replace("$(SolutionDir)", solnDir);
                confOutDir = confOutDir.Replace("$(ConfigurationName)", conf.Name);

                if (confOutDir.StartsWith(".\\"))
                {
                    confOutDir = confOutDir.Remove(0, 2);                   
                }

                if (!Path.IsPathRooted(confOutDir))
                {
                    confOutDir = Path.Combine(solnDir, confOutDir);
                }

                conf.OutputPath = confOutDir;

                // get type
                /*
                ProjectOutputType outType = ProjectOutputType.CommandLineExe;
                string actualOutName = strOutName;
                switch (int.Parse(confType.Value))
                {
                    case 1:
                        // exe
                        outType = ProjectOutputType.CommandLineExe | ProjectOutputType.WindowsExe;
                        actualOutName += ".exe";
                        break;
                    case 2:
                        outType = ProjectOutputType.DynamicLibrary;
                        actualOutName += ".dll";
                        break;
                    case 4:
                        outType = ProjectOutputType.StaticLibrary;
                        actualOutName += ".lib";
                        break;
                    default:
                        throw new ApplicationException("Cannot determine project type correctly");
                }
                */

                proj.InternalConfigurationList.Add(conf);
            }


            return proj;
        }

        public VcProject()
        {
            Language = ProjectLanguage.CPlusPlus;
            Framework = ProjectFramework.None;
        }

        /*public override string Version
        {
            get
            {
                return LoadVersionFromResource();
            }
            set
            {

            }
        }*/
        public override string Version
        {
            get { throw new NotImplementedException(); }
            set { throw new NotImplementedException(); }
        }
    }
}