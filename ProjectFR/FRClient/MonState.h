#pragma once
#include <BaseState.h>

class CBattleMonster;
class CMonState : public CBaseState
{
public:
	enum MONSTATE
	{
		MON_IDLE,
		MON_ACTION,
		MON_REACTION,
		MAXMONSTATE,
	};

protected:
	CBattleMonster*				m_pMonster;
	tstring						m_MonsterName;
	float						m_fAccTime;

public:
	void Init(void* _pLogic);

public:
	CBattleMonster* GetMonster();

public:
	CMonState();
	~CMonState();
};

