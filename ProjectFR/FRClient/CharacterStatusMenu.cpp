#include "Precom.h"
#include "CharacterStatusMenu.h"
#include "MainMenuMgr.h"
#include "MenuTab.h"
#include "CharStatusTab.h"
#include "CharacterStatus.h"
#include "GameMaster.h"

CCharacterStatusMenu::CCharacterStatusMenu()
	: m_MenuMainBack(nullptr), m_MenuSubBack(nullptr)
	, m_CurLineTabIndex(0), m_Status(nullptr)
{
	m_CharacterLineTab.clear();
	m_CharacterIcon.clear();
	m_ActiveIcon.clear();
	m_ActiveLineTab.clear();
}


CCharacterStatusMenu::~CCharacterStatusMenu()
{
	m_CharacterLineTab.clear();
	m_CharLineTabPos.clear();
	m_ActiveLineTab.clear();
}
void CCharacterStatusMenu::Init()
{
	MenuOff();

	m_MenuMainBack = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_UIBACK, 0.9f, true));
	m_MenuMainBack->SetSprite(_T("StatusMainBack"));
	Vec2 Pivot = m_MenuMainBack->GetCurSpriteSize();
	m_MenuMainBack->SetRenPivot(Vec2{ Pivot.x*0.17f ,0.0f });

	m_MenuSubBack = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_UIBACK, 0.9f, true));
	m_MenuSubBack->SetSprite(_T("StatusSubBack"));
	m_MenuSubBack->SetRenPivot(Vec2{ -400.f,0.f });

	for (size_t i = 0; i < 16; i++)
		m_CharLineTabPos.push_back(Vec3{ -400.f,300.f - 50 * i,7.f });
	

	for (size_t j = 0; j < CClientGlobal::CharacterLine.size(); ++j)
	{
		m_CharacterLineTab.push_back(ACTOR->CreateCom<CMenuTab>());
		m_CharacterLineTab[j]->SetTabStyle(_T("캐릭라인폰트"));
		m_CharacterLineTab[j]->SetTabSize(Vec2{ 241.f,40.f });
		size_t Index = CClientGlobal::CharacterLine[j][0].CharacterNo;
		m_CharacterLineTab[j]->SetTabName(CClientGlobal::CharacterName[Index].UnitKName);
	}

	for (size_t i = 0; i < MAXBATTLECHARACTER; i++)
		m_CharacterIcon.push_back(CActObject::CreateActObject(_T("CharacterIcon"), ACTOR->GetScene())->CreateCom<CCharStatusTab>((CharacterIndex)i));
	
	for (size_t i = 0; i < 7; i++)
		m_CharacterIconPos.push_back(Vec3{ -200.f + 130 * i, 318.f ,7.f });


	m_Status = CActObject::CreateActObject(_T("CharacterStatus"), ACTOR->GetScene())->CreateCom<CCharacterStatus>();
}
void CCharacterStatusMenu::Update()
{
	switch (m_CurTab)
	{
	case CCharacterStatusMenu::CHARACTERLINE_TAB:
		CharacterLineTabUpdate();
		return;
	case CCharacterStatusMenu::CHARACTER_TAB:
		CharacterTabUpdate();
		return;
	}
}
void CCharacterStatusMenu::MenuOnEvent()
{
	m_pMenuMgr->BattlePositionUIAllOff();

	for (size_t i = 0; i < m_CharacterLineTab.size(); i++)
	{
		m_CharacterLineTab[i]->SelectTab(false);
		m_CharacterLineTab[i]->MenuTabOff();
	}
	
	m_ActiveLineTab.clear();

	for (size_t i = 0; i < min(m_CharacterLineTab.size(), 16); i++)
	{
		if (!CClientGlobal::MasterMode && !CClientGlobal::GameMaster->IsOpenCharacterLine((CHARACTERLINE)i))
			continue;

		m_ActiveLineTab.push_back(m_CharacterLineTab[i]);
	}

	for (size_t i = 0; i < m_ActiveLineTab.size(); i++)
	{
		m_ActiveLineTab[i]->SetPivot(m_CharLineTabPos[i]);
		m_ActiveLineTab[i]->MenuTabOn();
	}

	m_CurTab = CHARACTERLINE_TAB;
	m_CurLineTabIndex = 0;
	m_ActiveLineTab[m_CurLineTabIndex]->SelectTab(true);
	ChangeCharacterIcon(GetCurLineIndex());
}
void CCharacterStatusMenu::CharacterLineTabUpdate() 
{
	if (true == KEYDOWN("MenuKey") || true == KEYDOWN("LEFT"))
	{
		AllCharacterIconOff();
		m_pMenuMgr->ChangeRootMenu();
		return;
	}

	if (true == KEYDOWN("UP")) 
	{
		if(true == PrevLineTab())
			ChangeCharacterIcon(GetCurLineIndex());
	}
	else if (true == KEYDOWN("DOWN")) 
	{
		if(true == NextLineTab())
			ChangeCharacterIcon(GetCurLineIndex());
	}
	else if (true == KEYDOWN("RIGHT") || true == KEYDOWN("Enter"))
	{
		m_CurCharIconIndex = 0;
		m_ActiveIcon[m_CurCharIconIndex]->SelectTab(true);
		m_Status->StatusOn(m_ActiveIcon[m_CurCharIconIndex]->GetCharacterNo());
		m_CurTab = CHARACTER_TAB;
	}
}
void CCharacterStatusMenu::CharacterTabUpdate()
{
	if (true == KEYDOWN("MenuKey"))
	{
		ReturnCharLineSelect();
		return;
	}

	else if (true == KEYDOWN("LEFT"))
		PrevCharacterTab();
	else if (true == KEYDOWN("RIGHT"))
		NextCharacterTab();
	else if (true == KEYDOWN("Enter"))
		int a = 0;
}
void CCharacterStatusMenu::ChangeCharacterIcon(const size_t& _LineIndex)
{
	if (_LineIndex >= CClientGlobal::CharacterLine.size())
		return;

	AllCharacterIconOff();
	m_ActiveIcon.clear();

	for (size_t i = 0; i < CClientGlobal::CharacterLine[_LineIndex].size(); i++)
	{
		size_t CharNo = CClientGlobal::CharacterLine[_LineIndex][i].CharacterNo;
		m_ActiveIcon.push_back(m_CharacterIcon[CharNo]);
	}

	for (size_t i = 0; i < min(7, m_ActiveIcon.size()); i++) 
	{
		m_ActiveIcon[i]->SetPos(m_CharacterIconPos[i]);
		m_ActiveIcon[i]->TabOn();
	}

}
void CCharacterStatusMenu::AllCharacterIconOff()
{
	for (size_t i = 0; i < m_CharacterIcon.size(); i++)
	{
		m_CharacterIcon[i]->TabOff();
		m_CharacterIcon[i]->SelectTab(false);
	}
}
void CCharacterStatusMenu::ReturnCharLineSelect()
{
	if (nullptr != m_ActiveIcon[m_CurCharIconIndex])
		m_ActiveIcon[m_CurCharIconIndex]->SelectTab(false);
	
	if(nullptr != m_Status)
		m_Status->StatusOff();

	m_CurTab = CHARACTERLINE_TAB;
}
const bool CCharacterStatusMenu::PrevLineTab()
{
	if (m_CurLineTabIndex <= 0)
		return false;

	if (nullptr != m_ActiveLineTab[m_CurLineTabIndex])
		m_ActiveLineTab[m_CurLineTabIndex]->SelectTab(false);

	--m_CurLineTabIndex;

	if (nullptr != m_ActiveLineTab[m_CurLineTabIndex])
		m_ActiveLineTab[m_CurLineTabIndex]->SelectTab(true);

	return true;
}
const bool CCharacterStatusMenu::NextLineTab()
{
	if (m_CurLineTabIndex >= m_ActiveLineTab.size() - 1)
		return false;

	if (nullptr != m_ActiveLineTab[m_CurLineTabIndex])
		m_ActiveLineTab[m_CurLineTabIndex]->SelectTab(false);
	 
	
	++m_CurLineTabIndex;

	if (nullptr != m_ActiveLineTab[m_CurLineTabIndex])
		m_ActiveLineTab[m_CurLineTabIndex]->SelectTab(true);

	return true;
}
const bool CCharacterStatusMenu::PrevCharacterTab()
{
	if (m_CurCharIconIndex <= 0)
		return false;

	if (nullptr != m_ActiveIcon[m_CurCharIconIndex])
		m_ActiveIcon[m_CurCharIconIndex]->SelectTab(false);

	--m_CurCharIconIndex;

	if (nullptr != m_ActiveIcon[m_CurCharIconIndex]) 
	{
		m_ActiveIcon[m_CurCharIconIndex]->SelectTab(true);
		m_Status->ChangeCharacterStatus(m_ActiveIcon[m_CurCharIconIndex]->GetCharacterNo());
	}

	return true;
}
const bool CCharacterStatusMenu::NextCharacterTab()
{
	if (m_CurCharIconIndex >= m_ActiveIcon.size() - 1)
		return false;

	if (nullptr != m_ActiveIcon[m_CurCharIconIndex])
		m_ActiveIcon[m_CurCharIconIndex]->SelectTab(false);
	
	++m_CurCharIconIndex;

	if (nullptr != m_ActiveIcon[m_CurCharIconIndex]) 
	{
		m_ActiveIcon[m_CurCharIconIndex]->SelectTab(true);
		m_Status->ChangeCharacterStatus(m_ActiveIcon[m_CurCharIconIndex]->GetCharacterNo());
	}
	return true;
}
const size_t CCharacterStatusMenu::GetLineIndex(SPTR<CMenuTab> _Tab)
{
	if (nullptr == _Tab)
		return -1;

	for (size_t i = 0; i < m_CharacterLineTab.size(); i++)
	{
		if (m_CharacterLineTab[i] == _Tab)
			return i;
	}

	return -1;
}
const size_t CCharacterStatusMenu::GetCurLineIndex()
{
	if (m_CurLineTabIndex >= m_ActiveLineTab.size() || nullptr == m_ActiveLineTab[m_CurLineTabIndex])
		return -1;

	for (size_t i = 0; i < m_CharacterLineTab.size(); i++)
	{
		if (m_CharacterLineTab[i] == m_ActiveLineTab[m_CurLineTabIndex])
			return i;
	}

	return -1;
}