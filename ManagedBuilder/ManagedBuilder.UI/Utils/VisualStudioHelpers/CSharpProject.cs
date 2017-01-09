// -----------------------------------------------------------------------
// <copyright file="CSharpProject.cs" company="GFI Software Development Ltd.">
//      Copyright (c) GFI Software All rights reserved.
// </copyright>
// -----------------------------------------------------------------------
// -----------------------------------------------------------------------
// <copyright file="CSharpProject.cs" company="GFI Software Development Ltd.">
//      Copyright (c) GFI Software All rights reserved.
// </copyright>
// -----------------------------------------------------------------------
﻿using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using System.IO;

namespace ManagedBuilder.UI.Utils.VisualStudioHelpers
{
    public class CSharpProject :BaseVsProject
    {
        public static CSharpProject Parse(string filename)
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

            var proj = new CSharpProject();

            proj.Filepath = filename;
            proj.SolutionPath = solnDir;

            var node = doc.SelectSingleNode("VisualStudioProject/CSHARP") as XmlElement;

            if (node != null)
            {
                if (!ParseVcs2003Project(proj, doc))
                    return null;
            }
            else
            {
                if (!ParseVcs2005Project(proj, doc))
                    return null;
            }

            return proj;
        }
        
        private static bool ParseVcs2003Project(CSharpProject proj, XmlDocument doc)
        {
            proj.Framework = ProjectFramework.VisualCs2003;

            var node = doc.SelectSingleNode("VisualStudioProject/CSHARP/Build/Settings") as XmlElement;
            if (node == null)
                return false;

            if (node.HasAttribute("AssemblyName"))
                proj.Name = node.Attributes["AssemblyName"].Value;

            if (node.HasAttribute("OutputType"))
            {
                /*
                if (string.Compare(node.InnerText, "Exe", true) == 0)
                    outputType = ProjectOutputType.CommandLineExe;
                else if (string.Compare(node.InnerText, "Winexe", true) == 0)
                    outputType = ProjectOutputType.WindowsExe;
                else if (string.Compare(node.InnerText, "Library", true) == 0)
                    outputType = ProjectOutputType.DynamicLibrary;
                */
            }

            var nodeList = doc.SelectNodes("VisualStudioProject/CSHARP/Build/Settings/*");
            if (nodeList == null)
                return true;

            foreach (XmlElement n in nodeList)
            {
                var conf = new ProjectConfiguration();
                conf.Name = n.HasAttribute("Name") ? n.Attributes["Name"].Value : string.Empty;
                conf.OutputPath = n.HasAttribute("OutputPath") ? n.Attributes["OutputPath"].Value : string.Empty;
                if (!Path.IsPathRooted(conf.OutputPath))
                {
                    var projectDir = Path.GetDirectoryName(proj.Filepath);
                    conf.OutputPath = Path.Combine(projectDir, conf.OutputPath);
                }

                proj.InternalConfigurationList.Add(conf);
            }

            return true;
        }

        private static bool ParseVcs2005Project(CSharpProject proj, XmlDocument doc)        
        {
            proj.Framework = ProjectFramework.VisualCs2005;

            // get assembly name            
            var nodeList = doc.GetElementsByTagName("AssemblyName");
            if (nodeList.Count == 0)
                return false;

            var node = nodeList[0] as XmlElement;
            if (node == null) return false;

            var assemblyName = node.InnerText;
            proj.Name = assemblyName;

            if (string.IsNullOrEmpty(assemblyName))
                return false;

            // get output type
            nodeList = doc.GetElementsByTagName("OutputType");
            if (nodeList.Count == 0)
                return false;

            node = nodeList[0] as XmlElement;
            if (node == null) return false;

            var outputType = ProjectOutputType.None;

            if (string.Compare(node.InnerText, "Exe", true) == 0)
                outputType = ProjectOutputType.CommandLineExe;
            else if (string.Compare(node.InnerText, "Winexe", true) == 0)
                outputType = ProjectOutputType.WindowsExe;
            else if (string.Compare(node.InnerText, "Library", true) == 0)
                outputType = ProjectOutputType.DynamicLibrary;

            switch (outputType)
            {
                case ProjectOutputType.DynamicLibrary:
                    assemblyName += ".dll";
                    break;
                default:
                    assemblyName += ".exe";
                    break;
            }


            //////////////////////////////////////////////////////////////////////////

            var confNodes = doc.GetElementsByTagName("PropertyGroup");
            if (confNodes.Count == 0)
                return false;

            foreach (XmlElement xe in confNodes)
            {
                if (!xe.HasAttribute("Condition"))
                    continue;

                // vs standard for project group condtion is " '$(Configuration)|$(Platform)' == 'Debug|AnyCPU' "
                var conds = xe.Attributes["Condition"].Value.Split(new string[] { "==" }, StringSplitOptions.None);
                if (conds.Length != 2)
                    continue;


                var condVal = conds[1].Trim('\'', ' ').Split('|');
                if (condVal.Length != 2)
                    continue;

                //
                var conf = new ProjectConfiguration();
                conf.Name = condVal[0];
                conf.Platform = condVal[1];                


                
                // get output path
                nodeList = xe.GetElementsByTagName("OutputPath");
                if (nodeList.Count == 0)
                    continue;

                node = nodeList[0] as XmlElement;
                if (node == null)
                    continue;


                var outputLoc = node.InnerText;
                if (!Path.IsPathRooted(outputLoc))
                    outputLoc = Path.Combine(Path.GetDirectoryName(proj.Filepath), outputLoc);

                conf.OutputPath = outputLoc;
                proj.InternalConfigurationList.Add(conf);
            }

            //////////////////////////////////////////////////////////////////////////
            /// 
            return true;
        }

        public CSharpProject()
        {
            Language = ProjectLanguage.CSharp;
            Framework = ProjectFramework.VisualCs2005;
        }

        public override string Version
        {
            get
            {
                return "123.456.789.1011";
            }
            set
            {
                
            }
        }
    }
}