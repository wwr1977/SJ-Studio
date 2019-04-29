#include "Precom.h"
#include "SelectPhase.h"
#include "TurnMgr.h"
#include"BattleUnit.h"
#include"TurnOrderUI.h"

CSelectPhase::CSelectPhase()
	:m_bCheckUnit(false)
{
}


CSelectPhase::~CSelectPhase()
{
}
void CSelectPhase::StateReset()
{
	m_bCheckUnit = false;
	m_CurSelectUnit = m_pTurnMgr->GetCurUnit();

	if (m_CurSelectUnit->isDead()) 
	{
		m_pTurnMgr->SetRotateCount(-1);
		m_pTurnMgr->ChangePhase(PHASE_ROTATE);
		return;
	}


	if (true == m_CurSelectUnit->IsCharacter())
		StandbyCharacter();
	else
		StandbyMonster();


}
void CSelectPhase::Update()
{
	// 유닛이 들어올때 처음 한순간 Update가 도는 데
	// 이전에 스킬이 선택된 유닛의 경우 Update를 무시
	if (m_bCheckUnit == true) 
	{
		m_bCheckUnit = false;
		return;
	}

	if (true == m_CurSelectUnit->IsCharacter()) 
	{
		CharacterSelect();
	}
	else 
	{
		MonsterSelect();
	}

}

void CSelectPhase::CharacterSelect() 
{
	/*if (true == KEYDOWN("ReAction"))
	{
		m_CurSelectUnit->SetReAction();
		return;
	}*/

	// 스킬 카운트를 계산하여 즉시발동 할건지 
	// 턴을 뒤로 미룰지 결정한 후 스킬을 작동
	if (true == m_pTurnMgr->isSkillTrigger()) 
	{
		int TurnCount = m_CurSelectUnit->CalTurnCount((int)m_pCurSkillLevelData->SkillCount);

		m_pTurnMgr->SetRotateCount(TurnCount);
		m_pTurnMgr->CurTargetUIOff();
		
		if (0 >= TurnCount)
		{
			m_CurSelectUnit->SelectEffectOff();
			m_pTurnMgr->ChangePhase(PHASE_ACTION);
		}
		else
		{
			m_CurSelectUnit->SelectAction();
			m_CurSelectUnit->SelectEffectOff();
			m_pTurnMgr->GetCurOrderUI()->ActionIconOn(GetActionIcon());
			m_pTurnMgr->ChangePhase(PHASE_ROTATE);
		}

		m_pTurnMgr->SetSkillTrigger(false);
		m_pTurnMgr->CurUnitCollisionOff();
	}

	// 현재 캐릭터가 기존에 이미 행동이 선택되었을 경우 바로 ActionPhase로 이동
	// 기절,침묵 등 디버프의 영향은 Unit의 Action State에서 판단
	
}
void CSelectPhase::MonsterSelect() 
{
	UINT SelectSkillNo = m_CurSelectUnit->GetRandomSkill();
	m_CurSelectUnit->SelectSkill(SelectSkillNo, 0);
	int TurnCount = m_CurSelectUnit->CalTurnCount(m_CurSelectUnit->CurRandomSkillCount());

	m_pTurnMgr->SetRotateCount(TurnCount);

	if (0 >= TurnCount)
	{
		m_CurSelectUnit->SelectEffectOff();
		m_pTurnMgr->ChangePhase(PHASE_ACTION);
	}
	else
	{
		m_CurSelectUnit->SelectAction();
		//m_CurSelectUnit->SelectEffectOff();
		m_pTurnMgr->GetCurOrderUI()->ActionIconOn(GetActionIcon());
		m_pTurnMgr->ChangePhase(PHASE_ROTATE);
	}

	m_pTurnMgr->SetSkillTrigger(false);
	m_pTurnMgr->CurUnitCollisionOff();

	/*m_CurSelectUnit->SelectAction(false);
	m_pTurnMgr->SetRotateCount(0);
	m_pTurnMgr->ChangePhase(PHASE_ACTION);*/
}
const int CSelectPhase::GetActionIcon()
{
	switch (m_pCurSkillData->SkillType)
	{
	case SKILLDATA::SKILLTYPE_ACTION:
		return AICON_ATTCK;
	case SKILLDATA::SKILLTYPE_ABILITY:
	case SKILLDATA::SKILLTYPE_BURSTSKILL:
		return AICON_COMBATLIMIT;
	case SKILLDATA::SKILLTYPE_ITEM:
		return AICON_ITEM;
	}

	return AICON_ATTCK;
}
bool CSelectPhase::CheckSelectUnit()
{
	//// 몬스터의 경우 선택 페이즈가 따로 없으므로 무시

	//if (false == m_CurSelectUnit->IsCharacter())
	//	return m_bCheckUnit;

	// 유닛의 경우 이전에 스킬을 고르고 스킬카운터가 밀린이후에 
	// 돌아온 턴인경우 바로 ActionPhase로 넘어가도록한다.
	if (true == m_CurSelectUnit->IsSelectAction()) 
	{
		m_CurSelectUnit->SelectAction(false);
		m_pTurnMgr->SetRotateCount(0);
		m_pTurnMgr->GetCurOrderUI()->ActionIconOff();
		m_bCheckUnit = true;
	}

	return m_bCheckUnit;
}
void CSelectPhase::StandbyCharacter()
{
	// 현재 선택된 캐릭터가 이미 그전에 스킬이 선택되었다면
	// 바로 Action Phase로 이동
	if (true == CheckSelectUnit())
	{
		m_pTurnMgr->ChangePhase(PHASE_ACTION);
		return;
	}

	m_CurSelectUnit->SelectEffectOn();
	m_pTurnMgr->ClearAllSkillList();


	unordered_map<size_t, size_t>* Temp = m_CurSelectUnit->GetUnitSkillDataMap();

	if (nullptr == Temp)
		return;

	unordered_map<size_t, size_t>::iterator Start = Temp->begin();
	unordered_map<size_t, size_t>::iterator End = Temp->end();


	for (; Start != End; ++Start)
	{
		size_t SkillIndex = Start->first;

		if (SkillIndex >= CClientGlobal::vecSkill.size())
			continue;

		SKILLDATA SkillData = CClientGlobal::vecSkill[SkillIndex].SkillData;

		if (SkillData.SkillType >= SKILLDATA::MAXSKILLTYPE)
			continue;

		m_pTurnMgr->PushSkillIndex(SkillData.SkillType, SkillIndex);
		m_pTurnMgr->SetSkillUILevel(SkillIndex, m_CurSelectUnit->GetSkillLevel(SkillIndex) );

		/*if (m_CurSelectUnit->GetSkillLevel(SkillIndex) >= LEVELDATA::MAXSKILLLEVEL)
			continue;

		if (false == m_CurSelectUnit->CheckSkillAvailable(CClientGlobal::vecSkillLevelData[SkillIndex][m_CurSelectUnit->GetSkillLevel(SkillIndex)])) 
		{
			m_pTurnMgr->
		}*/
	}

	m_pTurnMgr->SkillBoardOn();
	m_pTurnMgr->CurUnitCollisionOn();
}
void CSelectPhase::StandbyMonster()
{
	if (true == CheckSelectUnit())
	{
		m_pTurnMgr->ChangePhase(PHASE_ACTION);
		return;
	}

}