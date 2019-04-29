#pragma once
#include "EditBattleState.h"
class CEditCharReAction : public CEditBattleState
{

public:
	void StateReset();
	void Update();

public:
	CEditCharReAction();
	~CEditCharReAction();
};

