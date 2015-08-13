using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

namespace UWOLMaps
{
    public class ErrorDumper
    {
        [DllImport("clrdump.dll", CharSet = CharSet.Unicode, SetLastError = true)]
        internal static extern Int32 CreateDump(Int32 ProcessId, string FileName,
            MinidumpType DumpType, Int32 ExcThreadId, IntPtr ExtPtrs);

        [DllImport("clrdump.dll", CharSet = CharSet.Unicode, SetLastError = true)]
        internal static extern Int32 RegisterFilter(string FileName, MinidumpType DumpType);

        [DllImport("clrdump.dll", SetLastError = true)]
        internal static extern Int32 UnregisterFilter();

        [DllImport("clrdump.dll")]
        internal static extern Int32 SetFilterOptions(Int32 Options);
    }
}
