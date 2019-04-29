#pragma once
#include "PlayerState.h"
class CPlayerAttack : public CPlayerState
{
private:
	float					m_fAccTime;

public:
	void StateReset();
	void Update();


public:
	CPlayerAttack();
	~CPlayerAttack();
};

