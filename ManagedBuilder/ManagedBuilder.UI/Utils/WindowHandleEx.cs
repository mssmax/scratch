// -----------------------------------------------------------------------
// <copyright file="WindowHandleEx.cs" company="GFI Software Development Ltd.">
//      Copyright (c) GFI Software All rights reserved.
// </copyright>
// -----------------------------------------------------------------------
using System;
using System.Windows.Forms;

namespace ManagedBuilder.UI.Utils
{
public delegate void Win32MessageEventHandler(object sender, Win32MessageEventArgs e);
	
	public class Win32MessageEventArgs : EventArgs 
    {
	    public bool Handled { get; set; }

	    public Message Message { get; private set; }

	    public Win32MessageEventArgs(Message msg) 
        {
			Message = msg;
			Handled = false;
		}
	}

    public sealed class WindowHandleEx : NativeWindow, IDisposable
    {
        private CreateParams _createParams;

        public event Win32MessageEventHandler OnMessage;

        protected override void WndProc(ref Message m)
        {
            var handler = OnMessage;
            if (handler != null)
            {
                var e = new Win32MessageEventArgs(m);
                OnMessage(this, e);

                if (!e.Handled)
                    base.WndProc(ref m);
            }
            else
                base.WndProc(ref m);
        }

        /// <summary>
        /// Creates a new Win32 handle with the specified parameters
        /// </summary>
        public CreateParams Params
        {
            get { return _createParams; }
            set
            {
                if (_createParams == value)
                    return;

                _createParams = value;

                Dispose();
                CreateHandle(this._createParams);
            }
        }

        #region c'tor
        public WindowHandleEx(CreateParams createParams)
        {
            _createParams = createParams;
            CreateHandle(this._createParams);
        }

        public WindowHandleEx() : this(new CreateParams()) { }
        #endregion

        #region IDisposable
        public void Dispose()
        {
            if (Handle != (IntPtr)0)
            {
                DestroyHandle();
            }
        }
        #endregion

        #region Hooking to an already created handle
        public void Hook(System.Windows.Forms.Control ctrl)
        {
            ctrl.HandleCreated += form_HandleCreated;
            ctrl.HandleDestroyed += FormHandleDestroyed;
        }

        void FormHandleDestroyed(object sender, EventArgs e)
        {
            this.ReleaseHandle();
        }

        void form_HandleCreated(object sender, EventArgs e)
        {
            //this.AssignHandle((sender as System.Windows.Forms.Control).Handle);
        }
        #endregion
    }
}