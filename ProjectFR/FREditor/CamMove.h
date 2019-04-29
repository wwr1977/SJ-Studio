#pragma once
#include "EditCamState.h"

class CCamMove : public CEditCamState
{


public:
	void StateReset();
	void Update();

public:
	CCamMove();
	~CCamMove();
};

