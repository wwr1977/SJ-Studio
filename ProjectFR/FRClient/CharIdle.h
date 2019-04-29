#pragma once
#include "BattleState.h"
class CCharIdle : public CBattleState
{
public:
	void StateReset();
	void Update();

public:
	CCharIdle();
	~CCharIdle();
};

