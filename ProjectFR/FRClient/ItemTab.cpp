#include "Precom.h"
#include "ItemTab.h"
#include"InventoryMgr.h"
#include <GameMultiSprite.h>

CItemTab::CItemTab()
	:m_ItemRen(nullptr), m_CurMode(PURCHASE_MODE)
	, m_ItemNo(-1), m_ItemPrice(0), m_ItemSalePrice(0)
	, m_CurItemCount(0)
{
}


CItemTab::~CItemTab()
{
}
void CItemTab::Init() 
{
	CMenuTab::Init();

	m_ItemRen = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_UI, 0.9f, true));
	m_ItemRen->SetRenPivot(Vec2{ -205.f,0.f });
	m_ItemRen->SetCustomSize(Vec2{ 32.f,32.f });
}
void CItemTab::InitData(const TABMODE& _Mode)
{
	m_CurMode = _Mode;

	/*if (INVENTORY_MODE == m_CurMode)
		InitInventoryItemTab();
	else
		InitStoreItemTab();*/

	SetPivot(POINT{ -160,-8 });
	SetTabStyle(_T("큰UI레벨폰트"));
	SetTabSize(Vec2{ 390.f,30.f });
	m_TabSelectRen->SetRenPivot(Vec2{ 25.f,0.f });
	CreateSubTabFont(_T("큰UI레벨폰트"), POINT{ 210, 0 }, RIGHTSTART, _T(""), 1.3f);

}
void CItemTab::Update() 
{

}
void CItemTab::UpdateItemCount()
{
	m_CurItemCount = CClientGlobal::InventoryMgr->RequestInventoryItemCount(m_ItemNo);
	
	if(SALE_MODE == m_CurMode || INVENTORY_MODE == m_CurMode)
		SetTabName(CClientGlobal::vecItem[m_ItemNo].ItemKName + _T(" x") + to_wstring(m_CurItemCount));
	return;
}
void CItemTab::InitStoreItemTab() 
{
	SetPivot(POINT{ -160,-8 });
	SetTabStyle(_T("큰UI레벨폰트"));
	SetTabSize(Vec2{ 390.f,30.f });
	m_TabSelectRen->SetRenPivot(Vec2{25.f,0.f});
}
void CItemTab::InitInventoryItemTab() 
{

}
void CItemTab::SetItemTab(const size_t& _ItemNo)
{
	if (_ItemNo >= CClientGlobal::vecItem.size())
		return;

	m_ItemNo = _ItemNo;
	SetTabName(CClientGlobal::vecItem[m_ItemNo].ItemKName);
	m_ItemPrice = CClientGlobal::vecItem[m_ItemNo].ItemPrice;
	m_ItemSalePrice = m_ItemPrice / 10;
	UpdateItemCount();
	m_ItemRen->SetSprite(CClientGlobal::ItemIconMultiSprite->GetSprite(CClientGlobal::vecItem[m_ItemNo].ItemIconIndex));
	
	ChangeTabMode(m_CurMode);
	//tstring Price = to_wstring(m_ItemPrice);
	//m_SubTabFontRen[SUB_PRICE]->SetString(Price + _T(" G"));
	//CreateSubTabFont(_T("큰UI레벨폰트"), POINT{ 210, 0}, RIGHTSTART, Price+_T(" G"),1.3f);
}
void CItemTab::ChangeTabMode(const TABMODE& _Mode)
{
	if (_Mode >= MAXTABMODE)
		return;

	m_CurMode = _Mode;

	switch (_Mode)
	{
	case CItemTab::PURCHASE_MODE: 
	{
		SetTabName(CClientGlobal::vecItem[m_ItemNo].ItemKName);
		tstring Price = to_wstring(m_ItemPrice);
		m_SubTabFontRen[SUB_PRICE]->SetString(Price + _T(" G"));
		m_SubTabFontRen[SUB_PRICE]->On();
	}
		break;
	case CItemTab::SALE_MODE: 
	{
		SetTabName(CClientGlobal::vecItem[m_ItemNo].ItemKName +_T(" x")+to_wstring(m_CurItemCount));
		tstring Price = to_wstring(m_ItemSalePrice);
		m_SubTabFontRen[SUB_PRICE]->SetString(Price + _T(" G"));
		m_SubTabFontRen[SUB_PRICE]->On();
	}
		break;
	case CItemTab::INVENTORY_MODE:
	{
		SetTabName(CClientGlobal::vecItem[m_ItemNo].ItemKName + _T(" x") + to_wstring(m_CurItemCount));
		m_SubTabFontRen[SUB_PRICE]->Off();
	}
		break;
	}

}
void CItemTab::SetCurItemCount(const size_t& _Count)
{
	m_CurItemCount = _Count;
}
const size_t CItemTab::GetItemPrice()
{
	return m_ItemPrice;
}
const size_t CItemTab::GetItemSalePrice()
{
	return m_ItemSalePrice;
}
const size_t CItemTab::GetCurItemCount() 
{
	return m_CurItemCount;
}

const size_t CItemTab::GetItemNo()
{
	return m_ItemNo;
}