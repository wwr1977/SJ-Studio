#pragma once
#include<BaseState.h>

class CPlayer;
class CTileCheckMgr;
class CMainPlayerState : public CBaseState
{
	friend class CPlayer;

protected:
	static  SPTR<CTileCheckMgr>	TileCheckMgr;
	static	UINT				CurDirNo;
	static  float				PlayerSpeed;

	static  Vec3				PlayerCurDir;
	static  Vec3				PlayerPrevDir;

	static float				AccDirTime;

	static const Vec3 GetCurMoveDir();
public:
	static void SetTileCheckMgr(SPTR<CTileCheckMgr> _Mgr);
	static const UINT GetCurDirNo();
private:
	static void SetCurDirNo(const UINT& _No);



protected:
	CPlayer*					m_pPlayer;

public:
	void Init(void* _Player);
	void Update();
	void MovePlayer(const Vec3& _Dir, const float _Speed,const float& _DeltaTime);

	
public:
	CMainPlayerState();
	~CMainPlayerState();
};

