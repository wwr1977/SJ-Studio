#pragma once
#include"UserHeader.h"

class CCoreBuilder;
class CDX9Core
{
public:
	friend CCoreBuilder;

public:
	static bool					m_bInit;
	static bool					m_bCoreDeath;
	static HINSTANCE			m_hInst;
	const static type_info*		m_TransComInfo;

public:
	bool IsCoreDeath() { return m_bCoreDeath; }

public:
	static bool Init(HINSTANCE _hInst,SPTR<CCoreBuilder> _pBuilder);
	static bool Progress();
	static bool ReduceProgress();
	static bool ManagerRelease();
	const static type_info* TransComInfo();

public:
	CDX9Core() {}
	~CDX9Core() {}
};

