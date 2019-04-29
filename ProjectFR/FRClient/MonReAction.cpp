#include "Precom.h"
#include "MonReAction.h"
#include "BattleUnit.h"


CMonReAction::CMonReAction()
{
}


CMonReAction::~CMonReAction()
{

}
void CMonReAction::StateReset()
{
	if (nullptr != m_pUnit) 
	{
		m_pUnit->AniRenOff();
		m_pUnit->ReactionRenOn();
	}
}
void CMonReAction::Update()
{

}
