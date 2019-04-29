#pragma once
#include "MenuBase.h"

class CCharacterSkillMenu : public CMenuBase
{
	enum TABINDEX
	{
		CHARACTERLINE_TAB,
		CHARACTER_TAB,
		SKILL_TAB,
		MAXTABINDEX
	}m_CurTab;

	enum PAGEEVENT
	{
		PAGE_UP,
		PAGE_DOWN,
		MAXPAGEEVENT
	};

private:
	SPTR<CFixRenderer>						m_MenuMainBack;
	SPTR<CFixRenderer>						m_MenuSubBack;

	vector<SPTR<class CMenuTab>>			m_CharacterLineTab;
	vector<SPTR<CMenuTab>>					m_ActiveLineTab;
	vector<Vec3>							m_CharLineTabPos;
	int										m_CurLineTabIndex;

	vector<SPTR<class CCharStatusTab>>		m_CharacterIcon;
	vector<SPTR<CCharStatusTab>>			m_ActiveIcon;
	vector<Vec3>							m_CharacterIconPos;
	int										m_CurCharIconIndex;

	vector<SPTR<class CMenuSkillUI>>		m_vecCharacterSkillUI;
	vector<SPTR<CMenuSkillUI>>				m_ActiveSkillUI;
	size_t									m_MaxSkillIndex;
	size_t									m_MaxUICount;
	vector<Vec3>							m_SkillUIPos;
	int										m_CurSkillUIIndex;
	int										m_CurSkillUIPage;

	SPTR<class CSkillDetailUI>				m_SkillDetailUI;

	SPTR<class CSkillLevelUpUI>				m_SkillLevelUpUI;
	bool									m_bUpdate;
	
public:
	void Init();
	void Update();
	void MenuOnEvent();

public:
	void CharacterLineTabUpdate();
	void CharacterTabUpdate();
	void CharacterSkillTabUpdate();

	void ChangeCharacterIcon(const size_t& _LineIndex);
	void ChangeSkillUI(const CharacterIndex& _CharNo);
	const bool CheckChangePage(const PAGEEVENT& _Op ,const int _JumpIndex );
	void ChangeSkillPage(const PAGEEVENT& _Op, const int _JumpIndex);
	
	void AllCharacterIconOff();
	void AllSkillUIOff();
	void ActiveSkillUIOff();
	void ReturnCharLineSelect();
	void ReturnCharacterSelect();

public:
	CMenuSkillUI* CreateSkillUI(const size_t& _SkillNo);


public:
	const bool PrevLineTab();
	const bool NextLineTab();
	const bool PrevCharacterTab();
	const bool NextCharacterTab();
	const bool PrevSkillTab(const int& _JumpIndex = 1);
	const bool NextSkillTab(const int& _JumpIndex = 1);


public:
	void HideSkillMenu(const bool& _Hide);
	void CurSkillLevelUpEvent(const bool& _LevelUp);

public:
	void MenuActorOn();
	void MenuActorOff();

public:
	const size_t GetLineIndex(SPTR<CMenuTab> _Tab);

public:
	CCharacterSkillMenu();
	~CCharacterSkillMenu();
};

