#pragma once

public ref class IHTTPHandler abstract
{
public:
	virtual Boolean HandleRequest(System::String ^verb, System::String ^handlerParam, Dictionary<String^, String^>^ params, HttpListenerContext ^ctx) = 0;
};
