#include "Precom.h"
#include "RootMenu.h"
#include "MainMenuMgr.h"
#include "MenuTab.h"
#include "GameMaster.h"
#include "BattlePositionUI.h"

CRootMenu::CRootMenu()
	:m_MainBackRen(nullptr), m_SubBackRen(nullptr)
	,m_GoldBackRen(nullptr), m_CurTabIndex(0)
{
	m_vecRootTab.clear();
	m_vecUIPosition.clear();
}


CRootMenu::~CRootMenu()
{
	m_vecRootTab.clear();
	m_vecUIPosition.clear();
}
void CRootMenu::Init()
{
	MenuOff();

	m_MainBackRen = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_UIBACK, 0.9f, true));
	m_MainBackRen->SetSprite(_T("MenuMainBack"));
	Vec2 Pivot = m_MainBackRen->GetCurSpriteSize();
	m_MainBackRen->SetRenPivot(Vec2{ Pivot.x*0.17f ,0.0f });


	m_SubBackRen = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_UIBACK, 0.9f, true));
	m_SubBackRen->SetSprite(_T("MenuSubBack"));
	m_SubBackRen->SetRenPivot(Vec2{ -450.f,120.f });

	m_GoldBackRen = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_UIBACK, 0.9f, true));
	m_GoldBackRen->SetSprite(_T("RootGoldBack"));
	m_GoldBackRen->SetRenPivot(Vec2{ -450.0f,-334.f });

	m_vecRootTab.push_back(ACTOR->CreateCom<CMenuTab>());	// BattlePosition
	m_vecRootTab.push_back(ACTOR->CreateCom<CMenuTab>());	// Character Status
	m_vecRootTab.push_back(ACTOR->CreateCom<CMenuTab>());	// Character Equip
	m_vecRootTab.push_back(ACTOR->CreateCom<CMenuTab>());	// Character Skill
	m_vecRootTab.push_back(ACTOR->CreateCom<CMenuTab>());	// Quest 

	for (size_t i = 0; i < m_vecRootTab.size(); i++)
	{
		m_vecRootTab[i]->SetTabStyle(_T("메뉴탭"));
		m_vecRootTab[i]->SetTabSize(Vec2{ 340.f,40.f });
	}

	m_vecRootTab[0]->SetTabName(_T("진영"));
	m_vecRootTab[0]->SetPivot(Vec3{ -450.f, 310.f,0.f});

	m_vecRootTab[1]->SetTabName(_T("스테이터스"));
	m_vecRootTab[1]->SetPivot(Vec3{ -450.f, 250.f,0.f });

	m_vecRootTab[2]->SetTabName(_T("장비"));
	m_vecRootTab[2]->SetPivot(Vec3{ -450.f, 190.f,0.f });

	m_vecRootTab[3]->SetTabName(_T("스킬"));
	m_vecRootTab[3]->SetPivot(Vec3{ -450.f, 130.f,0.f });

	m_vecRootTab[4]->SetTabName(_T("퀘스트"));
	m_vecRootTab[4]->SetPivot(Vec3{ -450.f, 70.f,0.f });


	m_vecRootTab[m_CurTabIndex]->SelectTab(true);

	m_vecUIPosition.push_back(Vec3{ -100.0f,15.f,5.f });
	m_vecUIPosition.push_back(Vec3{ -100.0f,-205.f,5.f });
	m_vecUIPosition.push_back(Vec3{ 300.0f,135.f,5.f });
	m_vecUIPosition.push_back(Vec3{ 300.0f,-65.f,5.f });
	m_vecUIPosition.push_back(Vec3{ 300.0f,-295.f,5.f });


}
void CRootMenu::Update()
{
	if (true == KEYDOWN("MenuKey")) 
	{
		m_pMenuMgr->BattlePositionUIAllOff();
		m_pMenuMgr->MenuMgrOff();
		
		if (nullptr != m_vecRootTab[m_CurTabIndex])
		m_vecRootTab[m_CurTabIndex]->SelectTab(false);

		m_CurTabIndex = 0;

		if (nullptr != m_vecRootTab[m_CurTabIndex])
		m_vecRootTab[m_CurTabIndex]->SelectTab(true);

		CClientGlobal::GameMaster->MenuOffEvnet();
		return;
	}

	if (true == KEYDOWN("UP"))
		PrevTab();
	else if (true == KEYDOWN("DOWN"))
		NextTab();
	else if (true == KEYDOWN("RIGHT"))
	{
		m_pMenuMgr->ChangeMainMenu(m_CurTabIndex);
	}
	else if (true == KEYDOWN("Enter")) 
	{
		m_pMenuMgr->ChangeMainMenu(m_CurTabIndex);
	}
}
void CRootMenu::MenuOnEvent()
{
	if (nullptr == m_pMenuMgr)
		return;

	m_pMenuMgr->BattlePositionUIAllOff();

	for (size_t i = 0; i < MAXTEAM; i++)
	{
		CharacterIndex CharIndex = (CharacterIndex)CClientGlobal::GameMaster->GetCurTeam(i);
		
		if (CharIndex >= MAXBATTLECHARACTER)
			continue;
		
		m_pMenuMgr->BattlePositionUIOn(CharIndex,m_vecUIPosition[i],ROOTMENEVER);
	}

	/*if (nullptr != m_vecRootTab[m_CurTabIndex])
		m_vecRootTab[m_CurTabIndex]->SelectTab(false);

	m_CurTabIndex = 0;
	
	if (nullptr != m_vecRootTab[m_CurTabIndex])
		m_vecRootTab[m_CurTabIndex]->SelectTab(true);*/

}
void CRootMenu::PrevTab()
{
	m_vecRootTab[m_CurTabIndex]->SelectTab(false);

	if (--m_CurTabIndex < 0)
		m_CurTabIndex = (UINT)(m_vecRootTab.size() - 1);
	
	m_vecRootTab[m_CurTabIndex]->SelectTab(true);

}
void CRootMenu::NextTab()
{
	m_vecRootTab[m_CurTabIndex]->SelectTab(false);

	if (++m_CurTabIndex >= m_vecRootTab.size() )
		m_CurTabIndex = 0;

	m_vecRootTab[m_CurTabIndex]->SelectTab(true);
}