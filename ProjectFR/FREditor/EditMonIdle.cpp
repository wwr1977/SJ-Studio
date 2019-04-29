#include "stdafx.h"
#include "EditMonIdle.h"
#include "EditBattleUnit.h"


CEditMonIdle::CEditMonIdle()
{
}


CEditMonIdle::~CEditMonIdle()
{
}
void CEditMonIdle::StateReset()
{
	m_pUnit->ChangeAni(Idle(m_UnitName));
	m_fAccTime = 0.0f;
}
void CEditMonIdle::Update()
{

}