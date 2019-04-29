#pragma once
#include "EditBattleState.h"
class CEditCharIdle : public CEditBattleState
{
public:
	void StateReset();
	void Update();

public:
	CEditCharIdle();
	~CEditCharIdle();
};

