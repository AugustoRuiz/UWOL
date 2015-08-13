using System;
using System.Collections.Generic;
using System.Text;
using System.IO;

namespace UWOLMaps.CodeGenerators
{
    public interface ICodeGenerator
    {
        void CreateCode(Levels levels, Stream stream);
        string GetFileName(string baseName);
    }
}
