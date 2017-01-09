// -----------------------------------------------------------------------
// <copyright file="Hotkey.cs" company="GFI Software Development Ltd.">
//      Copyright (c) GFI Software All rights reserved.
// </copyright>
// -----------------------------------------------------------------------
using System;
using System.Runtime.InteropServices;
using System.Windows.Forms;

namespace ManagedBuilder.UI.Utils
{
    public enum KeyModifiers
    {
        None = 0,
        Alt = 1,
        Control = 2,
        Shift = 4,
        Windows = 8
    }

    class HotKey : IMessageFilter,IDisposable
    {
        #region P/Invoke Interop
        [DllImport("user32.dll", SetLastError = true)]
        private static extern bool RegisterHotKey(IntPtr hWnd, int id, KeyModifiers fsModifiers, Keys vk);

        [DllImport("user32.dll", SetLastError = true)]
        private static extern bool UnregisterHotKey(IntPtr hWnd, int id);
        #endregion

        private const int WM_HOTKEY = 0x0312;

        #region Properties

        public EventHandler OnHotKeyPressed { get; set; }
        public IntPtr Handle { get; set; }

        #endregion        

        #region c'tor
        public HotKey(Keys key, KeyModifiers modifier, EventHandler hotKeyPressed)
            : this(key, modifier, hotKeyPressed, IntPtr.Zero)
        {
            
        }

        public HotKey(Keys key, KeyModifiers modifier, EventHandler hotKeyPressed, IntPtr handle )
        {
            OnHotKeyPressed = hotKeyPressed ?? delegate { };
            Handle = handle;            
            RegisterHotKey(key, modifier);
            Application.AddMessageFilter(this);
        }
        #endregion

        #region IDisposable
        private bool _isDisposing = false;
        public void Dispose()
        {
            lock (this)
            {
                if (_isDisposing)
                    return;

                _isDisposing = true;
            }

            Application.RemoveMessageFilter(this);
            UnregisterHotKey(Handle, GetHashCode());
        }
        #endregion

        private void RegisterHotKey(Keys key, KeyModifiers modifier)
        {
            if (key == Keys.None)
                return;

            var isKeyRegisterd = RegisterHotKey(Handle, GetHashCode(), modifier, key);
            if (!isKeyRegisterd)
                throw new ApplicationException("Hotkey allready in use");
        }

        public bool PreFilterMessage(ref Message m)
        {
            switch (m.Msg)
            {
                case WM_HOTKEY:
                    if ((int)m.WParam == this.GetHashCode())
                    {                        
                        OnHotKeyPressed(this, new EventArgs());
                        return true;
                    }
                    break;
            }
            return false;
        }

    }
}