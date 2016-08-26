using GFI.MonitorEvents;
using System;
using System.Threading;
using System.Threading.Tasks;

namespace EventProc
{
    internal class EventXFormer
    {
        public OnEventProcessed FireEventProcessed
        {
            private get; set;
        }

        public void EventReceived(object o)
        {
            FMEVENT evt = o as FMEVENT;
            // DEBUG
            Console.WriteLine();
            Console.WriteLine("Event received from collector:");
            Console.WriteLine("ID: {0}", evt.EventID);
            Console.WriteLine("Time: {0}", DateTime.FromFileTimeUtc(evt.EventTime));
            Console.WriteLine("Category: {0}", evt.EventCategory);
            Console.WriteLine("Module: {0}", evt.EventModuleType);
            Console.WriteLine("SubModule: {0}", evt.EventSubModule);
            Console.WriteLine("AdditionalInfo: {0}", evt.EventAdditionalInfo);
            //DEBUG
            if(FireEventProcessed != null)
                ThreadPool.QueueUserWorkItem(new WaitCallback(FireEventProcessed), evt);
        }

        public void Init()
        {
            // TODO
        }

        public void Uninit()
        {
            // TODO
        }
    }
}
