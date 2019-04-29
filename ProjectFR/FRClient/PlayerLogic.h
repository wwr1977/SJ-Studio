#pragma once
#include <Logic.h>

class CCollision;
class CAniRenderer;
class CFixedRenderer;
class CPlayerLogic : public CLogic
{
private:
	SPTR<CStateMgr>					m_PairMgr;
	SPTR<CAniRenderer>				m_pAniRen;

	bool							m_bRun;
	bool							m_bChangeAni;
	int								m_iAniIndex;
	Vec3							m_MoveDir;

public:
	void Init();
	void PreUpdate();
	void Update();
	void PostUpdate();
	void DebugRender();
	

	void Enter(SPTR<CCollision> _pThis, SPTR<CCollision> _pOtherCol);
	void Stay(SPTR<CCollision> _pThis, SPTR<CCollision> _pOtherCol);
	void Exit(SPTR<CCollision> _pThis, SPTR<CCollision> _pOtherCol);

public:
	void ChangeState(int _Index);
	void ChangeAni(const tstring& _AniKey);

public:
	CPlayerLogic();
	~CPlayerLogic();
};

