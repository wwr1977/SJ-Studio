#include "Precom.h"
#include "CharStatusTab.h"


CCharStatusTab::CCharStatusTab()
	:m_bSelect(false)
	, m_CharacterIconRen(nullptr), m_SelectRen(nullptr)
{
}


CCharStatusTab::~CCharStatusTab()
{
}
void CCharStatusTab::Init()
{
	TabOff();

	m_CharacterIconRen = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_UI, 0.95f, true));
	
	m_SelectRen = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_UIBACK, 0.9f, true));
	m_SelectRen->SetSprite(_T("ColorBase"));
	//m_SelectRen->SetMaterial(D3DXCOLOR(0.97f, 1.0f, 0.1f, 1.0f));
	m_SelectRen->SetMaterial(D3DXCOLOR(0.65f, 0.65f, 0.65f, 1.0f));
	m_SelectRen->MaterialOn();
	m_SelectRen->Off();
	m_SelectRen->SetCustomSize(Vec2{ 90.f,97.f });
	m_SelectRen->SetRenPivot(Vec2{ 0.f,-2.f });
}
void CCharStatusTab::InitData(const CharacterIndex& _Index)
{
	if (MAXBATTLECHARACTER <= _Index)
		return;

	m_CharacterNo = _Index;
	m_CharacterIconRen->SetSprite(Icon(CClientGlobal::CharacterName[m_CharacterNo].UnitName));
	m_CharacterIconRen->SetCustomSize(Vec2{ 84.f,84.f });
}
void CCharStatusTab::SetPos(const Vec3& _Pos)
{
	TRANS->SetPos(_Pos);
}
void CCharStatusTab::SelectTab(const bool& _Select)
{
	m_bSelect = _Select;

	if (true == m_bSelect)
		m_SelectRen->On();
	else
		m_SelectRen->Off();
}
void CCharStatusTab::TabOn() 
{
	ACTOR->On();
}
void CCharStatusTab::TabOff() 
{
	ACTOR->Off();
}
const CharacterIndex CCharStatusTab::GetCharacterNo()
{
	return m_CharacterNo;
}