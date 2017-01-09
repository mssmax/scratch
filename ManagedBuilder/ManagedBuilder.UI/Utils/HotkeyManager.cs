// -----------------------------------------------------------------------
// <copyright file="HotkeyManager.cs" company="GFI Software Development Ltd.">
//      Copyright (c) GFI Software All rights reserved.
// </copyright>
// -----------------------------------------------------------------------
using System;
using System.Windows.Forms;

namespace ManagedBuilder.UI.Utils
{
    public static class HotkeyManager
    {
        public static void RegisterGlobalHotkey( Keys key, Keys modifiers, EventHandler handler )
        {
            RegisterLocalHotkey(key, modifiers, handler, null);
        }

        public static void RegisterLocalHotkey( Keys key, Keys modifiers, EventHandler handler, IWin32Window window )
        {
            var handle = IntPtr.Zero;

            if (window != null)
                handle = window.Handle;

            KeyModifiers km;
            switch (modifiers)
            {
                case Keys.Alt:
                    km = KeyModifiers.Alt;
                    break;
                case Keys.Shift:
                case Keys.ShiftKey:
                    km = KeyModifiers.Shift;
                    break;
                case Keys.Control:
                case Keys.ControlKey:
                    km = KeyModifiers.Control;
                    break;
                case Keys.LWin:
                case Keys.RWin:
                    km = KeyModifiers.Windows;
                    break;
                default:
                    throw new Exception("Invalid keyboard modifier");
            }

            new HotKey(key, km, handler, handle);
        }
    }
}