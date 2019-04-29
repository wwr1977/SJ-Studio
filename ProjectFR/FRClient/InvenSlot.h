#pragma once
#include "MenuTab.h"

class CInvenSlot : public CMenuTab
{
private:
	static class CInventory*				pInventory;

public:
	enum 
	{
		ITEMCOUNT,
	};
	static void SetInventory(CInventory* _Inven);

private:
	SPTR<CFixRenderer>				m_ItemIconRen;
	SPTR<CCollision>				m_InvenSlotCol;
	size_t							m_ItemNo;
	size_t							m_CurItemCount;
	bool							m_bClick;



public:
	void Init();
	void Update();

public:
	void InvenSlotEnter(SPTR<CCollision> _This, SPTR<CCollision> _Other);
	void InvenSlotStay(SPTR<CCollision> _This, SPTR<CCollision> _Other);
	void InvenSlotExit(SPTR<CCollision> _This, SPTR<CCollision> _Other);


public:
	void SetInvenSlot(const size_t& _ItemNo);
	const size_t UpdateInvenItemCount();

public:
	const size_t GetCurItemCount();
	const size_t GetItemNo();
public:
	CInvenSlot();
	~CInvenSlot();
};

