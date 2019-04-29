#pragma once
#include <ObjBase.h>


class CInventoryMgr : public CObjBase
{
public:
	enum  INVENMGRSTATE
	{
		INVENTORY_WAIT,
		INVENTORY_ON,
		UPDATE_PAUSE,
		MAXUPDATESTATE
	}m_CurUpdateState;

private:
	class CGameMaster*				m_pGameMaster;
	SPTR<class CInventory>			m_InvenWindow;

	// AllItemData Item Count = Inventory Item Count + Equip Item Count ;
 	// ���� ������ ��� ������ ������
	vector<ItemData>				m_AllItemData;
	// �κ��丮(�������� ���� ������)�ȿ� �����ϴ� ������
	vector<ItemData>				m_Inventory;
	// ���� �������� �������� ����Ʈ
	vector<ItemData>				m_EquipItemList;

	size_t							m_TotalGold;
public:
	void Init();
	void Update();

public:
	void ActiveInventoryPopUp(const bool& _PopUp);
	void OpenInventoryEvent();
	void CloseInventoryEvent();


public:
	void RequestItemList(list<ItemData>* _ItemList, const ITEMTYPE& _ItemType);
	void RequestItemList(list<ItemData>* _ItemList, const ITEMCATEGORY& _Category, const CharacterIndex& _CharNo );
	void RequestItemList(vector<ItemData>* _ItemList, const ITEMCATEGORY& _Category, const CharacterIndex& _CharNo);

	void RequestStoreItemList(list<ItemData>* _ItemList, const ITEMCATEGORY& _Category);
	void RequestInventoryItemList(list<ItemData>* _ItemList);
	const size_t RequestInventoryItemCount(const size_t& _ItemNo);
	
public:
	void MountEquipItem(const size_t& _ItemNo);
	void ReleaseEquipItem(const size_t& _ItemNo);


public:
	bool PurchaseItem(const size_t& _ItemNo, const size_t& _PurchaseCount);
	bool SaleItem(const size_t& _ItemNo, const size_t& _SaleCount);
	bool DropItem(const size_t& _ItemNo, const size_t& _DropCount = 1);

	const size_t GetGold();

public:
	void SetMouseCursor(SPTR<CMouseCursor> _Cursor);

public:
	CInventoryMgr();
	CInventoryMgr(CGameMaster* _Master);
	~CInventoryMgr();
};

