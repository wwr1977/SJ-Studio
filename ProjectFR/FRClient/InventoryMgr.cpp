#include "Precom.h"
#include "InventoryMgr.h"
#include "Inventory.h"
#include "GameMaster.h"
#include "MouseCursor.h"

CInventoryMgr::CInventoryMgr()
	:m_TotalGold(100000), m_InvenWindow(nullptr)
	,m_pGameMaster(nullptr), m_CurUpdateState(INVENTORY_WAIT)
{
	CClientGlobal::InventoryMgr = this;
	m_AllItemData.clear();
	m_Inventory.clear();
	m_EquipItemList.clear();
	Init();
}
CInventoryMgr::CInventoryMgr(CGameMaster* _Master)
	:m_TotalGold(100000), m_InvenWindow(nullptr)
	, m_pGameMaster(_Master), m_CurUpdateState(INVENTORY_WAIT)
{

	CClientGlobal::InventoryMgr = this;
	m_AllItemData.clear();
	m_Inventory.clear();
	m_EquipItemList.clear();
	Init();
}


CInventoryMgr::~CInventoryMgr()
{

}
void CInventoryMgr::Init()
{
	size_t ItemCount = CClientGlobal::vecItem.size();

	
	for (size_t i = 0; i < ItemCount; i++) 
	{
		size_t Category = CClientGlobal::vecItem[i].ItemCategory;
		m_AllItemData.push_back(ItemData(i, 1, Category));
		m_Inventory.push_back(ItemData(i, 1, Category));
		m_EquipItemList.push_back(ItemData(i, 0, Category));
	}
	
	m_AllItemData[24].ItemCount = 0;
	m_Inventory[24].ItemCount = 0;


	if(nullptr != m_pGameMaster && nullptr != m_pGameMaster->GetMainScene())
	m_InvenWindow = m_pGameMaster->CreateActObject(_T("Inventory"))->CreateCom<CInventory>();
	m_InvenWindow->SetInventoryMgr(this);


	//m_EquipItemList[0].ItemCount = 1;
	//m_EquipItemList[9].ItemCount = 1;
	//m_EquipItemList[20].ItemCount = 1;
	//m_EquipItemList[21].ItemCount = 1;
}
void CInventoryMgr::Update()
{
	switch (m_CurUpdateState)
	{
	case CInventoryMgr::INVENTORY_WAIT:
	{
		if (TRUE == KEYDOWN("InvenKey"))
			OpenInventoryEvent();
	}
		break;
	case CInventoryMgr::INVENTORY_ON:
		m_InvenWindow->Update();
		break;
	}
}
void CInventoryMgr::ActiveInventoryPopUp(const bool& _PopUp)
{
	if (_PopUp)
		m_CurUpdateState = INVENTORY_WAIT;
	else
		m_CurUpdateState = UPDATE_PAUSE;
}
void CInventoryMgr::OpenInventoryEvent() 
{
	m_pGameMaster->InventoryEvent(true);
	m_InvenWindow->StandBy();
	m_CurUpdateState = INVENTORY_ON;
}
void CInventoryMgr::CloseInventoryEvent()
{
	m_pGameMaster->InventoryEvent(false);
	m_CurUpdateState = INVENTORY_WAIT;
}
void CInventoryMgr::RequestItemList(list<ItemData>* _ItemList, const ITEMTYPE& _ItemType)
{
	if (nullptr == _ItemList)
		return;

	for (size_t i = 0; i < m_Inventory.size(); i++)
	{
		if (0 == m_Inventory[i].ItemCount)
			continue;

		if (_ItemType == CClientGlobal::vecItem[i].ItemType)
			_ItemList->push_back(m_Inventory[i]);
	}

	return;
}
// 마지막 변수의 디폴트 값은 모든 캐릭터가 착용가능한 아이템 모두를 반환한다.
void CInventoryMgr::RequestItemList(list<ItemData>* _ItemList, const ITEMCATEGORY& _Category, const CharacterIndex& _CharNo )
{
	if (nullptr == _ItemList)
		return;

	size_t LineNo = CClientGlobal::FindCharacterLineNo(_CharNo);
	
	if (-1 == LineNo)
		return;

	for (size_t i = 0; i < m_Inventory.size(); i++)
	{
		if (0 == m_Inventory[i].ItemCount)
			continue;

		if (_Category == CClientGlobal::vecItem[i].ItemCategory &&
			true == CClientGlobal::vecItem[i].CheckMountItem((CHARACTERLINE)LineNo))
			_ItemList->push_back(m_Inventory[i]);
	}
}
void CInventoryMgr::RequestItemList(vector<ItemData>* _ItemList, const ITEMCATEGORY& _Category, const CharacterIndex& _CharNo)
{
	if (nullptr == _ItemList)
		return;

	size_t LineNo = CClientGlobal::FindCharacterLineNo(_CharNo);

	if (-1 == LineNo)
		return;

	for (size_t i = 0; i < m_Inventory.size(); i++)
	{
		if (0 == m_Inventory[i].ItemCount)
			continue;

		if (_Category == CClientGlobal::vecItem[i].ItemCategory &&
			true == CClientGlobal::vecItem[i].CheckMountItem((CHARACTERLINE)LineNo))
			_ItemList->push_back(m_Inventory[i]);
	}
}
void CInventoryMgr::RequestStoreItemList(list<ItemData>* _ItemList, const ITEMCATEGORY& _Category)
{
	if (nullptr == _ItemList)
		return;

	for (size_t i = 0; i  < CClientGlobal::vecItem.size(); i++)
	{
		if (_Category == CClientGlobal::vecItem[i].ItemCategory )
			_ItemList->push_back(ItemData(i, m_Inventory[i].ItemCount, m_Inventory[i].ItemCategory));
	}

	return;
}
void CInventoryMgr::RequestInventoryItemList(list<ItemData>* _ItemList)
{
	if (nullptr == _ItemList)
		return;

	for (size_t i = 0; i < m_Inventory.size(); i++)
	{
		if (0 < m_Inventory[i].ItemCount )
			_ItemList->push_back(m_Inventory[i]);
	}


	stable_sort(_ItemList->begin(), _ItemList->end(),CompareItemDataCategory);
	return;
}
const size_t CInventoryMgr::RequestInventoryItemCount(const size_t& _ItemNo)
{
	if (_ItemNo >= m_Inventory.size())
		return 0;

	return m_Inventory[_ItemNo].ItemCount;
}
void CInventoryMgr::MountEquipItem(const size_t& _ItemNo)
{
	if (_ItemNo >= m_AllItemData.size())
		return;

	if (m_Inventory[_ItemNo].ItemCount <= 0)
		return;

	m_Inventory[_ItemNo].ItemCount -= 1;
	m_EquipItemList[_ItemNo].ItemCount += 1;
}
void CInventoryMgr::ReleaseEquipItem(const size_t& _ItemNo)
{
	if (_ItemNo >= m_AllItemData.size())
		return;

	if (m_EquipItemList[_ItemNo].ItemCount <= 0)
		return;

	m_EquipItemList[_ItemNo].ItemCount -= 1;
	m_Inventory[_ItemNo].ItemCount += 1;
}
bool CInventoryMgr::PurchaseItem(const size_t& _ItemNo, const size_t& _PurchaseCount) 
{
	if (_ItemNo >= CClientGlobal::vecItem.size())
		return false;

	size_t Price = CClientGlobal::vecItem[_ItemNo].ItemPrice*_PurchaseCount;
	
	if (Price > m_TotalGold)
		return false;

	m_TotalGold -= Price;

	m_Inventory[_ItemNo].ItemCount += _PurchaseCount;
	return true;
}
bool CInventoryMgr::SaleItem(const size_t& _ItemNo, const size_t& _SaleCount)
{
	if (_ItemNo >= CClientGlobal::vecItem.size())
		return false;

	size_t Price = (CClientGlobal::vecItem[_ItemNo].ItemPrice/10) * _SaleCount;

	if (m_Inventory[_ItemNo].ItemCount < _SaleCount)
		return false;

	m_TotalGold += Price;

	m_Inventory[_ItemNo].ItemCount -= _SaleCount;
	return true;
}
// 인벤토리에서 해당 아이템을 뺀다.(퀘스트템 전달 등등)
bool CInventoryMgr::DropItem(const size_t& _ItemNo, const size_t& _DropCount /*= 1*/)
{
	if (_ItemNo >= m_Inventory.size()  || m_Inventory[_ItemNo].ItemCount <= 0)
		return false;

	m_Inventory[_ItemNo].ItemCount = max(m_Inventory[_ItemNo].ItemCount - _DropCount, 0);
	m_AllItemData[_ItemNo].ItemCount = m_Inventory[_ItemNo].ItemCount;

	return true;
}
const size_t CInventoryMgr::GetGold()
{
	return m_TotalGold;
}

void CInventoryMgr::SetMouseCursor(SPTR<CMouseCursor> _Cursor)
{
	m_InvenWindow->SetMouseCursor(_Cursor);
}