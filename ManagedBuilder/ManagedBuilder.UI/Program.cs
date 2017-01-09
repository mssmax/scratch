// -----------------------------------------------------------------------
// <copyright file="Program.cs" company="GFI Software Development Ltd.">
//      Copyright (c) GFI Software All rights reserved.
// </copyright>
// -----------------------------------------------------------------------
using System;
using System.Collections.Generic;
using System.Windows.Forms;
using ManagedBuilder.UI.Forms;
using ManagedBuilder.UI.Utils;

namespace ManagedBuilder.UI
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            ToolStripManager.Renderer = new Office2007Renderer.Office2007Renderer();
            

            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new MainForm());
        }
    }
}