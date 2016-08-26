#pragma once
#include "stdafx.h"

namespace Telemetry
{
	ref class CTelemetry : public IHTTPHandler
	{
	public:
		CTelemetry();
		virtual ~CTelemetry();

		virtual Boolean HandleRequest(System::String ^verb, System::String ^handlerParam, Dictionary<String^, String^>^ params, HttpListenerContext ^ctx) override;
	};
}
