#include "Precom.h"
#include "MenuSkillUI.h"
#include <GameMultiSprite.h>


SPTR<CGameMultiSprite> CMenuSkillUI::MenuSkillMainIconSprite = nullptr;

D3DXCOLOR CMenuSkillUI::SelectColor = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);;
D3DXCOLOR CMenuSkillUI::NoneFocusColor = D3DXCOLOR(0.9f, 0.9f, 0.9f, 1.0f);
D3DXCOLOR CMenuSkillUI::UICoverColor = D3DXCOLOR(0.05f, 0.05f, 0.05f, 1.0f);

void CMenuSkillUI::InitMenuSkillUI(SPTR<CGameMultiSprite> _IconSprite)
{
	if (nullptr == _IconSprite)
		return;

	MenuSkillMainIconSprite = _IconSprite;
}
bool CMenuSkillUI::CompareMenuSkillUI(SPTR<class CMenuSkillUI> _Left, SPTR<CMenuSkillUI> _Right)
{
	if (nullptr == _Left || nullptr == _Right)
		return false;

	return _Left->GetSkillType() < _Right->GetSkillType();
}
CMenuSkillUI::CMenuSkillUI()
	:m_SkillUIData(SKILLUIDATA()), m_SkillUIFrame(nullptr)
	, m_SkillIcon(nullptr), m_SubSkillIcon(nullptr)
	, m_SkillNameRenderer(nullptr), m_SkillUIBack(nullptr)
	, m_bSelect(false)
	, m_SkillLevelRen(nullptr)
	, m_SkillUICover(nullptr)
	, m_SkillType(SKILLDATA::MAXSKILLTYPE)
{
}


CMenuSkillUI::~CMenuSkillUI()
{
}
void CMenuSkillUI::Init() 
{

}
void CMenuSkillUI::InitData(const SKILLUIDATA& _Data) 
{
	m_SkillUIData = _Data;


	m_SkillUIBack = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_UI, 0.9f, true));
	m_SkillUIBack->SetSprite(_T("SkillUIBack"));
	m_SkillUIBack->MaterialOn();
	m_SkillUIBack->SetMaterial(NoneFocusColor);
	//m_SkillUIBack->SetCustomSize(Vec2{ 202.0f,56.0f });
	m_SkillUIBack->SetCustomSize(Vec2{ 250.0f,70.0f });

	m_SkillUIFrame = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_UI, 1.0f, true));
	m_SkillUIFrame->SetSprite(_T("SkillUIFrame"));
	//m_SkillUIFrame->SetCustomSize(Vec2{ 195.0f,50.0f });
	m_SkillUIFrame->SetCustomSize(Vec2{ 240.0f,60.0f });

	m_SkillIcon = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_UI, 1.0f, true));


	SPTR<CGameSprite> IconSprite = MenuSkillMainIconSprite->GetSprite(m_SkillUIData.SkillIconIndex);

	if (nullptr != IconSprite)
		m_SkillIcon->SetSprite(IconSprite);

	//m_SkillIcon->SetCustomSize(Vec2{ 35.0f, 35.0f });
	m_SkillIcon->SetCustomSize(Vec2{ 46.0f, 44.0f });
	//m_IconPivot = Vec2{ -72.0f, 2.0f };
	m_IconPivot = Vec2{ -90.0f, 2.0f };
	m_SkillIcon->SetRenPivot(m_IconPivot);

	m_SkillNameRenderer = ACTOR->CreateCom<CFontRenderer>(RENDATA(LAYER_UI, 1.0f, true));
	m_SkillNameRenderer->SetFontStyle(_T("스킬타이틀"));
	m_SkillNameRenderer->SetFontMag(2.3f);
	m_SkillNameRenderer->SetString(m_SkillUIData.SkillKName);
	m_SkillNameRenderer->SetLeftStartPivot(POINT{ -60 , 5 });

	m_SkillLevelRen = ACTOR->CreateCom<CFontRenderer>(RENDATA(LAYER_UI, 1.0f, true));
	m_SkillLevelRen->SetFontStyle(_T("큰UI레벨폰트"));
	m_SkillLevelRen->SetFontMag(2.3f);
	m_SkillLevelRen->SetFontColor(D3DCOLOR_ARGB(255, 200, 200, 0));
	m_SkillLevelRen->SetLeftStartPivot(POINT{ 23 , -23 });


	m_SkillUICover = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_UI, 0.8f, true));
	m_SkillUICover->SetSprite(_T("SkillUIBack"));
	m_SkillUICover->MaterialOn();
	m_SkillUICover->SetMaterial(UICoverColor);
	m_SkillUICover->SetCustomSize(Vec2{ 250.0f,70.f });
	m_SkillUICover->Off();


	if (m_SkillUIData.SkillNo >= CClientGlobal::vecSkill.size())
		return;

	m_CasterNo = (CharacterIndex)CClientGlobal::vecSkill[m_SkillUIData.SkillNo].SkillActionScript.CharIndex;
	m_SkillType =(SKILLDATA::SKILLTYPE)CClientGlobal::vecSkill[m_SkillUIData.SkillNo].SkillData.SkillType;
}
void CMenuSkillUI::Update() 
{

}
void CMenuSkillUI::SetPos(const Vec3& _Pos)
{
	TRANS->SetPos(_Pos);
}
void CMenuSkillUI::SetLevel(const size_t& _Lv)
{
	if (_Lv >= LEVELDATA::MAXSKILLLEVEL)
		return;

	TCHAR Buf[MAXKEYLENGTH] = _T("");
	_stprintf_s(Buf, _T("Lv %2d"), (int)_Lv + 1);
	if (nullptr != m_SkillLevelRen)
		m_SkillLevelRen->SetString(Buf);

}
const CharacterIndex CMenuSkillUI::GetCasterNo()
{
	return m_CasterNo;
}
const SKILLDATA::SKILLTYPE CMenuSkillUI::GetSkillType()
{
	return m_SkillType;
}
const UINT CMenuSkillUI::GetSkillNo()
{
	return m_SkillUIData.SkillNo;
}
void CMenuSkillUI::SkillUIOn() 
{
	ACTOR->On();
}
void CMenuSkillUI::SkillUIOn(const Vec3& _Pos)
{
	TRANS->SetPos(_Pos);
	ACTOR->On();
}
void CMenuSkillUI::SkillUIOff()
{
	ACTOR->Off();
}
void CMenuSkillUI::SelectUI(const bool& _Select)
{
	m_bSelect = _Select;
	
	if (nullptr == m_SkillUIBack)
		return;

	if (true == m_bSelect)
		m_SkillUIBack->SetMaterial(SelectColor);
	else
		m_SkillUIBack->SetMaterial(NoneFocusColor);

}