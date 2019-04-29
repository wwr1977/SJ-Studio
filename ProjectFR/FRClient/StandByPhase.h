#pragma once
#include "BasePhase.h"
class CStandByPhase : public CBasePhase
{
private:
	float						m_fAccTime;
	SPTR<CCamera>				m_BattleCamera;
	Vec3						m_StandByCamPos;
	bool						m_bBGMPlay;
private:
	void InitPhase();
	void StateReset();
	void Update();

public:
	CStandByPhase();
	~CStandByPhase();
};

