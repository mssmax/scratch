using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Net;
using System.Net.Mail;
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
        private SmtpClient m_Smtp;

        public Form1()
        {
            InitializeComponent();

            string sServer = System.Configuration.ConfigurationManager.AppSettings["server"];
            int iPort = Convert.ToInt32(System.Configuration.ConfigurationManager.AppSettings["port"]);

            m_Smtp = new SmtpClient(sServer, iPort);
            m_Smtp.EnableSsl = true;
            m_Smtp.DeliveryFormat = SmtpDeliveryFormat.International;

            string s = System.Configuration.ConfigurationManager.AppSettings["defbody"];
            if (!String.IsNullOrEmpty(s))
                txtBodyFile.Text = s;
            s = System.Configuration.ConfigurationManager.AppSettings["defrcpt"];
            if (!String.IsNullOrEmpty(s))
                txtRcptFile.Text = s;
            s = System.Configuration.ConfigurationManager.AppSettings["deffrom"];
            if (!String.IsNullOrEmpty(s))
                txtFrom.Text = s;
            s = System.Configuration.ConfigurationManager.AppSettings["defusername"];
            if (!String.IsNullOrEmpty(s))
                txtUserName.Text = s;
        }

        private string GetSelectedFileName(string sPrevText)
        {
            using (OpenFileDialog dlg = new OpenFileDialog())
            {
                dlg.Multiselect = false;
                dlg.InitialDirectory = Environment.CurrentDirectory;
                dlg.FileName = sPrevText;
                if (dlg.ShowDialog() == DialogResult.OK)
                {
                    return dlg.FileName;
                }
                else
                {
                    return sPrevText;
                }
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
            Random rnd = new Random();

            string sBody = txtBodyFile.Text;

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

            string sUser = txtUserName.Text;
            string sPassword = txtPassword.Text;
            m_Smtp.Credentials = new NetworkCredential(sUser, sPassword);

            string sFrom = txtFrom.Text;
            string sSubject = txtSubject.Text;

            int iBatchSize = (int)udBatchSize.Value;
            int iEmailDelay = (int)udBetweenEmails.Value;
            int iBatchDelay = (int)udBetweenBatches.Value;
            int iMaxBCC = (int)udMaxBCC.Value;

            int i = 1;
            MailAddressCollection rcpts = new MailAddressCollection();
            foreach(string sRcpt in lstRcpts)
            {
                string sText = String.Empty;
                bool bBatchDelay = ((i % iBatchSize) == 0);

                sText = String.Format("Sending to {0} ( {1} of {2} )...", sRcpt, i, lstRcpts.Count);
                rcpts.Add(sRcpt);
                if (bBatchDelay)
                {
                    sText += "and pausing between batches...";
                }

                lblProgress.Invoke((MethodInvoker)delegate
                {
                    lblProgress.Text = sText;
                });

                // send the email to the recipient
                if ((i % iMaxBCC) == 0)
                {
                    SendEmail(sFrom, rcpts, sSubject, sBody);
                    rcpts = new MailAddressCollection();
                    // no need to sleep over the last recipient
                    if (i < lstRcpts.Count)
                        Thread.Sleep((bBatchDelay) ? iBatchDelay * 1000 * 60 : rnd.Next(iEmailDelay * 1000));
                }
                ++i;
            }

            // send to the remaining recipients
            if (rcpts.Count > 0)
            {
                SendEmail(sFrom, rcpts, sSubject, sBody);
            }

            string errs = Environment.CurrentDirectory + "\\errors.txt";
            if (File.Exists(errs))
            {
                MessageBox.Show(String.Format("There were errors while sending the mailout, please check {0} for more information", errs), "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }

            StopSending();
        }

        private void SendEmail(string sFrom, MailAddressCollection rcpts, string sSubject, string sBody)
        {
            try
            {
                using (MailMessage msg = new MailMessage())
                {
                    foreach (var rcpt in rcpts)
                    {
                        msg.Bcc.Add(rcpt);
                    }

                    msg.From = new MailAddress(sFrom);
                    msg.Subject = sSubject;
                    msg.SubjectEncoding = Encoding.UTF8;
                    string sExt = Path.GetExtension(sBody).TrimStart('.');

                    if (sExt == "txt")
                        sExt = "plain";

                    AlternateView view = new AlternateView(sBody, "text/" + sExt);
                    msg.AlternateViews.Add(view);
                    msg.BodyEncoding = Encoding.UTF8;

                    //add attachments if any
                    foreach(string att in lstAtts.Items)
                    {
                        // TODO: look into disposing of attachments properly since it seems to implement Dispose
                        msg.Attachments.Add(new Attachment(att));
                    }

                    m_Smtp.Send(msg);
                }
            }
            catch(Exception e)
            {
                LogError(rcpts, e);
            }
        }

        private void LogError(MailAddressCollection rcpts, Exception e)
        {
            //TODO: look into more efficient way of logging, e.g. without re-opening the file all the time
            using (TextWriter w = new StreamWriter("errors.txt", true))
            {
                if (rcpts != null)
                {
                    foreach (var rcpt in rcpts)
                    {
                        w.WriteLine(String.Format("Failed to send mail to [[[ {0} ]]], error === {1} ===", rcpt.Address, e));
                        w.WriteLine("===========================================================================================================");
                    }
                }
                else
                {
                    w.WriteLine(String.Format("General failure, error === {0} ===", e));
                    w.WriteLine("===========================================================================================================");
                }
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
                    ctrl.Invoke((MethodInvoker)delegate
                    {
                        ctrl.Enabled = v;
                    });                    
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
            txtBodyFile.Text = GetSelectedFileName(txtBodyFile.Text);
        }

        private void btnSelectRcpt_Click(object sender, EventArgs e)
        {
            txtRcptFile.Text = GetSelectedFileName(txtRcptFile.Text);
        }

        private void Form1_FormClosed(object sender, FormClosedEventArgs e)
        {
            if (m_Thread != null)
            {
                m_Thread.Abort();
                m_Thread.Join();
            }
        }

        private void lstAtts_DoubleClick(object sender, EventArgs e)
        {
            string sFile = GetSelectedFileName(String.Empty);
            string sFileName = Path.GetFileName(sFile);

            lstAtts.Items.Add(sFile);
        }

        private void lstAtts_KeyUp(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Delete)
            {
                for(; lstAtts.SelectedIndices.Count > 0;)
                {
                    lstAtts.Items.Remove(lstAtts.SelectedItem);
                }
            }
        }
    }
}
