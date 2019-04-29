#include "Precom.h"
#include "CharDead.h"
#include "BattleUnit.h"


CCharDead::CCharDead()
{
}


CCharDead::~CCharDead()
{
}
void CCharDead::StateReset() 
{
	m_pUnit->ChangeAni(Dead(m_UnitName));
	m_fAccTime = 0.0f;
}
void CCharDead::Update() 
{

}
