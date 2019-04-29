#pragma once
#include "MenuTab.h"

class CItemTab : public CMenuTab
{
public:
	enum TABMODE 
	{
		PURCHASE_MODE,			// 구매 아이템 탭
		SALE_MODE,				// 판매 아이템 탭
		INVENTORY_MODE,			// 인벤토리 아이템 탭
		MAXTABMODE,
	}m_CurMode;

	enum 
	{
		SUB_PRICE,
		MAXSUBFONT,

	};

private:
	SPTR<CFixRenderer>			m_ItemRen;
	size_t						m_ItemNo;
	size_t						m_ItemPrice;		// 구매 가격
	size_t						m_ItemSalePrice;	// 판매 가격
	size_t						m_CurItemCount;

public:
	void Init();
	void InitData(const TABMODE& _Mode);
	void Update();

public:
	void UpdateItemCount();

public:
	void InitStoreItemTab();
	void InitInventoryItemTab();

public:
	void SetItemTab(const size_t& _ItemNo);
	void ChangeTabMode(const TABMODE& _Mode);
	void SetCurItemCount(const size_t& _Count);

public:
	const size_t GetItemPrice();
	const size_t GetItemSalePrice();
	const size_t GetCurItemCount();
	const size_t GetItemNo();
public:
	CItemTab();
	~CItemTab();
};

