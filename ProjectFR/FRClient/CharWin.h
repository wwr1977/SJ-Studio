#pragma once
#include "BattleState.h"
class CCharWin : public CBattleState
{
public:
	void StateReset();
	void Update();

public:
	CCharWin();
	~CCharWin();
};

