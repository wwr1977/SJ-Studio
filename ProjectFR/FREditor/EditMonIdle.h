#pragma once
#include "EditBattleState.h"

class CEditMonIdle : public CEditBattleState
{
private:
	int					m_ReActionIndex;


public:
	void StateReset();
	void Update();

public:
	CEditMonIdle();
	~CEditMonIdle();
};

