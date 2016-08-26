using System;
using System.IO;
using System.Runtime.InteropServices;
using System.Threading.Tasks;
using Microsoft.Win32.SafeHandles;
using System.ComponentModel;

namespace GFI.MailSlots
{
    public class MailSlotServer<MSGTYPE> : IDisposable
    {
        FileStream m_stream = null;
        int m_nMaxMsgSize = 0;

        public MailSlotServer(string name, int readTimeout = Win32Utils.MAILSLOT_WAIT_FOREVER)
        {
            m_nMaxMsgSize = Marshal.SizeOf(typeof(MSGTYPE));
            SafeFileHandle hSlot = Win32Utils.CreateMailslot(name, m_nMaxMsgSize, 0, IntPtr.Zero);

            if (hSlot.IsInvalid)
                throw new Win32Exception();

            // initial attaching of FileStream object to the mailslot seems to issue some sort of IO
            // which blocks there in the case mailslot was created with wait timeout foverever. 
            // By creating the slot with with timeout 0 we force FileStream IO to complete immediatelly and finish
            // its initialization therefore allowing us to leave the MailSlotServer constructor to continue with
            // other tasks but before exiting, we switch the mailslot back to wait time out forever to simplify
            // data reading in the upper layers of the code that makes use of this class
            // P.S. This is just a theory which needs deeper investigation to be confirmed
            m_stream = new FileStream(hSlot, FileAccess.Read);

            Win32Utils.SetMailslotInfo(hSlot, readTimeout);
        }
  
        public MSGTYPE Read()
        {
            byte[] buffer = new byte[m_nMaxMsgSize];
            int bytesRead = m_stream.Read(buffer, 0, buffer.Length);

            MSGTYPE msg;
            GCHandle gcHndl = GCHandle.Alloc(buffer, GCHandleType.Pinned);
            try
            {
                msg = (MSGTYPE)Marshal.PtrToStructure(gcHndl.AddrOfPinnedObject(), typeof(MSGTYPE));
            }
            finally
            {
                gcHndl.Free();
            }

            return msg;
        }

        public void Dispose()
        {
            if (m_stream != null)
            {
                m_stream.Close();
                m_stream = null;
            }
        }
    }
}
