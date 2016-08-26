#pragma once
#include "stdafx.h"

namespace HTTPSrv
{

	value class HandlerInfo
	{
	public:
		System::String^ supportedVerbs;
		System::String ^URI;
		IHTTPHandler ^handler;
	};

	ref class CHTTPServer
	{
	protected:

		HttpListener m_listener;
		System::String ^m_Host;
		System::String ^m_Port;
		System::String ^m_Scheme;

		static void MainHandler(IAsyncResult^ res);

	public:
		void LoadConfig();

		void InitHandlers();

		property HttpListener^ Listener
		{
			HttpListener^ get()
			{
				return %m_listener;
			}
		}

		void Start();
		void Stop();

		array<HandlerInfo>^ m_Handlers;
	};
}