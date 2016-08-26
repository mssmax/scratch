// httpsrv.cpp : main project file.

#include "stdafx.h"
#include "httpsrv.h"

namespace HTTPSrv
{
	void CHTTPServer::MainHandler(IAsyncResult^ res)
	{
		CHTTPServer^ srv = static_cast<CHTTPServer^>(res->AsyncState);
		HttpListener^ listener = srv->Listener;
		if (!listener->IsListening)
			return;

		HttpListenerContext^ httpCtx = listener->EndGetContext(res);

		listener->BeginGetContext(gcnew AsyncCallback(MainHandler), srv);

		Boolean bRequestHandled = FALSE;
		String ^sURI = httpCtx->Request->Url->AbsolutePath;
		String ^sPQ = httpCtx->Request->Url->PathAndQuery;
		String ^sQuery = httpCtx->Request->Url->Query;
		for each(HandlerInfo h in srv->m_Handlers)
		{
			if (sURI->StartsWith(h.URI))
			{
				if (h.supportedVerbs->Contains(httpCtx->Request->HttpMethod))
				{
					System::String ^sHandlerData = sPQ->Substring(h.URI->Length, sPQ->Length - sQuery->Length - h.URI->Length);
					Dictionary<String^, String^> ^params = gcnew Dictionary<String^, String^>();
					if (sQuery->Length > 0)
					{
						// to remove the starting ? characters
						sQuery = sQuery->Substring(1);
						array<String^>^ argList = sQuery->Split('&');
						for each(String ^s in argList)
						{
							array<String^>^ paramVal = s->Split('=');
							if (paramVal->Length > 1)
							{
								params->Add(paramVal[0], paramVal[1]);
							}
							else
							{
								params->Add(paramVal[0], "1");
							}
						}
					}
					bRequestHandled = h.handler->HandleRequest(
						httpCtx->Request->HttpMethod,
						sHandlerData,
						params,
						httpCtx);
				}
			}
		}
		if (!bRequestHandled)
		{
			httpCtx->Response->ContentType = "text/plain";
			httpCtx->Response->StatusCode = 500;
			httpCtx->Response->Close(Encoding::ASCII->GetBytes("No handlers available to process request"), TRUE);
		}
	}

	void CHTTPServer::LoadConfig()
	{
		try
		{
			XmlDocument ^doc = gcnew XmlDocument();
			doc->Load("httpsettings.xml");

			XmlNode ^globalHost = doc->SelectSingleNode("/Settings/Global/Host");
			XmlNode ^globalPort = doc->SelectSingleNode("/Settings/Global/Port");
			XmlNode ^globalScheme = doc->SelectSingleNode("/Settings/Global/Scheme");

			m_Host = globalHost->InnerText;
			m_Port = globalPort->InnerText;
			m_Scheme = globalScheme->InnerText;

			XmlNodeList ^handlers = doc->SelectNodes("/Settings/Handlers/Handler");
			m_Handlers = gcnew array<HandlerInfo>(handlers->Count);
			int idx = 0;
			for each(XmlNode ^handler in handlers)
			{
				System::String ^prefix = handler->SelectSingleNode("URI")->InnerText;
				System::String ^type = handler->SelectSingleNode("Handler")->InnerText;
				System::String ^verbs = handler->SelectSingleNode("Verbs")->InnerText;
				System::String ^assembly = handler->SelectSingleNode("Path")->InnerText;

				HandlerInfo hinfo;
				hinfo.URI = prefix;
				hinfo.supportedVerbs = verbs;
				System::Runtime::Remoting::ObjectHandle ^oh = Activator::CreateInstanceFrom(assembly, type);
				hinfo.handler = (IHTTPHandler^)oh->Unwrap();
				m_Handlers[idx] = hinfo;
				++idx;
			}
		}
		catch (Exception ^e)
		{
			Console::WriteLine("Failed to load settings {0}", e);
		}
	}

	void CHTTPServer::InitHandlers()
	{
		LoadConfig();
		for each(HandlerInfo h in m_Handlers)
		{
			if (m_Port->Length == 0)
			{
				System::String ^s = String::Format("{0}://{1}{2}/", m_Scheme, m_Host, h.URI);
				m_listener.Prefixes->Add(s);
			}
			else
			{
				System::String ^s = String::Format("{0}://{1}:{2}{3}/", m_Scheme, m_Host, m_Port, h.URI);
				m_listener.Prefixes->Add(s);
			}
		}
	}

	void CHTTPServer::Start()
	{
		m_listener.Start();
		m_listener.BeginGetContext(gcnew AsyncCallback(MainHandler), this);
	}

	void CHTTPServer::Stop()
	{
		m_listener.Stop();
	}
}

extern void CreateHttpsCert();

int main(array<System::String ^> ^args)
{
	//CreateHttpsCert();


	HTTPSrv::CHTTPServer server;
	try
	{
		server.InitHandlers();
		server.Start();
		Console::ReadLine();
		server.Stop();
	}
	catch (Exception^ e)
	{
		Console::WriteLine("exception occured {0}", e);
	}
	return 0;
}
