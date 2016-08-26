#include "stdafx.h"

void CreateHttpsCert()
{
	TCHAR szName[1024] = { 0 };
	TCHAR szCN[1024] = { 0 };
	BYTE byName[1024] = { 0 };
	DWORD dwSize = _countof(szName);
	BOOL b = GetComputerNameEx(ComputerNamePhysicalDnsFullyQualified, szName, &dwSize);
	StringCbPrintf(szCN, sizeof(szCN), _T("CN=%s"), szName);
	dwSize = sizeof(byName);
	b = CertStrToName(X509_ASN_ENCODING, szCN, CERT_X500_NAME_STR, 0, byName, &dwSize, 0);
	CERT_NAME_BLOB blbName = { 0 };
	blbName.cbData = dwSize;
	blbName.pbData = byName;
	PCCERT_CONTEXT pCert = CertCreateSelfSignCertificate(NULL, &blbName, 0, 0, 0, 0, 0, 0);
	DWORD dw = GetLastError();
	if (pCert)
	{
		CERT_ENHKEY_USAGE usage = { 0 };
		LPSTR lpsz = szOID_PKIX_KP_SERVER_AUTH;
		usage.cUsageIdentifier = 1;
		usage.rgpszUsageIdentifier = &lpsz;
		b = CertSetEnhancedKeyUsage(pCert, &usage);
		dw = GetLastError();
		HCERTSTORE hStoreMy = 0;
		hStoreMy = CertOpenStore(CERT_STORE_PROV_SYSTEM, X509_ASN_ENCODING, 0,
			CERT_STORE_OPEN_EXISTING_FLAG | CERT_SYSTEM_STORE_LOCAL_MACHINE,
			L"MY");
		dw = GetLastError();
		HCERTSTORE hStoreRoot = CertOpenStore(CERT_STORE_PROV_SYSTEM, X509_ASN_ENCODING, 0,
			CERT_STORE_OPEN_EXISTING_FLAG | CERT_SYSTEM_STORE_LOCAL_MACHINE,
			L"Root");
		b = CertAddCertificateContextToStore(hStoreMy, pCert, CERT_STORE_ADD_REPLACE_EXISTING, 0);
		dw = GetLastError();
		b = CertAddCertificateContextToStore(hStoreRoot, pCert, CERT_STORE_ADD_REPLACE_EXISTING, 0);
		dw = GetLastError();
		SHA1 ^sha = gcnew SHA1CryptoServiceProvider();
		array<BYTE> ^cert = gcnew array<BYTE>(pCert->cbCertEncoded);
		pin_ptr<BYTE> p = &cert[0];
		CopyMemory(p, pCert->pbCertEncoded, pCert->cbCertEncoded);
		array<BYTE> ^hash = sha->ComputeHash(cert);
		HTTP_SERVICE_CONFIG_SSL_SET sslConfig = { 0 };
		SOCKADDR socket = { 0 };
		socket.sa_family = AF_INET;
		reinterpret_cast<sockaddr_in*>(&socket)->sin_port = htons(8080);
		sslConfig.KeyDesc.pIpPort = &socket;
		sslConfig.ParamDesc.SslHashLength = hash->Length;
		p = &hash[0];
		sslConfig.ParamDesc.pSslHash = p;
		CoCreateGuid(&sslConfig.ParamDesc.AppId);
		sslConfig.ParamDesc.DefaultCertCheckMode = 1;
		HTTPAPI_VERSION v = HTTPAPI_VERSION_1;
		ULONG ul = HttpInitialize(v, HTTP_INITIALIZE_CONFIG, 0);
		ul = HttpDeleteServiceConfiguration(
			0,
			HttpServiceConfigSSLCertInfo,
			&sslConfig,
			sizeof(sslConfig) + sizeof(HTTP_SERVICE_CONFIG_SSL_KEY) + sizeof(HTTP_SERVICE_CONFIG_SSL_PARAM),
			0);
		ul = HttpSetServiceConfiguration(
			0,
			HttpServiceConfigSSLCertInfo,
			&sslConfig,
			sizeof(sslConfig) + sizeof(HTTP_SERVICE_CONFIG_SSL_KEY) + sizeof(HTTP_SERVICE_CONFIG_SSL_PARAM),
			0);
		HttpTerminate(HTTP_INITIALIZE_CONFIG, 0);
		CertCloseStore(hStoreMy, CERT_CLOSE_STORE_FORCE_FLAG);
		CertCloseStore(hStoreRoot, CERT_CLOSE_STORE_FORCE_FLAG);
	}
}
