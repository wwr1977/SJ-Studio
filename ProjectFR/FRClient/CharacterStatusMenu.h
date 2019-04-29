#pragma once
#include "MenuBase.h"

class CCharacterStatusMenu : public CMenuBase
{
	enum TABINDEX
	{
		CHARACTERLINE_TAB,
		CHARACTER_TAB,
		MAXTABINDEX
	}m_CurTab;

private:
	SPTR<CFixRenderer>						m_MenuMainBack;
	SPTR<CFixRenderer>						m_MenuSubBack;

	vector<SPTR<class CMenuTab>>			m_CharacterLineTab;
	vector<SPTR< CMenuTab>>					m_ActiveLineTab;
	vector<Vec3>							m_CharLineTabPos;
	int										m_CurLineTabIndex;

	vector<SPTR<class CCharStatusTab>>		m_CharacterIcon;
	vector<SPTR<CCharStatusTab>>			m_ActiveIcon;
	vector<Vec3>							m_CharacterIconPos;
	int										m_CurCharIconIndex;


	SPTR<class CCharacterStatus>			m_Status;

public:
	void Init();
	void Update();
	void MenuOnEvent();

public:
	void CharacterLineTabUpdate();
	void CharacterTabUpdate();
	void ChangeCharacterIcon(const size_t& _LineIndex);
	void AllCharacterIconOff();
	void ReturnCharLineSelect();


public:
	const bool PrevLineTab();
	const bool NextLineTab();
	const bool PrevCharacterTab();
	const bool NextCharacterTab();


public:
	const size_t GetLineIndex(SPTR<CMenuTab> _Tab);
	const size_t GetCurLineIndex();
public:
	CCharacterStatusMenu();
	~CCharacterStatusMenu();
};

