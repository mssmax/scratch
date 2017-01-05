using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace mailer
{
    public partial class Form1 : Form
    {
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
            TextBox txt = sender as TextBox;
            if (txt.Text.Length > 0)
            {
                txt.BackColor = Color.LightGreen;
            }
            else
            {
                txt.BackColor = Color.Red;
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

        private void Send_Click(object sender, EventArgs e)
        {
            if (IsValidInput())
            {
                Button btn = (Button)sender;
                m_bSending = !m_bSending;
                btn.Text = (m_bSending) ? "Stop" : "Start";
                // continue with creating separate thread to send emails
                string sServer = System.Configuration.ConfigurationManager.AppSettings["server"];
                int iPort = Convert.ToInt32(System.Configuration.ConfigurationManager.AppSettings["port"]);
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
