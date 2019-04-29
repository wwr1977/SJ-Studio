#include "Precom.h"
#include "CheckPhase.h"
#include "TurnMgr.h"
#include "BattleUnit.h"


CCheckPhase::CCheckPhase()
	:m_ChangePhase(PHASE_ROTATE), m_fAccTime(0.0f)
	, m_CurActionUnit(nullptr), m_CurAccExp(0)
{
}


CCheckPhase::~CCheckPhase()
{
}
void CCheckPhase::StateReset() 
{
	m_CurActionUnit = m_pTurnMgr->GetCurUnit();

	m_fAccTime = 0.0f;
	m_CurAccExp = 0;

	m_pTurnMgr->BattleUnitDeadEvent(m_CurAccExp);
	

	if (true == m_CurActionUnit->IsCharacter())
		CharTurnCheckEvent();
	else
		MonTurnCheckEvent();


	//if (0 != m_CurActionUnit->GetExpEvent(m_CurAccExp)) 
	//	m_pTurnMgr->CharacterLevelUpEvent();
	//

	m_ChangePhase = m_pTurnMgr->GetNextPhaseToCheckPhase();

}
void CCheckPhase::Update()
{
	if (1.1f <= m_fAccTime)
	{
		m_pTurnMgr->ChangePhase(m_ChangePhase);
		return;
	}

	m_fAccTime += DELTATIME;
}
void CCheckPhase::CharTurnCheckEvent() 
{
	size_t SkillNo = m_pTurnMgr->GetCurSkillNo();
	size_t SkillLv = m_pTurnMgr->GetCurSkillLv();


	if (0 != m_CurActionUnit->GetExpEvent(m_CurAccExp))
		m_pTurnMgr->CharacterLevelUpEvent();

	UINT MaxExp = CClientGlobal::vecSkillMaxExp[SkillLv];
	UINT CurExp = CClientGlobal::vecSkill[SkillNo].CurSkillExp;
	
	if (CClientGlobal::vecSkill[SkillNo].CurSkillExp >= MaxExp)
		return;


	// 스킬 사용하여 몬스터를 죽인 경우 사용한 스킬의 경험치 보너스를 부여한다.
	if (0 != m_CurAccExp)
		CClientGlobal::vecSkill[SkillNo].CurSkillExp = min(CurExp + 2, MaxExp);
	else
		CClientGlobal::vecSkill[SkillNo].CurSkillExp = min(CurExp + 1, MaxExp);

}
void CCheckPhase::MonTurnCheckEvent()
{

}