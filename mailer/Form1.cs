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

        private void Send_Click(object sender, EventArgs e)
        {
            Button btn = (Button)sender;
            m_bSending = !m_bSending;
            btn.Text = (m_bSending) ? "Stop" : "Start";
            // continue with creating separate thread to send emails
        }
    }
}
