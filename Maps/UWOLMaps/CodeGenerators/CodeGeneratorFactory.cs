using System;
using System.Collections.Generic;
using System.Text;

namespace UWOLMaps.CodeGenerators
{
    class CodeGeneratorFactory
    {
        public static ICodeGenerator CreateGenerator(Version cpuVersion)
        {
            ICodeGenerator result = null;

            // A lo gorrino... :P
            switch (cpuVersion.CPU)
            {
                case CPUVersion.CPC:
                    result = new CPCCodeGenerator();
                    break;
                case CPUVersion.MSX:
                    result = new MSXCodeGenerator();
                    break;
                case CPUVersion.ZX:
                    result = new ZXCodeGenerator();
                    break;
                case CPUVersion.PC:
                    result = new PCCodeGenerator();
                    break;
                default:
                    result = null;
                    break;
            }

            return result;
        }
    }
}
