#include "Precom.h"
#include "BattlePositionMenu.h"
#include "MenuTab.h"
#include "MainMenuMgr.h"
#include "GameMaster.h"
#include "BattlePositionUI.h"


CBattlePositionMenu::CBattlePositionMenu()
	:m_SplitterLineRen(nullptr), m_MainBackRen(nullptr)
	, m_SubBackRen(nullptr), m_PositionTabIndex(0)
	, m_CurCharacterTabIndex(0)
	, m_CurTab(POSITION_TAB)
	, m_CurCharacterPage(0)
	, m_NoPositionTab(nullptr)
	, m_NoPositionGuide(nullptr)
	, m_GuidePivot(Vec2{ 10.f, 50.f })
{
}


CBattlePositionMenu::~CBattlePositionMenu()
{
	m_CharacterTab.clear();
	m_ActiveCharTab.clear();
}

void CBattlePositionMenu::Init()
{
	MenuOff();

	m_SplitterLineRen = ACTOR->CreateCom<CLineRenderer>(RENDATA(LAYER_UI, 0.8f, true));
	m_SplitterLineRen->SetSprite(_T("ColorBase"));
	m_SplitterLineRen->SetColor(D3DXCOLOR(0.85f, 0.85f, 0.85f, 1.0f));
	m_SplitterLineRen->SettingLineData(Vec3{ 100.f,330.0f,5.0f }, Vec3{ 100.0f,-330.0f,5.0f });


	m_MainBackRen = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_UIBACK, 0.9f, true));
	m_MainBackRen->SetSprite(_T("MenuMainBack"));
	Vec2 Pivot = m_MainBackRen->GetCurSpriteSize();
	m_MainBackRen->SetRenPivot(Vec2{ Pivot.x*0.17f ,0.0f });


	m_SubBackRen = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_UIBACK, 0.9f, true));
	m_SubBackRen->SetSprite(_T("MenuSubBack"));
	m_SubBackRen->SetRenPivot(Vec2{ -450.f,120.f });


	m_PositionTab.push_back(ACTOR->CreateCom<CMenuTab>());	// 1 번 자리
	m_PositionTab.push_back(ACTOR->CreateCom<CMenuTab>());	// 2 번 자리
	m_PositionTab.push_back(ACTOR->CreateCom<CMenuTab>());	// 3 번 자리
	m_PositionTab.push_back(ACTOR->CreateCom<CMenuTab>());	// 4 번 자리
	m_PositionTab.push_back(ACTOR->CreateCom<CMenuTab>());	// 5 번 자리

	for (size_t i = 0; i < m_PositionTab.size(); i++)
	{
		m_PositionTab[i]->SetTabStyle(_T("메뉴탭"));
		m_PositionTab[i]->SetTabSize(Vec2{ 340.f,40.f });
	}

	m_PositionTab[0]->SetTabName(_T("전방 1"));
	m_PositionTab[0]->SetPivot(Vec3{ -450.f, 310.f,0.f });

	m_PositionTab[1]->SetTabName(_T("전방 2"));
	m_PositionTab[1]->SetPivot(Vec3{ -450.f, 250.f,0.f });

	m_PositionTab[2]->SetTabName(_T("후방 1"));
	m_PositionTab[2]->SetPivot(Vec3{ -450.f, 190.f,0.f });

	m_PositionTab[3]->SetTabName(_T("후방 2"));
	m_PositionTab[3]->SetPivot(Vec3{ -450.f, 130.f,0.f });

	m_PositionTab[4]->SetTabName(_T("후방 3"));
	m_PositionTab[4]->SetPivot(Vec3{ -450.f, 70.f,0.f });

	m_PositionTab[m_PositionTabIndex]->SelectTab(true);

	m_vecUIPosition.push_back(Vec3{ 275.0f,20.f,5.f });
	m_vecUIPosition.push_back(Vec3{ 275.0f,-170.f,5.f });
	m_vecUIPosition.push_back(Vec3{ 425.0f,120.f,5.f });
	m_vecUIPosition.push_back(Vec3{ 425.0f,-65.f,5.f });
	m_vecUIPosition.push_back(Vec3{ 425.0f,-250.f,5.f });

	for (size_t i = 0; i < 12; i++)
		m_vecCharTabPos.push_back(Vec3{ -80.f,300.f - 55*i,7.f });

	m_NoPositionGuide = ACTOR->CreateCom<CAniRenderer>(RENDATA(LAYER_UI, 1.0, { 0.f,0.f,0.f }, true));
	m_NoPositionGuide->CreateAnimation(_T("SelectCircle2"), _T("SelectCircle2"), ANI_LOOP);
	m_NoPositionGuide->ChangeAni(_T("SelectCircle2"));
	m_NoPositionGuide->SetRenPivot(m_GuidePivot);
	m_NoPositionGuide->Off();


	m_NoPositionTab = CActObject::CreateActObject(_T("NoPositionTab"), ACTOR->GetScene())->CreateCom<CMenuTab>();
	m_NoPositionTab->SetTabStyle(_T("작은메뉴탭"));
	m_NoPositionTab->SetTabName(_T("배치 해제"));
	m_NoPositionTab->SetTabSize(Vec2{ 300.f,40.f });
	m_NoPositionTab->SetPivot(POINT{ -150,-15 });
	m_NoPositionTab->TabActorOff();


	for (size_t i = 0; i < MAXBATTLECHARACTER; i++) 
	{
		m_CharacterTab.push_back(CActObject::CreateActObject(_T("CharacterTab"), ACTOR->GetScene())->CreateCom<CMenuTab>());
		m_CharacterTab[i]->SetTabStyle(_T("작은메뉴탭"));
		m_CharacterTab[i]->SetTabName(CClientGlobal::CharacterName[i].UnitKName);
		m_CharacterTab[i]->SetTabSize(Vec2{ 300.f,40.f });
		m_CharacterTab[i]->SetPivot(POINT{ -150,-15 });
		m_CharacterTab[i]->CreateSubTabFont(_T("메뉴레벨폰트"), POINT{150,-10},RIGHTSTART);

		m_CharacterTab[i]->TabActorOff();

	}


}
void CBattlePositionMenu::Update()
{

	switch (m_CurTab)
	{
	case CBattlePositionMenu::POSITION_TAB:
		PositionSelectUpdate();
		return;
	case CBattlePositionMenu::CHARACTER_TAB:
		CharacterSelectUpdate();
		return;
	}

	
}
void CBattlePositionMenu::MenuOnEvent()
{
	if (nullptr == m_pMenuMgr)
		return;

	m_pMenuMgr->BattlePositionUIAllOff();
	
	m_vecCurPositionUI.assign(MAXTEAM, nullptr);
	m_vecCharacterData.clear();


	for (size_t i = 0; i < MAXTEAM; i++)
	{
		CharacterIndex CharIndex = (CharacterIndex)CClientGlobal::GameMaster->GetCurTeam(i);

		if (CharIndex >= MAXBATTLECHARACTER)
			continue;

		m_vecCurPositionUI[i] = m_pMenuMgr->BattlePositionUIOn(CharIndex, m_vecUIPosition[i], BATTLEPOSITIONVER);
	}

	if(nullptr != m_vecCurPositionUI[m_PositionTabIndex])
		m_vecCurPositionUI[m_PositionTabIndex]->SelectUIOn();
	else
	{
		m_NoPositionGuide->SetRenPivot(m_GuidePivot + Vec2{ m_vecUIPosition[m_PositionTabIndex].x,m_vecUIPosition[m_PositionTabIndex].y });
		m_NoPositionGuide->On();
	}

	if (true == CClientGlobal::MasterMode)
		MasterModeOn();
	else
		NormalModeOn();

	m_CurTab = POSITION_TAB;
}
void CBattlePositionMenu::PositionSelectUpdate()
{
	if (true == KEYDOWN("MenuKey") || true == KEYDOWN("LEFT"))
	{
		m_pMenuMgr->ChangeRootMenu();
		return;
	}

	if (true == KEYDOWN("UP"))
		PrevMenuTab();
	else if (true == KEYDOWN("DOWN"))
		NextMenuTab();
	else if (true == KEYDOWN("RIGHT") || true == KEYDOWN("Enter"))
	{
		m_ActiveCharTab.clear();
		CharacterListUpdate();
		m_CurCharacterTabIndex = 0;
		m_CurCharacterPage = 0;
		m_CurTab = CHARACTER_TAB;
	}
}
void CBattlePositionMenu::CharacterSelectUpdate()
{
	if (true == KEYDOWN("MenuKey") || true == KEYDOWN("LEFT"))
	{
		ReturnPositionSelect();
		return;
	}

	else if (true == KEYDOWN("UP"))
		PrevCharTab();
	else if (true == KEYDOWN("DOWN"))
		NextCharTab();
	else if (true == KEYDOWN("Enter"))
		ChangeCharacterPositionEvent();
}
void CBattlePositionMenu::AllActiveTabOff()
{
	for (size_t i = 0; i < m_ActiveCharTab.size(); i++)
	{
		m_ActiveCharTab[i]->TabActorOff();
		m_ActiveCharTab[i]->SelectTab(false);
	}
}
void CBattlePositionMenu::CharacterListUpdate()
{
	m_ActiveCharTab.push_back(m_NoPositionTab);

	for (size_t i = 0; i < m_vecCharacterData.size(); i++)
	{
		size_t CharNo = m_vecCharacterData[i].CharacterNo;
		
		if (true == CClientGlobal::GameMaster->CheckCurBelongTeam(CharNo))
			continue;

		if (!CClientGlobal::MasterMode && !CClientGlobal::GameMaster->IsOpenCharacterLine((CharacterIndex)CharNo))
			continue;

		TCHAR Temp[MAXKEYLENGTH] = _T("");
		_stprintf_s(Temp,_T("Lv %3zd"), m_vecCharacterData[i].CharacterLv);
		size_t NewIndex = m_ActiveCharTab.size();
		m_ActiveCharTab.push_back(m_CharacterTab[CharNo]);
		
		m_ActiveCharTab[NewIndex]->SetSubFontString(0, Temp);
	}

	for (size_t i = 0; i < min(m_ActiveCharTab.size(),12); i++)
	{
		m_ActiveCharTab[i]->SetPos(m_vecCharTabPos[i]);
		m_ActiveCharTab[i]->TabActorOn();
	}

	m_ActiveCharTab[0]->SelectTab(true);
}
void CBattlePositionMenu::ReturnPositionSelect()
{
	m_NoPositionTab->TabActorOff();
	m_NoPositionTab->SelectTab(false);

	for (size_t i = 0; i < m_ActiveCharTab.size(); i++)
	{
		m_ActiveCharTab[i]->TabActorOff();
		m_ActiveCharTab[i]->SelectTab(false);
	}
	m_CurTab = POSITION_TAB;
}
void CBattlePositionMenu::NormalModeOn() 
{
	for (size_t j = 0; j < CClientGlobal::CharacterLine.size(); j++)
		m_vecCharacterData.push_back(CClientGlobal::GameMaster->GetCurCharData((CHARACTERLINE)j));
}
void CBattlePositionMenu::MasterModeOn() 
{
	for (size_t j = 0; j < CClientGlobal::CharacterLine.size(); j++)
	{
		for (size_t i = 0; i <  CClientGlobal::CharacterLine[j].size(); i++)
			m_vecCharacterData.push_back(CClientGlobal::CharacterLine[j][i]);
	}
}
void CBattlePositionMenu::PrevMenuTab() 
{
	m_PositionTab[m_PositionTabIndex]->SelectTab(false);

	if (nullptr != m_vecCurPositionUI[m_PositionTabIndex])
		m_vecCurPositionUI[m_PositionTabIndex]->SelectUIOff();
	else
		m_NoPositionGuide->Off();

	if (--m_PositionTabIndex < 0)
		m_PositionTabIndex = (UINT)(m_PositionTab.size() - 1);

	m_PositionTab[m_PositionTabIndex]->SelectTab(true);

	if (nullptr != m_vecCurPositionUI[m_PositionTabIndex])
		m_vecCurPositionUI[m_PositionTabIndex]->SelectUIOn();
	else 
	{
		m_NoPositionGuide->SetRenPivot(m_GuidePivot + Vec2{ m_vecUIPosition[m_PositionTabIndex].x,m_vecUIPosition[m_PositionTabIndex].y });
		m_NoPositionGuide->On();
	}

}
void CBattlePositionMenu::NextMenuTab() 
{
	m_PositionTab[m_PositionTabIndex]->SelectTab(false);
	
	if (nullptr != m_vecCurPositionUI[m_PositionTabIndex])
		m_vecCurPositionUI[m_PositionTabIndex]->SelectUIOff();
	else 
		m_NoPositionGuide->Off();

	if (++m_PositionTabIndex >= m_PositionTab.size())
		m_PositionTabIndex = 0;

	m_PositionTab[m_PositionTabIndex]->SelectTab(true);

	if (nullptr != m_vecCurPositionUI[m_PositionTabIndex])
		m_vecCurPositionUI[m_PositionTabIndex]->SelectUIOn();
	else
	{
		m_NoPositionGuide->SetRenPivot(m_GuidePivot + Vec2{ m_vecUIPosition[m_PositionTabIndex].x,m_vecUIPosition[m_PositionTabIndex].y });
		m_NoPositionGuide->On();
	}
}
void CBattlePositionMenu::PrevCharTab()
{

	if (0 >= m_CurCharacterTabIndex ) 
	{
		if(false == CheckChangePage(PAGE_UP_EVENT))
			return;

		ChangePage(PAGE_UP_EVENT);
		return;
	}

	int RealIndex = m_CurCharacterPage * 12 + m_CurCharacterTabIndex;

	if(nullptr != m_ActiveCharTab[RealIndex])
		m_ActiveCharTab[RealIndex]->SelectTab(false);

	--RealIndex;
	--m_CurCharacterTabIndex;

	if (nullptr != m_ActiveCharTab[RealIndex])
		m_ActiveCharTab[RealIndex]->SelectTab(true);

}
void CBattlePositionMenu::NextCharTab()
{
	int RealIndex = m_CurCharacterPage * 12 + m_CurCharacterTabIndex;
	
	if (RealIndex >= m_ActiveCharTab.size() - 1)
		return;

	if (11 <= m_CurCharacterTabIndex) 
	{
		if (false == CheckChangePage(PAGE_DOWN_EVENT))
			return;

		ChangePage(PAGE_DOWN_EVENT);
		return;
	}

	if (nullptr != m_ActiveCharTab[RealIndex])
		m_ActiveCharTab[RealIndex]->SelectTab(false);

	++RealIndex;
	++m_CurCharacterTabIndex;

	if (nullptr != m_ActiveCharTab[RealIndex])
		m_ActiveCharTab[RealIndex]->SelectTab(true);
}
const bool CBattlePositionMenu::CheckChangePage(const PAGEEVENT& _Event)
{
	switch (_Event)
	{
	case CBattlePositionMenu::PAGE_UP_EVENT: 
	{
		if (m_CurCharacterPage <= 0)
			return false;
	}
		return true;
	case CBattlePositionMenu::PAGE_DOWN_EVENT: 
	{
		if ( m_CurCharacterPage * 12 + 11 >= m_ActiveCharTab.size() - 1  )
			return false;
	}
		return true;
	}

	return false;
}
void CBattlePositionMenu::ChangePage(const PAGEEVENT& _Event)
{
	switch (_Event)
	{
	case CBattlePositionMenu::PAGE_UP_EVENT:
	{
		AllActiveTabOff();
		--m_CurCharacterPage;
		m_CurCharacterTabIndex = 11;
		int Index = m_CurCharacterPage * 12;
		int Count = 0;

		while (Count < 12) 
		{
			if (Index >= m_ActiveCharTab.size())
				break;

			m_ActiveCharTab[Index]->SetPos(m_vecCharTabPos[Count]);
			m_ActiveCharTab[Index]->TabActorOn();

			++Index;
			++Count;
		}

		m_ActiveCharTab[Index-1]->SelectTab(true);

	}
		return;
	case CBattlePositionMenu::PAGE_DOWN_EVENT: 
	{
		AllActiveTabOff();
		++m_CurCharacterPage;
		m_CurCharacterTabIndex = 0;
		int Index = m_CurCharacterPage * 12;
		int Count = 0;

		while (Count < 12)
		{
			if (Index >= m_ActiveCharTab.size())
				break;

			m_ActiveCharTab[Index]->SetPos(m_vecCharTabPos[Count]);
			m_ActiveCharTab[Index]->TabActorOn();

			++Index;
			++Count;
		}

		m_ActiveCharTab[m_CurCharacterPage * 12]->SelectTab(true);
	}
		return;
	}
}
void CBattlePositionMenu::PopCurPositionCharacter()
{
	if (nullptr == m_vecCurPositionUI[m_PositionTabIndex]) 
	{
		ReturnPositionSelect();
		return;
	}


	m_vecCurPositionUI[m_PositionTabIndex]->SelectUIOff();
	m_vecCurPositionUI[m_PositionTabIndex]->UIOff();

	CClientGlobal::GameMaster->PopCharacterTeam(m_PositionTabIndex);
	m_vecCurPositionUI[m_PositionTabIndex] = nullptr;

	m_NoPositionGuide->SetRenPivot(m_GuidePivot + Vec2{ m_vecUIPosition[m_PositionTabIndex].x,m_vecUIPosition[m_PositionTabIndex].y });
	m_NoPositionGuide->On();

	ReturnPositionSelect();
}
void CBattlePositionMenu::ChangeCharacterPositionEvent()
{
	size_t Index = m_CurCharacterPage * 12 + m_CurCharacterTabIndex;

	if (0 == Index)
	{
		PopCurPositionCharacter();
		return;
	}

	if (Index >= m_ActiveCharTab.size() )
		return;

	CharacterIndex CharNo = MAXBATTLECHARACTER;

	for (size_t i = 0; i < m_CharacterTab.size(); i++)
	{
		if (m_ActiveCharTab[Index] == m_CharacterTab[i])
		{
			CharNo = (CharacterIndex)i;
			break;
		}
	}

	if (MAXBATTLECHARACTER == CharNo)
		return;
	
	if (nullptr != m_vecCurPositionUI[m_PositionTabIndex]) 
	{
		m_vecCurPositionUI[m_PositionTabIndex]->SelectUIOff();
		m_vecCurPositionUI[m_PositionTabIndex]->UIOff();
	}

	CClientGlobal::GameMaster->SetCharacterTeam(m_PositionTabIndex, CharNo);
	m_vecCurPositionUI[m_PositionTabIndex] = m_pMenuMgr->BattlePositionUIOn(CharNo, m_vecUIPosition[m_PositionTabIndex], BATTLEPOSITIONVER);
	
	if(nullptr != m_vecCurPositionUI[m_PositionTabIndex])
		m_vecCurPositionUI[m_PositionTabIndex]->SelectUIOn();

	ReturnPositionSelect();

	return;
}