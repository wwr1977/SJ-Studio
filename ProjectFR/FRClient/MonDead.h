#pragma once
#include "BattleState.h"
class CMonDead : public CBattleState
{
	bool				m_bUpdate;
	bool				m_bUITriger;
public:
	void StateReset();
	void Update();

public:
	CMonDead();
	~CMonDead();
};

