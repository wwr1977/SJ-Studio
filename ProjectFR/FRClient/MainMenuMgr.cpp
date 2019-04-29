#include "Precom.h"
#include "MainMenuMgr.h"
#include "MenuBase.h"
#include "MainSceneBuilder.h"
#include "RootMenu.h"
#include "BattlePositionMenu.h"
#include "CharacterStatusMenu.h"
#include "CharacterEquipmentMenu.h"
#include "CharacterSkillMenu.h"
#include "QuestMenu.h"
#include "Player.h"
#include "BattlePositionUI.h"
#include "InventoryMgr.h"

bool	CMainMenuMgr::bMenuOn = true;

CMainMenuMgr::CMainMenuMgr()
	:m_RootMenu(nullptr), m_CurMenu(nullptr)
	, m_pMainSceneBuilder(nullptr)
{
	m_vecMainMenu.clear();
	m_vecPositionUI.clear();
}


CMainMenuMgr::~CMainMenuMgr()
{
	m_vecMainMenu.clear();
	m_vecPositionUI.clear();
}
void CMainMenuMgr::Init()
{
	

}
void CMainMenuMgr::InitData(void* _Builder)
{
	if (nullptr != _Builder)
		m_pMainSceneBuilder =  (CMainSceneBuilder*)_Builder;
}
void CMainMenuMgr::Update()
{
	if(nullptr != m_CurMenu)
		m_CurMenu->MenuUpdate();
}
void CMainMenuMgr::CreateMainMenu()
{
	if (nullptr == m_pMainSceneBuilder)
		return;

	m_RootMenu = m_pMainSceneBuilder->CreateActObject(_T("RootMenu"))->CreateCom<CRootMenu>(this);
	m_vecMainMenu.push_back(m_pMainSceneBuilder->CreateActObject(_T("PositionMenu"))->CreateCom<CBattlePositionMenu>(this));
	m_vecMainMenu.push_back(m_pMainSceneBuilder->CreateActObject(_T("StatusMenu"))->CreateCom<CCharacterStatusMenu>(this));
	m_vecMainMenu.push_back(m_pMainSceneBuilder->CreateActObject(_T("EquipmentMenu"))->CreateCom<CCharacterEquipmentMenu>(this));
	m_vecMainMenu.push_back(m_pMainSceneBuilder->CreateActObject(_T("SkillMenu"))->CreateCom<CCharacterSkillMenu>(this));
	m_vecMainMenu.push_back(m_pMainSceneBuilder->CreateActObject(_T("QuestMenu"))->CreateCom<CQuestMenu>(this));

	m_CurMenu = m_RootMenu;

	
	for (size_t i = 0; i < MAXBATTLECHARACTER; i++)
	{
		m_vecPositionUI.push_back(m_pMainSceneBuilder->CreateActObject(_T("BattlePositionUI"))->CreateCom<CBattlePositionUI>((CharacterIndex)i));
		m_vecPositionUI[i]->UIOff();
	}
}
void CMainMenuMgr::ChangeRootMenu()
{
	if (nullptr != m_CurMenu)
		m_CurMenu->MenuOff();

	m_CurMenu = m_RootMenu;

	if (nullptr != m_CurMenu)
		m_CurMenu->MenuOn();
}
void CMainMenuMgr::ChangeMainMenu(const UINT& _MenuIndex)
{
	if (m_vecMainMenu.size() <= _MenuIndex || nullptr == m_vecMainMenu[_MenuIndex])
		return;

	if (nullptr != m_CurMenu)
		m_CurMenu->MenuOff();

	m_CurMenu = m_vecMainMenu[_MenuIndex];
	
	if (nullptr != m_CurMenu)
		m_CurMenu->MenuOn();

}
void CMainMenuMgr::MenuMgrOn()
{
	ACTOR->On();
	m_RootMenu->MenuOn();

	m_CurMenu = m_RootMenu;
	//m_CurMenu = nullptr;
	CClientGlobal::MainPlayer->PlayerPause(true);
	CClientGlobal::InventoryMgr->ActiveInventoryPopUp(false);
}
void CMainMenuMgr::MenuMgrOff() 
{
	ACTOR->Off();

	m_RootMenu->MenuOff();
	
	if (nullptr != m_CurMenu) 
	{
		m_CurMenu->Off();
		m_CurMenu = nullptr;
	}

	CClientGlobal::MainPlayer->PlayerPause(false);
	CClientGlobal::InventoryMgr->ActiveInventoryPopUp(true);
}
const bool CMainMenuMgr::IsMenuActive()
{
	return ACTOR->IsActive();
}
void CMainMenuMgr::BattlePositionUIAllOff() 
{
	for (size_t i = 0; i < m_vecPositionUI.size(); i++) 
	{
		m_vecPositionUI[i]->UIOff();
		m_vecPositionUI[i]->SelectUIOff();
	}
}
SPTR<CBattlePositionUI> CMainMenuMgr::BattlePositionUIOn(const CharacterIndex& _Index, const Vec3& _Pos, const UIRENDERINGVERSION& _Ver)
{
	if (_Index >= m_vecPositionUI.size())
		return nullptr;

	m_vecPositionUI[_Index]->UIOn(_Pos);
	m_vecPositionUI[_Index]->ChangeRenderingVersion(_Ver);
	return m_vecPositionUI[_Index];
}
