using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net;
using System.Net.Sockets;
using System.IO;

namespace smtpsrv
{
    class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine("Starting SMTP server...");
            TcpListener server = null;
            try
            {
                Directory.CreateDirectory(@".\MailDrop");

                server = new TcpListener(IPAddress.Parse("127.0.0.1"), 25);
                server.Start();
                while(true)
                {
                    Console.WriteLine("Waiting for connection...");
                    TcpClient client = server.AcceptTcpClient();

                    Console.WriteLine("Accepted connection from {0}", client.Client.LocalEndPoint);

                    NetworkStream inStream = client.GetStream();
                    byte[] buffer = new byte[16384];
                    using (FileStream outStream = new FileStream(@".\MailDrop\" + Guid.NewGuid().ToString() + ".eml", FileMode.CreateNew))
                    {
                        int bytesRead = 0;
                        while((bytesRead = inStream.Read(buffer, 0, buffer.Length)) != 0)
                        {
                            Console.WriteLine(Encoding.ASCII.GetString(buffer, 0, bytesRead));
                            if (buffer[0] == '.' && buffer[1] == 0x0D && buffer[2] == 0x0A)
                                break;
                            
                            outStream.Write(buffer, 0, bytesRead);
                        }
                    }

                    client.Close();
                }
            }
            catch (Exception e)
            {
                Console.WriteLine("Exception occurred {0}", e);
            }

            Console.WriteLine("Stopping SMTP server...");
            if (server != null)
                server.Stop();
        }
    }
}
