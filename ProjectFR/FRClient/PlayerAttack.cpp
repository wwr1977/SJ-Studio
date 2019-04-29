#include "Precom.h"
#include "PlayerAttack.h"
#include"PlayerLogic.h"

CPlayerAttack::CPlayerAttack()
	:m_fAccTime(0.0f)
{
}


CPlayerAttack::~CPlayerAttack()
{
}
void CPlayerAttack::StateReset()
{
	m_pPlayer->ChangeAni(_T("DemonRain_DoubleAtk"));
	m_fAccTime = 0.0f;
}
void CPlayerAttack::Update()
{
	m_fAccTime += DELTATIME;


	if (true == KEY("Cancle")) 
	{
		m_pPlayer->ChangeState(IDLE);
	}

	if (m_fAccTime >= 6.0f) 
	{
		m_pPlayer->ChangeState(IDLE);
	}

}
