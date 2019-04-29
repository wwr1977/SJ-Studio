#include "Precom.h"
#include "InvenSlot.h"
#include "InventoryMgr.h"
#include "Inventory.h"
#include <GameMultiSprite.h>


CInventory*	CInvenSlot::pInventory = nullptr;

void CInvenSlot::SetInventory(CInventory* _Inven)
{
	if (nullptr == _Inven)
		return;

	pInventory = _Inven;
}
CInvenSlot::CInvenSlot()
	:m_ItemIconRen(nullptr),m_ItemNo(-1)
	, m_CurItemCount(0), m_InvenSlotCol(nullptr)
	, m_bClick(false)
{
}


CInvenSlot::~CInvenSlot()
{
}
void CInvenSlot::Init() 
{
	CMenuTab::Init();

	m_ItemIconRen = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_UI, 0.9f, true));
	m_ItemIconRen->SetRenPivot(Vec2{ -205.f,0.f });
	m_ItemIconRen->SetCustomSize(Vec2{ 32.f,32.f });

	m_InvenSlotCol = ACTOR->CreateCom<CCollision>(_T("InvenSlot"));
	m_InvenSlotCol->SetColType(CT_RECT);
	m_InvenSlotCol->SetColSize(Vec2{ 350.f,30.f });
	m_InvenSlotCol->SetEnter(this, &CInvenSlot::InvenSlotEnter);
	m_InvenSlotCol->SetStay(this, &CInvenSlot::InvenSlotStay);
	m_InvenSlotCol->SetExit(this, &CInvenSlot::InvenSlotExit);


	SetPivot(POINT{ -160,-8 });
	SetTabStyle(_T("큰UI레벨폰트"));
	SetTabSize(Vec2{ 350.f,30.f });
	m_TabSelectRen->SetRenPivot(Vec2{ 5.f,0.f });
	CreateSubTabFont(_T("큰UI레벨폰트"), POINT{ 210, 0 }, RIGHTSTART, _T(""), 1.3f);

}
void CInvenSlot::Update() 
{

}
void CInvenSlot::InvenSlotEnter(SPTR<CCollision> _This, SPTR<CCollision> _Other) 
{
	SelectTab(true);
	m_bClick = false;
}
void CInvenSlot::InvenSlotStay(SPTR<CCollision> _This, SPTR<CCollision> _Other)
{
	if (TRUE == KEYDOWN("LMouse") && !m_bClick)
	{
		if (nullptr == pInventory)
			return;

		m_bClick = true;

		Vec3 Pos = TRANS->GetPos();
		Vec3 MousePos = _Other->GetActorPos();

		if (Pos.x < MousePos.x)
			pInventory->SlotClickEvent(m_ItemNo, false);
		else
			pInventory->SlotClickEvent(m_ItemNo, true);
		
		return;
	}
	
}
void CInvenSlot::InvenSlotExit(SPTR<CCollision> _This, SPTR<CCollision> _Other) 
{
	SelectTab(false);
	m_bClick = false;

	if (nullptr != pInventory)
		pInventory->ItemGuideOffEvent();
}
void CInvenSlot::SetInvenSlot(const size_t& _ItemNo)
{
	if (_ItemNo >= CClientGlobal::vecItem.size())
		return;

	m_ItemNo = _ItemNo;
	SetTabName(CClientGlobal::vecItem[m_ItemNo].ItemKName);
	m_ItemIconRen->SetSprite(CClientGlobal::ItemIconMultiSprite->GetSprite(CClientGlobal::vecItem[m_ItemNo].ItemIconIndex));
	UpdateInvenItemCount();
}
const size_t  CInvenSlot::UpdateInvenItemCount()
{
	m_CurItemCount = CClientGlobal::InventoryMgr->RequestInventoryItemCount(m_ItemNo);

	m_SubTabFontRen[ITEMCOUNT]->SetString(to_wstring(m_CurItemCount));

	return m_CurItemCount;
}
const size_t CInvenSlot::GetCurItemCount()
{
	return m_CurItemCount;
}
const size_t CInvenSlot::GetItemNo()
{
	return m_ItemNo;
}