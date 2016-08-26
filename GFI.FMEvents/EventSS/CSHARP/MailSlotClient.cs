using System;
using System.ComponentModel;
using System.IO;
using System.Runtime.InteropServices;
using Microsoft.Win32.SafeHandles;

namespace GFI.MailSlots
{
    public class MailSlotClient<MSGTYPE>
    {
        // special reserved event id to signal the listening server to stop listening and bail out
        public const int cQuitEventID = 0xeeee;
        private string m_sSlotName;

        public MailSlotClient(string sSlotName)
        {
            m_sSlotName = sSlotName;
        }

        public void SendMessage(MSGTYPE msg)
        {
            SafeFileHandle h = Win32Utils.CreateFileA(m_sSlotName, Win32Utils.FileDesiredAccess.GenericWrite,
                Win32Utils.FileShareMode.FileShareRead, IntPtr.Zero, Win32Utils.FileCreationDisposition.OpenExisting, 0, IntPtr.Zero);

            if (h.IsInvalid)
            {
                // TODO: see what to dod here, thechnically it means the server closed the connection
                // so the event is going to be lost anyway
                //throw new Win32Exception();
            }
            
            byte[] rawBytes = new byte[Marshal.SizeOf(typeof(MSGTYPE))];
            GCHandle gc = GCHandle.Alloc(rawBytes, GCHandleType.Pinned);
            try
            {
                IntPtr pData = gc.AddrOfPinnedObject();
                Marshal.StructureToPtr(msg, pData, true);
                uint dummy = 0;
                bool b = Win32Utils.WriteFile(h, pData, rawBytes.Length, out dummy, IntPtr.Zero);
            }
            finally
            {
                h.Close();
                gc.Free();
            }
        }
    }
}
