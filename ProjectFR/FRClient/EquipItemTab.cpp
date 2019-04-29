#include "Precom.h"
#include "EquipItemTab.h"
#include"InventoryMgr.h"
#include <GameMultiSprite.h>

CEquipItemTab::CEquipItemTab()
	:m_ItemBackRen(nullptr), m_ItemIconRen(nullptr)
	,m_ItemNameRen(nullptr), m_ItemCountRen(nullptr)
	, m_CurItemData(ItemData()), m_bItemCountRen(false)
{
}


CEquipItemTab::~CEquipItemTab()
{

}
void CEquipItemTab::Init()
{
	m_ItemBackRen = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_UIBACK, 0.5f, true));
	m_ItemBackRen->SetSprite(_T("ColorBase"));
	m_ItemBackRen->SetMaterial(D3DXCOLOR(0.7f, 0.7f, 0.7f, 0.5f));
	m_ItemBackRen->MaterialOn();
	m_ItemBackRen->SetCustomSize(Vec2{ 256.f,44.f });
	m_ItemBackRen->SetRenPivot(Vec2{ 100.f,0.f });

	m_ItemIconRen = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_UI, 1.f, true));
	m_ItemIconRen->SetSprite(_T("ColorBase"));

	m_ItemNameRen = ACTOR->CreateCom<CFontRenderer>(RENDATA(LAYER_UI, 1.f, true));
	m_ItemNameRen->SetFontStyle(_T("º¼µå±¼¸²"));
	m_ItemNameRen->SetLeftStartPivot(POINT{ 30, -7 });
	m_ItemNameRen->SetFontColor(D3DXCOLOR(1.f, 0.53f, 0.f, 1.f));

	m_ItemCountRen = ACTOR->CreateCom<CFontRenderer>(RENDATA(LAYER_UI, 1.f, true));
	m_ItemCountRen->SetFontStyle(_T("º¼µå±¼¸²"));
	m_ItemCountRen->SetLeftStartPivot(POINT{ 230, -7 });
}
void CEquipItemTab::ChangeItemData(const ItemData& _Data)
{
	if (_Data.ItemNo >= CClientGlobal::vecItem.size())
		return;

	m_CurItemData = _Data;
	TCHAR Buf[MAXKEYLENGTH];
	size_t BufSize = sizeof(Buf) / sizeof(TCHAR);

	ITEM Item = CClientGlobal::vecItem[m_CurItemData.ItemNo];
	m_CurItemNo = m_CurItemData.ItemNo;
	m_ItemIconRen->SetSprite(CClientGlobal::ItemIconMultiSprite->GetSprite(Item.ItemIconIndex));
	m_ItemIconRen->On();

	_stprintf_s(Buf, BufSize, _T("x%2zd"), m_CurItemData.ItemCount);
	m_ItemCountRen->SetString(Buf);

	m_ItemNameRen->SetString(Item.ItemKName);
	m_ItemNameRen->SetFontColor(D3DXCOLOR(1.f, 0.53f, 0.f, 1.f));
}
void CEquipItemTab::ChangeItemData(const size_t& _ItemNo)
{
	if (-1 == _ItemNo) 
	{
		SetNoItemTab();
		return;
	}

	if (_ItemNo >= CClientGlobal::vecItem.size())
		return;

	ITEM Item = CClientGlobal::vecItem[_ItemNo];
	m_CurItemNo = _ItemNo;
	m_ItemIconRen->SetSprite(CClientGlobal::ItemIconMultiSprite->GetSprite(Item.ItemIconIndex));
	m_ItemIconRen->On();

	m_ItemCountRen->SetString(_T(""));
	m_ItemCountRen->Off();

	m_ItemNameRen->SetString(Item.ItemKName);
}
void CEquipItemTab::SetNoItemTab()
{
	m_ItemIconRen->SetSprite(_T("NoItem"));

	m_ItemCountRen->SetString(_T(""));
	m_ItemNameRen->SetString(_T("¹ÌÂø¿ë"));
}
void CEquipItemTab::SetEquipReleaseTab()
{
	//m_ItemIconRen->SetSprite(_T("NoItem"));
	m_ItemIconRen->Off();
	m_ItemCountRen->SetString(_T(""));
	m_ItemNameRen->SetString(_T("ÀåÂø ÇØÁ¦"));
	m_ItemNameRen->SetFontColor(D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
}
const size_t CEquipItemTab::GetCurItemNo()
{
	tstring Check = m_ItemNameRen->GetFontString();

	if (Check == _T("¹ÌÂø¿ë") || Check == _T("ÀåÂø ÇØÁ¦"))
		return -1;

	return m_CurItemNo;
}
void CEquipItemTab::SelectTab(const bool& _Select)
{
	if (true == _Select)
		m_ItemBackRen->On();
	else
		m_ItemBackRen->Off();
}
void CEquipItemTab::ItemCountRender(const bool& _Render) 
{
	if (true == _Render)
		m_ItemCountRen->On();
	else
		m_ItemCountRen->Off();
}