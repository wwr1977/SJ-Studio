#pragma once
#include"EditBattleState.h"

class CEditMonReAction :public CEditBattleState
{

public:
	void StateReset();
	void Update();
	
public:
	CEditMonReAction();
	~CEditMonReAction();
};

