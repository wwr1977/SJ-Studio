#pragma once
#include <Logic.h>
class CTestMonster : public CLogic
{
private:
	SPTR<CAniRenderer>				m_AniRen;

public:
	void Init();
	void Update();

public:
	CTestMonster();
	~CTestMonster();
};

