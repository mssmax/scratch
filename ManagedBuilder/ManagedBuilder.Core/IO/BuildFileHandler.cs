// -----------------------------------------------------------------------
// <copyright file="BuildFileHandler.cs" company="GFI Software Development Ltd.">
//      Copyright (c) GFI Software All rights reserved.
// </copyright>
// -----------------------------------------------------------------------
using System.IO;
using System.Xml;
using System.Xml.Schema;
using System.Xml.Serialization;
using ManagedBuilder.Core.Model;

namespace ManagedBuilder.Core.IO
{
    public static class BuildFileHandler
    {
        public static BuildFile Load(string path)
        {            
            BuildFile pf;
            FileStream fs = null;

            try
            {                
                fs = new FileStream(path, FileMode.Open, FileAccess.Read, FileShare.Read);
                var bfxSerializer = new BuildFileXmlSerializer();
                var rdr = new XmlTextReader(fs);
                rdr.MoveToContent();
                bfxSerializer.ReadXml(rdr);
                pf = bfxSerializer.File;
            }
            finally
            {
                if (fs != null)
                    fs.Close();
            }

            return pf;
        }        

        public static void Save(string path, BuildFile pf)
        {                        
            FileStream fs = null;
            try
            {                
                fs = new FileStream(path, FileMode.Create, FileAccess.Write, FileShare.Write);
                var writer = new XmlTextWriter(fs, null);
                var bfxSerializer = new BuildFileXmlSerializer(pf);
                bfxSerializer.WriteXml(writer);
                writer.Flush();
                writer.Close();

            }
            finally
            {
                if (fs != null)
                    fs.Close();
            }
        }
    }
}