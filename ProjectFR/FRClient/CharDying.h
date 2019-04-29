#pragma once
#include "BattleState.h"
class CCharDying : public CBattleState
{

public:
	void StateReset();
	void Update();

public:
	CCharDying();
	~CCharDying();
};

