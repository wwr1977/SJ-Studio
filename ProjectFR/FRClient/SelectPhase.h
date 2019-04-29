#pragma once
#include "BasePhase.h"

class CBattleUnit;
class CSelectPhase :public CBasePhase
{
private:
	SPTR<CBattleUnit>				m_CurSelectUnit;
	// 현재 선택된 유닛이 이미 이전에 스킬이 선택되었을때 true
	bool							m_bCheckUnit;
private:
	void StateReset(); 
	void Update(); 
	void CharacterSelect();
	void MonsterSelect();
	const int GetActionIcon();
	bool CheckSelectUnit();

	void StandbyCharacter();
	void StandbyMonster();

public:
	CSelectPhase();
	~CSelectPhase();
};

