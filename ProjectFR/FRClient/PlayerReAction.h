#pragma once
#include "PlayerState.h"
class CPlayerReAction : public CPlayerState
{
private:
	float					m_fAccTime;

public:
	void StateReset();
	void Update();
	

public:
	CPlayerReAction();
	~CPlayerReAction();
};

