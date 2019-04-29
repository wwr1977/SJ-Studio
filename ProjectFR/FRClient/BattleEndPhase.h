#pragma once
#include "BasePhase.h"
class CBattleEndPhase : public CBasePhase
{

	bool							m_bBattleWin;
	bool							m_bWinPose;
	float							m_fAccTime;

	bool							m_bSceneEffectTriger;
public:
	void StateReset();
	void Update();

public:
	void UpdateWinEvent();
	void UpdateLoseEvent();

public:
	CBattleEndPhase();
	~CBattleEndPhase();
};

