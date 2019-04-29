#include "Precom.h"
#include "RelocationPhase.h"
#include "TurnMgr.h"

CRelocationPhase::CRelocationPhase()
{
}


CRelocationPhase::~CRelocationPhase()
{
}
void CRelocationPhase::StateReset()
{
	if (nullptr  == m_pTurnMgr)
		return;

	m_pTurnMgr->PopDeadUnitUI();
	m_pTurnMgr->RelocationNextMonster();
	m_pTurnMgr->CurCharTargetOverride();
}
void CRelocationPhase::Update()
{

	if (nullptr == m_pTurnMgr)
		return;

	if (true == m_pTurnMgr->FinishRelocation())
	{
		m_pTurnMgr->RelocationEnd();
		m_pTurnMgr->ChangePhase(PHASE_ROTATE);
	}
}
