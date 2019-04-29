#include "Precom.h"
#include "CharWin.h"
#include "BattleUnit.h"


CCharWin::CCharWin()
{
}


CCharWin::~CCharWin()
{
}
void CCharWin::StateReset() 
{
	if (nullptr == m_pUnit &&false ==  m_pUnit->IsCharacter())
		return;

	m_pUnit->ChangeAni(WinBe(m_pUnit->GetUnitName()));
}
void CCharWin::Update() 
{
	if (nullptr == m_pUnit && false == m_pUnit->IsCharacter())
		return;

	if( true == m_pUnit->IsCurAniEnd())
		m_pUnit->ChangeAni(Win(m_pUnit->GetUnitName()));
}