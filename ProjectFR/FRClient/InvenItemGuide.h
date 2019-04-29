#pragma once
#include <Logic.h>
class CInvenItemGuide : public CLogic
{
	enum 
	{
		ITEM_DEMAGE,
		ITEM_DEFENSE,
		ITEM_STR,
		ITEM_DEX,
		ITEM_INT,
		ITEM_LUCK,
		ITEM_HP,
		ITEM_MP,
		ITEM_CRI,
		ITEM_CRIDEM,
		ITEM_AVOID,
		MAXITEMDATA,
	};

private:
	class CInventory*					m_pInventory;
	SPTR<CFixRenderer>					m_GuideBackRen;
	SPTR<CFixRenderer>					m_ItemIconRen;
	SPTR<CFontRenderer>					m_ItemNameRen;
	SPTR<CFontRenderer>					m_ItemLevelRen;
	SPTR<CScriptRenderer>				m_ItemDescriptionRen;

	vector<SPTR<CFontRenderer>>			m_vecItemStatRen;
	
public:
	void SetInventory(CInventory* _pInven);

public:
	void Init();
	void Update();
	void UIFontRender();

public:
	void SetItemData(const size_t& _ItemNo);

public:
	void GuideActiveEvent(const Vec3& _Pos);

public:
	CInvenItemGuide();
	~CInvenItemGuide();
};

