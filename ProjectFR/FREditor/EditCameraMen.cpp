#include "stdafx.h"
#include "EditCameraMen.h"
#include"CamWaiting.h"
#include"CamMove.h"
#include<PersCamera.h>


CEditCameraMen::CEditCameraMen()
	:m_fSpeed(100.0f),m_PairMgr(nullptr)
{
}


CEditCameraMen::~CEditCameraMen()
{
}

void CEditCameraMen::Init() 
{
	CEditGlobal::EditMainCamera = ParentActObject()->CreateCom<CCamera>();
	ParentActObject()->CreateCom<CPersCamera>();
	m_PairMgr = new CStateMgr();
	m_PairMgr->CreateState<CCamWaiting>(this);
	m_PairMgr->CreateState<CCamMove>(this);

	m_PairMgr->ChangeState<CCamWaiting>();
	TransForm()->SetPos({ 0.0f,0.0f,0.0f });
}
void CEditCameraMen::PreUpdate() 
{

}
void CEditCameraMen::Update() 
{
	m_PairMgr->Update();
}
void CEditCameraMen::PostUpdate() 
{

}
void CEditCameraMen::DebugRender()
{

}

void CEditCameraMen::ChangeState(int _Index)
{
#ifdef _DEBUG
	bool Change = m_PairMgr->ChangeState(_Index);
#endif
	m_PairMgr->ChangeState(_Index);
}
void CEditCameraMen::CamMove(const Vec3& _Dir)
{
	TransForm()->Move(_Dir*DELTATIME*m_fSpeed);

}