#pragma once
#include "BasePhase.h"
class CCheckPhase : public CBasePhase
{
	SPTR<class CBattleUnit>				m_CurActionUnit;
	BATTLEPHASE							m_ChangePhase;
	float								m_fAccTime;
	UINT								m_CurAccExp;

public:
	void StateReset();
	void Update();

public:
	void CharTurnCheckEvent();
	void MonTurnCheckEvent();

public:
	CCheckPhase();
	~CCheckPhase();
};

