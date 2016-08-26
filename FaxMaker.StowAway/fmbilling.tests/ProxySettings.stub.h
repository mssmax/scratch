#include "stdafx.h"
#include "IProxySettings.h"


class ProxySettings_Stub : public IProxySettings
{
public:
	bool LoadProxySettings(ILogInterface *pLog) {return true;};
    bool RetrieveSystemProxySettings() { return true; };

};