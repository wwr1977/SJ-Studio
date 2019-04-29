#include "Precom.h"
#include "CharIdle.h"
#include"BattleUnit.h"

CCharIdle::CCharIdle()
{

}


CCharIdle::~CCharIdle()
{
}
void CCharIdle::StateReset()
{
	m_pUnit->ChangeAni(Idle(m_UnitName));
	m_fAccTime = 0.0f;
}
void CCharIdle::Update()
{

}
