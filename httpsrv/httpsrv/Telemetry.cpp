#include "StdAfx.h"
#include "Telemetry.h"

namespace Telemetry
{
	CTelemetry::CTelemetry()
	{
	}


	CTelemetry::~CTelemetry()
	{
	}

	Boolean CTelemetry::HandleRequest(System::String ^verb, System::String ^handlerParam, Dictionary<String^, String^>^ params, HttpListenerContext ^httpCtx)
	{
		StringBuilder sb;
		sb.Append("The dynamically built response\n");
		sb.AppendFormat("Handler Param: {0}\n", handlerParam);
		sb.Append("*** Telemetry call ***\n");

		sb.AppendFormat("Verb: {0}\n", verb);
		sb.AppendFormat("Content-Type: {0}\n", httpCtx->Request->ContentType);
		sb.Append("Headers:\n");
		for each(String^ hdr in httpCtx->Request->Headers)
		{
			sb.AppendFormat("\t{0}: {1}\n", hdr, httpCtx->Request->Headers[hdr]);
		}

		sb.Append("Parameters:\n");

		for each(String^ key in params->Keys)
		{
			String^ value = params[key];
			sb.AppendFormat("\tname [{0}], value [{1}]\n", key, value);
		}

		SHA256Managed hash;
		hash.ComputeHash(Encoding::Unicode->GetBytes(sb.ToString()));
		sb.Append(Convert::ToBase64String(hash.Hash));

		if (httpCtx->Request->HasEntityBody)
		{
			array<unsigned char>^ data = gcnew array<unsigned char>(
				static_cast<int>(httpCtx->Request->ContentLength64)
				);
			httpCtx->Request->InputStream->Read(data, 0, data->Length);
			sb.AppendFormat("Raw data:\n{0}\n", Encoding::GetEncoding(1251)->GetString(data));
		}

		sb.Append("\nEnd of response");
		array<unsigned char>^ bytes = Encoding::GetEncoding(1251)->GetBytes(sb.ToString());
		httpCtx->Response->ContentType = "text/plain";
		httpCtx->Response->ContentLength64 = bytes->Length;
		httpCtx->Response->Close(bytes, true);

		return TRUE;
	}
}
