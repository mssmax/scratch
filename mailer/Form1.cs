using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
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

            string sRcpts = txtRcptFile.Text;
            string sBody = txtBodyFile.Text;
            string sFrom = txtFrom.Text;
            string sUser = txtUserName.Text;
            string sPassword = txtPassword.Text;
            string sSubject = txtSubject.Text;

            int iBatchSize = (int)udBatchSize.Value;
            int iEmailDelay = (int)udBetweenEmails.Value;
            int iBatchDelay = (int)udBetweenBatches.Value;

            for (int i = 1; i < 100; i++)
            {
                string sText = String.Empty;
                bool bBatchDelay = ((i % iBatchSize) == 0);

                sText = String.Format("Sending to {0}...", i);
                if (bBatchDelay)
                {
                    sText += "and pausing between batches...";
                }

                lblProgress.Invoke((MethodInvoker)delegate
                {
                    lblProgress.Text = sText;
                });

                // send the email to the recipient

                Thread.Sleep((bBatchDelay) ? iBatchDelay * 1000 * 60 : rnd.Next(iEmailDelay * 1000));
            }
        }

        private void Send_Click(object sender, EventArgs e)
        {
            Button btn = (Button)sender;
            m_bSending = !m_bSending;
            btn.Text = (m_bSending) ? "Stop" : "Start";
            if (m_bSending)
            {
                // continue with creating separate thread to send emails
                m_Thread = new Thread(SenderThread);
                m_Thread.Start();
            }
            else
            {
                m_Thread.Abort();
                m_Thread.Join();
                m_Thread = null;
                lblProgress.Text = String.Empty;
            }
        }

        private void btnSelectBody_Click(object sender, EventArgs e)
        {
            txtBodyFile.Text = GetSelectedFileName();
        }

        private void btnSelectRcpt_Click(object sender, EventArgs e)
        {
            txtRcptFile.Text = GetSelectedFileName();
        }
    }
}
