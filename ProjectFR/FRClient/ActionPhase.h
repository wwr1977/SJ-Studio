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
	// TurnMgr안의 DemageFont가 모두 리턴될때까지의 최대 대기시간(4초 한정)
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
	// 스크립트 큐에서 시간이 지난 스크립트를 활성화
	void ScriptUpdate();
	// 스크립트 업데이트에서 활성화된 이펙트를 업데이트
	void ActionUpdate();
	// 스킬 데이터 관련된 데이터를 타이밍에 맞추워 활성화
	void SkillDataTimingCheck();
	// 스킬데이터 업데이트에서 활성화된 Hit 데이터를 업데이트
	void SkillDataUpdate();

	// 스크립트 데이터 세부 업데이트 
	void HideUpdate();
	void MoveUpdate();
	void SkillEffectUpdate();

	// SkillData 세부 업데이트
	// 나중에 Update함수가 비대해질것을 대비하여 따로 분류
	void HitDataUpdate(const HITDATA& _HitData);

	// Action Phase에 들어온 유닛의 타겟이 유효한 타겟인지 확인하고
	// 그렇지 않은 경우 TurnMgr에게 타겟을 임의로 재지정하도록 요청
	void CheckUnitTarget();
	void CheckTargerOverride();
	void SetInitCharPos();

	

	void CreateDemageGroup();
	const bool CheckSkillCost();
	void CalSkillCost();

	// 몬스터 전용 타겟지정 함수
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

