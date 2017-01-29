#pragma once
class CPropBag
{
private:
	std::map<int, std::wstring> m_kv;
public:
	CPropBag();
	virtual ~CPropBag();

	void Set(int idx, LPCWSTR value);
	std::wstring Get(int idx);
};

