#pragma once
#include "PlayerState.h"
class CPlayerDeath : public CPlayerState
{
public:
	void StateReset();
	void Update();

public:
	CPlayerDeath();
	~CPlayerDeath();
};

