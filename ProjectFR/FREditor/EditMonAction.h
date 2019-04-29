#pragma once
#include "EditBattleState.h"

class CEditMonAction :	public CEditBattleState
{
private:
	bool							m_bEnd;
	int								m_iActionIndex;
	int								m_iAniLooping; 

public:
	void StateReset();
	void Update();

public:
	CEditMonAction();
	~CEditMonAction();
};

