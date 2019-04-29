#pragma once
#include "BasePhase.h"
class CRelocationPhase : public CBasePhase
{

private:
	void StateReset();
	void Update();
public:
	CRelocationPhase();
	~CRelocationPhase();
};

