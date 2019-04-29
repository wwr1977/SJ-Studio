#include "Precom.h"
#include "CharDying.h"
#include "BattleUnit.h"


CCharDying::CCharDying()
{
}


CCharDying::~CCharDying()
{
}
void CCharDying::StateReset() 
{
	m_pUnit->ChangeAni(Dying(m_UnitName));
	m_pUnit->FilterOff();
	m_fAccTime = 0.0f;
}
void CCharDying::Update() 
{

}
