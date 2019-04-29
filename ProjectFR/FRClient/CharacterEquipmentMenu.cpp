#include "Precom.h"
#include "CharacterEquipmentMenu.h"
#include "MainMenuMgr.h"
#include "CharacterEquipTab.h"
#include "InventoryMgr.h"
#include "EquipItemTab.h"
#include "GameMaster.h"
#include "MountItemFailGuide.h"

CCharacterEquipmentMenu::CCharacterEquipmentMenu()
	:m_MenuMainBack(nullptr), m_EquipTab(nullptr)
	, m_CurCharacterPage(0), m_CurCharacterIndex(0)
	, m_CurCharacterNo(MAXBATTLECHARACTER)
	, m_CurItemStat(STAT()), m_CharacterIconRen(nullptr)
	, m_CurEquipTabIndex(0)
	, m_CurSelectEquipIndex(0), m_CurSelectEquipPage(0)
	, m_ItemDetailRen(nullptr), m_ItemDiffStat(STAT())
	, m_SelectItem(ITEM())
{
	m_vecSimpleEquipTab.clear();
	m_vecActiveEquipTab.clear();
	m_SelectEquipItemList.clear();

	size_t Right = 0;
	size_t Row = 0;

	for (size_t i = 0; i < MAXCHARCTERTAB; i++)
	{
		Right = i % 3;
		Row = i / 3;
		m_vecEquipTabPos.push_back(Vec3{ -332.f + Right * 333.f ,278.f - 140.f*Row,7.f });
	}

	m_vecCharDataString.assign(MAXCHARDATA, _T(""));
	m_ItemDataString.assign(MAXITEMDATA, _T(""));

	for (size_t i = 0; i < MAXCHARDATA; i++)
		m_vecCharDataString[i].reserve(128);

	for (size_t i = 0; i < MAXITEMDATA; i++)
		m_ItemDataString[i].reserve(128);

}


CCharacterEquipmentMenu::~CCharacterEquipmentMenu()
{
	m_CurEquipItemTab.clear();
	m_SelectEquipItemTab.clear();
	m_SelectEquipItemList.clear();
}

void CCharacterEquipmentMenu::Init() 
{
	MenuOff();

	m_MenuMainBack = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_UIBACK, 0.9f, true));
	m_MenuMainBack->SetSprite(_T("EquipMenuBack"));

	m_EquipTab = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_UIBACK, 0.9f, true));
	m_EquipTab->SetSprite(_T("EquipTab"));

	m_CharacterIconRen = CActObject::CreateActObject(_T("CharacterIllust"),ACTOR->GetScene())->CreateCom<CFixRenderer>(RENDATA(LAYER_UI, 1.f, true));
	m_CharacterIconRen->SetImageMag(1.3f);
	m_CharacterIconRen->SetRenPivot(Vec2{ -430.f,310.f });
	m_CharacterIconRen->ActorOff();

	Vec2 Pivot = m_MenuMainBack->GetCurSpriteSize() * 0.5f;
	m_EquipTab->SetRenPivot(Pivot + Vec2{ m_EquipTab->GetCurSpriteSize().x*0.5f,-133.f });


	size_t Index = 0;

	for (size_t j = 0; j < CClientGlobal::CharacterLine.size(); j++)
	{
		for (size_t i = 0; i < CClientGlobal::CharacterLine[j].size(); i++)
		{
			m_vecSimpleEquipTab.push_back(CActObject::CreateActObject(_T("SimpleEquipTab"), ACTOR->GetScene())->CreateCom<CCharacterEquipTab>((CharacterIndex)CClientGlobal::CharacterLine[j][i].CharacterNo));
			m_vecSimpleEquipTab[Index++]->ActorOff();
		}
	}

	int Right,Row;
	for (int i = 0; i < MAXITEMEQUIP; i++)
	{
		Row = i % 2;
		Right = i / 2;

		m_CurEquipItemTab.push_back(CActObject::CreateActObject(_T("EquipItemTab"), ACTOR->GetScene())->CreateCom<CEquipItemTab>());
		m_CurEquipItemTab[i]->SetActorPos(Vec3{ -420.f + Right*280.f,30.f - Row*100.f ,7.f });
		m_CurEquipItemTab[i]->ItemCountRender(false);
		m_CurEquipItemTab[i]->SelectTab(false);
		m_CurEquipItemTab[i]->ActorOff();
	}

	for (size_t i = 0; i < MAXSELECTITEMTAB; i++)
	{
		m_SelectEquipItemTab.push_back(CActObject::CreateActObject(_T("EquipItemTab"), ACTOR->GetScene())->CreateCom<CEquipItemTab>());
		m_SelectEquipItemTab[i]->SetActorPos(Vec3{ 180.f ,70.f - 52.f*i ,7.f });
		m_SelectEquipItemTab[i]->ItemCountRender(true);
		m_SelectEquipItemTab[i]->SelectTab(false);
		m_SelectEquipItemTab[i]->ActorOff();
	}
	
	m_ItemDetailRen = ACTOR->CreateCom<CScriptRenderer>(RENDATA(LAYER_UI, 1.f, true));
	m_ItemDetailRen->SetFontStyle(_T("Å«UI·¹º§ÆùÆ®"));
	m_ItemDetailRen->SetLineInterval(30);
	m_ItemDetailRen->SetLeftStartPivot(POINT{ 170,-180 });

	m_MountItemFailGuide = CActObject::CreateActObject(_T("MountItemFailGuide"), ACTOR->GetScene())->CreateCom<CMountItemFailGuide>();
	
}
void CCharacterEquipmentMenu::Update() 
{
	switch (m_CurTab)
	{
	case CCharacterEquipmentMenu::SELECT_CHARACTER:
		SelectCharacter();
		return;
	case CCharacterEquipmentMenu::SELECT_CURRENT_EQUIPITEM:
		SelectCurEquipItem();
		return;
	case CCharacterEquipmentMenu::SELECT_CHANGE_EQUIPITEM:
		SelectChangeEquipItem();
		return;
	}
}
void CCharacterEquipmentMenu::MenuOnEvent() 
{
	m_pMenuMgr->BattlePositionUIAllOff();

	m_CurCharacterPage = 0;
	m_CurCharacterIndex = 0;
	m_CurTab = SELECT_CHARACTER;

	m_vecActiveEquipTab.clear();

	for (size_t i = 0,Index = -1; i < min(MAXCHARCTERTAB, m_vecSimpleEquipTab.size()); i++)
	{
		if (!CClientGlobal::MasterMode &&  !CClientGlobal::GameMaster->IsOpenCharacterLine(m_vecSimpleEquipTab[i]->GetCharacterNo()))
			continue;

		m_vecActiveEquipTab.push_back(m_vecSimpleEquipTab[i]);
		++Index;
		m_vecActiveEquipTab[Index]->TabOn(m_vecEquipTabPos[Index]);
		m_vecActiveEquipTab[Index]->SelectTab(false);
	}

	m_vecActiveEquipTab[0]->SelectTab(true);

}
void CCharacterEquipmentMenu::UIFontRender()
{
	switch (m_CurTab)
	{
	case CCharacterEquipmentMenu::SELECT_CURRENT_EQUIPITEM:
		ChracterDataRender();
		return;
	case CCharacterEquipmentMenu::SELECT_CHANGE_EQUIPITEM: 
	{
		ChracterDataRender();
		ItemDataRender();
	}
		return;
	}
}
void CCharacterEquipmentMenu::ChracterDataRender()
{
	COMRESMGR->DrawFont(_T("Ä³¸¯¶óÀÎÆùÆ®"), m_vecCharDataString[CHARDATA_NAME], POINT{ -350 ,310 }, 2.3f, D3DCOLOR_ARGB(255, 255, 220, 0));
	COMRESMGR->DrawFont(_T("Ä³¸¯¶óÀÎÆùÆ®"), m_vecCharDataString[CHARDATA_LV], POINT{ -350 ,260 }, 2.3f, D3DCOLOR_ARGB(255, 255, 255, 255));

	COMRESMGR->DrawFont(_T("º¼µå±¼¸²"), m_vecCharDataString[CHARDATA_STR], POINT{ -470 , 225 }, 2.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
	COMRESMGR->DrawFont(_T("º¼µå±¼¸²"), m_vecCharDataString[CHARDATA_DEX], POINT{ -470 , 195 }, 2.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
	COMRESMGR->DrawFont(_T("º¼µå±¼¸²"), m_vecCharDataString[CHARDATA_INT], POINT{ -470 , 165 }, 2.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
	COMRESMGR->DrawFont(_T("º¼µå±¼¸²"), m_vecCharDataString[CHARDATA_LUK], POINT{ -470 , 135 }, 2.3f, D3DCOLOR_ARGB(255, 255, 255, 255));

	if(m_CurItemStat.Str > 0)
		COMRESMGR->DrawFont(_T("º¼µå±¼¸²"), m_vecCharDataString[CHARDATA_ITEMSTR], POINT{ -370 , 225 }, 2.3f, D3DCOLOR_ARGB(255, 255, 25, 25));
	else if (m_CurItemStat.Str < 0)
		COMRESMGR->DrawFont(_T("º¼µå±¼¸²"), m_vecCharDataString[CHARDATA_ITEMSTR], POINT{ -370 , 225 }, 2.3f, D3DCOLOR_ARGB(255, 25, 25, 255));
	else 
		COMRESMGR->DrawFont(_T("º¼µå±¼¸²"), m_vecCharDataString[CHARDATA_ITEMSTR], POINT{ -370 , 225 }, 2.3f, D3DCOLOR_ARGB(255, 255, 255, 255));

	if (m_CurItemStat.Dex > 0)
		COMRESMGR->DrawFont(_T("º¼µå±¼¸²"), m_vecCharDataString[CHARDATA_ITEMDEX], POINT{ -370 , 195 }, 2.3f, D3DCOLOR_ARGB(255, 255, 25, 25));
	else if (m_CurItemStat.Dex < 0)
		COMRESMGR->DrawFont(_T("º¼µå±¼¸²"), m_vecCharDataString[CHARDATA_ITEMDEX], POINT{ -370 , 195 }, 2.3f, D3DCOLOR_ARGB(255, 25, 25, 255));
	else
		COMRESMGR->DrawFont(_T("º¼µå±¼¸²"), m_vecCharDataString[CHARDATA_ITEMDEX], POINT{ -370 , 195 }, 2.3f, D3DCOLOR_ARGB(255, 255, 255, 255));


	if (m_CurItemStat.Int > 0)
		COMRESMGR->DrawFont(_T("º¼µå±¼¸²"), m_vecCharDataString[CHARDATA_ITEMINT], POINT{ -370 , 165 }, 2.3f, D3DCOLOR_ARGB(255, 255, 25, 25));
	else if (m_CurItemStat.Int < 0)
		COMRESMGR->DrawFont(_T("º¼µå±¼¸²"), m_vecCharDataString[CHARDATA_ITEMINT], POINT{ -370 , 165 }, 2.3f, D3DCOLOR_ARGB(255, 25, 25, 255));
	else
		COMRESMGR->DrawFont(_T("º¼µå±¼¸²"), m_vecCharDataString[CHARDATA_ITEMINT], POINT{ -370 , 165 }, 2.3f, D3DCOLOR_ARGB(255, 255, 255, 255));


	if (m_CurItemStat.Luck > 0)
		COMRESMGR->DrawFont(_T("º¼µå±¼¸²"), m_vecCharDataString[CHARDATA_ITEMLUK], POINT{ -370 , 135 }, 2.3f, D3DCOLOR_ARGB(255, 255, 25, 25));
	else if (m_CurItemStat.Luck < 0)
		COMRESMGR->DrawFont(_T("º¼µå±¼¸²"), m_vecCharDataString[CHARDATA_ITEMLUK], POINT{ -370 , 135 }, 2.3f, D3DCOLOR_ARGB(255, 25, 25, 255));
	else
		COMRESMGR->DrawFont(_T("º¼µå±¼¸²"), m_vecCharDataString[CHARDATA_ITEMLUK], POINT{ -370 , 135 }, 2.3f, D3DCOLOR_ARGB(255, 255, 255, 255));

	COMRESMGR->DrawFont(_T("º¼µå±¼¸²"), m_vecCharDataString[CHARDATA_HP], POINT{ -190,225 }, 2.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
	COMRESMGR->DrawFont(_T("º¼µå±¼¸²"), m_vecCharDataString[CHARDATA_MP], POINT{ 30 ,225 }, 2.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
	COMRESMGR->DrawFont(_T("º¼µå±¼¸²"), m_vecCharDataString[CHARDATA_DEM], POINT{ -190 ,195 }, 2.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
	COMRESMGR->DrawFont(_T("º¼µå±¼¸²"), m_vecCharDataString[CHARDATA_DEF], POINT{ -190 ,165 }, 2.3f, D3DCOLOR_ARGB(255, 255, 255, 255));

	if (m_CurItemStat.Heath > 0)
		COMRESMGR->DrawFont(_T("º¼µå±¼¸²"), m_vecCharDataString[CHARDATA_ITEMHP], POINT{ -35 , 225 }, 2.3f, D3DCOLOR_ARGB(255, 255, 25, 25));
	else if (m_CurItemStat.Heath < 0)
		COMRESMGR->DrawFont(_T("º¼µå±¼¸²"), m_vecCharDataString[CHARDATA_ITEMHP], POINT{ -35 , 225 }, 2.3f, D3DCOLOR_ARGB(255, 25, 25, 255));
	else
		COMRESMGR->DrawFont(_T("º¼µå±¼¸²"), m_vecCharDataString[CHARDATA_ITEMHP], POINT{ -35 , 225 }, 2.3f, D3DCOLOR_ARGB(255, 255, 255, 255));

	if (m_CurItemStat.Mana > 0)
		COMRESMGR->DrawFont(_T("º¼µå±¼¸²"), m_vecCharDataString[CHARDATA_ITEMMP], POINT{ 160 , 225 }, 2.3f, D3DCOLOR_ARGB(255, 255, 25, 25));
	else if (m_CurItemStat.Mana < 0)
		COMRESMGR->DrawFont(_T("º¼µå±¼¸²"), m_vecCharDataString[CHARDATA_ITEMMP], POINT{ 160 , 225 }, 2.3f, D3DCOLOR_ARGB(255, 25, 25, 255));
	else
		COMRESMGR->DrawFont(_T("º¼µå±¼¸²"), m_vecCharDataString[CHARDATA_ITEMMP], POINT{ 160 , 225 }, 2.3f, D3DCOLOR_ARGB(255, 255, 255, 255));

	if (m_CurItemStat.Damage > 0)
		COMRESMGR->DrawFont(_T("º¼µå±¼¸²"), m_vecCharDataString[CHARDATA_ITEMDEM], POINT{ -70 , 195 }, 2.3f, D3DCOLOR_ARGB(255, 255, 25, 25));
	else if (m_CurItemStat.Damage < 0)
		COMRESMGR->DrawFont(_T("º¼µå±¼¸²"), m_vecCharDataString[CHARDATA_ITEMDEM], POINT{ -70 , 195 }, 2.3f, D3DCOLOR_ARGB(255, 25, 25, 255));
	else
		COMRESMGR->DrawFont(_T("º¼µå±¼¸²"), m_vecCharDataString[CHARDATA_ITEMDEM], POINT{ -70 , 195 }, 2.3f, D3DCOLOR_ARGB(255, 255, 255, 255));

	if (m_CurItemStat.Defense > 0)
		COMRESMGR->DrawFont(_T("º¼µå±¼¸²"), m_vecCharDataString[CHARDATA_ITEMDEF], POINT{ -70 , 165 }, 2.3f, D3DCOLOR_ARGB(255, 255, 25, 25));
	else if (m_CurItemStat.Damage < 0)
		COMRESMGR->DrawFont(_T("º¼µå±¼¸²"), m_vecCharDataString[CHARDATA_ITEMDEF], POINT{ -70 , 165 }, 2.3f, D3DCOLOR_ARGB(255, 25, 25, 255));
	else
		COMRESMGR->DrawFont(_T("º¼µå±¼¸²"), m_vecCharDataString[CHARDATA_ITEMDEF], POINT{ -70 , 165 }, 2.3f, D3DCOLOR_ARGB(255, 255, 255, 255));

	COMRESMGR->DrawFont(_T("º¼µå±¼¸²"), m_vecCharDataString[CHARDATA_CRI], POINT{ 220 ,225 }, 2.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
	COMRESMGR->DrawFont(_T("º¼µå±¼¸²"), m_vecCharDataString[CHARDATA_CRIDEM], POINT{ 220 ,195 }, 2.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
	COMRESMGR->DrawFont(_T("º¼µå±¼¸²"), m_vecCharDataString[CHARDATA_AVOID], POINT{ 220 ,165 }, 2.3f, D3DCOLOR_ARGB(255, 255, 255, 255));

	if (m_CurItemStat.Critical > 0)
		COMRESMGR->DrawFont(_T("º¼µå±¼¸²"), m_vecCharDataString[CHARDATA_ITEMCRI], POINT{ 375 , 225 }, 2.3f, D3DCOLOR_ARGB(255, 255, 25, 25));
	else if (m_CurItemStat.Critical < 0)
		COMRESMGR->DrawFont(_T("º¼µå±¼¸²"), m_vecCharDataString[CHARDATA_ITEMCRI], POINT{ 375 , 225 }, 2.3f, D3DCOLOR_ARGB(255, 25, 25, 255));
	else
		COMRESMGR->DrawFont(_T("º¼µå±¼¸²"), m_vecCharDataString[CHARDATA_ITEMCRI], POINT{ 375 , 225 }, 2.3f, D3DCOLOR_ARGB(255, 255, 255, 255));

	if (m_CurItemStat.CriticalDamage > 0)
		COMRESMGR->DrawFont(_T("º¼µå±¼¸²"), m_vecCharDataString[CHARDATA_ITEMCRIDEM], POINT{ 435, 195 }, 2.3f, D3DCOLOR_ARGB(255, 255, 25, 25));
	else if (m_CurItemStat.CriticalDamage < 0)
		COMRESMGR->DrawFont(_T("º¼µå±¼¸²"), m_vecCharDataString[CHARDATA_ITEMCRIDEM], POINT{ 435 , 195 }, 2.3f, D3DCOLOR_ARGB(255, 25, 25, 255));
	else
		COMRESMGR->DrawFont(_T("º¼µå±¼¸²"), m_vecCharDataString[CHARDATA_ITEMCRIDEM], POINT{ 435 , 195 }, 2.3f, D3DCOLOR_ARGB(255, 255, 255, 255));

	if (m_CurItemStat.Avoidability > 0)
		COMRESMGR->DrawFont(_T("º¼µå±¼¸²"), m_vecCharDataString[CHARDATA_ITEMAVOID], POINT{ 350 , 165 }, 2.3f, D3DCOLOR_ARGB(255, 255, 25, 25));
	else if (m_CurItemStat.Avoidability < 0)
		COMRESMGR->DrawFont(_T("º¼µå±¼¸²"), m_vecCharDataString[CHARDATA_ITEMAVOID], POINT{ 350 , 165 }, 2.3f, D3DCOLOR_ARGB(255, 25, 25, 255));
	else
		COMRESMGR->DrawFont(_T("º¼µå±¼¸²"), m_vecCharDataString[CHARDATA_ITEMAVOID], POINT{ 350 , 165 }, 2.3f, D3DCOLOR_ARGB(255, 255, 255, 255));


	COMRESMGR->DrawFont(_T("Å«º¼µå±¼¸²"),_T("¹«±â"), POINT{ -470 , 60 }, 2.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
	COMRESMGR->DrawFont(_T("Å«º¼µå±¼¸²"), _T("¹æ¾î±¸"), POINT{ -470 ,  -40}, 2.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
	COMRESMGR->DrawFont(_T("Å«º¼µå±¼¸²"), _T("¾Ç¼¼»ç¸®"), POINT{ -190 , 60 }, 2.3f, D3DCOLOR_ARGB(255, 255, 255, 255));

}
void CCharacterEquipmentMenu::ItemDataRender() 
{

	COMRESMGR->DrawFont(_T("Ä³¸¯¶óÀÎÆùÆ®"), m_ItemDataString[ITEMDATA_NAME], POINT{ -470 ,-190 }, 2.3f, D3DCOLOR_ARGB(255, 255, 135, 0));
	COMRESMGR->DrawFont(_T("Å«º¼µå±¼¸²"), m_ItemDataString[ITEMDATA_LV], POINT{ -470 ,-230 }, 2.3f, D3DCOLOR_ARGB(255, 255, 255, 255));

	if (-1 == m_SelectItem.ItemNo || CATEGORY_ACCESSORIES == m_SelectItem.ItemCategory) 
	{
		COMRESMGR->DrawFont(_T("º¼µå±¼¸²"), m_ItemDataString[ITEMDATA_DEM], POINT{ -310 ,-320 }, 2.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
		
		if (m_ItemDiffStat.Damage > 0)
			COMRESMGR->DrawFont(_T("º¼µå±¼¸²"), m_ItemDataString[ITEMDATA_DIFFDEM], POINT{ -190 , -320 }, 2.3f, D3DCOLOR_ARGB(255, 255, 25, 25));
		else if (m_ItemDiffStat.Damage < 0)
			COMRESMGR->DrawFont(_T("º¼µå±¼¸²"), m_ItemDataString[ITEMDATA_DIFFDEM], POINT{ -190 , -320 }, 2.3f, D3DCOLOR_ARGB(255, 25, 25, 255));
		else
			COMRESMGR->DrawFont(_T("º¼µå±¼¸²"), m_ItemDataString[ITEMDATA_DIFFDEM], POINT{ -190 , -320 }, 2.3f, D3DCOLOR_ARGB(255, 255, 255, 255));

		COMRESMGR->DrawFont(_T("º¼µå±¼¸²"), m_ItemDataString[ITEMDATA_DEF], POINT{ -310 ,-345 }, 2.3f, D3DCOLOR_ARGB(255, 255, 255, 255));

		if (m_ItemDiffStat.Defense > 0)
			COMRESMGR->DrawFont(_T("º¼µå±¼¸²"), m_ItemDataString[ITEMDATA_DIFFDEF], POINT{ -190 , -345 }, 2.3f, D3DCOLOR_ARGB(255, 255, 25, 25));
		else if (m_ItemDiffStat.Defense < 0)
			COMRESMGR->DrawFont(_T("º¼µå±¼¸²"), m_ItemDataString[ITEMDATA_DIFFDEF], POINT{ -190 , -345 }, 2.3f, D3DCOLOR_ARGB(255, 25, 25, 255));
		else
			COMRESMGR->DrawFont(_T("º¼µå±¼¸²"), m_ItemDataString[ITEMDATA_DIFFDEF], POINT{ -190 , -345 }, 2.3f, D3DCOLOR_ARGB(255, 255, 255, 255));

	}
	else if (CATEGORY_ARMOR == m_SelectItem.ItemCategory) 
	{
		COMRESMGR->DrawFont(_T("Å«º¼µå±¼¸²"), m_ItemDataString[ITEMDATA_DEF], POINT{ -140 ,-230 }, 2.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
		
		if (m_ItemDiffStat.Defense > 0)
			COMRESMGR->DrawFont(_T("Å«º¼µå±¼¸²"), m_ItemDataString[ITEMDATA_DIFFDEF], POINT{ 40 , -230 }, 2.3f, D3DCOLOR_ARGB(255, 255, 25, 25));
		else if (m_ItemDiffStat.Defense < 0)
			COMRESMGR->DrawFont(_T("Å«º¼µå±¼¸²"), m_ItemDataString[ITEMDATA_DIFFDEF], POINT{ 40 , -230 }, 2.3f, D3DCOLOR_ARGB(255, 25, 25, 255));
		else
			COMRESMGR->DrawFont(_T("Å«º¼µå±¼¸²"), m_ItemDataString[ITEMDATA_DIFFDEF], POINT{ 40 , -230 }, 2.3f, D3DCOLOR_ARGB(255, 255, 255, 255));


		COMRESMGR->DrawFont(_T("º¼µå±¼¸²"), m_ItemDataString[ITEMDATA_DEM], POINT{ -310 ,-320 }, 2.3f, D3DCOLOR_ARGB(255, 255, 255, 255));

		if (m_ItemDiffStat.Damage > 0)
			COMRESMGR->DrawFont(_T("º¼µå±¼¸²"), m_ItemDataString[ITEMDATA_DIFFDEM], POINT{ -190 , -320 }, 2.3f, D3DCOLOR_ARGB(255, 255, 25, 25));
		else if (m_ItemDiffStat.Damage < 0)
			COMRESMGR->DrawFont(_T("º¼µå±¼¸²"), m_ItemDataString[ITEMDATA_DIFFDEM], POINT{ -190 , -320 }, 2.3f, D3DCOLOR_ARGB(255, 25, 25, 255));
		else
			COMRESMGR->DrawFont(_T("º¼µå±¼¸²"), m_ItemDataString[ITEMDATA_DIFFDEM], POINT{ -190 , -320 }, 2.3f, D3DCOLOR_ARGB(255, 255, 255, 255));

	}
	else if (CATEGORY_WEAPON == m_SelectItem.ItemCategory) 
	{
		COMRESMGR->DrawFont(_T("Å«º¼µå±¼¸²"), m_ItemDataString[ITEMDATA_DEM], POINT{ -140 ,-230 }, 2.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
		
		if (m_ItemDiffStat.Damage > 0)
			COMRESMGR->DrawFont(_T("Å«º¼µå±¼¸²"), m_ItemDataString[ITEMDATA_DIFFDEM], POINT{ 40 , -230 }, 2.3f, D3DCOLOR_ARGB(255, 255, 25, 25));
		else if (m_ItemDiffStat.Damage < 0)
			COMRESMGR->DrawFont(_T("Å«º¼µå±¼¸²"), m_ItemDataString[ITEMDATA_DIFFDEM], POINT{ 40 , -230 }, 2.3f, D3DCOLOR_ARGB(255, 25, 25, 255));
		else
			COMRESMGR->DrawFont(_T("Å«º¼µå±¼¸²"), m_ItemDataString[ITEMDATA_DIFFDEM], POINT{ 40 , -230 }, 2.3f, D3DCOLOR_ARGB(255, 255, 255, 255));


		COMRESMGR->DrawFont(_T("º¼µå±¼¸²"), m_ItemDataString[ITEMDATA_DEF], POINT{ -310 ,-320 }, 2.3f, D3DCOLOR_ARGB(255, 255, 255, 255));

		if (m_ItemDiffStat.Defense > 0)
			COMRESMGR->DrawFont(_T("º¼µå±¼¸²"), m_ItemDataString[ITEMDATA_DIFFDEF], POINT{ -190 , -320 }, 2.3f, D3DCOLOR_ARGB(255, 255, 25, 25));
		else if (m_ItemDiffStat.Defense < 0)
			COMRESMGR->DrawFont(_T("º¼µå±¼¸²"), m_ItemDataString[ITEMDATA_DIFFDEF], POINT{ -190 , -320 }, 2.3f, D3DCOLOR_ARGB(255, 25, 25, 255));
		else
			COMRESMGR->DrawFont(_T("º¼µå±¼¸²"), m_ItemDataString[ITEMDATA_DIFFDEF], POINT{ -190 , -320 }, 2.3f, D3DCOLOR_ARGB(255, 255, 255, 255));

	}

	COMRESMGR->DrawFont(_T("º¼µå±¼¸²"), m_ItemDataString[ITEMDATA_STR], POINT{ -480 , -270 }, 2.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
	COMRESMGR->DrawFont(_T("º¼µå±¼¸²"), m_ItemDataString[ITEMDATA_DEX], POINT{ -480 , -295 }, 2.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
	COMRESMGR->DrawFont(_T("º¼µå±¼¸²"), m_ItemDataString[ITEMDATA_INT], POINT{ -480 , -320 }, 2.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
	COMRESMGR->DrawFont(_T("º¼µå±¼¸²"), m_ItemDataString[ITEMDATA_LUK], POINT{ -480 , -345 }, 2.3f, D3DCOLOR_ARGB(255, 255, 255, 255));

	if (m_ItemDiffStat.Str > 0)
		COMRESMGR->DrawFont(_T("º¼µå±¼¸²"), m_ItemDataString[ITEMDATA_DIFFSTR], POINT{ -380 , -270 }, 2.3f, D3DCOLOR_ARGB(255, 255, 25, 25));
	else if (m_ItemDiffStat.Str < 0)
		COMRESMGR->DrawFont(_T("º¼µå±¼¸²"), m_ItemDataString[ITEMDATA_DIFFSTR], POINT{ -380 , -270 }, 2.3f, D3DCOLOR_ARGB(255, 25, 25, 255));
	else
		COMRESMGR->DrawFont(_T("º¼µå±¼¸²"), m_ItemDataString[ITEMDATA_DIFFSTR], POINT{ -380 , -270 }, 2.3f, D3DCOLOR_ARGB(255, 255, 255, 255));

	if (m_ItemDiffStat.Dex > 0)
		COMRESMGR->DrawFont(_T("º¼µå±¼¸²"), m_ItemDataString[ITEMDATA_DIFFDEX], POINT{ -380 , -295 }, 2.3f, D3DCOLOR_ARGB(255, 255, 25, 25));
	else if (m_ItemDiffStat.Dex < 0)
		COMRESMGR->DrawFont(_T("º¼µå±¼¸²"), m_ItemDataString[ITEMDATA_DIFFDEX], POINT{ -380 , -295 }, 2.3f, D3DCOLOR_ARGB(255, 25, 25, 255));
	else
		COMRESMGR->DrawFont(_T("º¼µå±¼¸²"), m_ItemDataString[ITEMDATA_DIFFDEX], POINT{ -380 , -295 }, 2.3f, D3DCOLOR_ARGB(255, 255, 255, 255));


	if (m_ItemDiffStat.Int > 0)
		COMRESMGR->DrawFont(_T("º¼µå±¼¸²"), m_ItemDataString[ITEMDATA_DIFFINT], POINT{ -380 , -320 }, 2.3f, D3DCOLOR_ARGB(255, 255, 25, 25));
	else if (m_ItemDiffStat.Int < 0)
		COMRESMGR->DrawFont(_T("º¼µå±¼¸²"), m_ItemDataString[ITEMDATA_DIFFINT], POINT{ -380 , -320 }, 2.3f, D3DCOLOR_ARGB(255, 25, 25, 255));
	else
		COMRESMGR->DrawFont(_T("º¼µå±¼¸²"), m_ItemDataString[ITEMDATA_DIFFINT], POINT{ -380 , -320 }, 2.3f, D3DCOLOR_ARGB(255, 255, 255, 255));


	if (m_ItemDiffStat.Luck > 0)
		COMRESMGR->DrawFont(_T("º¼µå±¼¸²"), m_ItemDataString[ITEMDATA_DIFFLUK], POINT{ -380 , -345 }, 2.3f, D3DCOLOR_ARGB(255, 255, 25, 25));
	else if (m_ItemDiffStat.Luck < 0)
		COMRESMGR->DrawFont(_T("º¼µå±¼¸²"), m_ItemDataString[ITEMDATA_DIFFLUK], POINT{ -380 , -345 }, 2.3f, D3DCOLOR_ARGB(255, 25, 25, 255));
	else
		COMRESMGR->DrawFont(_T("º¼µå±¼¸²"), m_ItemDataString[ITEMDATA_DIFFLUK], POINT{ -380 , -345 }, 2.3f, D3DCOLOR_ARGB(255, 255, 255, 255));

	

	COMRESMGR->DrawFont(_T("º¼µå±¼¸²"), m_ItemDataString[ITEMDATA_HP], POINT{ -310,-270 }, 2.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
	COMRESMGR->DrawFont(_T("º¼µå±¼¸²"), m_ItemDataString[ITEMDATA_MP], POINT{ -310 ,-295 }, 2.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
	
	if (m_ItemDiffStat.Heath > 0)
		COMRESMGR->DrawFont(_T("º¼µå±¼¸²"), m_ItemDataString[ITEMDATA_DIFFHP], POINT{ -220 , -270 }, 2.3f, D3DCOLOR_ARGB(255, 255, 25, 25));
	else if (m_ItemDiffStat.Heath < 0)
		COMRESMGR->DrawFont(_T("º¼µå±¼¸²"), m_ItemDataString[ITEMDATA_DIFFHP], POINT{ -220 , -270 }, 2.3f, D3DCOLOR_ARGB(255, 25, 25, 255));
	else
		COMRESMGR->DrawFont(_T("º¼µå±¼¸²"), m_ItemDataString[ITEMDATA_DIFFHP], POINT{ -220 , -270 }, 2.3f, D3DCOLOR_ARGB(255, 255, 255, 255));

	if (m_ItemDiffStat.Mana > 0)
		COMRESMGR->DrawFont(_T("º¼µå±¼¸²"), m_ItemDataString[ITEMDATA_DIFFMP], POINT{ -220 , -295 }, 2.3f, D3DCOLOR_ARGB(255, 255, 25, 25));
	else if (m_ItemDiffStat.Mana < 0)
		COMRESMGR->DrawFont(_T("º¼µå±¼¸²"), m_ItemDataString[ITEMDATA_DIFFMP], POINT{ -220 , -295 }, 2.3f, D3DCOLOR_ARGB(255, 25, 25, 255));
	else
		COMRESMGR->DrawFont(_T("º¼µå±¼¸²"), m_ItemDataString[ITEMDATA_DIFFMP], POINT{ -220 , -295 }, 2.3f, D3DCOLOR_ARGB(255, 255, 255, 255));


	COMRESMGR->DrawFont(_T("º¼µå±¼¸²"), m_ItemDataString[ITEMDATA_CRI], POINT{ -120,-270 }, 2.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
	COMRESMGR->DrawFont(_T("º¼µå±¼¸²"), m_ItemDataString[ITEMDATA_CRIDEM], POINT{ -120 ,-295 }, 2.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
	COMRESMGR->DrawFont(_T("º¼µå±¼¸²"), m_ItemDataString[ITEMDATA_AVOID], POINT{ -120 ,-320 }, 2.3f, D3DCOLOR_ARGB(255, 255, 255, 255));

	if (m_ItemDiffStat.Critical > 0)
		COMRESMGR->DrawFont(_T("º¼µå±¼¸²"), m_ItemDataString[ITEMDATA_DIFFCRI], POINT{ 30 , -270 }, 2.3f, D3DCOLOR_ARGB(255, 255, 25, 25));
	else if (m_ItemDiffStat.Critical < 0)
		COMRESMGR->DrawFont(_T("º¼µå±¼¸²"), m_ItemDataString[ITEMDATA_DIFFCRI], POINT{ 30, -270}, 2.3f, D3DCOLOR_ARGB(255, 25, 25, 255));
	else
		COMRESMGR->DrawFont(_T("º¼µå±¼¸²"), m_ItemDataString[ITEMDATA_DIFFCRI], POINT{ 30 , -270 }, 2.3f, D3DCOLOR_ARGB(255, 255, 255, 255));

	if (m_ItemDiffStat.CriticalDamage > 0)
		COMRESMGR->DrawFont(_T("º¼µå±¼¸²"), m_ItemDataString[ITEMDATA_DIFFCRIDEM], POINT{ 95 , -295 }, 2.3f, D3DCOLOR_ARGB(255, 255, 25, 25));
	else if (m_ItemDiffStat.CriticalDamage < 0)
		COMRESMGR->DrawFont(_T("º¼µå±¼¸²"), m_ItemDataString[ITEMDATA_DIFFCRIDEM], POINT{ 95, -295 }, 2.3f, D3DCOLOR_ARGB(255, 25, 25, 255));
	else
		COMRESMGR->DrawFont(_T("º¼µå±¼¸²"), m_ItemDataString[ITEMDATA_DIFFCRIDEM], POINT{ 95 , -295 }, 2.3f, D3DCOLOR_ARGB(255, 255, 255, 255));

	if (m_ItemDiffStat.Avoidability > 0)
		COMRESMGR->DrawFont(_T("º¼µå±¼¸²"), m_ItemDataString[ITEMDATA_DIFFAVOID], POINT{ 10 , -320 }, 2.3f, D3DCOLOR_ARGB(255, 255, 25, 25));
	else if (m_ItemDiffStat.Avoidability < 0)
		COMRESMGR->DrawFont(_T("º¼µå±¼¸²"), m_ItemDataString[ITEMDATA_DIFFAVOID], POINT{ 10, -320 }, 2.3f, D3DCOLOR_ARGB(255, 25, 25, 255));
	else
		COMRESMGR->DrawFont(_T("º¼µå±¼¸²"), m_ItemDataString[ITEMDATA_DIFFAVOID], POINT{ 10 , -320 }, 2.3f, D3DCOLOR_ARGB(255, 255, 255, 255));


}
void CCharacterEquipmentMenu::ChangeCharacterData(const CharacterIndex& _CharNo)
{
	if (_CharNo >= MAXBATTLECHARACTER)
		return;

	m_CurCharacterNo = _CharNo;
	
	TCHAR Buf[128] = _T("");
	size_t BufSize = sizeof(Buf) / sizeof(TCHAR);
	size_t CurLv = CClientGlobal::GetCurCharacterLevel(m_CurCharacterNo);

	UNITNAME CharName = CClientGlobal::CharacterName[m_CurCharacterNo];
	STATPARAMETERDATA Para = CClientGlobal::CharacterParameter[CharName.UnitName];

	STAT BaseStat = CClientGlobal::GetCurCharacterStat(m_CurCharacterNo, CurLv);
	CHARACTERDATA CurData = CClientGlobal::GetCurCharData(m_CurCharacterNo);

	m_CurItemStat = STAT();

	for (size_t i = 0; i < MAXITEMEQUIP; i++)
	{
		size_t ItemNo = CurData.EquipItem[i];
		if (-1 == ItemNo)
			continue;

		m_CurItemStat += CClientGlobal::vecItem[ItemNo].ItemStat;
	}

	STAT ResultStat = BaseStat + m_CurItemStat;


	m_vecCharDataString[CHARDATA_NAME] = CharName.UnitNickName + +_T(" ") + CharName.UnitKName;

	_stprintf_s(Buf, BufSize, _T("Lv %3d"), (int)CurLv);
	m_vecCharDataString[CHARDATA_LV] = Buf;

	
	_stprintf_s(Buf, BufSize, _T("Ã¼·Â : %3d"), ResultStat.Str);
	m_vecCharDataString[CHARDATA_STR] = Buf;

	_stprintf_s(Buf, BufSize, _T("¹ÎÃ¸ : %3d"), ResultStat.Dex);
	m_vecCharDataString[CHARDATA_DEX] = Buf;

	_stprintf_s(Buf, BufSize, _T("Áö´É : %3d"), ResultStat.Int);
	m_vecCharDataString[CHARDATA_INT] = Buf;

	_stprintf_s(Buf, BufSize, _T("Çà¿î : %3d"), ResultStat.Luck);
	m_vecCharDataString[CHARDATA_LUK] = Buf;


	_stprintf_s(Buf, BufSize, _T("HP : %5zd/%5zd"), CurData.CurHp, CurData.MaxHp);
	m_vecCharDataString[CHARDATA_HP] = Buf;

	_stprintf_s(Buf, BufSize, _T("MP : %3zd/%3zd"), CurData.CurMp, CurData.MaxMp);
	m_vecCharDataString[CHARDATA_MP] = Buf;

	_stprintf_s(Buf, BufSize, _T("°ø°Ý·Â : %3d"), ResultStat.Damage);
	m_vecCharDataString[CHARDATA_DEM] = Buf;

	_stprintf_s(Buf, BufSize, _T("¹æ¾î·Â : %3d"), ResultStat.Defense);
	m_vecCharDataString[CHARDATA_DEF] = Buf;


	_stprintf_s(Buf, BufSize, _T("Å©¸®Æ¼ÄÃ : %3d%%"), ResultStat.Critical);
	m_vecCharDataString[CHARDATA_CRI] = Buf;

	_stprintf_s(Buf, BufSize, _T("Å©¸®Æ¼ÄÃ µ¥¹ÌÁö : %3d%%"), ResultStat.CriticalDamage);
	m_vecCharDataString[CHARDATA_CRIDEM] = Buf;

	_stprintf_s(Buf, BufSize, _T("È¸ÇÇÀ² : %3d%%"), ResultStat.Avoidability);
	m_vecCharDataString[CHARDATA_AVOID] = Buf;


	
	if(0 == m_CurItemStat.Str)
		_stprintf_s(Buf, BufSize, _T("(-)"));
	else 
		_stprintf_s(Buf, BufSize, _T("(%+d)"), m_CurItemStat.Str);

	m_vecCharDataString[CHARDATA_ITEMSTR] = Buf;

	if (0 == m_CurItemStat.Dex)
		_stprintf_s(Buf, BufSize, _T("(-)"));
	else
		_stprintf_s(Buf, BufSize, _T("(%+d)"), m_CurItemStat.Dex);

	m_vecCharDataString[CHARDATA_ITEMDEX] = Buf;

	if (0 == m_CurItemStat.Int)
		_stprintf_s(Buf, BufSize, _T("(-)"));
	else
		_stprintf_s(Buf, BufSize, _T("(%+d)"), m_CurItemStat.Int);

	m_vecCharDataString[CHARDATA_ITEMINT] = Buf;

	if (0 == m_CurItemStat.Luck)
		_stprintf_s(Buf, BufSize, _T("(-)"));
	else
		_stprintf_s(Buf, BufSize, _T("(%+d)"), m_CurItemStat.Luck);

	m_vecCharDataString[CHARDATA_ITEMLUK] = Buf;

	if (0 == m_CurItemStat.Heath)
		_stprintf_s(Buf, BufSize, _T("(-)"));
	else
		_stprintf_s(Buf, BufSize, _T("(%+d)"), m_CurItemStat.Heath);

	m_vecCharDataString[CHARDATA_ITEMHP] = Buf;

	if (0 == m_CurItemStat.Mana)
		_stprintf_s(Buf, BufSize, _T("(-)"));
	else
		_stprintf_s(Buf, BufSize, _T("(%+d)"), m_CurItemStat.Mana);

	m_vecCharDataString[CHARDATA_ITEMMP] = Buf;

	if (0 == m_CurItemStat.Damage)
		_stprintf_s(Buf, BufSize, _T("(-)"));
	else
		_stprintf_s(Buf, BufSize, _T("(%+d)"), m_CurItemStat.Damage);

	m_vecCharDataString[CHARDATA_ITEMDEM] = Buf;

	if (0 == m_CurItemStat.Defense)
		_stprintf_s(Buf, BufSize, _T("(-)"));
	else
		_stprintf_s(Buf, BufSize, _T("(%+d)"), m_CurItemStat.Defense);

	m_vecCharDataString[CHARDATA_ITEMDEF] = Buf;

	if (0 == m_CurItemStat.Critical)
		_stprintf_s(Buf, BufSize, _T("(-)"));
	else
		_stprintf_s(Buf, BufSize, _T("(%+d%%)"), m_CurItemStat.Critical);

	m_vecCharDataString[CHARDATA_ITEMCRI] = Buf;

	if (0 == m_CurItemStat.CriticalDamage)
		_stprintf_s(Buf, BufSize, _T("(-)"));
	else
		_stprintf_s(Buf, BufSize, _T("(%+d%%)"), m_CurItemStat.CriticalDamage);

	m_vecCharDataString[CHARDATA_ITEMCRIDEM] = Buf;

	if (0 == m_CurItemStat.Avoidability)
		_stprintf_s(Buf, BufSize, _T("(-)"));
	else
		_stprintf_s(Buf, BufSize, _T("(%+d%%)"), m_CurItemStat.Avoidability);

	m_vecCharDataString[CHARDATA_ITEMAVOID] = Buf;
	
	if (nullptr != m_CharacterIconRen)
		m_CharacterIconRen->SetSprite(Illust(CharName.UnitName));

}
void CCharacterEquipmentMenu::ChangeItemData(const size_t& _ItemNo)
{
	size_t CurItemNo = m_CurEquipItemTab[m_CurEquipTabIndex]->GetCurItemNo();
	size_t SelItemNo = m_SelectEquipItemTab[m_CurSelectEquipIndex]->GetCurItemNo();

	ITEM CurItem;
	ITEM SelItem;
	m_SelectItem = SelItem;

	STAT CurItemStat;
	STAT SelItemStat;

	if (CurItemNo < CClientGlobal::vecItem.size()) 
	{
		CurItem = CClientGlobal::vecItem[CurItemNo];
		CurItemStat = CurItem.ItemStat;
	}
	
	if (SelItemNo < CClientGlobal::vecItem.size()) 
	{
		SelItem = CClientGlobal::vecItem[SelItemNo];
		m_SelectItem = SelItem;
		SelItemStat = SelItem.ItemStat;
	}
	

	m_ItemDiffStat = SelItemStat - CurItemStat;

	TCHAR Buf[MAXKEYLENGTH];
	size_t BufSize = sizeof(Buf) / sizeof(TCHAR);

	if (-1 == SelItem.ItemNo)
		m_ItemDataString[ITEMDATA_NAME] = _T("-");
	else
		m_ItemDataString[ITEMDATA_NAME] = SelItem.ItemKName;

	if(-1 == SelItem.ItemNo)
		m_ItemDataString[ITEMDATA_LV] = _T("Âø¿ë ·¹º§ : -");
	else 
	{
		_stprintf_s(Buf, BufSize, _T("Âø¿ë ·¹º§ : %3d"), SelItem.ItemStat.Level);
		m_ItemDataString[ITEMDATA_LV] = Buf;
	}

	_stprintf_s(Buf, BufSize, _T("Ã¼·Â : %3d"), SelItemStat.Str);
	m_ItemDataString[ITEMDATA_STR] = Buf;

	_stprintf_s(Buf, BufSize, _T("¹ÎÃ¸ : %3d"), SelItemStat.Dex);
	m_ItemDataString[ITEMDATA_DEX] = Buf;

	_stprintf_s(Buf, BufSize, _T("Áö´É : %3d"), SelItemStat.Int);
	m_ItemDataString[ITEMDATA_INT] = Buf;

	_stprintf_s(Buf, BufSize, _T("Çà¿î : %3d"), SelItemStat.Luck);
	m_ItemDataString[ITEMDATA_LUK] = Buf;


	_stprintf_s(Buf, BufSize, _T("HP : %d"), SelItemStat.Heath);
	m_ItemDataString[ITEMDATA_HP] = Buf;

	_stprintf_s(Buf, BufSize, _T("MP : %d"), SelItemStat.Mana);
	m_ItemDataString[ITEMDATA_MP] = Buf;

	_stprintf_s(Buf, BufSize, _T("°ø°Ý·Â : %3d"), SelItemStat.Damage);
	m_ItemDataString[ITEMDATA_DEM] = Buf;

	_stprintf_s(Buf, BufSize, _T("¹æ¾î·Â : %3d"), SelItemStat.Defense);
	m_ItemDataString[ITEMDATA_DEF] = Buf;

	_stprintf_s(Buf, BufSize, _T("Å©¸®Æ¼ÄÃ : %3d%%"), SelItemStat.Critical);
	m_ItemDataString[ITEMDATA_CRI] = Buf;

	_stprintf_s(Buf, BufSize, _T("Å©¸®Æ¼ÄÃ µ¥¹ÌÁö : %3d%%"), SelItemStat.CriticalDamage);
	m_ItemDataString[ITEMDATA_CRIDEM] = Buf;

	_stprintf_s(Buf, BufSize, _T("È¸ÇÇÀ² : %3d%%"), SelItemStat.Avoidability);
	m_ItemDataString[ITEMDATA_AVOID] = Buf;

	if (0 == m_ItemDiffStat.Str)
		_stprintf_s(Buf, BufSize, _T("(-)"));
	else
		_stprintf_s(Buf, BufSize, _T("(%+d)"), m_ItemDiffStat.Str);

	m_ItemDataString[ITEMDATA_DIFFSTR] = Buf;

	if (0 == m_ItemDiffStat.Dex)
		_stprintf_s(Buf, BufSize, _T("(-)"));
	else
		_stprintf_s(Buf, BufSize, _T("(%+d)"), m_ItemDiffStat.Dex);

	m_ItemDataString[ITEMDATA_DIFFDEX] = Buf;

	if (0 == m_ItemDiffStat.Int)
		_stprintf_s(Buf, BufSize, _T("(-)"));
	else
		_stprintf_s(Buf, BufSize, _T("(%+d)"), m_ItemDiffStat.Int);

	m_ItemDataString[ITEMDATA_DIFFINT] = Buf;

	if (0 == m_ItemDiffStat.Luck)
		_stprintf_s(Buf, BufSize, _T("(-)"));
	else
		_stprintf_s(Buf, BufSize, _T("(%+d)"), m_ItemDiffStat.Luck);

	m_ItemDataString[ITEMDATA_DIFFLUK] = Buf;

	if (0 == m_ItemDiffStat.Heath)
		_stprintf_s(Buf, BufSize, _T("(-)"));
	else
		_stprintf_s(Buf, BufSize, _T("(%+d)"), m_ItemDiffStat.Heath);

	m_ItemDataString[ITEMDATA_DIFFHP] = Buf;

	if (0 == m_ItemDiffStat.Mana)
		_stprintf_s(Buf, BufSize, _T("(-)"));
	else
		_stprintf_s(Buf, BufSize, _T("(%+d)"), m_ItemDiffStat.Mana);

	m_ItemDataString[ITEMDATA_DIFFMP] = Buf;

	if (0 == m_ItemDiffStat.Damage)
		_stprintf_s(Buf, BufSize, _T("(-)"));
	else
		_stprintf_s(Buf, BufSize, _T("(%+d)"), m_ItemDiffStat.Damage);

	m_ItemDataString[ITEMDATA_DIFFDEM] = Buf;

	if (0 == m_ItemDiffStat.Defense)
		_stprintf_s(Buf, BufSize, _T("(-)"));
	else
		_stprintf_s(Buf, BufSize, _T("(%+d)"), m_ItemDiffStat.Defense);

	m_ItemDataString[ITEMDATA_DIFFDEF] = Buf;

	if (0 == m_ItemDiffStat.Critical)
		_stprintf_s(Buf, BufSize, _T("(-)"));
	else
		_stprintf_s(Buf, BufSize, _T("(%+d%%)"), m_ItemDiffStat.Critical);

	m_ItemDataString[ITEMDATA_DIFFCRI] = Buf;

	if (0 == m_ItemDiffStat.CriticalDamage)
		_stprintf_s(Buf, BufSize, _T("(-)"));
	else
		_stprintf_s(Buf, BufSize, _T("(%+d%%)"), m_ItemDiffStat.CriticalDamage);

	m_ItemDataString[ITEMDATA_DIFFCRIDEM] = Buf;

	if (0 == m_ItemDiffStat.Avoidability)
		_stprintf_s(Buf, BufSize, _T("(-)"));
	else
		_stprintf_s(Buf, BufSize, _T("(%+d%%)"), m_ItemDiffStat.Avoidability);

	m_ItemDataString[ITEMDATA_DIFFAVOID] = Buf;

	m_ItemDetailRen->SetScriptString(SelItem.ItemDescription);
	
}
void CCharacterEquipmentMenu::SelectCharacter()
{
	if (true == KEYDOWN("MenuKey"))
	{
		m_pMenuMgr->ChangeRootMenu();
		AllActiveCharEquipOff();
		m_vecActiveEquipTab.clear();
		return;
	}

	else if (true == KEYDOWN("LEFT"))
		PrevCharTab();
	else if (true == KEYDOWN("RIGHT"))
		NextCharTab();
	else if (true == KEYDOWN("UP"))
		PrevCharTab(3);
	else if (true == KEYDOWN("DOWN"))
		NextCharTab(3);
	else if (true == KEYDOWN("Enter"))
	{
		m_CurTab = SELECT_CURRENT_EQUIPITEM;

		AllActiveCharEquipOff();
		ChangeMenuBack(_T("EquipMenuBack2"));
		int Index = m_CurCharacterPage * MAXCHARCTERTAB + m_CurCharacterIndex;
		CharacterIndex CharNo;
		CHARACTERDATA CurData;

		if (Index < m_vecActiveEquipTab.size()) 
		{
			CharNo = m_vecActiveEquipTab[Index]->GetCharacterNo();
			ChangeCharacterData(CharNo);
			CurData = CClientGlobal::GetCurCharData(CharNo);
		}

		if (nullptr != m_CharacterIconRen)
			m_CharacterIconRen->ActorOn();

		if (-1 == CurData.CharacterNo)
			return;

		for (size_t i = 0; i < m_CurEquipItemTab.size(); i++)
		{
			m_CurEquipItemTab[i]->ChangeItemData(CurData.EquipItem[i]);
			m_CurEquipItemTab[i]->ActorOn();
			m_CurEquipItemTab[i]->SelectTab(false);
		}
		m_CurEquipTabIndex = 0;
		m_CurEquipItemTab[m_CurEquipTabIndex]->SelectTab(true);
		ChangeSelectItemList(m_CurEquipTabIndex);

	}

	

}
void CCharacterEquipmentMenu::SelectCurEquipItem() 
{
	if (true == KEYDOWN("MenuKey"))
	{
		CurActiveCharEquipOn();
		CurEquiptItemTabOn(false);
		SelectEquipItemTabOn(false);
		ChangeMenuBack(_T("EquipMenuBack"));
		
		if (nullptr != m_CharacterIconRen)
			m_CharacterIconRen->ActorOff();
		m_CurTab = SELECT_CHARACTER;
		return;
	}

	else if (true == KEYDOWN("LEFT")) 
	{
		if (true == ChangeCurEquipTab(-2))
			ChangeSelectItemList(m_CurEquipTabIndex);
	}
	else if (true == KEYDOWN("RIGHT"))
	{
		if (true == ChangeCurEquipTab(2))
			ChangeSelectItemList(m_CurEquipTabIndex);
	}
	else if (true == KEYDOWN("UP"))
	{
		if (true == ChangeCurEquipTab(-1))
			ChangeSelectItemList(m_CurEquipTabIndex);
	}
	else if (true == KEYDOWN("DOWN"))
	{
		if (true == ChangeCurEquipTab(1))
			ChangeSelectItemList(m_CurEquipTabIndex);
	}
	else if (true == KEYDOWN("Enter"))
	{
		m_CurTab = SELECT_CHANGE_EQUIPITEM;
		m_CurSelectEquipPage = 0;
		m_CurSelectEquipIndex = 0;
		m_SelectEquipItemTab[m_CurSelectEquipIndex]->SelectTab(true);
		ChangeItemData(m_SelectEquipItemTab[m_CurSelectEquipIndex]->GetCurItemNo());
		m_ItemDetailRen->On();
	}


}
void CCharacterEquipmentMenu::SelectChangeEquipItem()
{
	if (true == KEYDOWN("MenuKey"))
	{
		if(nullptr != m_SelectEquipItemTab[m_CurSelectEquipIndex])
			m_SelectEquipItemTab[m_CurSelectEquipIndex]->SelectTab(false);
		
		m_CurTab = SELECT_CURRENT_EQUIPITEM;
		m_ItemDetailRen->Off();
		return;
	}
	else if (true == KEYDOWN("UP"))
	{
		if (true == PrevSelectEquipTab()) 
			ChangeItemData(m_SelectEquipItemTab[m_CurSelectEquipIndex]->GetCurItemNo());
	}
	else if (true == KEYDOWN("DOWN"))
	{
		if (true == NextSelectEquipTab())
			ChangeItemData(m_SelectEquipItemTab[m_CurSelectEquipIndex]->GetCurItemNo());
	}
	else if (true == KEYDOWN("Enter")) 
	{
		if (true == ChangeEquipItemEvent()) 
			SuccessChangeEquipItem();// Àåºñ ±³Ã¼ ¼º°ø½Ã 
		else 
			FailChangeEquipItem();// Àåºñ ±³Ã¼ ½ÇÆÐ½Ã
	}
}
void CCharacterEquipmentMenu::ChangeMenuBack(const tstring& _BackSpriteName)
{
	m_MenuMainBack->SetSprite(_BackSpriteName);
}
const bool CCharacterEquipmentMenu::PrevCharTab(const int& _JumpIndex)
{
	if (m_CurCharacterIndex - _JumpIndex < 0)
	{
		if (false == CheckChangeCharPage(PAGE_UP, _JumpIndex))
			return false;

		ChangeCharPage(PAGE_UP, _JumpIndex);
		return true;
	}

	int RealIndex = m_CurCharacterPage * MAXCHARCTERTAB + m_CurCharacterIndex;

	if (nullptr != m_vecActiveEquipTab[RealIndex])
		m_vecActiveEquipTab[RealIndex]->SelectTab(false);

	RealIndex -= _JumpIndex;
	m_CurCharacterIndex -= _JumpIndex;

	if (nullptr != m_vecActiveEquipTab[RealIndex])
		m_vecActiveEquipTab[RealIndex]->SelectTab(true);


	return true;

}
const bool CCharacterEquipmentMenu::NextCharTab(const int& _JumpIndex)
{
	if (m_CurCharacterIndex + _JumpIndex >= MAXCHARCTERTAB)
	{
		if (false == CheckChangeCharPage(PAGE_DOWN, _JumpIndex))
			return false;

		ChangeCharPage(PAGE_DOWN, _JumpIndex);
		return true;
	}

	int RealIndex = m_CurCharacterPage * (int)MAXCHARCTERTAB + m_CurCharacterIndex;

	if (RealIndex == m_vecActiveEquipTab.size() -1 )
		return false;
	
	if (RealIndex + _JumpIndex > m_vecActiveEquipTab.size() - 1) 
	{
		if (nullptr != m_vecActiveEquipTab[RealIndex])
			m_vecActiveEquipTab[RealIndex]->SelectTab(false);

		RealIndex = (int)m_vecActiveEquipTab.size() - 1;
		m_CurCharacterIndex = RealIndex - m_CurCharacterPage*MAXCHARCTERTAB;

		if (nullptr != m_vecActiveEquipTab[RealIndex])
			m_vecActiveEquipTab[RealIndex]->SelectTab(true);
	}
	else 
	{
		if (nullptr != m_vecActiveEquipTab[RealIndex])
			m_vecActiveEquipTab[RealIndex]->SelectTab(false);

		RealIndex += _JumpIndex;
		m_CurCharacterIndex += _JumpIndex;

		if (nullptr != m_vecActiveEquipTab[RealIndex])
			m_vecActiveEquipTab[RealIndex]->SelectTab(true);
	}


	return true;
}
const bool CCharacterEquipmentMenu::CheckChangeCharPage(const PAGEEVENT& _Evnet, const int& _JumpIndex)
{
	int JumpIndex = _JumpIndex;

	switch (_Evnet)
	{
	case CCharacterEquipmentMenu::PAGE_UP:
		JumpIndex *= -1;
		break;
	case CCharacterEquipmentMenu::PAGE_DOWN:
		break;
	default:
		return false;
	}

	int RealIndex = m_CurCharacterPage *MAXCHARCTERTAB + m_CurCharacterIndex + JumpIndex;

	switch (_Evnet)
	{
	case CCharacterEquipmentMenu::PAGE_UP:
	{
		if (RealIndex <= 0)
			return false;
	}
	return true;
	case CCharacterEquipmentMenu::PAGE_DOWN:
	{
		if ((m_CurCharacterPage + 1) * MAXCHARCTERTAB >= m_vecActiveEquipTab.size())
			return false;
	}
	return true;

	}

	return false;
}
void CCharacterEquipmentMenu::ChangeCharPage(const PAGEEVENT& _Evnet, const int& _JumpIndex)
{
	AllActiveCharEquipOff();
	int JumpIndex = _JumpIndex;

	switch (_Evnet)
	{
	case CCharacterEquipmentMenu::PAGE_UP:
		JumpIndex *= -1;
		break;
	case CCharacterEquipmentMenu::PAGE_DOWN:
		break;
	default:
		return;
	}

	switch (_Evnet)
	{
	case CCharacterEquipmentMenu::PAGE_UP:
	{
		--m_CurCharacterPage;
		int Index = m_CurCharacterIndex * (int)MAXCHARCTERTAB;
		int Count = 0;

		while (Count < MAXCHARCTERTAB)
		{
			if (Index >= m_vecActiveEquipTab.size())
				break;

			if (nullptr == m_vecActiveEquipTab[Index])
			{
				++Index;
				continue;
			}

			m_vecActiveEquipTab[Index++]->TabOn(m_vecEquipTabPos[Count++]);
		}


		m_CurCharacterIndex = MAXCHARCTERTAB + m_CurCharacterIndex + JumpIndex;
		m_vecActiveEquipTab[m_CurCharacterPage * MAXCHARCTERTAB + m_CurCharacterIndex]->SelectTab(true);
	}
	return;
	case CCharacterEquipmentMenu::PAGE_DOWN:
	{
		++m_CurCharacterPage;
		int Index = m_CurCharacterPage * (int)MAXCHARCTERTAB;
		int Count = 0;

		while (Count < MAXCHARCTERTAB)
		{
			if (Index >= m_vecActiveEquipTab.size())
				break;

			if (nullptr == m_vecActiveEquipTab[Index])
			{
				++Index;
				continue;
			}

			m_vecActiveEquipTab[Index++]->TabOn(m_vecEquipTabPos[Count++]);
		}


		m_CurCharacterIndex = (m_CurCharacterIndex + JumpIndex) % MAXCHARCTERTAB;

		if (m_CurCharacterPage * MAXCHARCTERTAB + m_CurCharacterIndex >= m_vecActiveEquipTab.size())
			m_CurCharacterIndex = (int)m_vecActiveEquipTab.size() - 1 - m_CurCharacterPage * MAXCHARCTERTAB;

		m_vecActiveEquipTab[m_CurCharacterPage * MAXCHARCTERTAB + m_CurCharacterIndex]->SelectTab(true);
	}
	return;

	}
}
const bool CCharacterEquipmentMenu::ChangeCurEquipTab(const int& _JumpIndex)
{
	if (0 > _JumpIndex) 
	{
		if (m_CurEquipTabIndex + _JumpIndex < 0)
			return false;

		if(nullptr != m_CurEquipItemTab[m_CurEquipTabIndex])
			m_CurEquipItemTab[m_CurEquipTabIndex]->SelectTab(false);

		m_CurEquipTabIndex += _JumpIndex;

		if (nullptr != m_CurEquipItemTab[m_CurEquipTabIndex])
			m_CurEquipItemTab[m_CurEquipTabIndex]->SelectTab(true);
	}
	else 
	{
		if (m_CurEquipTabIndex + _JumpIndex >= m_CurEquipItemTab.size())
			return false;

		if (nullptr != m_CurEquipItemTab[m_CurEquipTabIndex])
			m_CurEquipItemTab[m_CurEquipTabIndex]->SelectTab(false);

		m_CurEquipTabIndex += _JumpIndex;

		if (nullptr != m_CurEquipItemTab[m_CurEquipTabIndex])
			m_CurEquipItemTab[m_CurEquipTabIndex]->SelectTab(true);
	}
	return true;
}
const bool CCharacterEquipmentMenu::PrevSelectEquipTab()
{
	if (0 == m_CurSelectEquipIndex) 
	{
		if (true == CheckChangeSelectEquipTabPage(PAGE_UP)) 
		{
			ChangeSelectEquipTabPage(PAGE_UP);
			return true;
		}
		return false;
	}
	
	if (nullptr != m_SelectEquipItemTab[m_CurSelectEquipIndex])
		m_SelectEquipItemTab[m_CurSelectEquipIndex--]->SelectTab(false);

	if (nullptr != m_SelectEquipItemTab[m_CurSelectEquipIndex])
		m_SelectEquipItemTab[m_CurSelectEquipIndex]->SelectTab(true);

	return true;
}
const bool CCharacterEquipmentMenu::NextSelectEquipTab() 
{
	if (MAXSELECTITEMTAB -1  == m_CurSelectEquipIndex)
	{
		if (true == CheckChangeSelectEquipTabPage(PAGE_DOWN))
		{
			ChangeSelectEquipTabPage(PAGE_DOWN);
			return true;
		}

		return false;
	}

	int Next = m_CurSelectEquipIndex + 1;

	if (nullptr == m_SelectEquipItemTab[Next] || false == m_SelectEquipItemTab[Next]->IsActorOn())
		return false;
	
	if (nullptr != m_SelectEquipItemTab[m_CurSelectEquipIndex])
		m_SelectEquipItemTab[m_CurSelectEquipIndex++]->SelectTab(false);

	if (nullptr != m_SelectEquipItemTab[m_CurSelectEquipIndex])
		m_SelectEquipItemTab[m_CurSelectEquipIndex]->SelectTab(true);
	
	return true;
}
const bool CCharacterEquipmentMenu::CheckChangeSelectEquipTabPage(const PAGEEVENT& _Op)
{
	int RealIndex = 0;

	switch (_Op)
	{
	case CCharacterEquipmentMenu::PAGE_UP: 
	{
		RealIndex = m_CurSelectEquipPage* MAXSELECTITEMTAB + m_CurSelectEquipIndex - 1;

		if (-1 >= RealIndex)
			return false;
	}
		return true;
	case CCharacterEquipmentMenu::PAGE_DOWN: 
	{
		if ((m_CurSelectEquipPage + 1)* MAXSELECTITEMTAB - 1 >= m_SelectEquipItemList.size())
			return false;
	}
		return true;
	}

	return false;
}
void CCharacterEquipmentMenu::ChangeSelectEquipTabPage(const PAGEEVENT& _Op)
{
	switch (_Op)
	{
	case CCharacterEquipmentMenu::PAGE_UP:
	{
		--m_CurSelectEquipPage;
		m_CurSelectEquipIndex = 0;
		SelectEquipItemTabOn(false);

		int Start = (m_CurSelectEquipPage)* MAXSELECTITEMTAB - 1;
		
		for (; m_CurSelectEquipIndex < MAXSELECTITEMTAB; ++m_CurSelectEquipIndex,++Start)
		{
			if (-1 == Start)
				m_SelectEquipItemTab[m_CurSelectEquipIndex]->SetEquipReleaseTab();
			else if (Start <= m_SelectEquipItemList.size() - 1)
				m_SelectEquipItemTab[m_CurSelectEquipIndex]->ChangeItemData(m_SelectEquipItemList[Start]);
			else
				break;
			m_SelectEquipItemTab[m_CurSelectEquipIndex]->SelectTab(false);
			m_SelectEquipItemTab[m_CurSelectEquipIndex]->ActorOn();
		}
		
	
		m_CurSelectEquipIndex -= 1;

		m_SelectEquipItemTab[m_CurSelectEquipIndex]->SelectTab(true);
		
	}
		return;
	case CCharacterEquipmentMenu::PAGE_DOWN: 
	{
		++m_CurSelectEquipPage;
		m_CurSelectEquipIndex = 0;
		SelectEquipItemTabOn(false);

		int Start = (m_CurSelectEquipPage)* MAXSELECTITEMTAB - 1;

		for (; m_CurSelectEquipIndex < MAXSELECTITEMTAB; ++m_CurSelectEquipIndex, ++Start)
		{
			if (-1 == Start)
				m_SelectEquipItemTab[m_CurSelectEquipIndex]->SetEquipReleaseTab();
			else if (Start <= m_SelectEquipItemList.size() - 1)
				m_SelectEquipItemTab[m_CurSelectEquipIndex]->ChangeItemData(m_SelectEquipItemList[Start]);
			else
				break;

			m_SelectEquipItemTab[m_CurSelectEquipIndex]->SelectTab(false);
			m_SelectEquipItemTab[m_CurSelectEquipIndex]->ActorOn();
		}


		m_CurSelectEquipIndex = 0;

		m_SelectEquipItemTab[m_CurSelectEquipIndex]->SelectTab(true);
	}
		return;
	}
	return;
}
const bool CCharacterEquipmentMenu::ChangeEquipItemEvent()
{
	size_t ItemNo = m_SelectEquipItemTab[m_CurSelectEquipIndex]->GetCurItemNo();
	
	if (ItemNo < CClientGlobal::vecItem.size()) 
	{
		size_t ItemLevel = CClientGlobal::vecItem[ItemNo].ItemStat.Level;
		size_t CharLevel = CClientGlobal::GetCurCharData(m_CurCharacterNo).CharacterLv;

		if (ItemLevel > CharLevel)
			return false;
	}

	CClientGlobal::ChangeEquipItem(m_CurCharacterNo, (ITEMEQUIP)m_CurEquipTabIndex, ItemNo);
	return true;
}
void CCharacterEquipmentMenu::SuccessChangeEquipItem()
{
	ChangeCharacterData(m_CurCharacterNo);
	
	m_CurEquipItemTab[m_CurEquipTabIndex]->ChangeItemData(CClientGlobal::GetCurCharData(m_CurCharacterNo).EquipItem[m_CurEquipTabIndex]);
	
	ChangeSelectItemList(m_CurEquipTabIndex);
	m_CurTab = SELECT_CURRENT_EQUIPITEM;
	int CharIndex = m_CurCharacterPage * MAXCHARCTERTAB + m_CurCharacterIndex;
	m_vecActiveEquipTab[CharIndex]->UpdateEquipTab();

	m_ItemDetailRen->Off();

}
void CCharacterEquipmentMenu::FailChangeEquipItem()
{
	if (nullptr != m_MountItemFailGuide)
		m_MountItemFailGuide->StandBy();
	
}
void CCharacterEquipmentMenu::CurActiveCharEquipOn()
{
	int Start = m_CurCharacterPage * MAXCHARCTERTAB;
	int End = (m_CurCharacterPage + 1) * MAXCHARCTERTAB;
	int Index = 0;

	for (int i = Start; i < min(End,m_vecActiveEquipTab.size()); i++)
	{
		if (nullptr != m_vecActiveEquipTab[i])
			m_vecActiveEquipTab[i]->TabOn(m_vecEquipTabPos[Index++]);
	}
}
void CCharacterEquipmentMenu::AllActiveCharEquipOff()
{
	for (size_t i = 0; i < m_vecActiveEquipTab.size(); i++)
	{
		if (nullptr != m_vecActiveEquipTab[i])
			m_vecActiveEquipTab[i]->ActorOff();
	}
}
void CCharacterEquipmentMenu::CurEquiptItemTabOn(const bool& _On)
{
	if (true == _On)
	{
		for (size_t i = 0; i < m_CurEquipItemTab.size(); i++)
		{
			if (nullptr != m_CurEquipItemTab[i])
				m_CurEquipItemTab[i]->ActorOn();
		}
	}
	else 
	{
		for (size_t i = 0; i < m_CurEquipItemTab.size(); i++)
		{
			if (nullptr != m_CurEquipItemTab[i])
				m_CurEquipItemTab[i]->ActorOff();
		}
	}
}
void CCharacterEquipmentMenu::SelectEquipItemTabOn(const bool& _On)
{
	if (true == _On)
	{
		for (size_t i = 0; i < m_CurEquipItemTab.size(); i++)
		{
			if (nullptr != m_CurEquipItemTab[i])
				m_SelectEquipItemTab[i]->ActorOn();
		}
	}
	else
	{
		for (size_t i = 0; i < m_CurEquipItemTab.size(); i++)
		{
			if (nullptr != m_CurEquipItemTab[i])
				m_SelectEquipItemTab[i]->ActorOff();
		}
	}
}
void CCharacterEquipmentMenu::ChangeSelectItemList(const size_t& _Op)
{
	if (_Op >= 4)
		return;

	m_SelectEquipItemList.clear();
	m_CurSelectEquipPage = 0;
	m_CurSelectEquipIndex = 0;

	switch (_Op)
	{
	case 0:							// ¹«±â ¸®½ºÆ®
		CClientGlobal::InventoryMgr->RequestItemList(&m_SelectEquipItemList, CATEGORY_WEAPON, m_CurCharacterNo);
		break;
	case 1:							// ¹æ¾î±¸ ¸®½ºÆ®
		CClientGlobal::InventoryMgr->RequestItemList(&m_SelectEquipItemList, CATEGORY_ARMOR, m_CurCharacterNo);
		break;
	case 2:							// Àå½Å±¸ ¸®½ºÆ®
	case 3: 
		CClientGlobal::InventoryMgr->RequestItemList(&m_SelectEquipItemList, CATEGORY_ACCESSORIES, m_CurCharacterNo);
	break;
	}
	
	SelectEquipItemTabOn(false);

	m_SelectEquipItemTab[0]->SetEquipReleaseTab();
	m_SelectEquipItemTab[0]->SelectTab(false);
	m_SelectEquipItemTab[0]->ActorOn();

	for (size_t i = 0; i < min(3, m_SelectEquipItemList.size()); i++)
	{
		m_SelectEquipItemTab[i + 1]->ChangeItemData(m_SelectEquipItemList[i]);
		m_SelectEquipItemTab[i + 1]->ActorOn();
		m_SelectEquipItemTab[i + 1]->SelectTab(false);
	}
	
}