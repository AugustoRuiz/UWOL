using System;
using System.Collections.Generic;
using System.Text;
using System.ComponentModel;
using System.Xml.Serialization;
using System.IO;

namespace UWOLMaps
{
    public class Levels : BindingList<Level>
    {
        public static Levels Load(string fileName)
        {
            Levels loadedLevels;
            XmlSerializer formatter = new XmlSerializer(typeof(Levels));
            Stream objNewFileStream = new FileStream(fileName, FileMode.Open, FileAccess.Read, FileShare.Read);
            loadedLevels = (Levels)formatter.Deserialize(objNewFileStream);
            objNewFileStream.Close();

            return loadedLevels;
        }

        public void Save(string fileName)
        {
            XmlSerializer formatter = new XmlSerializer(typeof(Levels));
            Stream objFileStream = new FileStream(fileName, FileMode.Create, FileAccess.Write, FileShare.None);
            formatter.Serialize(objFileStream, this);
            objFileStream.Close();
        }
    }
}
