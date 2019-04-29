#include "Precom.h"
#include "MainPlayerState.h"
#include "Player.h"
#include"TileCheckMgr.h"

SPTR<CTileCheckMgr>	CMainPlayerState::TileCheckMgr = nullptr;
UINT	CMainPlayerState::CurDirNo = 0 ;
Vec3	CMainPlayerState::PlayerCurDir = Vec3{0.0f,0.0f,0.0f};
Vec3	CMainPlayerState::PlayerPrevDir = Vec3{ 0.0f,0.0f,0.0f };

float	CMainPlayerState::PlayerSpeed = 0.0f;
float	CMainPlayerState::AccDirTime = 0.0f;

const Vec3 CMainPlayerState::GetCurMoveDir()
{
	return PlayerCurDir;
}
void CMainPlayerState::SetTileCheckMgr(SPTR<CTileCheckMgr> _Mgr)
{
	if (nullptr == _Mgr)
		return;

	TileCheckMgr = _Mgr;
}

const UINT CMainPlayerState::GetCurDirNo()
{
	return CurDirNo;
}
void CMainPlayerState::SetCurDirNo(const UINT& _No)
{
	CurDirNo = _No;
}
CMainPlayerState::CMainPlayerState()
	:m_pPlayer(nullptr)
{
}


CMainPlayerState::~CMainPlayerState()
{
}
void CMainPlayerState::Init(void* _Player)
{
	if (nullptr != _Player)
		m_pPlayer = (CPlayer*)_Player;

}
void CMainPlayerState::Update()
{

}
void CMainPlayerState::MovePlayer(const Vec3& _Dir, const float _Speed, const float& _DeltaTime)
{
	Vec3 MoveDir = Vec3{ 0.0f,0.0f,0.0f };


	MoveDir = m_pPlayer->GetSyncMoveDir(_Dir, _Speed, _DeltaTime);

	/*if (true == m_pPlayer->ChangeTileIndex(PlayerCurDir* PlayerSpeed* DELTATIME))
		MoveDir = m_pPlayer->SynchroMoveDir(PlayerCurDir, PlayerSpeed, DELTATIME);*/

	m_pPlayer->MovePlayer(MoveDir);
}