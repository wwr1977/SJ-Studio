#pragma once
#include "BattleState.h"
class CCharDead : public CBattleState
{
public:
	void StateReset();
	void Update();

public:
	CCharDead();
	~CCharDead();
};

