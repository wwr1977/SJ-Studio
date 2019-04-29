#pragma once
#include "BattleState.h"

class CCharAppear : public CBattleState
{
	float				m_fAccTime;
	const float			m_fAppearTime;

public:
	void StateReset();
	void Update();

public:
	CCharAppear();
	~CCharAppear();
};

