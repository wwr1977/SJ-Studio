#pragma once
#include <Logic.h>
class CEditCameraMen : public CLogic
{
private:
	SPTR<CStateMgr>					m_PairMgr;
	float							m_fSpeed;
public:
	void Init();
	void PreUpdate();
	void Update();
	void PostUpdate();
	void DebugRender();

public:
	void CamMove(const Vec3& _Dir);
	void ChangeState(int _Index);

public:
	CEditCameraMen();
	~CEditCameraMen();
};

