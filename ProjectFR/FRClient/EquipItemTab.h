#pragma once
#include <Logic.h>

class CEquipItemTab : public CLogic
{

private:
	SPTR<CFixRenderer>				m_ItemBackRen;
	SPTR<CFixRenderer>				m_ItemIconRen;
	SPTR<CFontRenderer>				m_ItemNameRen;
	SPTR<CFontRenderer>				m_ItemCountRen;

	size_t							m_CurItemNo;
	ItemData						m_CurItemData;
	bool							m_bItemCountRen;


public:
	void Init();

public:
	void ChangeItemData(const ItemData& _Data);
	void ChangeItemData(const size_t& _ItemNo);
	void SetNoItemTab();
	void SetEquipReleaseTab();
	const size_t GetCurItemNo();
public:
	void SelectTab(const bool& _Select);
	void ItemCountRender(const bool& _Render);
public:
	CEquipItemTab();
	~CEquipItemTab();
};

