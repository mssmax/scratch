using GFI.MailSlots;
using GFI.MonitorEvents;
using System;
using System.IO;
using System.Threading;
using System.Threading.Tasks;

namespace EventProc
{
    internal class EventCollector
    {
        // TODO: can be defined as something more unique
        // gets posted to break the blocking read from the mailslot
        private const int cQUITEvent = 0xeeee;

        private MailSlotServer<FMEVENT> m_MSServer;
        private Thread m_ReadThread;
        bool m_bExit = false;

        public OnEventReceived FireEventReceived
        {
            set; private get;
        }

        public void Start()
        {
            m_MSServer = new MailSlotServer<FMEVENT>(CONSTANTS.MONITOR_SLOTNAME);
            m_ReadThread = new Thread(new ThreadStart(ReadEvents));
            m_ReadThread.Start();
        }

        public void Stop()
        {
            m_bExit = true;
            FMEVENT quitEvt = new FMEVENT();
            quitEvt.EventID = cQUITEvent;
            MailSlotClient<FMEVENT> mc = new MailSlotClient<FMEVENT>(CONSTANTS.MONITOR_SLOTNAME);
            mc.SendMessage(quitEvt);
            m_ReadThread.Join();
            m_MSServer.Dispose();
            m_MSServer = null;
        }

        private void ReadEvents()
        {
            for (; !m_bExit;)
            {
                try
                {
                    FMEVENT evt = m_MSServer.Read();
                    if (evt.EventID == cQUITEvent)
                        break;

                    if (FireEventReceived != null)
                        ThreadPool.QueueUserWorkItem(new WaitCallback(FireEventReceived), evt);
                }
                catch(IOException)
                {
                    // this one is caught in case the mailslot was configured with finite timeout
                    // in which case it will throw a "semapore timeout" exception
                    // TODO: filter only "semaphore timeout exception, for everything else re-throw or log
                }
            }
        }
    }
}
