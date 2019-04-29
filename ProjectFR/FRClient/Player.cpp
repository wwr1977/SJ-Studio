#include "Precom.h"
#include "Player.h"
//#include"MainPlayerState.h"
#include "PlayerIdle.h"
#include "PlayerWalk.h"
#include "PlayerRun.h"
#include "TileCheckMgr.h"
#include "TileMap.h"
#include "Tile.h"
#include "GameMaster.h"


CPlayer::CPlayer()
	: m_pAniRen(nullptr), m_PairMgr(nullptr), m_PlayerSize(Vec3{0.0f,0.0f,1.0f})
	, m_TileCheckMgr(nullptr), m_CurTile(nullptr)
	, m_InitTileIndex(POINT{20,20})
	, m_CheckPivot(Vec3{0.0f,0.0f,0.0f})
	, m_TilePivot(Vec3{0.0f,0.0f,0.0f})
	, m_PortalPivot(Vec3{0.0f,0.0f,0.0f})
	, m_CurMoveDir(Vec3{0.f,0.f,0.f})
	, m_bPlayerPause(false), m_PlayerCol(nullptr)
	, m_bAutoMove(false)
	, m_StartPoint(Vec3{ 0.f,0.f,0.f }), m_EndPoint(Vec3{0.f,0.f,0.f})
	, m_AutoDir(Vec3{ 0.f,0.f,0.f }), m_AutoDistance(0.f)
	, m_fAutoSpeed(150.f)
{
	m_PairMgr = new CStateMgr();
	m_TileCheckMgr = new CTileCheckMgr(this);
	CMainPlayerState::SetTileCheckMgr(m_TileCheckMgr);
	m_AnimationPairDir.clear();
	m_TileCheckPivot.clear();
}


CPlayer::~CPlayer()
{
	m_AnimationPairDir.clear();
	m_TileCheckPivot.clear();
}
void CPlayer::Init()
{
	ACTOR->SetPos(Vec3{ 0.f,0.f,5.0f });

	m_pAniRen = ACTOR->CreateCom<CAniRenderer>(RENDATA(LAYER_MAPOBJECT, 0.85f));
	ACTOR->SetImageScale(1.5f);


	Vec3 TempVec;

	TempVec = Vec3{ 0.0f,-1.0f , 0.0f};
	D3DXVec3Normalize(&TempVec, &TempVec);
	m_AnimationPairDir.push_back(DIRSET(TempVec, _T("Player Dir : Down     ")));

	TempVec = Vec3{ 0.0f,1.0f, 0.0f };
	D3DXVec3Normalize(&TempVec, &TempVec);
	m_AnimationPairDir.push_back(DIRSET(TempVec, _T("Player Dir : Up       ")));

	TempVec = Vec3{ -1.0f,0.0f , 0.0f };
	D3DXVec3Normalize(&TempVec, &TempVec);
	m_AnimationPairDir.push_back(DIRSET(TempVec, _T("Player Dir : Left     ")));


	TempVec = Vec3{ 1.0f,0.0f, 0.0f };
	D3DXVec3Normalize(&TempVec, &TempVec);
	m_AnimationPairDir.push_back(DIRSET(TempVec, _T("Player Dir : Right    ")));
	
	TempVec = Vec3{ -1.0f,-1.0f, 0.0f };
	D3DXVec3Normalize(&TempVec, &TempVec);
	m_AnimationPairDir.push_back(DIRSET(TempVec, _T("Player Dir : LeftDown ")));

	TempVec = Vec3{ 1.0f,-1.0f ,0.0f };
	D3DXVec3Normalize(&TempVec, &TempVec);
	m_AnimationPairDir.push_back(DIRSET(TempVec, _T("Player Dir : RightDown")));

	TempVec = Vec3{ -1.0f,1.0f ,0.0f };
	D3DXVec3Normalize(&TempVec, &TempVec);
	m_AnimationPairDir.push_back(DIRSET(TempVec, _T("Player Dir : LeftUp   ")));
	TempVec = Vec3{ 1.0f,1.0f ,0.0f };
	D3DXVec3Normalize(&TempVec, &TempVec);
	m_AnimationPairDir.push_back(DIRSET(TempVec, _T("Player Dir : RightUp  ")));

	for (int i = 0; i < 8; i++)
	{
		TCHAR AniName[256] = _T("");
		_stprintf_s(AniName, _T("MapRain_Idle%d"), i);
		m_pAniRen->CreateAnimation(AniName, _T("MapRain"), ANI_LOOP, { 0 + 7 * i,6 + 7 * i });
		_stprintf_s(AniName, _T("MapRain_Run%d"), i);
		m_pAniRen->CreateAnimation(AniName, _T("MapRain"), ANI_COUNTCYCLE_REPEAT, { 56 + i * 7 ,62 + i * 7 }, 1, { 57 + i * 7, 62 + i * 7 });
		_stprintf_s(AniName, _T("MapRain_Walk%d"), i);
		m_pAniRen->CreateAnimation(AniName, _T("MapRain"), ANI_COUNTCYCLE_REPEAT, { 112 + i * 7 ,118 + i * 7 }, 1, { 113 + i * 7, 118 + i * 7 });
	}

	m_pAniRen->ChangeAni(_T("MapRain_Idle0"));
	
	m_PlayerSize = Vec3{ m_pAniRen->GetCurAniSpriteSize().x ,m_pAniRen->GetCurAniSpriteSize().y ,1.0f};
	//m_MiddlePivot = Vec3{ 0.0f,m_PlayerSize.y*-0.25f,0.0f };
	m_PortalPivot = Vec3{ 0.0f,m_PlayerSize.y*-0.25f,0.0f };
	m_TilePivot = Vec3{ 0.0f, m_PlayerSize.y*-0.5f ,0.0f };

	float XPivot = m_pAniRen->GetCurAniSpriteSize().x*0.25f;
	float YUpPivot = 0.0f;
	float YDownPivot = m_TilePivot.y - 10.0f;


	m_TileCheckPivot.assign(8, vector<Vec3>());
	m_TileCheckPivot[0].push_back(Vec3{ -XPivot,YDownPivot,0.0f });
	m_TileCheckPivot[0].push_back(Vec3{ XPivot,YDownPivot,0.0f });
	m_TileCheckPivot[1].push_back(Vec3{ -XPivot,YUpPivot,0.0f });
	m_TileCheckPivot[1].push_back(Vec3{ XPivot,YUpPivot,0.0f });
	m_TileCheckPivot[2].push_back(Vec3{ -XPivot,m_PortalPivot.y,0.0f });
	m_TileCheckPivot[2].push_back(Vec3{ -XPivot,YDownPivot,0.0f });
	m_TileCheckPivot[3].push_back(Vec3{ XPivot,m_PortalPivot.y,0.0f });
	m_TileCheckPivot[3].push_back(Vec3{ XPivot,YDownPivot,0.0f });
	m_TileCheckPivot[4].push_back(Vec3{ -XPivot,YDownPivot,0.0f });
	m_TileCheckPivot[4].push_back(Vec3{ 0.0f,YDownPivot,0.0f });
	m_TileCheckPivot[4].push_back(Vec3{ -XPivot,m_PortalPivot.y,0.0f });
	m_TileCheckPivot[5].push_back(Vec3{ XPivot,YDownPivot,0.0f });
	m_TileCheckPivot[5].push_back(Vec3{ 0.0f,YDownPivot,0.0f });
	m_TileCheckPivot[5].push_back(Vec3{ XPivot,m_PortalPivot.y,0.0f });
	m_TileCheckPivot[6].push_back(Vec3{ -XPivot,YUpPivot,0.0f });
	m_TileCheckPivot[6].push_back(Vec3{ 0.0f,YUpPivot,0.0f });
	m_TileCheckPivot[6].push_back(Vec3{ -XPivot,YDownPivot,0.0f });
	//m_TileCheckPivot[6].push_back(Vec3{ -XPivot,m_PortalPivot.y,0.0f });
	m_TileCheckPivot[7].push_back(Vec3{ XPivot,YUpPivot,0.0f });
	m_TileCheckPivot[7].push_back(Vec3{ 0.0f,YUpPivot,0.0f });
	m_TileCheckPivot[7].push_back(Vec3{ XPivot,YDownPivot,0.0f });
	//m_TileCheckPivot[7].push_back(Vec3{ XPivot,m_PortalPivot.y,0.0f });
	
	/*
	m_TileCheckPivot.push_back(Vec3{ 0.0f,YDownPivot,0.0f });
	m_TileCheckPivot.push_back(Vec3{ 0.0f,YUpPivot,0.0f });
	m_TileCheckPivot.push_back(Vec3{ -XPivot,YDownPivot,0.0f });
	m_TileCheckPivot.push_back(Vec3{ XPivot,YDownPivot,0.0f });
	m_TileCheckPivot.push_back(Vec3{ -XPivot,YDownPivot,0.0f });
	m_TileCheckPivot.push_back(Vec3{ XPivot,YDownPivot,0.0f });
	m_TileCheckPivot.push_back(Vec3{ -XPivot,YUpPivot,0.0f });
	m_TileCheckPivot.push_back(Vec3{ XPivot,YUpPivot,0.0f });
	*/

	m_PairMgr->CreateState<CPlayerIdle>(this);
	m_PairMgr->CreateState<CPlayerWalk>(this);
	m_PairMgr->CreateState<CPlayerRun>(this);

	m_PairMgr->ChangeState<CPlayerIdle>();

	m_PlayerCol = ACTOR->CreateCom<CCollision>(COLDATA(_T("Player")));
	m_PlayerCol->SetColSize(Vec2{36.f,64.f});
	m_PlayerCol->SetPivotPos(Vec3{ 0.f,-15.f ,0.f});
}
void CPlayer::PreUpdate()
{
	if(false == m_bPlayerPause)
		m_PairMgr->Update();
}
void CPlayer::Update()
{
	//m_PairMgr->Update();
	AutoMoveUpdate();
}
void CPlayer::PostUpdate()
{
	
}
void CPlayer::DebugRender()
{
	//COMDEBUGMGR->RectRender(D3DCOLOR_ARGB(255, 255, 0, 0), TRANS->GetPos(), m_PlayerSize);
	//COMDEBUGMGR->RectRender(D3DCOLOR_ARGB(255, 255, 0, 0), TRANS->GetPos() + Vec3{0.0f,m_PlayerSize.y*-0.25f,0.0f}, Vec3{ 2.0f,2.0f,2.0f });
	
	COMDEBUGMGR->RectRender(D3DCOLOR_ARGB(255, 255, 0, 0),GetTilePos() , Vec3{ 1.0f,1.0f,1.0f });
	COMDEBUGMGR->RectRender(D3DCOLOR_ARGB(255, 255, 0, 0), GetPortalPos(), Vec3{ 1.0f,1.0f,1.0f });
	
	UINT DirNo = CMainPlayerState::GetCurDirNo();
	

	if (DirNo < m_TileCheckPivot.size()) 
	{
		for (size_t i = 1; i < m_TileCheckPivot[DirNo].size(); ++i)
		{
			Vec3 Mid = 0.5f * (m_TileCheckPivot[DirNo][0] + m_TileCheckPivot[DirNo][i]);
			Vec3 Rect;

			if (m_TileCheckPivot[DirNo][0].x == m_TileCheckPivot[DirNo][i].x)
			{
				Rect = Vec3{ 1.0f,m_TileCheckPivot[DirNo][0].y - m_TileCheckPivot[DirNo][i].y ,2.0f };
				if (Rect.y <= 0.0f)
					Rect.y = -Rect.y;
			}
			else
			{
				Rect = Vec3{ m_TileCheckPivot[DirNo][0].x - m_TileCheckPivot[DirNo][i].x,1.0f ,2.0f };
				if (Rect.x <= 0.0f)
					Rect.x = -Rect.x;
			}
			
			COMDEBUGMGR->RectRender(D3DCOLOR_ARGB(255, 255, 0, 0), TRANS->GetPos()+ Mid, Rect);
		}
	}


}
void CPlayer::ChangeState(const UINT& _StateIndex)
{
	if (nullptr == m_PairMgr || _StateIndex >= MAXPLAYERSTATE)
		return;

	m_PairMgr->ChangeState(_StateIndex);
}
const int CPlayer::GetCurStateIndex()
{
	return m_PairMgr->GetCurStateIndex();
}
CTransForm* CPlayer::GetTransForm()
{
	return TRANS;
}
void CPlayer::SetAutoMoveData(const PLAYER_STATE& _State, const Vec3& _End, const Vec3& _Dir) 
{
	m_bAutoMove = true;
	m_PairMgr->ChangeState(STATE_IDLE);

	Vec3 Dir = _Dir;
	D3DXVec3Normalize(&Dir, &Dir);
	ChangeAni(_State, Dir);

	m_StartPoint = TRANS->GetPos();
	m_EndPoint = _End;
	m_EndPoint.z = m_StartPoint.z;

	m_AutoDir = Dir;
	m_AutoDir.z = 0.f;

	Vec3 Dis = m_EndPoint - m_StartPoint;
	m_AutoDistance = D3DXVec3Length(&Dis);


	switch (_State)
	{
	case CPlayer::STATE_IDLE:
		m_fAutoSpeed = 0.f;
		break;
	case CPlayer::STATE_WALK:
		m_fAutoSpeed = 150.f;
		break;
	case CPlayer::STATE_RUN:
		m_fAutoSpeed = 230.f;
		break;
	}

}
void CPlayer::AutoMoveUpdate() 
{
	if (!m_bAutoMove)
		return;

	Vec3 MoveDir = m_AutoDir * DELTATIME * m_fAutoSpeed;
	Vec3 Dis = TRANS->GetPos() + MoveDir - m_StartPoint;
	
	float CurDis = D3DXVec3Length(&Dis);

	if ( CurDis >= m_AutoDistance)
	{
		TRANS->SetPos(m_EndPoint);
		ChangeAni(STATE_IDLE, m_AutoDir);
		m_bAutoMove = false;
	}
	else
		TRANS->Move(MoveDir);
}
void CPlayer::ChangeAni(const tstring& _AniKey)
{
	if (nullptr == m_pAniRen)
		return;

	m_pAniRen->ChangeAni(_AniKey);
}
void CPlayer::ChangeAni(const CPlayer::PLAYER_STATE& _State, const Vec3& _Dir)
{
	if (nullptr == m_pAniRen)
		return;

	for (int i = 0; i < (int)m_AnimationPairDir.size(); i++)
	{
		if (m_AnimationPairDir[i].Dir == _Dir)
		{
			TCHAR Buf[MAXKEYLENGTH] = _T("");

			switch (_State)
			{
			case CPlayer::STATE_IDLE: 
				_stprintf_s(Buf, _T("MapRain_Idle%d"), i);
				break;
			case CPlayer::STATE_WALK:
				_stprintf_s(Buf, _T("MapRain_Walk%d"), i);
				break;
			case CPlayer::STATE_RUN:
				_stprintf_s(Buf, _T("MapRain_Run%d"), i);
				break;
			}
			m_pAniRen->ChangeAni(Buf);
			CMainPlayerState::SetCurDirNo(i);
			return;
		}
	}

}
const tstring CPlayer::GetCurDirString()
{
	UINT DirNo = CMainPlayerState::GetCurDirNo();
	if (DirNo >= m_AnimationPairDir.size())
		return _T("");

	return m_AnimationPairDir[DirNo].DirString;
}
const tstring CPlayer::GetCurTileStateString()
{
	if (nullptr == m_CurTile)
		return _T("");

	TILEOPTION Op = m_TileCheckMgr->CurTileOption();
	
	switch (Op)
	{
	case STEPTILE:
		return _T("CurTile : Step Tile");
	case STAIRTILE:
		return _T("CurTile : Stair Tile");
	case STEPSTAIRTILE:
		return _T("CurTile : StepStair Tile");
	case MAPCHANGETILE:
		return _T("CurTile : Portal Tile");
	case NOENTERTILE:
		return _T("CurTile : No Enter Tile");
	}

	return _T("");
}
const tstring CPlayer::GetCurTileColorString()
{
	if (nullptr == m_CurTile)
		return _T("");

	//TILEOPTION Op = m_TileCheckMgr->ChangeTileState();
	TILEOPTION Op = m_CurTile->GetTileOption();

	if (STEPTILE != Op && STEPSTAIRTILE != Op)
		return _T("");

	UINT ColorNo = m_CurTile->GetTileColorNo();

	switch (ColorNo)
	{
	case 0:
		return _T("Tile Color : Green");
	case 1:
		return _T("Tile Color : Blue");
	case 2:
		return _T("Tile Color : Yellow");
	}

	return _T("");
}
bool CPlayer::RunCheck()
{
	if (true == KEYDOUBLE("UP") || true == KEYDOUBLE("DOWN") ||
		true == KEYDOUBLE("LEFT") || true == KEYDOUBLE("RIGHT"))
		return true;

	return false;

}
bool CPlayer::KeyCheck()
{
	if (true == KEY("UP") || true == KEY("DOWN") ||
		true == KEY("LEFT") || true == KEY("RIGHT"))
		return true;

	return false;
}
bool CPlayer::DirUpdate(Vec3& _PrevDir, Vec3& _CurDir, UINT& _DirNo)
{
	if (nullptr == &_PrevDir || nullptr == &_CurDir || nullptr == &_DirNo)
		return false;


	_CurDir = Vec3{ 0.0f,0.0f ,0.0f };

	if (true == KEY("UP"))
		_CurDir += GAMEVEC::UP;
	if (true == KEY("DOWN"))
		_CurDir += GAMEVEC::DOWN;
	if (true == KEY("LEFT"))
		_CurDir += GAMEVEC::LEFT;
	if (true == KEY("RIGHT"))
		_CurDir += GAMEVEC::RIGHT;

	D3DXVec3Normalize(&_CurDir, &_CurDir);

	bool Check = (_PrevDir != _CurDir);
	_PrevDir = _CurDir;


	GetDirNo(_CurDir, _DirNo);
		

	return Check;
}
void CPlayer::GetDirNo(const Vec3& _Dir, UINT& _DirNo)
{
	for (size_t i = 0; i < m_AnimationPairDir.size(); i++)
	{
		if (_Dir == m_AnimationPairDir[i].Dir)
		{
			_DirNo = (UINT)i;
			return;
		}
	}
}
const Vec3 CPlayer::GetCurMoveDir()
{
	return m_CurMoveDir;
}
void CPlayer::MovePlayer(const Vec3& _MoveDir)
{
	ACTOR->Move(_MoveDir);
	
	D3DXVec3Normalize(&m_CurMoveDir, &_MoveDir);
	
	Vec3 PPos = GetPortalPos();
	Vec3 TPos = GetTilePos();

	SPTR<CTile> PTile = CClientGlobal::CurTileMap->GetTile(PPos);

	if (nullptr == CClientGlobal::CurTileMap)
		return;

	if (nullptr != CClientGlobal::CurTileMap->GetTile(TPos))
		m_CurTile = CClientGlobal::CurTileMap->GetTile(TPos);

	if (m_CurTile->GetTileOption() <= STEPSTAIRTILE)
		m_TileCheckMgr->ChangeTileState(m_CurTile->GetTileOption());

	if (nullptr == PTile)
		return;

	TILEOPTION Op = PTile->GetTileOption();

	if (Op == MAPCHANGETILE || Op == WORLDMAPTILE)
		CClientGlobal::GameMaster->MapChangeEvent(CClientGlobal::CurTileMap->GetTile(PPos));

}
void CPlayer::PushPlayer(const Vec3& _PushDir)
{
	ACTOR->Move(_PushDir);

	Vec3 PPos = GetPortalPos();
	Vec3 TPos = GetTilePos();

	SPTR<CTile> PTile = CClientGlobal::CurTileMap->GetTile(PPos);

	if (nullptr == CClientGlobal::CurTileMap)
		return;

	if (nullptr != CClientGlobal::CurTileMap->GetTile(TPos))
		m_CurTile = CClientGlobal::CurTileMap->GetTile(TPos);

	if (m_CurTile->GetTileOption() <= STEPSTAIRTILE)
		m_TileCheckMgr->ChangeTileState(m_CurTile->GetTileOption());

	if (nullptr == PTile)
		return;

	TILEOPTION Op = PTile->GetTileOption();

	if (Op == MAPCHANGETILE || Op == WORLDMAPTILE)
		CClientGlobal::GameMaster->MapChangeEvent(CClientGlobal::CurTileMap->GetTile(PPos));

}
const Vec3 CPlayer::GetSyncMoveDir(const Vec3& _Dir, const float& _Speed, const float _DeltaTime)
{
	Vec3 MoveDir = _Dir*_Speed*_DeltaTime;
	

	Vec3 CheckPivot = Vec3{ 0.0f,0.0f,0.0f };

	if (nullptr == CClientGlobal::CurTileMap)
		return Vec3{0.0f,0.0f,0.0f};

	UINT Dir = CMainPlayerState::GetCurDirNo();

	for (size_t i = 1; i < m_TileCheckPivot[Dir].size(); ++i)
	{
		Vec3 Start = m_TileCheckPivot[Dir][0];
		Vec3 End = m_TileCheckPivot[Dir][i];

		SPTR<CTile> CheckTile = nullptr;
		float t = 0.0f;

		while (t <= 1.0f)
		{
			CheckPivot = CSJMath::Vec3Lerp(&Start, &End, t);
			CheckTile = GetMoveAfterTile(MoveDir, CheckPivot);

			if (true == ChangeTileIndex(CheckTile)) 
			{
				if (false == CheckChangeOKTile(CheckTile))
				{
					m_CheckPivot = CheckPivot;

					return SynchroMoveDir(_Dir, _Speed, _DeltaTime);
				}
			}

			t += 0.1f;
		}			
			
	}

	return MoveDir;
}
const Vec3 CPlayer::SynchroMoveDir(const Vec3& _Dir, const float& _Speed,const float _DeltaTime)
{
	if (nullptr == CClientGlobal::CurTileMap)
		return Vec3{ 0.0f,0.0f,0.0f };

	Vec3 PivotPos = TRANS->GetPos()+ m_CheckPivot;
	Vec3 ChangePos = PivotPos + _Dir*_Speed*_DeltaTime;

	SPTR<CTile> PivotTile = CClientGlobal::CurTileMap->GetTile(PivotPos);
	SPTR<CTile> ChangeTile = CClientGlobal::CurTileMap->GetTile(ChangePos);

	if (nullptr == ChangeTile) 
		return Vec3{ 0.0f,0.0f,0.0f };

	if (true == m_TileCheckMgr->CheckChangeTile(PivotTile,ChangeTile))
		return _Dir*_Speed*_DeltaTime;
	else 
	{
		Vec3 SyncDir = Vec3{ 0.0f,0.0f,0.0f };
		
		int Delta = 5;
		
		float dt = _DeltaTime / (float)Delta;
		float DeltaDir = 0.0f;

		SPTR<CTile> CheckTile = nullptr;

		for (int  i = 1; i <= Delta; ++i)
		{
			DeltaDir = _Dir.x*_Speed*dt*i;
			SyncDir.x = _Dir.x*_Speed*dt*(i-1);

			CheckTile = CClientGlobal::CurTileMap->GetTile(PivotPos + Vec3{ DeltaDir,0.0f,0.0f });
			
			if (nullptr != CheckTile && false == m_TileCheckMgr->CheckChangeTile(PivotTile,CheckTile))
				break;
		}

		CheckTile = nullptr;

		for (int j = 1; j <= Delta; ++j)
		{
			DeltaDir = _Dir.y*_Speed*dt*j;
			SyncDir.y = _Dir.y*_Speed*dt*(j - 1);
			
			CheckTile = CClientGlobal::CurTileMap->GetTile(PivotPos + Vec3{ 0.0f,DeltaDir,0.0f });

			if (nullptr != CheckTile && false == m_TileCheckMgr->CheckChangeTile(PivotTile,CheckTile))
				break;
		}

		return SyncDir;
	}
}
const POINT CPlayer::CurTileIndex()
{
	return m_CurTile->GetTileIndex();
}
const bool	CPlayer::ChangeTileIndex(const Vec3& _MoveDir,const Vec3& _Check)
{
	if (nullptr == CClientGlobal::CurTileMap)
		return false;

	UINT Dir = CMainPlayerState::GetCurDirNo();

	Vec3 MovePos = TRANS->GetPos() + _MoveDir + _Check;
	SPTR<CTile> MoveTile = CClientGlobal::CurTileMap->GetTile(MovePos);

	if (nullptr != MoveTile)
	{
		if (m_CurTile == MoveTile)
			return false;
		else
			return true;
	}

	return false;
}
const bool	CPlayer::ChangeTileIndex(SPTR<CTile> _CheckTile)
{
	if (nullptr != _CheckTile)
	{
		if (m_CurTile == _CheckTile)
			return false;
		else
			return true;
	}

	return false;
}
const bool  CPlayer::CheckChangeOKTile(SPTR<CTile> _CheckTile)
{
	if (nullptr == _CheckTile)
		return false;

	return m_TileCheckMgr->CheckChangeTile(m_CurTile , _CheckTile);

}
SPTR<CTile>	CPlayer::GetMoveAfterTile(const Vec3& _MoveDir, const Vec3& _Pivot)
{
	Vec3 MovePos = TRANS->GetPos() + _MoveDir + _Pivot;
	return CClientGlobal::CurTileMap->GetTile(MovePos);
}
void CPlayer::InitCurTile()
{
	if (nullptr == CClientGlobal::CurTileMap)
		return;

	SetPos(CClientGlobal::CurTileMap->TileIndexToWolrdPos2(m_InitTileIndex) - Vec2{ m_PortalPivot.x,m_PortalPivot.y });
	m_CurTile = CClientGlobal::CurTileMap->GetTile(TRANS->GetPos() + m_PortalPivot);
	
	if ( m_CurTile->GetTileOption() <= STEPSTAIRTILE)
		m_TileCheckMgr->ChangeTileState(m_CurTile->GetTileOption());

	return;
}
void CPlayer::SetPos(const Vec3& _Pos)
{
	TRANS->SetPos(_Pos);
}
void CPlayer::SetPos(const Vec2& _Pos)
{
	TRANS->SetPos(Vec3{ _Pos.x,_Pos.y,TRANS->GetPos().z } );
}
//Vec3 CPlayer::GetMiddlePos()
//{
//	return TRANS->GetPos() + m_MiddlePivot;
//}
void CPlayer::SetInitTileIndex(const POINT& _Index)
{
	m_InitTileIndex = _Index;
}
Vec3 CPlayer::GetTilePos()
{
	return TRANS->GetPos() + m_TilePivot;
}
Vec3 CPlayer::GetPortalPos()
{
	return TRANS->GetPos() + m_PortalPivot;
}
void CPlayer::On() 
{
	ACTOR->On();
}
void CPlayer::Off()
{
	ACTOR->Off();
}
void CPlayer::InitPlayerMoveState()
{
	CMainPlayerState::CurDirNo = 1;
	ChangeState(PLAYER_IDLE);
	//ChangeAni(_T("MapRain_Idle1"));
}
void CPlayer::GameStart()
{
	m_InitTileIndex = POINT{ 20,20 };
	CMainPlayerState::CurDirNo = 0;
	ChangeState(PLAYER_IDLE);
}
void CPlayer::PlayerPause(const bool& _Pause)
{
	m_bPlayerPause = _Pause;
	
	ChangeState(PLAYER_IDLE);
}