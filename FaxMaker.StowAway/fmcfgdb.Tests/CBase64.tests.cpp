/*
 *  Unit tests CBase64 (base64 encoding/decoding class).
 */
#include "StdAfx.h"
#include <string>
#include <CBase64.h>

#ifdef PERFORM_CBASE64_TESTS

/*
 *  ANSI strings tests.
 */

TEST(CBase64, Encode_TestString_CorrectANSIBase64EncodedStr)
{
	BYTE	buffer[99] = {0};
	LPSTR	base64[99] = {0};
	DWORD	base64Size = sizeof(base64) / sizeof(base64[0]);
	CBase64	cbase64;

	strcpy((char *)buffer, "test");

	EXPECT_EQ(GFICRYPTO_SUCCESS, cbase64.Encode((BYTE *)buffer, strlen((LPSTR)buffer), (LPSTR)base64, &base64Size));
	EXPECT_STREQ("dGVzdA==", (LPCSTR)base64);
}


TEST(CBase64, Decode_TestANSIBase64String_CorrectString)
{
	LPSTR	base64[99] = {0};
	BYTE	buffer[99] = {0};
	DWORD	bufferSize = sizeof(buffer);
	CBase64	cbase64;

	strcpy((LPSTR)base64, "bWFsdGE=");

	EXPECT_EQ(GFICRYPTO_SUCCESS, cbase64.Decode((LPSTR)base64, (BYTE *)buffer, &bufferSize));
	EXPECT_STREQ("malta", (LPCSTR)buffer);
}


/*
 *  UNICODE strings tests.
 */

TEST(CBase64, Encode_TestString_CorrectUNICODEBase64EncodedString)
{
	BYTE	buffer[99] = {0};
	LPWSTR	base64[99] = {0};
	DWORD	base64Size = sizeof(base64) / sizeof(base64[0]);
	CBase64	cbase64;

	strcpy((LPSTR)buffer, "massachusetts");

	EXPECT_EQ(GFICRYPTO_SUCCESS, cbase64.Encode((BYTE *)buffer, strlen((LPSTR)buffer), (LPWSTR)base64, &base64Size));
	EXPECT_EQ(0, wcscmp(L"bWFzc2FjaHVzZXR0cw==", (LPWSTR)base64));
}


TEST(CBase64, Decode_TestUNICODEBase64String_CorrectString)
{
	LPWSTR	base64[99] = {0};
	BYTE	buffer[99] = {0};
	DWORD	bufferSize = sizeof(buffer);
	CBase64	cbase64;

	wcscpy((LPWSTR)base64, L"c3RvY2tob2xt");

	EXPECT_EQ(GFICRYPTO_SUCCESS, cbase64.Decode((LPWSTR)base64, (BYTE *)buffer, &bufferSize));
	EXPECT_STREQ("stockholm", (LPCSTR)buffer);
}


/*
 *  NULL parameters tests.
 */

TEST(CBase64, Encode_NullANSIParameters_ReturnsError)
{
	CBase64	cbase64;

	EXPECT_EQ(GFICRYPTO_INVALIDPARAMETER, cbase64.Encode((BYTE *)NULL, 1, (LPSTR)1,    (DWORD*)1));
	EXPECT_EQ(GFICRYPTO_INVALIDPARAMETER, cbase64.Encode((BYTE *)1,    1, (LPSTR)NULL, (DWORD*)1));
	EXPECT_EQ(GFICRYPTO_INVALIDPARAMETER, cbase64.Encode((BYTE *)1,    1, (LPSTR)1,    (DWORD*)NULL));
}


TEST(CBase64, Decode_NullANSIParameters_ReturnsError)
{
	CBase64	cbase64;

	EXPECT_EQ(GFICRYPTO_INVALIDPARAMETER, cbase64.Decode((LPSTR)NULL, (BYTE *)1,    (DWORD*)1));
	EXPECT_EQ(GFICRYPTO_INVALIDPARAMETER, cbase64.Decode((LPSTR)1,    (BYTE *)NULL, (DWORD*)1));
	EXPECT_EQ(GFICRYPTO_INVALIDPARAMETER, cbase64.Decode((LPSTR)1,    (BYTE *)1,    (DWORD*)NULL));
}


TEST(CBase64, Encode_NullUNICODEParameters_ReturnsError)
{
	CBase64	cbase64;

	EXPECT_EQ(GFICRYPTO_INVALIDPARAMETER, cbase64.Encode((BYTE *)NULL, 1, (LPWSTR)1,    (DWORD*)1));
	EXPECT_EQ(GFICRYPTO_INVALIDPARAMETER, cbase64.Encode((BYTE *)1,    1, (LPWSTR)NULL, (DWORD*)1));
	EXPECT_EQ(GFICRYPTO_INVALIDPARAMETER, cbase64.Encode((BYTE *)1,    1, (LPWSTR)1,    (DWORD*)NULL));
}


TEST(CBase64, Decode_NullUNICODEParameters_ReturnsError)
{
	CBase64	cbase64;

	EXPECT_EQ(GFICRYPTO_INVALIDPARAMETER, cbase64.Decode((LPWSTR)NULL, (BYTE *)1,    (DWORD*)1));
	EXPECT_EQ(GFICRYPTO_INVALIDPARAMETER, cbase64.Decode((LPWSTR)1,    (BYTE *)NULL, (DWORD*)1));
	EXPECT_EQ(GFICRYPTO_INVALIDPARAMETER, cbase64.Decode((LPWSTR)1,    (BYTE *)1,    (DWORD*)NULL));
}

#endif	// PERFORM_CBASE64_TESTS