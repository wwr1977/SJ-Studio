#pragma once
#include "BasePhase.h"

class CBattleUnit;
class CSelectPhase :public CBasePhase
{
private:
	SPTR<CBattleUnit>				m_CurSelectUnit;
	// ���� ���õ� ������ �̹� ������ ��ų�� ���õǾ����� true
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

