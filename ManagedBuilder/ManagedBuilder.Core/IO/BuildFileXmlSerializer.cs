// -----------------------------------------------------------------------
// <copyright file="BuildFileXmlSerializer.cs" company="GFI Software Development Ltd.">
//      Copyright (c) GFI Software All rights reserved.
// </copyright>
// -----------------------------------------------------------------------
using System;
using System.Text;
using System.Xml;
using System.Xml.Schema;
using System.Xml.Serialization;
using ManagedBuilder.Core.Model;

namespace ManagedBuilder.Core.IO
{    
    public class BuildFileXmlSerializer
    {
        public BuildFile File
        {
            get; set;
        }

        #region c'tor
        public BuildFileXmlSerializer() : this(new BuildFile()) { }

        public BuildFileXmlSerializer(BuildFile buildFile)
        {
            File = buildFile;
        }
        #endregion

        public XmlSchema GetSchema()
        {
            throw new NotImplementedException();
        }

        public void ReadXml(XmlTextReader reader)
        {            
            while (reader.NodeType != XmlNodeType.EndElement)
            {
                // TODO: determine if we are parsing a ProjectsFile or a BuildsFile

                switch (reader.Name.ToLower())
                {
                    case "project":
                        var p = ParseProjectEntryXml(reader);
                        File.Projects.Add(p);
                        break;
                    default:
                        if (reader.NodeType == XmlNodeType.Comment)
                            reader.MoveToContent();
                        else
                            reader.ReadString();
                        break;
                }
            }
        }

        private static Project ParseProjectEntryXml(XmlReader reader)
        {
            var p = new Project();

            // first step: read the name of the project from the start element
            if (!reader.MoveToAttribute("Name"))
            {
                // TODO: handle missing name

                return null;
            }
            
            p.Name = reader.Value;

            // second step: move out of the project (start) element and go forward to the children
            //while (reader.NodeType != XmlNodeType.EndElement)
            reader.ReadStartElement("Project");
            while (reader.IsStartElement())
            {                
                var xmlElementName = reader.Name;
                switch (xmlElementName.ToLower())
                {
                    case "framework":
                        p.Framework = reader.ReadElementString();
                        break;
                    case "buildopt":
                        p.BuildOpt = reader.ReadElementString();
                        break;
                    case "path":
                        p.Path = reader.ReadElementString();
                        break;
                    case "sccpath":
                        p.SccPath = reader.ReadElementString();
                        break;
                    case "output":
                        var outputEntry = new Output();

                        if (reader.MoveToAttribute("DestPath"))
                            outputEntry.DestPath = reader.Value;

                        outputEntry.PathList.AddRange(
                            reader.ReadElementString().Split(';'));

                        p.Outputs.Add(outputEntry);
                        break;
                    case "dependencies":
                        p.Dependencies.Clear();
                        var depString = reader.ReadElementString();
                        if (depString.Trim().Length > 0)
                            p.Dependencies.AddRange(depString.Trim().Split(';'));
                        break;
                    default:
                        reader.Skip();
                        break;
                }
            }

            reader.ReadEndElement();

            //
            return p;
        }

        public void WriteXml(XmlWriter writer)
        {
            // <projectsfile>
            writer.WriteStartElement("ProjectsFile");

            // <projects>
            writer.WriteWhitespace("\r\n  ");
            writer.WriteStartElement("Projects");            

            foreach (var p in File.Projects)
            {
                // <project>
                writer.WriteWhitespace("\r\n    ");
                WriteProjectEntryXml(writer, p);
            }

            // </projects>
            writer.WriteWhitespace("\r\n  ");
            writer.WriteEndElement();

            // </projectsfile>
            writer.WriteWhitespace("\r\n");
            writer.WriteEndElement();
        }

        private void WriteProjectEntryXml(XmlWriter writer, Project project)
        {
            var sb = new StringBuilder();

            writer.WriteStartElement("Project");            
            
            // write name
            writer.WriteAttributeString("Name", project.Name);

            writer.WriteWhitespace("\r\n      ");
            writer.WriteElementString("Framework", project.Framework);
            
            writer.WriteWhitespace("\r\n      ");
            writer.WriteElementString("BuildOpt", project.BuildOpt);

            writer.WriteWhitespace("\r\n      ");
            writer.WriteElementString("Path", project.Path);

            writer.WriteWhitespace("\r\n      ");
            writer.WriteElementString("Platform", project.PlatformName);

            writer.WriteWhitespace("\r\n      ");
            writer.WriteElementString("Configuration", project.ConfigurationName);

            writer.WriteWhitespace("\r\n      ");
            writer.WriteElementString("SccPath", project.SccPath);            

            // write outputs
            foreach (var output in project.Outputs)
            {
                writer.WriteWhitespace("\r\n      ");
                writer.WriteStartElement("Output");

                writer.WriteAttributeString("DestPath", output.DestPath);

                sb.Length = 0;
                foreach (var s in output.PathList)
                {
                    if (sb.Length > 0) sb.Append(";");
                    sb.Append(s);
                }

                writer.WriteString(sb.ToString());

                writer.WriteEndElement();                
            }

            // write dependencies
            sb.Length = 0;
            foreach (var s in project.Dependencies)
            {
                if (sb.Length > 0) sb.Append(";");
                sb.Append(s);
            }
            writer.WriteWhitespace("\r\n      ");
            writer.WriteElementString("Dependencies", sb.ToString());
            
            writer.WriteWhitespace("\r\n    ");
            writer.WriteEndElement();
        }
    }
}