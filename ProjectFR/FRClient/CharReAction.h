#pragma once
#include "BattleState.h"
class CCharReAction : public CBattleState
{

public:
	void StateReset();
	void Update();

public:
	CCharReAction();
	~CCharReAction();
};

