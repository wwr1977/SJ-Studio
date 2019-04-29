#pragma once
#include "MainPlayerState.h"

class CPlayerIdle : public CMainPlayerState
{
public:
	void StateReset();
	void Update();

public:
	bool IdleUpdate();
	const tstring GetCurAnimationName();


public:
	CPlayerIdle();
	~CPlayerIdle();
};

