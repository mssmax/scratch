using FirebirdSql.Data.FirebirdClient;
using GFI.MonitorEvents;
using System;
using System.IO;
using System.Threading.Tasks;

namespace EventProc
{
    internal class EventWriter : IDisposable
    {
        private FbConnection m_DBConn;
        private FbCommand m_DBCommand;

        public void EventProcessed(object o)
        {
            FMEVENT evt = o as FMEVENT;
            if (m_DBCommand != null)
            {
                m_DBCommand.Parameters["@eid"].Value = evt.EventID;
                m_DBCommand.Parameters["@etime"].Value = DateTime.FromFileTimeUtc(evt.EventTime);
                m_DBCommand.Parameters["@emod"].Value = evt.EventModuleType;
                m_DBCommand.Parameters["@esubmod"].Value = evt.EventSubModule;
                m_DBCommand.Parameters["@eauxinfo"].Value = evt.EventAdditionalInfo;
                m_DBCommand.Parameters["@ecat"].Value = evt.EventCategory;
                m_DBCommand.ExecuteNonQuery();
            }
        }

        private void OpenOrCreateDatabase()
        {
            FbConnectionStringBuilder bld = new FbConnectionStringBuilder();
            bld["initial catalog"] = "fmevents.fdb";
            bld.UserID = "SysDBA";
            bld.Password = "masterkey";
            bld.ServerType = FbServerType.Embedded;

            // TODO: revise the name and path to the database
            if (File.Exists("fmevents.fdb"))
            {
                m_DBConn = new FbConnection(bld.ConnectionString);
                m_DBConn.Open();

                return;
            }
            else
            {
                FbConnection.CreateDatabase(bld.ConnectionString);
                // now create the database table(s)
                m_DBConn = new FbConnection(bld.ConnectionString);
                m_DBConn.Open();
                using (FbCommand cmd = m_DBConn.CreateCommand())
                {
                    cmd.CommandType = System.Data.CommandType.Text;
                    cmd.CommandText = @"create table fmevents(evtid integer not null, evttime timestamp not null, 
                                    evtcategory int not null, 
                                    evtmodule int not null, 
                                    evtsubmodule varchar(256), 
                                    evtauxinfo varchar(2048))";
                    cmd.ExecuteNonQuery();
                }
            }
        }
        private void PrepareCommand()
        {
            m_DBCommand = m_DBConn.CreateCommand();

            m_DBCommand.CommandType = System.Data.CommandType.Text;
            m_DBCommand.CommandText = @"insert into fmevents(evtid, evttime, evtcategory, evtmodule, evtsubmodule, evtauxinfo)
                                        values(@eid, @etime, @ecat, @emod, @esubmod, @eauxinfo)";

            m_DBCommand.Parameters.Add("@eid", null);
            m_DBCommand.Parameters.Add("@etime", null);
            m_DBCommand.Parameters.Add("@ecat", null);
            m_DBCommand.Parameters.Add("@emod", null);
            m_DBCommand.Parameters.Add("@esubmod", null);
            m_DBCommand.Parameters.Add("@eauxinfo", null);
        }

        public void Init()
        {
            OpenOrCreateDatabase();
            PrepareCommand();
        }

        public void Uninit()
        {
            m_DBConn.Close();
            m_DBConn = null;
            Dispose();
        }

        public void Dispose()
        {
            if(m_DBCommand != null)
            {
                m_DBCommand.Dispose();
                m_DBCommand = null;
            }
        }
    }
}
