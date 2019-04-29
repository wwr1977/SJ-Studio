#pragma once
#include "BattleState.h"
class CMonAppear : public CBattleState
{
	float				m_fAccTime;
	const float			m_fAppearTime;
public:
	void StateReset();
	void Update();

public:
	CMonAppear();
	~CMonAppear();
};

