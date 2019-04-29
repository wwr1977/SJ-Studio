#include "stdafx.h"
#include "EditBattleState.h"
#include "EditBattleUnit.h"

CEditBattleState::CEditBattleState()
	:m_UnitName(_T("")),m_pUnit(nullptr)
	,m_fAccTime(0.0f), m_UnitSoundPlayer(nullptr)
	, m_fActionTime(0.0f)
{

}


CEditBattleState::~CEditBattleState()
{
	
}
void CEditBattleState::Init(void* _pLogic)
{
	m_pUnit = (CEditBattleUnit*)_pLogic;
	m_UnitSoundPlayer = m_pUnit->GetUnitSoundPlayer();
	m_UnitName = m_pUnit->GetUnitName();
}
CEditBattleUnit* CEditBattleState::GetPlayer()
{
	return m_pUnit;
}

