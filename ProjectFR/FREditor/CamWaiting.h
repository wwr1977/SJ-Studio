#pragma once
#include "EditCamState.h"

class CCamWaiting : public CEditCamState
{


public:
	void StateReset();
	void Update();

public:
	CCamWaiting();
	~CCamWaiting();
};

