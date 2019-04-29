#include "stdafx.h"
#include "EditCharIdle.h"
#include"EditBattleUnit.h"

CEditCharIdle::CEditCharIdle()
{

}


CEditCharIdle::~CEditCharIdle()
{
}
void CEditCharIdle::StateReset()
{
	m_pUnit->ChangeAni(Idle(m_UnitName));
	m_fAccTime = 0.0f;
}
void CEditCharIdle::Update()
{

}
