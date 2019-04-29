#include "stdafx.h"
#include "EditMonReAction.h"
#include "EditBattleUnit.h"


CEditMonReAction::CEditMonReAction()
{
}


CEditMonReAction::~CEditMonReAction()
{

}
void CEditMonReAction::StateReset()
{
	if (nullptr != m_pUnit) 
	{
		m_pUnit->AniRenOff();
		m_pUnit->ReactionRenOn();
	}
}
void CEditMonReAction::Update()
{

}
