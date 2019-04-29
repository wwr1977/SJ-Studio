#include "Precom.h"
#include "PlayerWalk.h"
#include"Player.h"

CPlayerWalk::CPlayerWalk()
{
}


CPlayerWalk::~CPlayerWalk()
{
}
void CPlayerWalk::StateReset() 
{
	if (nullptr == m_pPlayer)
		return;

	AccDirTime = 0.0f;
	PlayerSpeed = 150.0f;

	m_pPlayer->DirUpdate(PlayerPrevDir, PlayerCurDir, CurDirNo);
	m_pPlayer->ChangeAni(GetCurAnimationName());

	MovePlayer(PlayerCurDir, PlayerSpeed, DELTATIME);
}
void CPlayerWalk::Update()
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
		if (true == WalkUpdate())
			return;

		AccDirTime = 0.0f;
	}

	MovePlayer(PlayerCurDir, PlayerSpeed, DELTATIME);
}
bool CPlayerWalk::WalkUpdate()
{
	bool Check = m_pPlayer->DirUpdate(PlayerPrevDir, PlayerCurDir, CurDirNo);

	if (false == m_pPlayer->KeyCheck() || GAMEVEC::ZERO == PlayerCurDir)
	{
		m_pPlayer->ChangeState(PLAYER_IDLE);
		return true;
	}

	if (true == Check)
		m_pPlayer->ChangeAni(GetCurAnimationName());
	
	return false;
}
const tstring CPlayerWalk::GetCurAnimationName()
{
	TCHAR Temp[SOUNDNAMELENGTH] = _T("");
	_stprintf_s(Temp, _T("MapRain_Walk%u"), CurDirNo);

	return Temp;
}