using System;
using System.Runtime.InteropServices;

namespace GFI.MonitorEvents
{
    public static class CONSTANTS
    {
        public static string MONITOR_SLOTNAME = "\\\\.\\mailslot\\gfifm\\mntrevents";
    }

    public enum FMEMODULETYPE
    {
        FMTNONE = 0,
        FMTSERVER = 1,
        FMTMTA = 2,
        FMTATTENDANT = 3
    }

    public enum FMECATEGORY
    {
        CATNONE = 0,
        CATINFO = 1,
        CATWARNING = 2,
        CATERROR = 3
    }

    [StructLayout(LayoutKind.Sequential, Pack = 1, CharSet = CharSet.Unicode)]
    public class FMEVENT
    {
        public FMEVENT()
        {
            // just set the time to save the hassle
            EventTime = DateTime.Now.ToFileTimeUtc();
        }
        // represents the FILETIME structure with the time ( in UTC ) the event was submitted on
        public Int64 EventTime;
        // ID of the event which is used to retrieve localizable text to be displayed in the monitor
        public Int32 EventID;
        // Event category, such as error, warning or information, barely used right now, but introduced for the future
        public FMECATEGORY EventCategory;
        // Represents the module which logged the event
        public FMEMODULETYPE EventModuleType;
        // Literal string used by indivudual modules to distninguish themselves ( non-localizable )
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 256)]
        public string EventSubModule;
        // Ady addtional informarmation the logging module deems useful
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 2048)]
        public string EventAdditionalInfo;
    }
}
