#pragma once
#include"BattleState.h"

class CMonReAction :public CBattleState
{

public:
	void StateReset();
	void Update();
	
public:
	CMonReAction();
	~CMonReAction();
};

