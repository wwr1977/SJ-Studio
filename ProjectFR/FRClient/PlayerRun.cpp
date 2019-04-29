#include "Precom.h"
#include "PlayerRun.h"
#include"Player.h"

CPlayerRun::CPlayerRun()
{
}


CPlayerRun::~CPlayerRun()
{
}

void CPlayerRun::StateReset() 
{
	if (nullptr == m_pPlayer)
		return;


	AccDirTime = 0.0f;
	PlayerSpeed = 230.0f;

	m_pPlayer->DirUpdate(PlayerPrevDir, PlayerCurDir, CurDirNo);
	m_pPlayer->ChangeAni(GetCurAnimationName());
	
	MovePlayer(PlayerCurDir, PlayerSpeed, DELTATIME);
}
void CPlayerRun::Update()
{
	if (nullptr == m_pPlayer)
		return;

	AccDirTime += DELTATIME;


	if (AccDirTime >= 0.08f)
	{
		if (true == RunUpdate())
			return;

		AccDirTime = 0.0f;
	}

	
	MovePlayer(PlayerCurDir, PlayerSpeed, DELTATIME);
}
bool CPlayerRun::RunUpdate()
{
	bool Check = m_pPlayer->DirUpdate(PlayerPrevDir, PlayerCurDir, CurDirNo);
	
	if (false == m_pPlayer->KeyCheck()|| GAMEVEC::ZERO == PlayerCurDir)
	{
		m_pPlayer->ChangeState(PLAYER_IDLE);
		return true;
	}

	if(true == Check)
		m_pPlayer->ChangeAni(GetCurAnimationName());


	return false;
}
const tstring CPlayerRun::GetCurAnimationName()
{
	TCHAR Temp[SOUNDNAMELENGTH] = _T("");
	_stprintf_s(Temp, _T("MapRain_Run%u"), CurDirNo);


	return Temp;
}