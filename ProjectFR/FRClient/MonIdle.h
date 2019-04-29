#pragma once
#include "BattleState.h"

class CMonIdle : public CBattleState
{
private:
	int					m_ReActionIndex;

public:
	void StateReset();
	void Update();

public:
	CMonIdle();
	~CMonIdle();
};

