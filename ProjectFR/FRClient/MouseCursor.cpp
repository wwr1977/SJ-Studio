#include "Precom.h"
#include "MouseCursor.h"


CMouseCursor::CMouseCursor()
	:m_AniMouse(nullptr), m_FixMouse(nullptr), m_CurMousePos({ 0.0f,0.0f })
	, m_MouseColCom(nullptr), m_PrevMousePos({ 0.f,0.f })
	, m_MoveDir(Vec2{ 0.f,0.f })
{
}


CMouseCursor::~CMouseCursor()
{
}
void CMouseCursor::Init() 
{
	m_CurMousePos = ACTOR->GetScene()->GetWindow()->GetMousePos();
	m_FixMouse = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_MOUSE, 1.0f,true));
	m_MouseColCom = ACTOR->CreateCom<CCollision>(COLDATA(_T("Mouse")));
	m_FixMouse->SetSprite(_T("BlueCursor"));
	TRANS->SetPos(m_CurMousePos);
	m_MouseColCom->SetPivotPos(Vec2{1.0f,-1.0f});
	m_MouseColCom->SetColSize(Vec2{1.0f,1.0f});
	m_MouseColCom->SetColType(COLTYPE::CT_POINT);
	m_MouseColCom->ColDebugRenActive(false);
	//m_MouseColCom->SetColSize(m_FixMouse->GetCurSpriteSize());
}
void CMouseCursor::PreUpdate()
{
	m_CurMousePos = ACTOR->GetScene()->GetWindow()->GetMousePos();
	
	if (m_CurMousePos == m_PrevMousePos) 
	{
		m_MoveDir = Vec2{ 0.f,0.f };
		m_bMove = false;
		return;
	}
	else 
	{
		m_bMove = true;
		m_MoveDir = m_CurMousePos - m_PrevMousePos;
		TRANS->SetPos(m_CurMousePos);
	}

	m_PrevMousePos = m_CurMousePos;
}
void CMouseCursor::Update() 
{

}
void CMouseCursor::DebugRender()
{
	COMDEBUGMGR->RectRender(D3DCOLOR_ARGB(255, 255, 0, 0), TRANS->GetPos(), Vec3{ 2.0f,2.0f,1.0f });
	
}
const bool CMouseCursor::IsMouseMove()
{
	return m_bMove;
}
const Vec2 CMouseCursor::GetMoveDir() 
{
	return m_MoveDir;
}