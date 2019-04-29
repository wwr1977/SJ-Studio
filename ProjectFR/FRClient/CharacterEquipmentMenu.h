#pragma once
#include "MenuBase.h"
class CCharacterEquipmentMenu : public CMenuBase
{
	enum TABINDEX
	{
		SELECT_CHARACTER,
		SELECT_CURRENT_EQUIPITEM,
		SELECT_CHANGE_EQUIPITEM,
		MAXSELECT
	}m_CurTab;

	enum PAGEEVENT
	{
		PAGE_UP,
		PAGE_DOWN,
		MAXPAGEEVENT,
	};

	enum 
	{
		MAXCHARCTERTAB = 15,
		MAXSELECTITEMTAB = 4,
	};

	enum 
	{
		CHARDATA_NAME,
		CHARDATA_LV,
		CHARDATA_STR,
		CHARDATA_ITEMSTR,
		CHARDATA_DEX,
		CHARDATA_ITEMDEX,
		CHARDATA_INT,
		CHARDATA_ITEMINT,
		CHARDATA_LUK,
		CHARDATA_ITEMLUK,
		CHARDATA_HP,
		CHARDATA_ITEMHP,
		CHARDATA_MP,
		CHARDATA_ITEMMP,
		CHARDATA_DEM,
		CHARDATA_ITEMDEM,
		CHARDATA_DEF,
		CHARDATA_ITEMDEF,
		CHARDATA_CRI,
		CHARDATA_ITEMCRI,
		CHARDATA_CRIDEM,
		CHARDATA_ITEMCRIDEM,
		CHARDATA_AVOID,
		CHARDATA_ITEMAVOID,
		MAXCHARDATA,
	};

	enum 
	{
		ITEMDATA_NAME,
		ITEMDATA_LV,
		ITEMDATA_STR,
		ITEMDATA_DIFFSTR,
		ITEMDATA_DEX,
		ITEMDATA_DIFFDEX,
		ITEMDATA_INT,
		ITEMDATA_DIFFINT,
		ITEMDATA_LUK,
		ITEMDATA_DIFFLUK,
		ITEMDATA_HP,
		ITEMDATA_DIFFHP,
		ITEMDATA_MP,
		ITEMDATA_DIFFMP,
		ITEMDATA_DEM,
		ITEMDATA_DIFFDEM,
		ITEMDATA_DEF,
		ITEMDATA_DIFFDEF,
		ITEMDATA_CRI,
		ITEMDATA_DIFFCRI,
		ITEMDATA_CRIDEM,
		ITEMDATA_DIFFCRIDEM,
		ITEMDATA_AVOID,
		ITEMDATA_DIFFAVOID,
		MAXITEMDATA,
	};
private:
	SPTR<CFixRenderer>						m_MenuMainBack;
	SPTR<CFixRenderer>						m_EquipTab;

	vector<SPTR<class CCharacterEquipTab>>	m_vecSimpleEquipTab;
	vector<SPTR<CCharacterEquipTab>>		m_vecActiveEquipTab;
	vector<Vec3>							m_vecEquipTabPos;
	int										m_CurCharacterPage;
	int										m_CurCharacterIndex;

private:
	CharacterIndex							m_CurCharacterNo;
	STAT									m_CurItemStat;
	vector<tstring>							m_vecCharDataString;
	SPTR<CFixRenderer>						m_CharacterIconRen;

private:
	vector<SPTR<class CEquipItemTab>>		m_CurEquipItemTab;
	int										m_CurEquipTabIndex;
	
	vector<SPTR<CEquipItemTab>>				m_SelectEquipItemTab;
	int										m_CurSelectEquipPage;
	int										m_CurSelectEquipIndex;
	vector<ItemData>						m_SelectEquipItemList;

private:
	vector<tstring>							m_ItemDataString;
	SPTR<CScriptRenderer>					m_ItemDetailRen;
	STAT									m_ItemDiffStat;
	ITEM									m_SelectItem;
	SPTR<class CMountItemFailGuide>			m_MountItemFailGuide;

public:
	void Init();
	void Update();
	void MenuOnEvent();
	void UIFontRender();

public:
	void ChracterDataRender();
	void ItemDataRender();
	void ChangeCharacterData(const CharacterIndex& _CharNo);
	void ChangeItemData(const size_t& _ItemNo);

public:
	void SelectCharacter();
	void SelectCurEquipItem();
	void SelectChangeEquipItem();

public:
	void ChangeMenuBack(const tstring& _BackSpriteName);

public:
	const bool PrevCharTab(const int& _JumpIndex = 1);
	const bool NextCharTab(const int& _JumpIndex = 1);
	const bool CheckChangeCharPage(const PAGEEVENT& _Evnet,const int& _JumpIndex);
	void ChangeCharPage(const PAGEEVENT& _Evnet, const int& _JumpIndex);

	const bool ChangeCurEquipTab(const int& _JumpIndex);

	const bool PrevSelectEquipTab();
	const bool NextSelectEquipTab();
	const bool CheckChangeSelectEquipTabPage(const PAGEEVENT& _Op);
	void ChangeSelectEquipTabPage(const PAGEEVENT& _Op);

	const bool ChangeEquipItemEvent();

	// 장비가 교체되었을때 현재 캐릭터의 스탯수치와
	// 현재 장비 탭의 정보를 교체 해야한다.
	void SuccessChangeEquipItem();
	void FailChangeEquipItem();

public:
	void CurActiveCharEquipOn();
	void AllActiveCharEquipOff();
	void CurEquiptItemTabOn(const bool& _On);
	void SelectEquipItemTabOn(const bool& _On);
public:
	void ChangeSelectItemList(const size_t& _Op);
	
public:
	CCharacterEquipmentMenu();
	~CCharacterEquipmentMenu();
};

