using System;
using System.Threading.Tasks;
using GFI.MailSlots;
using GFI.MonitorEvents;
using System.Threading;

namespace EventProc
{
    internal delegate void OnEventReceived(object o);
    internal delegate void OnEventProcessed(object o);

    public class EventProcessor
    {
        private EventCollector m_Collector = new EventCollector();
        private EventXFormer m_EventXFormer = new EventXFormer();
        private EventWriter m_Writer = new EventWriter();

        public EventProcessor()
        {
        }

        public void Init()
        {
            m_Collector.FireEventReceived = m_EventXFormer.EventReceived;
            m_EventXFormer.FireEventProcessed = m_Writer.EventProcessed;

            // TODO: potentially here we init xFormer and Writer
            m_EventXFormer.Init();
            m_Writer.Init();
        }

        public void Uninit()
        {
            m_Collector.FireEventReceived = null;
            m_EventXFormer.FireEventProcessed = null;

            m_EventXFormer.Uninit();
            m_Writer.Uninit();
        }

        public void Start()
        {
            m_Collector.Start();
        }

        public void Stop()
        {
            m_Collector.Stop();
        }
    }
}
