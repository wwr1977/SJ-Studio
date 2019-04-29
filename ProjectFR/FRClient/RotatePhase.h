#pragma once
#include "BasePhase.h"

class CTurnOrderUI;
class CRotatePhase : public CBasePhase
{
private:
	// 맨위의 Turn UI를 얼마나 뒤로 밀어버릴지를 결정
	// 디폴트(0)은 가장 아래로 밀어버린다
	SPTR<CTurnOrderUI>				m_CurTurnUI;
	int								m_iRotateCount;
	float							m_fAccTime;

private:
	void StateReset();
	void Update();

private:
	bool TurnRotation();

public:
	CRotatePhase();
	~CRotatePhase();
};

