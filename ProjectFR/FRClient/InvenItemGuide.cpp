#include "Precom.h"
#include "InvenItemGuide.h"
#include <GameMultiSprite.h>



CInvenItemGuide::CInvenItemGuide()
	:m_GuideBackRen(nullptr), m_ItemIconRen(nullptr)
	, m_pInventory(nullptr), m_ItemNameRen(nullptr)
	, m_ItemLevelRen(nullptr), m_ItemDescriptionRen(nullptr)
{
}


CInvenItemGuide::~CInvenItemGuide()
{
	m_vecItemStatRen.clear();
}
void CInvenItemGuide::SetInventory(CInventory* _pInven)
{
	if (nullptr == _pInven)
		return;

	m_pInventory = _pInven;
}
void CInvenItemGuide::Init() 
{
	m_GuideBackRen = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_UIBACK, 0.85f, true));
	m_GuideBackRen->SetSprite(_T("ColorBase"));
	m_GuideBackRen->SetMaterial(D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.f));
	m_GuideBackRen->MaterialOn();
	m_GuideBackRen->SetCustomSize(Vec2{ 370.f,500.f });

	m_ItemIconRen = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_UI, 1.f, true));
	m_ItemIconRen->SetCustomSize(Vec2{ 48.f,48.f });
	m_ItemIconRen->SetSprite(_T("ColorBase"));
	m_ItemIconRen->SetRenPivot(Vec2{ -140.f,205.f });

	m_ItemNameRen = ACTOR->CreateCom<CFontRenderer>(RENDATA(LAYER_UI, 1.f, true));
	m_ItemNameRen->SetFontStyle(_T("큰UI레벨폰트"));
	m_ItemNameRen->SetLeftStartPivot(POINT{ -95,210 });
	m_ItemNameRen->SetFontColor(D3DCOLOR_ARGB(255, 255, 135, 0));

	m_ItemLevelRen = ACTOR->CreateCom<CFontRenderer>(RENDATA(LAYER_UI, 1.f, true));
	m_ItemLevelRen->SetFontStyle(_T("볼드굴림"));
	m_ItemLevelRen->SetFontColor(D3DCOLOR_ARGB(230, 220, 30, 30));
	m_ItemLevelRen->SetLeftStartPivot(POINT{ -95,180 });

	m_ItemDescriptionRen = ACTOR->CreateCom<CScriptRenderer>(RENDATA(LAYER_UI, 1.f, true));
	m_ItemDescriptionRen->SetLineInterval(20);
	m_ItemDescriptionRen->SetFontStyle(_T("볼드굴림"));
	m_ItemDescriptionRen->SetLeftStartPivot(POINT{ -155,-180 });

	for (size_t i = 0; i < MAXITEMDATA; i++)
	{
		m_vecItemStatRen.push_back(ACTOR->CreateCom<CFontRenderer>(RENDATA(LAYER_UI, 1.f, true)));
		m_vecItemStatRen[i]->SetFontStyle(_T("좀큰볼드굴림"));
	}

	m_vecItemStatRen[ITEM_DEMAGE]->SetLeftStartPivot(POINT{ 30,140 });
	m_vecItemStatRen[ITEM_DEFENSE]->SetLeftStartPivot(POINT{ 30,110 });
	m_vecItemStatRen[ITEM_STR]->SetLeftStartPivot(POINT{-165,  50 });
	m_vecItemStatRen[ITEM_DEX]->SetLeftStartPivot(POINT{ -165, 25 });
	m_vecItemStatRen[ITEM_INT]->SetLeftStartPivot(POINT{ -165, 0 });
	m_vecItemStatRen[ITEM_LUCK]->SetLeftStartPivot(POINT{ -165, -25 });
	m_vecItemStatRen[ITEM_HP]->SetLeftStartPivot(POINT{ -40, 50 });
	m_vecItemStatRen[ITEM_MP]->SetLeftStartPivot(POINT{ -40, 25 });
	m_vecItemStatRen[ITEM_CRI]->SetLeftStartPivot(POINT{ -40, 0 });
	m_vecItemStatRen[ITEM_CRIDEM]->SetLeftStartPivot(POINT{ -40, -25 });
	m_vecItemStatRen[ITEM_AVOID]->SetLeftStartPivot(POINT{ -40, -50 });

}
void CInvenItemGuide::Update() 
{

}
void CInvenItemGuide::UIFontRender() 
{

}
void CInvenItemGuide::SetItemData(const size_t& _ItemNo)
{
	if (_ItemNo >= CClientGlobal::vecItem.size())
		return;

	ITEM Item = CClientGlobal::vecItem[_ItemNo];
	STAT Stat = Item.ItemStat;

	m_ItemNameRen->SetString(Item.ItemKName);
	m_ItemIconRen->SetSprite(CClientGlobal::ItemIconMultiSprite->GetSprite(Item.ItemIconIndex));
	m_ItemDescriptionRen->SetScriptString(Item.ItemDescription);

	if (CATEGORY_ETC == Item.ItemCategory)
	{
		m_ItemLevelRen->Off();
		for (size_t i = 0; i < MAXITEMDATA; i++)
			m_vecItemStatRen[i]->Off();
	}
	else 
	{
		m_ItemLevelRen->On();
		for (size_t i = 0; i < MAXITEMDATA; i++)
			m_vecItemStatRen[i]->On();

		m_ItemLevelRen->SetString(_T("Lv ") + to_wstring(Item.ItemStat.Level));
		m_vecItemStatRen[ITEM_DEMAGE]->SetString(_T("공격력 : ") + to_wstring(Stat.Damage));
		m_vecItemStatRen[ITEM_DEFENSE]->SetString(_T("방어력 : ") + to_wstring(Stat.Defense));
		m_vecItemStatRen[ITEM_STR]->SetString(_T("체력 : ") + to_wstring(Stat.Str));
		m_vecItemStatRen[ITEM_DEX]->SetString(_T("민첩 : ") + to_wstring(Stat.Dex));
		m_vecItemStatRen[ITEM_INT]->SetString(_T("지능 : ") + to_wstring(Stat.Int));
		m_vecItemStatRen[ITEM_LUCK]->SetString(_T("행운 : ") + to_wstring(Stat.Luck));
		m_vecItemStatRen[ITEM_HP]->SetString(_T("HP : ") + to_wstring(Stat.Heath));
		m_vecItemStatRen[ITEM_MP]->SetString(_T("MP : ") + to_wstring(Stat.Mana));
		m_vecItemStatRen[ITEM_CRI]->SetString(_T("크리티컬 : ") + to_wstring(Stat.Critical) + _T("%"));
		m_vecItemStatRen[ITEM_CRIDEM]->SetString(_T("크리티컬 데미지 : ") + to_wstring(Stat.CriticalDamage) + _T("%"));
		m_vecItemStatRen[ITEM_AVOID]->SetString(_T("회피율 : ") + to_wstring(Stat.Avoidability) + _T("%"));
	}
}
void CInvenItemGuide::GuideActiveEvent(const Vec3& _Pos)
{
	ACTOR->On();

	TRANS->SetPos(_Pos);
}