#pragma once
#include<Logic.h>


class CMainMenuMgr : public CLogic
{
	enum 
	{
		POSITION_MENU,
		CHRACTERSTATUS_MENU,
		ITEM_MENU,
		SKILL_MENU,
		QUEST_MENU,
		MAXMENU
	};
public:
	static bool			bMenuOn;
private: 
	friend class CGameMaster;
private:
	class CMainSceneBuilder*				m_pMainSceneBuilder;
	SPTR<class CMenuBase>					m_RootMenu;
	SPTR<CMenuBase>							m_CurMenu;
	vector<SPTR<CMenuBase>>					m_vecMainMenu;
	bool									m_bMgrOn;
	vector<SPTR<class CBattlePositionUI>>	m_vecPositionUI;



public:
	void Init();
	void InitData(void* _Builder);
	void Update();
	
	

public:
	void CreateMainMenu();
	void ChangeRootMenu();
	void ChangeMainMenu(const UINT& _MenuIndex);


public:
	void MenuMgrOn();
	void MenuMgrOff();
	const bool IsMenuActive();

public:
	void BattlePositionUIAllOff();
	SPTR<CBattlePositionUI> BattlePositionUIOn(const CharacterIndex& _Index, const Vec3& _Pos,const  UIRENDERINGVERSION& _Ver);
	
public:
	CMainMenuMgr();
	~CMainMenuMgr();
};

