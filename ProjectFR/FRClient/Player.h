#pragma once
#include <Logic.h>


class CTileCheckMgr;
class CAniRenderer;
class CTile;
class CPlayer : public CLogic
{
public:
	enum PLAYER_STATE
	{
		STATE_IDLE,
		STATE_WALK,
		STATE_RUN,
		MAXPLAYER_STATE,

	};

private:
	SPTR<CStateMgr>					m_PairMgr;
	SPTR<CTileCheckMgr>				m_TileCheckMgr;
	SPTR<CAniRenderer>				m_pAniRen;
	
	Vec3							m_PlayerSize;
	Vec3							m_CurMoveDir;
	
	//Vec3							m_MiddlePivot;
	Vec3							m_TilePivot;
	Vec3							m_PortalPivot;
	
	vector<DIRSET>					m_AnimationPairDir;
	SPTR<CTile>						m_CurTile;
	
	vector<vector<Vec3>>			m_TileCheckPivot;
	//vector<Vec3>					m_TileCheckPivot;
	POINT							m_InitTileIndex;
	// 타일이 변경됨을 감지하였을때 충돌선분위의 점을 저장한다.
	Vec3							m_CheckPivot;
	bool							m_bPlayerPause;

	SPTR<CCollision>				m_PlayerCol;

	bool							m_bAutoMove;
	Vec3							m_StartPoint;
	Vec3							m_EndPoint;
	float 							m_AutoDistance;
	Vec3							m_AutoDir;
	float							m_fAutoSpeed;


public:
	void Init();
	void PreUpdate();
	void Update();
	void PostUpdate();
	void DebugRender();
	void ChangeState(const UINT& _StateIndex);
	const int GetCurStateIndex();
	CTransForm* GetTransForm();

public:
	void SetAutoMoveData(const PLAYER_STATE& _State,const Vec3& _End,const Vec3& _Dir);
	void AutoMoveUpdate();

public:
	void ChangeAni(const tstring& _AniKey);
	void ChangeAni(const PLAYER_STATE& _State,const Vec3& _Dir);
	const tstring GetCurDirString();
	const tstring GetCurTileStateString();
	const tstring GetCurTileColorString();
	
public:
	bool RunCheck();
	bool KeyCheck();
	// Prev Dir과 CurDir이 다를때 true를 리턴
	bool DirUpdate(Vec3& _PrevDir, Vec3& CurDir, UINT& _DirNo);
	void GetDirNo(const Vec3& _Dir, UINT& _DirNo);
	const Vec3 GetCurMoveDir();
	void MovePlayer(const Vec3& _MoveDir);
	void PushPlayer(const Vec3& _PushDir);
	const Vec3 GetSyncMoveDir(const Vec3& _Dir, const float& _Speed, const float _DeltaTime);
	const Vec3 SynchroMoveDir(const Vec3& _Dir, const float& _Speed, const float _DeltaTime);
	const POINT CurTileIndex();
	// 타일의 인덱스가 바뀌었는지 체크
	const bool	ChangeTileIndex(const Vec3& _MoveDir, const Vec3& _Check);
	const bool	ChangeTileIndex(SPTR<CTile> _CheckTile);
	// 밟을수 있는 타일인지 체크하는 함수
	const bool  CheckChangeOKTile(SPTR<CTile> _CheckTile);
	SPTR<CTile>	GetMoveAfterTile(const Vec3& _MoveDir, const Vec3& _Pivot);
	void InitCurTile();
	void SetPos(const Vec3& _Pos);
	void SetPos(const Vec2& _Pos);
	//Vec3 GetMiddlePos();
	void SetInitTileIndex(const POINT& _Index);
	Vec3 GetTilePos();
	Vec3 GetPortalPos();

	void On();
	void Off();
	void InitPlayerMoveState();
	void GameStart();
	void PlayerPause(const bool& _Pause);

public:
	CPlayer();
	~CPlayer();
};

