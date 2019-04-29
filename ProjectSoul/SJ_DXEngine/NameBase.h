#pragma once
#include<string>
#include<set>


class CNameBase
{
private:
	friend class Engine;

private:
	std::wstring									m_Name;
	std::set<std::wstring>							m_setTag;
	

protected:
	void SetName(const wchar_t* _Name) { m_Name = _Name; }
	void SetName(const std::wstring& _Name) { m_Name = _Name; }
public:
	std::wstring GetName() { return m_Name; }

public:
	void AddTag(const wchar_t* __Tag);
	void DelTag(const wchar_t* _Tag);
	const bool ExistTag(const wchar_t* _Tag);



public:
	CNameBase();
	virtual ~CNameBase() = 0;
};

