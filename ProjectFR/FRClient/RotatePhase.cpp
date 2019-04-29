#include "Precom.h"
#include "BattleUnit.h"
#include "RotatePhase.h"
#include "TurnMgr.h"
#include "TurnOrderUI.h"

CRotatePhase::CRotatePhase()
	:m_iRotateCount(-1), m_fAccTime(0.0f), m_CurTurnUI(nullptr)
{
}


CRotatePhase::~CRotatePhase()
{
}
void CRotatePhase::StateReset()
{
	if (nullptr != m_pCurTarget) 
	{
		for (size_t i = 0; i < m_pCurTarget->size(); i++)
			(*m_pCurTarget)[i]->EndReAction();	
	}

	m_CurTurnUI = m_pTurnMgr->GetCurOrderUI();
	
	assert(m_CurTurnUI);
	m_pTurnMgr->PopDeadUnitUI();

	m_iRotateCount = m_pTurnMgr->GetRotateCount();
	m_fAccTime = 0.0f;

	
}
void CRotatePhase::Update()
{

	if (true == TurnRotation()) 
	{
		m_pTurnMgr->MergeDemageList();
		m_pTurnMgr->ChangePhase(PHASE_SELECT);	
	}


}
bool CRotatePhase::TurnRotation()
{
	return m_pTurnMgr->UpdateTurnUIPos();

}