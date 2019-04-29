#pragma once
#include "BattleState.h"

class CMonAction :	public CBattleState
{
private:
	bool							m_bEnd;
	int								m_iActionIndex;
	int								m_iAniLooping; 

public:
	void StateReset();
	void Update();

public:
	CMonAction();
	~CMonAction();
};

