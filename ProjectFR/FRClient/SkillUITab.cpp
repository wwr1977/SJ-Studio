#include "Precom.h"
#include "SkillUITab.h"
#include "SkillUIBoard.h"

D3DXCOLOR CSkillUITab::NoneFocusTabColor = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
D3DXCOLOR CSkillUITab::FocusTabColor = D3DXCOLOR(0.0f, 0.2f, 0.8f, 1.0f);

D3DXCOLOR CSkillUITab::NoneFocusFrameColor = D3DXCOLOR(0.75f, 0.75f, 0.75f, 1.0f);
D3DXCOLOR CSkillUITab::FocusFrameColor = D3DXCOLOR(1.0f, 0.75f, 0.0f, 1.0f);
DWORD CSkillUITab::NoneFocusFontColor = D3DCOLOR_ARGB(255,0,0,255);
DWORD CSkillUITab::FocusFontColor = D3DCOLOR_ARGB(255, 255, 0, 0);;
CTurnMgr* CSkillUITab::pSkillTabTurnMgr =nullptr ;

CSkillUIBoard* CSkillUITab::pSkillUIBoard = nullptr;


void CSkillUITab::SetTurnMgr(CTurnMgr* _Mgr)
{
	if (nullptr == _Mgr)
		return;

	pSkillTabTurnMgr = _Mgr;
}


CSkillUITab::CSkillUITab()
	:m_SkillType(-1), m_TabRen(nullptr), m_TabFrameRen(nullptr)
	, m_TabFontRen(nullptr), m_TabCol(nullptr), m_bSelect(false)
{
}


CSkillUITab::~CSkillUITab()
{
}
void CSkillUITab::SetSkillUIBoard(CSkillUIBoard* _pBoard)
{
	if (nullptr == _pBoard)
		return;

	pSkillUIBoard = _pBoard;
}
void CSkillUITab::SetSkillType(const size_t& _Type)
{
	if (SKILLDATA::MAXSKILLTYPE <= _Type) 
		return;
	
	m_SkillType = _Type;
	SetTabFontString();
}
void CSkillUITab::Init() 
{
	m_TabFrameRen = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_UIBACK, 0.9f, true));
	m_TabRen = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_UI, 1.0f, true));
	m_TabFontRen = ACTOR->CreateCom<CFontRenderer>(RENDATA(LAYER_UI, 1.0f, true));
	
	m_TabFontRen->SetFontStyle(_T("탭굴림"));
	m_TabFontRen->SetLeftStartPivot(POINT{ -25, -1 });
	m_TabFontRen->SetFontMag(1.0001f);
	
	m_TabFrameRen->SetSprite(_T("TabBase"));
	m_TabFrameRen->SetMaterial(NoneFocusFrameColor);
	m_TabFrameRen->MaterialOn();

	m_TabRen->SetSprite(_T("TabBase"));
	m_TabRen->SetMaterial(NoneFocusTabColor);
	m_TabRen->MaterialOn();

	m_TabFrameRen->SetCustomSize(Vec2{ 130.0f,38.0f });
	m_TabRen->SetCustomSize(Vec2{ 120.0f,30.0f });

	m_TabCol = ACTOR->CreateCom<CCollision>(COLDATA(_T("UI")));
	m_TabCol->SetColSize(Vec2{ 120.0f, 30.0f });
	m_TabCol->SetEnter(this, &CSkillUITab::Enter);
	m_TabCol->SetStay(this, &CSkillUITab::Stay);
	m_TabCol->SetExit(this, &CSkillUITab::Exit);
}
void CSkillUITab::Update() 
{

}
void CSkillUITab::DebugRender()
{
	//COMDEBUGMGR->RectRender(D3DCOLOR_ARGB(255, 255, 0, 0), TRANS->GetPos(), GetSize3());
}
void CSkillUITab::SetTabFontString()
{
	switch (m_SkillType)
	{
	case SKILLDATA::SKILLTYPE_ACTION:
		m_TabFontRen->SetString(_T("Action"));
	break;
	case SKILLDATA::SKILLTYPE_ABILITY:
		m_TabFontRen->SetString(_T("Ability"));
	break;
	case SKILLDATA::SKILLTYPE_BURSTSKILL:
		m_TabFontRen->SetString(_T("Burst"));
	break;
	case SKILLDATA::SKILLTYPE_ITEM:
		m_TabFontRen->SetString(_T("Item"));
	break;

	}
}
void CSkillUITab::SetPos(const Vec2& _Pos)
{
	Vec3 NewPos = Vec3(_Pos.x, _Pos.y, TRANS->GetPos().z);
	TRANS->SetPos(NewPos);
}
void CSkillUITab::SetPos(const Vec3& _Pos)
{
	TRANS->SetPos(_Pos);
}
void CSkillUITab::SetPivot(const Vec2& _Pivot)
{
	m_TabFrameRen->SetRenPivot(_Pivot);
	m_TabRen->SetRenPivot(_Pivot);
	m_TabFontRen->SetRenPivot(_Pivot);
}
const Vec2 CSkillUITab::GetSize()
{
	return m_TabFrameRen->GetCustomSize();
}
const Vec3 CSkillUITab::GetSize3()
{
	return Vec3(GetSize().x, GetSize().y, 1.0f);
}
void CSkillUITab::UIOff()
{
	ACTOR->Off();
}
void CSkillUITab::UIOn()
{
	ACTOR->On();
}
const bool CSkillUITab::IsSelect()
{
	return m_bSelect;
}
void CSkillUITab::Enter(SPTR<CCollision> _pThis, SPTR<CCollision> _pOtherCol)
{
	m_TabFrameRen->SetMaterial(FocusFrameColor);
}
void CSkillUITab::Stay(SPTR<CCollision> _pThis, SPTR<CCollision> _pOtherCol) 
{
	if (true == KEYDOWN("LMouse"))
		TabClickEvent();
		
}
void CSkillUITab::Exit(SPTR<CCollision> _pThis, SPTR<CCollision> _pOtherCol)
{
	m_TabFrameRen->SetMaterial(NoneFocusFrameColor);
}
void CSkillUITab::TabClickEvent()
{
	// 이전에 선택된 탭인경우(중복 처리 무시)
	if (true == m_bSelect)
		return;

	if (nullptr != pSkillUIBoard)
		pSkillUIBoard->TabClickEvent(m_SkillType);


	SelectTab();
}
void CSkillUITab::SelectTab() 
{
	m_bSelect = true;
   	m_TabRen->SetMaterial(FocusTabColor);
}
void CSkillUITab::UnSelectTab()
{
	m_bSelect = false;
	m_TabRen->SetMaterial(NoneFocusTabColor);
}
