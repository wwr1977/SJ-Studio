#include "Precom.h"
#include "CharReAction.h"
#include"BattleUnit.h"

CCharReAction::CCharReAction()
{
}


CCharReAction::~CCharReAction()
{
}
void CCharReAction::StateReset()
{
	m_fAccTime = 0.0f;
	m_pUnit->ChangeAni(Dying(m_UnitName));
	////m_pUnit->SetFilter(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), 0.7f);
	//int Random = CSJRandom::RandomInt(REACTION_DEFAULT, REACTION_VIBRATION);
	//
	//m_pUnit->ReActionEvent(REACTION_VIBRATION);
}
void CCharReAction::Update()
{
	/*if (m_fAccTime >= 0.7f)
	{
		m_pUnit->ChangeState(BATTLE_IDLE);
		return;
	}
*/
	

	m_fAccTime += DELTATIME;

}
