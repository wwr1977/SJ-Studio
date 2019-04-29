#pragma once
#include "BasePhase.h"


class CBattleUnit;
class CEffect;
class CDemageFontGroup;
class CActionPhase : public CBasePhase
{
	enum 
	{
		SKILL_UPDATE,
		SKILL_ENDWAIT,
		MAXACTPHASESTATE
	};
private:
	float								m_fAccTime;
	bool								m_bCurSkillEnd;
	float								m_fAccWaitTime;
	// TurnMgr���� DemageFont�� ��� ���ϵɶ������� �ִ� ���ð�(4�� ����)
	float								m_fMaxWaitTime;

	SPTR<CBattleUnit>					m_CurActionUnit;
	int									CurIndex;
	
	ANIMATIONSCRIPT						m_CurAniScript;
	vector<SPTR<CEffect>>				m_pCurEffect;
	MOVEDATA							m_CurMoveScript;
	MOVEDATA							m_CurHideScript;
	
	vector<SPTR<CDemageFontGroup>>		m_CurDemageGroup;
	//vector<SPTR<CDemageFontGroup>>	m_CurMonDemageGroup;

	UINT								m_AccHitCount;
	bool								m_bLastHitCheck;
	
	UINT								m_CurActPhaseState;
	bool								m_bCharacterTurn;

private:
	void StateReset();
	void SettingCharacterSkill();
	void SettingMonsterSkill();
	void CharacterStandby();
	void MonsterStandby();
	void Update();
	
	
	void SkillProcess();
	bool SkillEndChck();
	void CharacterSkillProcess();
	void MonsterSkillProcess();

	void CheckTurnChange();

public:
	// ��ũ��Ʈ ť���� �ð��� ���� ��ũ��Ʈ�� Ȱ��ȭ
	void ScriptUpdate();
	// ��ũ��Ʈ ������Ʈ���� Ȱ��ȭ�� ����Ʈ�� ������Ʈ
	void ActionUpdate();
	// ��ų ������ ���õ� �����͸� Ÿ�ֿ̹� ���߿� Ȱ��ȭ
	void SkillDataTimingCheck();
	// ��ų������ ������Ʈ���� Ȱ��ȭ�� Hit �����͸� ������Ʈ
	void SkillDataUpdate();

	// ��ũ��Ʈ ������ ���� ������Ʈ 
	void HideUpdate();
	void MoveUpdate();
	void SkillEffectUpdate();

	// SkillData ���� ������Ʈ
	// ���߿� Update�Լ��� ����������� ����Ͽ� ���� �з�
	void HitDataUpdate(const HITDATA& _HitData);

	// Action Phase�� ���� ������ Ÿ���� ��ȿ�� Ÿ������ Ȯ���ϰ�
	// �׷��� ���� ��� TurnMgr���� Ÿ���� ���Ƿ� �������ϵ��� ��û
	void CheckUnitTarget();
	void CheckTargerOverride();
	void SetInitCharPos();

	

	void CreateDemageGroup();
	const bool CheckSkillCost();
	void CalSkillCost();

	// ���� ���� Ÿ������ �Լ�
	void MonsterTargetSetting();
	void RandomTarget(const UINT& _RanCount);
	void CharRandomTarget(const UINT& _RanCount);
	void CharRandomTarget2(const UINT& _RanCount);
	void MonRandomTarget(const UINT& _RanCount);
	void RandomColumnTarget();
	void SettingColumnTarget(const UNITCOLUMN& _Op);
	void SettingAllTarget();
	void CurTargetUnitEndReaction();


public:
	CActionPhase();
	~CActionPhase();
};

