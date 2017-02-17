#pragma once
#include <sshost.h>
#include <functional>

class CPropBagMock
{
public:
	virtual ~CPropBagMock() {}
	MOCK_METHOD2(Set, void(int, LPCWSTR));	
	MOCK_METHOD1(Get, std::wstring(int));
};

class CPropBag
{
public:
	DECLARE_MOCK_FACTORY(CPropBag);

	SHIM_METHOD(void, Set, (int idx, LPCWSTR value), idx, value);
	SHIM_METHOD(std::wstring, Get, (int idx), idx);
};