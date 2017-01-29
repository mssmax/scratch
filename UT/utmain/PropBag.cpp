#include "stdafx.h"
#include "PropBag.h"


CPropBag::CPropBag()
{
}


CPropBag::~CPropBag()
{
}

void CPropBag::Set(int idx, LPCWSTR value)
{
	m_kv[idx] = value;
}

std::wstring CPropBag::Get(int idx)
{
	return m_kv[idx];
}