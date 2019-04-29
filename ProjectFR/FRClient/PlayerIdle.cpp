#include "Precom.h"
#include "PlayerIdle.h"
#include"Player.h"


CPlayerIdle::CPlayerIdle()
{
}


CPlayerIdle::~CPlayerIdle()
{
}
void CPlayerIdle::StateReset()
{
	if (nullptr == m_pPlayer)
		return;
	
	PlayerPrevDir = Vec3{ 0.0f,0.0f,0.0f };
	AccDirTime = 0.0f;
	m_pPlayer->ChangeAni(GetCurAnimationName());
}
void CPlayerIdle::Update()
{
	if (nullptr == m_pPlayer)
		return;

	if (true == m_pPlayer->RunCheck())
	{
		m_pPlayer->ChangeState(PLAYER_RUN);
		return;
	}

	AccDirTime += DELTATIME;
	
	if (AccDirTime >= 0.08f)
	{
		if (true == IdleUpdate())
			return;

		AccDirTime = 0.0f;
	}
	
}
bool CPlayerIdle::IdleUpdate()
{
	m_pPlayer->DirUpdate(PlayerPrevDir, PlayerCurDir, CurDirNo);

	if (GAMEVEC::ZERO != PlayerCurDir) 
	{
		m_pPlayer->ChangeState(PLAYER_WALK);
		return true;
	}

	return false;
}
const tstring CPlayerIdle::GetCurAnimationName()
{
	TCHAR Temp[SOUNDNAMELENGTH] = _T("");
	_stprintf_s(Temp,_T("MapRain_Idle%u"), CurDirNo);

	return Temp;
}