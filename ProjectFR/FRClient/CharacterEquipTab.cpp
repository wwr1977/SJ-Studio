#include "Precom.h"
#include "CharacterEquipTab.h"
#include <GameMultiSprite.h>

D3DXCOLOR		CCharacterEquipTab::FocusColor = D3DXCOLOR(1.f,0.2f,0.2f,1.f);
D3DXCOLOR		CCharacterEquipTab::NoFocusColor = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.f);

CCharacterEquipTab::CCharacterEquipTab()
	:m_TabBack(nullptr), m_CharacterRen(nullptr)
	, m_CharacterNo(MAXBATTLECHARACTER)
	, m_TabFrame(nullptr)
{
}


CCharacterEquipTab::~CCharacterEquipTab()
{
	m_vecItemIcon.clear();
}

void CCharacterEquipTab::Init()
{

	m_TabBack = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_UIBACK, 0.9f, true));
	m_TabBack->SetSprite(_T("ColorBase"));
	m_TabBack->SetCustomSize(Vec2{ 300.f,120.f });
	m_TabBack->SetMaterial(D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.f));
	m_TabBack->MaterialOn();

	m_TabFrame = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_UIBACK, 1.f, true));
	m_TabFrame->SetSprite(_T("SimpleEquipTabFrame"));
	m_TabFrame->SetCustomSize(Vec2{ 320.f,135.f });
	m_TabFrame->SetMaterial(NoFocusColor);
	m_TabFrame->MaterialOn();

	m_CharacterRen = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_UI, 0.95f, true));
	m_CharacterRen->SetRenPivot(Vec2{ -100.f,5.f });

	for (size_t i = 0; i < MAXITEMEQUIP; i++) 
	{
		m_vecItemIcon.push_back(ItemIcon(ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_UI, 1.f, true))));
		m_vecItemIcon[i].m_ItemIconRen->SetCustomSize(Vec2{ 28.f,28.f });
		m_vecItemIcon[i].m_ItemIconRen->SetRenPivot(Vec2{ -35.f,44.f - 30.f*i });
	}

	
}
void CCharacterEquipTab::InitData(const CharacterIndex& _Index)
{
	if (_Index >= MAXBATTLECHARACTER)
		return;

	m_CharacterNo = _Index;

	if (nullptr != m_CharacterRen)
		m_CharacterRen->SetSprite(Illust(CClientGlobal::CharacterName[m_CharacterNo].UnitName));

	//CHARACTERDATA CurData1  = CClientGlobal::GetCurCharData(_Index);
	
	CHARACTERDATA CurData ;
	CurData = CClientGlobal::GetCurCharData(_Index);

	if (-1 == CurData.CharacterNo)
		return;

	for (size_t i = 0; i < MAXITEMEQUIP; i++)
	{
		size_t ItemNo = CurData.EquipItem[i];
		
		if (-1 == ItemNo) 
		{
			m_vecItemIcon[i].m_ItemName = _T("¹ÌÂø¿ë");
			continue;
		}

		size_t IconIndex = CClientGlobal::vecItem[ItemNo].ItemIconIndex;
		m_vecItemIcon[i].m_ItemIconRen->SetSprite(CClientGlobal::ItemIconMultiSprite->GetSprite(IconIndex));
		m_vecItemIcon[i].m_ItemName = CClientGlobal::vecItem[ItemNo].ItemKName;
	}
}
void CCharacterEquipTab::UIFontRender()
{
	POINT CurPos = POINT{ (LONG)TRANS->GetPos().x,(LONG)TRANS->GetPos().y };
	
	for (size_t i = 0; i < MAXITEMEQUIP; i++)
		COMRESMGR->DrawFont(_T("ÀÛÀºº¼µå±¼¸²"), m_vecItemIcon[i].m_ItemName, CurPos + POINT{ -15,35 - 30*(LONG)i }, 2.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
	
}
void CCharacterEquipTab::SelectTab(const bool& _Select)
{
	if (nullptr == m_TabFrame)
		return;

	if (true == _Select)
		m_TabFrame->SetMaterial(FocusColor);
	else
		m_TabFrame->SetMaterial(NoFocusColor);

}
void CCharacterEquipTab::UpdateEquipTab()
{
	CHARACTERDATA CurData;
	CurData = CClientGlobal::GetCurCharData(m_CharacterNo);

	for (size_t i = 0; i < MAXITEMEQUIP; i++)
	{
		size_t ItemNo = CurData.EquipItem[i];

		if (-1 == ItemNo)
		{
			m_vecItemIcon[i].m_ItemName = _T("¹ÌÂø¿ë");
			m_vecItemIcon[i].m_ItemIconRen->Off();
			continue;
		}

		size_t IconIndex = CClientGlobal::vecItem[ItemNo].ItemIconIndex;
		m_vecItemIcon[i].m_ItemIconRen->SetSprite(CClientGlobal::ItemIconMultiSprite->GetSprite(IconIndex));
		m_vecItemIcon[i].m_ItemIconRen->On();
		m_vecItemIcon[i].m_ItemName = CClientGlobal::vecItem[ItemNo].ItemKName;
	}
}
void CCharacterEquipTab::TabOn(const Vec3& _Pos)
{
	ACTOR->On();
	TRANS->SetPos(_Pos);
}
const CharacterIndex CCharacterEquipTab::GetCharacterNo()
{
	return m_CharacterNo;
}