using System;
using System.Collections.Generic;
using System.Text;
using System.IO;

namespace UWOLMaps.CodeGenerators
{
    public class CPCCodeGenerator : ICodeGenerator
    {
        #region ICodeGenerator Members

        public void CreateCode(Levels levels, System.IO.Stream stream)
        {
            foreach (Level nivel in levels)
            {
                string code;
                nivel.Ordena();
                code = nivel.ToString(Version.DameVersion(CPUVersion.CPC));
                stream.Write(Encoding.UTF8.GetBytes(code), 0, Encoding.UTF8.GetByteCount(code));
            }
        }

        public string GetFileName(string baseName)
        {
            return string.Format("CPC{0}.asm", baseName);
        }

        #endregion
    }
}
