#pragma once
#include <sshost.h>

class CPropBagMock
{
public:
	MOCK_METHOD2(Set, void(int, LPCWSTR));
	MOCK_METHOD1(Get, std::wstring(int));
};

class CPropBag
{
	DECLARE_MOCK_FACTORY(CPropBag)

	SHIM_METHOD(void, Set, (int idx, LPCWSTR value), idx, value)
	SHIM_METHOD(std::wstring, Get, (int idx), idx)
};