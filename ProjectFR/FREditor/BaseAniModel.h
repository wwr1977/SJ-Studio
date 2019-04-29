#pragma once
#include<Logic.h>

class CBaseAniModel : public CLogic
{
private:
	SPTR<CAniRenderer>						m_BaseAniRenderer;

public:
	void Init();
	void DebugRender();


public:
	CBaseAniModel();
	~CBaseAniModel();
};

