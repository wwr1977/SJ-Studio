#include "Precom.h"
#include "MonIdle.h"
#include "BattleUnit.h"


CMonIdle::CMonIdle()
{
}


CMonIdle::~CMonIdle()
{
}
void CMonIdle::StateReset() 
{
	m_pUnit->ChangeAni(Idle(m_UnitName));
	m_fAccTime = 0.0f;
}
void CMonIdle::Update()
{

}