#pragma once
#include <Logic.h>


class CStoreUI : public CLogic
{
	enum STORETABINDEX
	{
		WEAPON_TAB,
		ARMOR_TAB,
		ACCESSORIES_TAB,
		ETC_TAB,
		SALE_TAB,
		MAXSTORETAB
	};

	enum 
	{
		SELECT_STORE_TAB ,
		SELECT_ITEM_TAB ,
	}m_CurSelectTab;

	enum 
	{
		MAXITEMTAB = 10,
	};

private:
	STORETYPE								m_CurStoreType;
	SPTR<CFixRenderer>						m_GoldFrameRen;
	SPTR<CFixRenderer>						m_StoreTabFrameRen;
	SPTR<CFixRenderer>						m_ItemListFrameRen;
	SPTR<CFixRenderer>						m_ItemDetailFrameRen;
	SPTR<CFontRenderer>						m_InvenGoldRen;
	
	vector<tstring>							m_StoreTabString;
	vector<Vec3>							m_StoreTabPivot;
	vector<SPTR<class CMenuTab>>			m_vecStoreMenuTab;
	vector<SPTR<CMenuTab>>					m_ActiveStoreTab;
	size_t									m_CurStoreIndex;

	vector<Vec3>							m_ItemTabPivot;
	vector<SPTR<class CItemTab>>			m_vecItemTab;
	vector<vector<SPTR<CItemTab>>>			m_ActiveItemTab;
	size_t									m_CurPage;
	size_t									m_CurItemIndex;

	size_t									m_InventoryGold;
	SPTR<CScriptRenderer>					m_ItemDetailFontRen;

	bool									m_bKeyCheck;

private:
	SPTR<class CStoreSubUI>					m_StoreSubUI;

public:
	void Init();
	void Update();

public:
	void StoreUpdate();
	void UpdateInvenGold();
	void UpdateItemDetail();
	

public:
	void StoreTabUpdate();
	void ItemTabUpdate();
	void StandBy(const STORETYPE& _StoreType);

public:
	bool PrevStoreTab();
	bool NextStoreTab();
	bool PrevItemTab();
	bool NextItemTab();
	bool ChangeItemListPage(const bool& _NextPage);


public:
	void AllStoreTabActive(const bool& _Active);
	void AllItemTabActive(const bool& _Active);
	void SettingStore(const STORETYPE& _StoreType);
	void ChangeItemListEvent();
	void SelectItemTabEvent();
	void PurchaseItemEvent();
	void SaleItemEvent();

public:
	void SetKeyCheck(const bool& _Check);

	
private:
	void ChangeItemList(const  STORETABINDEX& _Index);
	void PushActiveItemTab(list<ItemData>* _ItemList,const STORETABINDEX& _Index);
	// 판매 아이템 리스트 셋팅
	//void SettingSalingItem();	

public:
	void OpenStoreUI();
	void CloseStoreUI();


public:
	CStoreUI();
	~CStoreUI();
};

