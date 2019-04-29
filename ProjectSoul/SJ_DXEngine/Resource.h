#pragma once
#include"ReferenceBase.h"
#include"TypeBase.h"
#include<string>
#include"DXMacro.h"


class CResource : public CReferenceBase, public CTypeBase
{
protected:
	std::wstring						m_Path;

public:
	void SetPath(const wchar_t* _Path) { m_Path = _Path; }
	void SetPath(const std::wstring _Path) { m_Path = _Path; }

public:
	std::wstring		GetPath() { return m_Path; }
	std::wstring		GetFullFileName();
	std::wstring		GetFileName();
	std::wstring		GetExtension();


public:
	CResource();
	virtual ~CResource() = 0;
};

