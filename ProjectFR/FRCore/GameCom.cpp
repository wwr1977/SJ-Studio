#include"stdafx.h"
#include "GameCom.h"



CGameCom::CGameCom()
	:m_bMulti(true),m_pParentObject(nullptr),m_pTrans(nullptr), m_SceneMainCamera(nullptr)
{

}
CGameCom::CGameCom(CActObject* _ActObject)
	:m_bMulti(true), m_pParentObject(_ActObject), m_pTrans(nullptr), m_SceneMainCamera(nullptr)
{
	
}

CGameCom::~CGameCom()
{

}
void CGameCom::Init()
{
	SetTypeInfo(this);
}
void CGameCom::InitData(void* pData)
{

}

void CGameCom::PreUpdate()
{

}
void CGameCom::Update()
{

}
void CGameCom::PostUpdate()
{

}
void CGameCom::Render()
{

}
void CGameCom::FontRender()
{

}
void CGameCom::UIFontRender()
{

}
void CGameCom::DebugRender()
{

}
void CGameCom::Collision()
{

}
void CGameCom::Release()
{
	
}
void CGameCom::LastUpdate()
{

}
CTransForm*	CGameCom::TransForm()
{
	return m_pTrans;
}
CActObject* CGameCom::ParentActObject()
{
	return m_pParentObject;
}
const tstring CGameCom::GetActorName()
{
	return m_pParentObject->Name();
}
bool CGameCom::IsDeath()
{
	if (nullptr == m_pParentObject) 
	{
		return true;
	}
	return CObjBase::IsDeath() || m_pParentObject->IsDeath();
}
bool CGameCom::IsUpdate()
{
	if (nullptr == m_pParentObject)
	{
		return false;
	}
	if ( false == m_pParentObject->IsActive() || false == IsActive() || true == IsDeath()) 
	{
		return false;
	}

	return true;
}
const bool CGameCom::IsActorOn()
{
	return ACTOR->IsActive();
}
void CGameCom::SetActObject(CActObject* _Object)
{
	m_pParentObject = _Object;
}
void CGameCom::SetTransForm(CTransForm* _pTrans)
{
	m_pTrans = _pTrans;
}
void CGameCom::BaseComInit()
{
	m_SceneMainCamera = ParentActObject()->GetScene()->GetMainCamCom();
	m_ComResourceMgr = ParentActObject()->GetScene()->GetWindow()->GetResourceMgr();
	m_pComDevice = ParentActObject()->GetScene()->GetWindow()->GetLPDevice();
	m_ComDebugMgr = ParentActObject()->GetScene()->GetWindow()->GetDebugMgr();
}
LPDIRECT3DDEVICE9 CGameCom::GetComDevice()
{
	return m_pComDevice;
}

SPTR<CResourceMgr> CGameCom::GetComResourceMgr()
{
	return m_ComResourceMgr;
}
SPTR<CDebugMgr> CGameCom::GetComDebugMgr()
{
	return m_ComDebugMgr;
}
void CGameCom::ActorOn() 
{
	ACTOR->On();
}
void CGameCom::ActorOff() 
{
	ACTOR->Off();
}
void CGameCom::ActorActive(const bool& _Enable)
{
	ACTOR->Active(_Enable);
}