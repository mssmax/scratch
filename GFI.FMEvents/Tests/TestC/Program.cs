using GFI.MailSlots;
using GFI.MonitorEvents;
using System;
using System.Threading.Tasks;

namespace TestC
{
    class Program
    {
        static MailSlotClient<FMEVENT> ms = new MailSlotClient<FMEVENT>(CONSTANTS.MONITOR_SLOTNAME);
        static void Main(string[] args)
        {
            for (int i = 0;; i++)
            {
                FMEVENT evt = new FMEVENT();
                evt.EventID = i;
                evt.EventTime = DateTime.Now.ToFileTimeUtc();
                ms.SendMessage(evt);
                System.Threading.Thread.Sleep(500);
            }
        }
    }
}
