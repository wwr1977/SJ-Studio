#include "Precom.h"
#include "PlayerReAction.h"
#include "PlayerLogic.h"

CPlayerReAction::CPlayerReAction()
{
}


CPlayerReAction::~CPlayerReAction()
{
}
void CPlayerReAction::StateReset()
{
	m_pPlayer->ChangeAni(_T("DemonRain_limit_atk"));
	m_fAccTime = 0.0f;
}
void CPlayerReAction::Update()
{
	m_fAccTime += DELTATIME;


	if (true == KEY("Cancle"))
	{
		m_pPlayer->ChangeState(IDLE);
	}

	if (m_fAccTime >= 10.0f)
	{
		m_pPlayer->ChangeState(IDLE);
	}
}