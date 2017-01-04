using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Net;
using System.Net.Mail;

namespace mailer
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
/*
            SmtpClient c = new SmtpClient("smtp.gmail.com", 587);
            c.Credentials = new NetworkCredential("mssmax@gmail.com", "papaxray");
            c.EnableSsl = true;
            c.Send("mssmax@gmail.com", "olpinika@gmail.com", "From Malta with love", "Hey, join our corporation. С русским текстом.");
*/
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new Form1());
        }
    }
}
