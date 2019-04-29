#pragma once
#include <Logic.h>

class ItemIcon 
{
public:
	SPTR<CFixRenderer>				m_ItemIconRen;
	tstring							m_ItemName;

public:
	void operator=(const ItemIcon& _Other) 
	{
		if (nullptr == _Other.m_ItemIconRen)
			return;

		m_ItemIconRen = _Other.m_ItemIconRen;
		m_ItemName = _Other.m_ItemName;
	}

	ItemIcon() :m_ItemIconRen(nullptr), m_ItemName(_T(""))
	{}
	ItemIcon(SPTR<CFixRenderer>	_Ren)
		:m_ItemIconRen(_Ren), m_ItemName(_T(""))
	{}
	ItemIcon(const ItemIcon& _Other) 
	:m_ItemIconRen(_Other.m_ItemIconRen), m_ItemName(_Other.m_ItemName)
	{}
	~ItemIcon() 
	{
		m_ItemIconRen = nullptr;

	}
};
class CCharacterEquipTab : public CLogic
{
	static D3DXCOLOR					FocusColor;
	static D3DXCOLOR					NoFocusColor;
private:
	CharacterIndex						m_CharacterNo;
	SPTR<CFixRenderer>					m_TabBack;
	SPTR<CFixRenderer>					m_TabFrame;
	SPTR<CFixRenderer>					m_CharacterRen;
	vector<ItemIcon>					m_vecItemIcon;

public:
	void Init();
	void InitData(const CharacterIndex& _Index);
	void UIFontRender();

public:
	void SelectTab(const bool& _Select);
	void UpdateEquipTab();
public:
	void TabOn(const Vec3& _Pos);

public:
	const CharacterIndex GetCharacterNo();

public:
	CCharacterEquipTab();
	~CCharacterEquipTab();
};

