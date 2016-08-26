using System;
using System.Runtime.InteropServices;
using Microsoft.Win32.SafeHandles;

namespace GFI.MailSlots
{
    internal class Win32Utils
    {
        [Flags]
        internal enum FileDesiredAccess : uint
        {
            GenericRead = 0x80000000,
            GenericWrite = 0x40000000,
            GenericExecute = 0x20000000,
            GenericAll = 0x10000000
        }

        [Flags]
        internal enum FileShareMode : uint
        {
            Zero = 0x00000000,
            FileShareDelete = 0x00000004,
            FileShareRead = 0x00000001,
            FileShareWrite = 0x00000002
        }

        internal enum FileCreationDisposition : uint
        {
            CreateNew = 1,
            CreateAlways = 2,
            OpenExisting = 3,
            OpenAlways = 4,
            TruncateExisting = 5
        }

        [DllImport("kernel32.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        internal static extern SafeFileHandle CreateFileA(string fileName,
                                            FileDesiredAccess desiredAccess, FileShareMode shareMode,
                                            IntPtr securityAttributes,
                                            FileCreationDisposition creationDisposition,
                                            int flagsAndAttributes, IntPtr hTemplateFile);

        [DllImport("kernel32.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        internal static extern bool WriteFile(SafeFileHandle h,
            IntPtr buf, int length, out uint written, IntPtr ov);


        [DllImport("kernel32.dll", CharSet = CharSet.Auto, CallingConvention = CallingConvention.StdCall)]
        internal static extern SafeFileHandle CreateMailslot(string sName, int nMaxMessageSize, int nReadTimeout, IntPtr lpSecurityAttributes);

        [DllImport("kernel32.dll", CharSet = CharSet.Auto, CallingConvention = CallingConvention.StdCall)]
        internal static extern bool SetMailslotInfo(SafeFileHandle hSlot, int nReadTimeout);

        [DllImport("kernel32.dll", CharSet = CharSet.Auto, CallingConvention = CallingConvention.StdCall)]
        internal static extern bool GetMailslotInfo(SafeFileHandle hSlot, out int maxsize, out int nextSize, out int count, out int nReadTimeout);

        internal const int MAILSLOT_WAIT_FOREVER = -1;
    }
}
