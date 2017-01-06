using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace mailer
{
    public partial class Form1 : Form
    {
        private bool m_bSending;
        private Thread m_Thread;

        public Form1()
        {
            InitializeComponent();
        }

        private string GetSelectedFileName()
        {
            OpenFileDialog dlg = new OpenFileDialog();
            dlg.Multiselect = false;
            if (dlg.ShowDialog() == DialogResult.OK)
            {
                return dlg.FileName;
            }
            else
            {
                return String.Empty;
            }
        }

        private void RecolorTextBox(object sender, EventArgs e)
        {
            
            if (IsValidInput())
            {
                btnSend.Enabled = true;
            }
            else
            {
                btnSend.Enabled = false;
            }
        }

        private bool IsValidInput()
        {
            // check that all the text boxes are filled in
            int i = txtUserName.Text.Length;
            i *= txtPassword.Text.Length;
            i *= txtFrom.Text.Length;
            i *= txtSubject.Text.Length;
            i *= txtBodyFile.Text.Length;
            i *= txtRcptFile.Text.Length;
            i *= (int)udBatchSize.Value;

            return i != 0;
        }

        private void SenderThread()
        {
            string sServer = System.Configuration.ConfigurationManager.AppSettings["server"];
            int iPort = Convert.ToInt32(System.Configuration.ConfigurationManager.AppSettings["port"]);
            Random rnd = new Random();

            string sBody = String.Empty;
            try
            {
                using (TextReader r = new StreamReader(txtBodyFile.Text))
                {
                    sBody = r.ReadToEnd();
                }
            }
            catch(Exception e)
            {
                MessageBox.Show(String.Format("Exception occurred while reading body file: {0}", e), "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                StopSending();
                return;
            }

            List<string> lstRcpts = new List<string>();

            try
            {
                using (TextReader r = new StreamReader(txtRcptFile.Text))
                {
                    for (;;)
                    {
                        string s = r.ReadLine();
                        if (String.IsNullOrEmpty(s))
                            break;
                        else
                            lstRcpts.Add(s);
                    }
                }
            }
            catch(Exception e)
            {
                MessageBox.Show(String.Format("Exception occurred while reading recipients file: {0}", e), "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                StopSending();
                return;
            }

            string sFrom = txtFrom.Text;
            string sUser = txtUserName.Text;
            string sPassword = txtPassword.Text;
            string sSubject = txtSubject.Text;

            int iBatchSize = (int)udBatchSize.Value;
            int iEmailDelay = (int)udBetweenEmails.Value;
            int iBatchDelay = (int)udBetweenBatches.Value;

            int i = 1;
            foreach(string sRcpt in lstRcpts)
            {
                string sText = String.Empty;
                bool bBatchDelay = ((i % iBatchSize) == 0);
                ++i;

                sText = String.Format("Sending to {0}...", sRcpt);
                if (bBatchDelay)
                {
                    sText += "and pausing between batches...";
                }

                lblProgress.Invoke((MethodInvoker)delegate
                {
                    lblProgress.Text = sText;
                });

                // send the email to the recipient
                SendEmail(sFrom, sRcpt, sUser, sPassword, sBody);

                Thread.Sleep((bBatchDelay) ? iBatchDelay * 1000 * 60 : rnd.Next(iEmailDelay * 1000));
            }
        }

        private void SendEmail(string sFrom, string sRcpt, string sUser, string sPassword, string sBody)
        {
            try
            {

            }
            catch(Exception e)
            {
                LogError(sRcpt, e);
            }
        }

        private void LogError(string sRcpt, Exception e)
        {
            //TODO: look into more efficient way of logging, e.g. without re-opening the file all the time
            using (TextWriter w = new StreamWriter("errors.txt", true))
            {
                w.WriteLine(String.Format("Failed to send mail to {0}, error {1}", sRcpt, e));
            }
        }

        private void Send_Click(object sender, EventArgs e)
        {
            m_bSending = !m_bSending;
            
            if (m_bSending)
            {
                // continue with creating separate thread to send emails
                StartSending();
            }
            else
            {
                StopSending();
            }
        }

        private void StopSending()
        {
            EnableControls(true);

            m_bSending = false;

            lblProgress.Invoke((MethodInvoker)delegate
            {
                lblProgress.Text = String.Empty;
            });
            
            btnSend.Invoke((MethodInvoker)delegate
            {
                btnSend.Text = (m_bSending) ? "Stop" : "Start";
            });

            if (Thread.CurrentThread != m_Thread)
            {
                m_Thread.Abort();
                m_Thread.Join();
            }

            m_Thread = null;
        }

        private void EnableControls(bool v)
        {
            foreach(Control ctrl in this.Controls)
            {
                if(ctrl != btnSend && ctrl != lblProgress)
                {
                    ctrl.Enabled = v;
                }
            }
        }

        private void StartSending()
        {
            EnableControls(false);

            m_bSending = true;

            btnSend.Invoke((MethodInvoker)delegate
            {
                btnSend.Text = (m_bSending) ? "Stop" : "Start";
            });

            m_Thread = new Thread(SenderThread);
            m_Thread.Start();
        }

        private void btnSelectBody_Click(object sender, EventArgs e)
        {
            txtBodyFile.Text = GetSelectedFileName();
        }

        private void btnSelectRcpt_Click(object sender, EventArgs e)
        {
            txtRcptFile.Text = GetSelectedFileName();
        }

        private void Form1_FormClosed(object sender, FormClosedEventArgs e)
        {
            if (m_Thread != null)
            {
                m_Thread.Abort();
                m_Thread.Join();
            }
        }
    }
}
