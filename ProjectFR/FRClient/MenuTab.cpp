#include "Precom.h"
#include "MenuTab.h"


CMenuTab::CMenuTab()
	:m_bSelect(false), m_TabFontRen(nullptr)
	, m_TabSelectRen(nullptr)
{
}


CMenuTab::~CMenuTab()
{
	m_SubTabFontRen.clear();
}
void CMenuTab::Init()
{
	m_TabSelectRen = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_UI, 0.7f, true));
	m_TabSelectRen->SetSprite(_T("ColorBase"));
	m_TabSelectRen->SetCustomSize(Vec2{ 314.f,50.f });
	
	m_TabFontRen = ACTOR->CreateCom<CFontRenderer>(RENDATA(LAYER_UI, 0.85f, true));
	m_TabFontRen->SetFontMag(2.0f);

	//m_TabSelectRen = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_UIBACK, 0.7f, true));
	//m_TabSelectRen->SetSprite(_T("ColorBase"));
	//m_TabSelectRen->SetCustomSize(Vec2{ 314.f,50.f });

	//m_TabSelectRen->SetMaterial(D3DXCOLOR(0.8f, 0.5f, 0.2f, 1.0f));
	m_TabSelectRen->SetMaterial(D3DXCOLOR(0.65f, 0.65f, 0.65f, 1.0f));
	m_TabSelectRen->MaterialOn();

	m_TabSelectRen->Off();
}
void CMenuTab::Update()
{

}
const size_t CMenuTab::CreateSubTabFont(const tstring& _Style
	, const Vec3& _MidPivot /*= Vec3{ 0.f,0.f,0.f }*/
	, const tstring _String /*= _T("")*/
	, const float _FontMag /*= 1.3f*/)
{
	size_t NewIndex = m_SubTabFontRen.size();

	m_SubTabFontRen.push_back(ACTOR->CreateCom<CFontRenderer>(RENDATA(LAYER_UI, 0.8f, true)));
	m_SubTabFontRen[NewIndex]->SetString(_String);
	m_SubTabFontRen[NewIndex]->SetFontStyle(_Style);
	m_SubTabFontRen[NewIndex]->SetFontMag(_FontMag);
	m_SubTabFontRen[NewIndex]->SetMiddleStartPivot(_MidPivot);
	return NewIndex;
}
const size_t CMenuTab::CreateSubTabFont(const tstring& _Style, const POINT& StartPivot
	, const FONTPIVOTOPTION& _StartOp /*= LEFTSTART*/
	, const tstring& _String /*= _T("")*/
	, const float& _FontMag/* = 1.3f*/) 
{
	size_t NewIndex = m_SubTabFontRen.size();

	m_SubTabFontRen.push_back(ACTOR->CreateCom<CFontRenderer>(RENDATA(LAYER_UI, 0.8f, true)));
	m_SubTabFontRen[NewIndex]->SetString(_String);
	m_SubTabFontRen[NewIndex]->SetFontStyle(_Style);
	m_SubTabFontRen[NewIndex]->SetFontMag(_FontMag);

	switch (_StartOp)
	{
	case LEFTSTART:
		m_SubTabFontRen[NewIndex]->SetLeftStartPivot(StartPivot);
		break;
	case RIGHTSTART:
		m_SubTabFontRen[NewIndex]->SetRightStartPivot(StartPivot);
		break;
	}

	return NewIndex;
}
void CMenuTab::SetPos(const Vec3& _Pos)
{
	TRANS->SetPos(_Pos);
}
void CMenuTab::SetPivot(const Vec3& _Pivot)
{
	m_TabFontRen->SetMiddleStartPivot(_Pivot);

	Vec3 SelectRenPivot = Vec3{ 0.f,m_TabSelectRen->GetCustomSize().y*0.5f - 3.f ,0.f };
	m_TabSelectRen->SetRenPivot(_Pivot + SelectRenPivot);
}
void CMenuTab::SetFontPivot(const Vec3& _Pivot)
{
	m_TabFontRen->SetMiddleStartPivot(_Pivot);
}

void CMenuTab::SetPivot(const POINT& _Pivot, const FONTPIVOTOPTION& _Op /*= LEFTSTART*/)
{
	switch (_Op)
	{
	case LEFTSTART:
		m_TabFontRen->SetLeftStartPivot(_Pivot);
		break;
	case RIGHTSTART:
		m_TabFontRen->SetRightStartPivot(_Pivot);
		break;
	default:
		return;
	}

	//Vec3 SelectRenPivot = Vec3{ 0.f,m_TabSelectRen->GetCustomSize().y*0.5f - 3.f ,0.f };
	//m_TabSelectRen->SetRenPivot(Vec3{ (float)_Pivot.x,(float)_Pivot.y ,0.f} +SelectRenPivot);
}
void CMenuTab::SetTabName(const tstring& _TabName)
{
	if(nullptr == m_TabFontRen)
		return;
	
	m_TabFontRen->SetString(_TabName);
}
void CMenuTab::SetTabFontColor(const DWORD& _Color)
{
	if (nullptr == m_TabFontRen)
		return;

	m_TabFontRen->SetFontColor(_Color);
}
void CMenuTab::SetTabStyle(const tstring& _FontStyle) 
{
	if (nullptr != m_TabFontRen)
		m_TabFontRen->SetFontStyle(_FontStyle);
	
}
void CMenuTab::SetSelectColor(const D3DXCOLOR& _Color)
{
	if (nullptr == m_TabSelectRen)
		return;

	m_TabSelectRen->SetMaterial(_Color);
}
void CMenuTab::SetTabSize(const Vec2& _TabSize)
{
	if (nullptr != m_TabSelectRen)
		m_TabSelectRen->SetCustomSize(_TabSize);
}
void CMenuTab::SelectTab(const bool& _Select)
{
	m_bSelect = _Select;
	
	if(true == m_bSelect)
		m_TabSelectRen->On();
	else 
		m_TabSelectRen->Off();
}
void CMenuTab::SetSubFontString(const size_t& _SubIndex, const tstring& _String)
{
	if (_SubIndex >= m_SubTabFontRen.size() || nullptr == m_SubTabFontRen[_SubIndex])
		return;

	m_SubTabFontRen[_SubIndex]->SetString(_String);
}
void CMenuTab::SetSubFontStyle(const size_t& _SubIndex, const tstring& _Style)
{
	if (_SubIndex >= m_SubTabFontRen.size() || nullptr == m_SubTabFontRen[_SubIndex])
		return;

	m_SubTabFontRen[_SubIndex]->SetFontStyle(_Style);
}
void CMenuTab::SetSubFontColor(const size_t& _SubIndex, const DWORD& _Color) 
{
	if (_SubIndex >= m_SubTabFontRen.size() || nullptr == m_SubTabFontRen[_SubIndex])
		return;

	m_SubTabFontRen[_SubIndex]->SetFontColor(_Color);
}
void CMenuTab::MenuTabOn() 
{
	m_TabFontRen->On();

	if(m_bSelect)
		m_TabSelectRen->On();
	else
		m_TabSelectRen->Off();

	for (size_t i = 0; i < m_SubTabFontRen.size(); i++)
		m_SubTabFontRen[i]->On();
}
void CMenuTab::MenuTabOff() 
{
	m_TabFontRen->Off();
	m_TabSelectRen->Off();
	for (size_t i = 0; i < m_SubTabFontRen.size(); i++)
		m_SubTabFontRen[i]->Off();
}
void CMenuTab::TabActorOn()
{
	ACTOR->On();
}
void CMenuTab::TabActorOff() 
{
	ACTOR->Off();
}