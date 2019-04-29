#include "Precom.h"
#include "CharacterSkillMenu.h"
#include "MainMenuMgr.h"
#include "MenuTab.h"
#include "CharStatusTab.h"
#include "MenuSkillUI.h"
#include "SkillDetailUI.h"
#include"GameMaster.h"
#include "SkillLevelUpUI.h"
#include <ResourceMgr.h>
#include <GameMultiSprite.h>


CCharacterSkillMenu::CCharacterSkillMenu()
	:m_MenuMainBack(nullptr), m_MenuSubBack(nullptr)
	, m_CurLineTabIndex(0), m_CurCharIconIndex(0)
	, m_MaxUICount(0), m_SkillDetailUI(nullptr)
	, m_SkillLevelUpUI(nullptr), m_bUpdate(true)
{

}


CCharacterSkillMenu::~CCharacterSkillMenu()
{
	m_CharacterLineTab.clear();
	m_ActiveLineTab.clear();
	m_CharacterIcon.clear();
	m_ActiveIcon.clear();
	m_vecCharacterSkillUI.clear();
	m_ActiveSkillUI.clear();

}
void CCharacterSkillMenu::Init() 
{
	MenuOff();
	m_vecCharacterSkillUI.assign(256, nullptr);

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
		m_CharacterLineTab[j]->MenuTabOff();
	}

	for (size_t i = 0; i < MAXBATTLECHARACTER; i++)
		m_CharacterIcon.push_back(CActObject::CreateActObject(_T("CharacterIcon"), ACTOR->GetScene())->CreateCom<CCharStatusTab>((CharacterIndex)i));

	for (size_t i = 0; i < 7; i++)
		m_CharacterIconPos.push_back(Vec3{ -200.f + 130 * i, 318.f ,7.f });

	m_MaxSkillIndex = CClientGlobal::vecSkill.size() -1 ;


	CMenuSkillUI::InitMenuSkillUI(COMRESMGR->FindMultiSprite(_T("SkillMainIcon")));

	for (size_t i = 0; i <= m_MaxSkillIndex; i++)
		CreateSkillUI(i);


	for (size_t i = 0; i < 12; i++) 
	{
		size_t Right = i % 3;
		size_t Row = i / 3;

		m_SkillUIPos.push_back(Vec3{ -107.f + 280.f*Right, 200.f + -80.f* Row,7.f });
	}

	m_MaxUICount = m_SkillUIPos.size();


	int Count = 0;
	size_t Index = 0;

	while (Count < m_MaxUICount)
	{
		if (nullptr == m_vecCharacterSkillUI[Index])
			continue;

		m_vecCharacterSkillUI[Index++]->SetPos(m_SkillUIPos[Count++]);
	}

	CSkillDetailUI::InitDetailSkillUI(COMRESMGR->FindMultiSprite(_T("SkillMainIcon")));
	m_SkillDetailUI = CActObject::CreateActObject(_T("DetailUI"), ACTOR->GetScene())->CreateCom<CSkillDetailUI>();
	m_SkillDetailUI->SetPos(Vec3{ 175.f,-225.f ,7.f});
	m_SkillDetailUI->UIOff();

	m_SkillLevelUpUI = CActObject::CreateActObject(_T("SkillLevelUpUI"), ACTOR->GetScene())->CreateCom<CSkillLevelUpUI>(this);
	//m_SkillLevelUpUI->UIOff();
}
void CCharacterSkillMenu::Update() 
{
	if (false == m_bUpdate)
		return;

	switch (m_CurTab)
	{
	case CCharacterSkillMenu::CHARACTERLINE_TAB:
		CharacterLineTabUpdate();
		return;
	case CCharacterSkillMenu::CHARACTER_TAB:
		CharacterTabUpdate();
		return;
	case CCharacterSkillMenu::SKILL_TAB:
		CharacterSkillTabUpdate();
		return;
	}
}
void CCharacterSkillMenu::MenuOnEvent()
{
	m_pMenuMgr->BattlePositionUIAllOff();

	for (size_t i = 0; i < m_CharacterLineTab.size(); i++)
		if (nullptr != m_CharacterLineTab[i])
			m_CharacterLineTab[i]->MenuTabOff();
	

	m_ActiveLineTab.clear();

	for (size_t i = 0; i < min(m_CharacterLineTab.size(), 16); i++)
	{
		if ( !CClientGlobal::MasterMode  && !CClientGlobal::GameMaster->IsOpenCharacterLine((CHARACTERLINE)i))
			continue;

		m_ActiveLineTab.push_back(m_CharacterLineTab[i]);
	}

	for (size_t i = 0; i < m_ActiveLineTab.size(); i++)
	{
		m_ActiveLineTab[i]->SetPivot(m_CharLineTabPos[i]);
		m_ActiveLineTab[i]->MenuTabOn();
		m_ActiveLineTab[i]->SelectTab(false);
	}

	m_CurTab = CHARACTERLINE_TAB;
	m_CurLineTabIndex = 0;
	m_ActiveLineTab[m_CurLineTabIndex]->SelectTab(true);
	ChangeCharacterIcon(m_CurLineTabIndex);
}
void CCharacterSkillMenu::CharacterLineTabUpdate() 
{
	if (true == KEYDOWN("MenuKey") || true == KEYDOWN("LEFT"))
	{
		AllCharacterIconOff();
		m_pMenuMgr->ChangeRootMenu();
		return;
	}

	if (true == KEYDOWN("UP"))
	{
		if (true == PrevLineTab()) 
		{
			ChangeCharacterIcon(GetLineIndex(m_ActiveLineTab[m_CurLineTabIndex]));
		}
	}
	else if (true == KEYDOWN("DOWN"))
	{
		if (true == NextLineTab())
			ChangeCharacterIcon(GetLineIndex(m_ActiveLineTab[m_CurLineTabIndex]));
	}
	else if (true == KEYDOWN("RIGHT") || true == KEYDOWN("Enter"))
	{
		m_CurCharIconIndex = 0;
		m_ActiveIcon[m_CurCharIconIndex]->SelectTab(true);
		m_CurTab = CHARACTER_TAB;
		ChangeSkillUI(m_ActiveIcon[m_CurCharIconIndex]->GetCharacterNo());


	/*	m_ActiveSkillUI.clear();

		for (size_t i = 0; i <= m_MaxSkillIndex; i++)
		{
			if (nullptr != m_vecCharacterSkillUI[i])
				m_ActiveSkillUI.push_back(m_vecCharacterSkillUI[i]);
		}

		for (size_t i = 0; i < min(m_MaxUICount, m_ActiveSkillUI.size()); i++)
			m_ActiveSkillUI[i]->SkillUIOn(m_SkillUIPos[i]);*/
		
	}
}
void CCharacterSkillMenu::CharacterTabUpdate() 
{
	if (true == KEYDOWN("MenuKey"))
	{
		ReturnCharLineSelect();
		return;
	}

	else if (true == KEYDOWN("LEFT")) 
	{
		if (true == PrevCharacterTab() && nullptr != m_ActiveIcon[m_CurCharIconIndex]) {}
			ChangeSkillUI(m_ActiveIcon[m_CurCharIconIndex]->GetCharacterNo());
	}
	else if (true == KEYDOWN("RIGHT")) 
	{
		if (true == NextCharacterTab() && nullptr != m_ActiveIcon[m_CurCharIconIndex]) {}
			ChangeSkillUI(m_ActiveIcon[m_CurCharIconIndex]->GetCharacterNo());
	}
	else if (true == KEYDOWN("Enter") || true == KEYDOWN("DOWN"))
	{
		m_CurSkillUIIndex = 0;
		m_CurSkillUIPage = 0;
		m_CurTab = SKILL_TAB;
		m_ActiveSkillUI[0]->SelectUI(true);
		m_SkillDetailUI->UIOn();
		m_SkillDetailUI->SetSkillDetail(m_ActiveSkillUI[0]->GetSkillNo());
	}
}
void CCharacterSkillMenu::CharacterSkillTabUpdate()
{
	if (true == KEYDOWN("MenuKey"))
	{
		ReturnCharacterSelect();
		return;
	}

	else if (true == KEYDOWN("LEFT")) 
	{
		if (true == PrevSkillTab())
			m_SkillDetailUI->SetSkillDetail(m_ActiveSkillUI[m_CurSkillUIPage * (int)m_MaxUICount + m_CurSkillUIIndex]->GetSkillNo());
	}
	else if (true == KEYDOWN("RIGHT"))
	{
		if (true == NextSkillTab()) 
			m_SkillDetailUI->SetSkillDetail(m_ActiveSkillUI[m_CurSkillUIPage * (int)m_MaxUICount + m_CurSkillUIIndex]->GetSkillNo());	
	}
	else if (true == KEYDOWN("UP")) 
	{
		if (true == PrevSkillTab(3))
			m_SkillDetailUI->SetSkillDetail(m_ActiveSkillUI[m_CurSkillUIPage * (int)m_MaxUICount + m_CurSkillUIIndex]->GetSkillNo());
	}
	else if (true == KEYDOWN("DOWN")) 
	{
		if (true == NextSkillTab(3))
			m_SkillDetailUI->SetSkillDetail(m_ActiveSkillUI[m_CurSkillUIPage * (int)m_MaxUICount + m_CurSkillUIIndex]->GetSkillNo());
	}
	else if (true == KEYDOWN("Enter")) 
	{
		int RealIndex = m_CurSkillUIPage * (int)m_MaxUICount + m_CurSkillUIIndex;

		if (nullptr == m_ActiveSkillUI[RealIndex] || nullptr == m_SkillLevelUpUI)
			return;
		
		size_t SkillNo = m_ActiveSkillUI[RealIndex]->GetSkillNo();
		size_t CurLv = CClientGlobal::vecSkill[SkillNo].CurSkillLevel;
		
		if (CurLv + 1 >= SKILLDATA::MAXSKILLLEVEL )
			return;
			
		if (true == CClientGlobal::MasterMode) 
		{
			m_SkillLevelUpUI->UIOn();
			m_SkillLevelUpUI->SetLevelUpSkill(SkillNo);
			m_SkillLevelUpUI->ChangeUIMode(CSkillLevelUpUI::SKILL_LEVEL_UP_QUERY);
			HideSkillMenu(true);
		}
		else 
		{
			if (true == CClientGlobal::CheckSkillLevelUp(SkillNo)) 
			{
				m_SkillLevelUpUI->UIOn();
				m_SkillLevelUpUI->SetLevelUpSkill(SkillNo);
				m_SkillLevelUpUI->ChangeUIMode(CSkillLevelUpUI::SKILL_LEVEL_UP_QUERY);
				HideSkillMenu(true);
			}
		}
	}
}
void CCharacterSkillMenu::ChangeCharacterIcon(const size_t& _LineIndex) 
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
void CCharacterSkillMenu::ChangeSkillUI(const CharacterIndex& _CharNo)
{
	if (_CharNo >= MAXBATTLECHARACTER)
		return;

	ActiveSkillUIOff();
	m_ActiveSkillUI.clear();

	for (size_t i = 0; i <= m_MaxSkillIndex; i++)
	{
		if (nullptr != m_vecCharacterSkillUI[i] && _CharNo == m_vecCharacterSkillUI[i]->GetCasterNo()) 
		{
			m_vecCharacterSkillUI[i]->SetLevel(CClientGlobal::vecSkill[i].CurSkillLevel);
			m_ActiveSkillUI.push_back(m_vecCharacterSkillUI[i]);
		}
	}

	sort(m_ActiveSkillUI.begin(), m_ActiveSkillUI.end(), CMenuSkillUI::CompareMenuSkillUI);

	for (size_t i = 0; i < min(m_ActiveSkillUI.size(), m_MaxUICount); i++)
		if (nullptr != m_ActiveSkillUI[i]) 
			m_ActiveSkillUI[i]->SkillUIOn(m_SkillUIPos[i]);
		

}
const bool CCharacterSkillMenu::CheckChangePage(const PAGEEVENT& _Op, const int _JumpIndex)
{
	int JumpIndex = _JumpIndex;

	switch (_Op)
	{
	case CCharacterSkillMenu::PAGE_UP:
		JumpIndex *= -1;
		break;
	case CCharacterSkillMenu::PAGE_DOWN:
		break;
	default:
		return false;
	}

	int RealIndex = m_CurSkillUIPage * (int)m_MaxUICount + m_CurSkillUIIndex + JumpIndex;

	switch (_Op)
	{
	case CCharacterSkillMenu::PAGE_UP: 
	{
		if (RealIndex <= 0)
			return false;
	}
		return true;
	case CCharacterSkillMenu::PAGE_DOWN: 
	{
		//if (RealIndex >= m_ActiveSkillUI.size() )
		//	return false;
		if ( (m_CurSkillUIPage + 1) * (int)m_MaxUICount >= m_ActiveSkillUI.size())
			return false;
	}
		return true;

	}

	return false;
}
void CCharacterSkillMenu::ChangeSkillPage(const PAGEEVENT& _Op, const int _JumpIndex)
{
	ActiveSkillUIOff();
	int JumpIndex = _JumpIndex;

	switch (_Op)
	{
	case CCharacterSkillMenu::PAGE_UP:
		JumpIndex *= -1;
		break;
	case CCharacterSkillMenu::PAGE_DOWN:
		break;
	default:
		return;
	}

	switch (_Op)
	{
	case CCharacterSkillMenu::PAGE_UP: 
	{
		--m_CurSkillUIPage;
		int Index = m_CurSkillUIPage * (int)m_MaxUICount;
		int Count = 0;

		while (Count < m_MaxUICount) 
		{
			if (Index >= m_ActiveSkillUI.size())
				break;

			if (nullptr == m_ActiveSkillUI[Index])
			{
				++Index;
				continue;
			}

			m_ActiveSkillUI[Index++]->SkillUIOn(m_SkillUIPos[Count++]);
		}

	
		m_CurSkillUIIndex = (int)m_MaxUICount  + m_CurSkillUIIndex +  JumpIndex;
		m_ActiveSkillUI[m_CurSkillUIPage * (int)m_MaxUICount + m_CurSkillUIIndex]->SelectUI(true);
	}
		return;
	case CCharacterSkillMenu::PAGE_DOWN: 
	{
		++m_CurSkillUIPage;
		int Index = m_CurSkillUIPage * (int)m_MaxUICount;
		int Count = 0;

		while (Count < m_MaxUICount)
		{
			if (Index >= m_ActiveSkillUI.size())
				break;

			if (nullptr == m_ActiveSkillUI[Index])
			{
				++Index;
				continue;
			}

			m_ActiveSkillUI[Index++]->SkillUIOn(m_SkillUIPos[Count++]);
		}


		m_CurSkillUIIndex = (m_CurSkillUIIndex +  JumpIndex) % m_MaxUICount;
		
		if (m_CurSkillUIPage * m_MaxUICount + m_CurSkillUIIndex >= m_ActiveSkillUI.size())
			m_CurSkillUIIndex =(int)m_ActiveSkillUI.size() -1 - m_CurSkillUIPage * (int)m_MaxUICount;

		m_ActiveSkillUI[m_CurSkillUIPage * m_MaxUICount + m_CurSkillUIIndex]->SelectUI(true);
	}
	return;
	
	}
}

void CCharacterSkillMenu::AllCharacterIconOff()
{
	for (size_t i = 0; i < m_CharacterIcon.size(); i++)
	{
		m_CharacterIcon[i]->TabOff();
		m_CharacterIcon[i]->SelectTab(false);
	}
}
void CCharacterSkillMenu::AllSkillUIOff()
{
	for (size_t i = 0; i < m_vecCharacterSkillUI.size(); i++)
	{
		if (nullptr != m_vecCharacterSkillUI[i]) 
		{
			m_vecCharacterSkillUI[i]->SkillUIOff();
			m_vecCharacterSkillUI[i]->SelectUI(false);
		}
	}
}
void CCharacterSkillMenu::ActiveSkillUIOff()
{
	for (size_t i = 0; i < m_ActiveSkillUI.size(); i++)
	{
		if(nullptr != m_ActiveSkillUI[i])
		{
			m_ActiveSkillUI[i]->SkillUIOff();
			m_ActiveSkillUI[i]->SelectUI(false);
		}
	}
}
void CCharacterSkillMenu::ReturnCharLineSelect()
{
	if (nullptr != m_ActiveIcon[m_CurCharIconIndex])
		m_ActiveIcon[m_CurCharIconIndex]->SelectTab(false);

	for (size_t i = 0; i < m_ActiveSkillUI.size(); i++)
		if (nullptr != m_ActiveSkillUI[i])
			m_ActiveSkillUI[i]->SkillUIOff();

	m_ActiveSkillUI.clear();

	m_CurTab = CHARACTERLINE_TAB;
}
void CCharacterSkillMenu::ReturnCharacterSelect()
{
	int RealIndex = m_CurSkillUIPage * (int)m_MaxUICount + m_CurSkillUIIndex;
	
	if (nullptr != m_ActiveSkillUI[RealIndex])
		m_ActiveSkillUI[RealIndex]->SelectUI(false);
	
	m_SkillDetailUI->UIOff();
	m_CurSkillUIPage = 0;
	m_CurSkillUIIndex = 0;
	m_CurTab = CHARACTER_TAB;
}
CMenuSkillUI* CCharacterSkillMenu::CreateSkillUI(const size_t& _SkillNo)
{
	if (CClientGlobal::vecSkill.size() <= _SkillNo || false == CClientGlobal::vecSkill[_SkillNo].SkillData.CharacterSkill)
		return nullptr;

	while(m_vecCharacterSkillUI.size() <= _SkillNo) 
	{
		size_t CurSize = m_vecCharacterSkillUI.size();
		m_vecCharacterSkillUI.resize(CurSize * 2);
	}

	SKILLUIDATA Data = CClientGlobal::vecSkill[_SkillNo].SkillUIData;

	CActObject* NewUIAct = CActObject::CreateActObject(Data.SkillName, ACTOR->GetScene());
	CMenuSkillUI* NewUI = NewUIAct->CreateCom<CMenuSkillUI>(Data);

	if (nullptr == NewUI)
	{
		assert(nullptr);
		return nullptr;
	}

	m_vecCharacterSkillUI[_SkillNo] = NewUI;
	NewUI->SkillUIOff();

	return NewUI;
}
const bool CCharacterSkillMenu::PrevLineTab() 
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
const bool CCharacterSkillMenu::NextLineTab() 
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
const bool CCharacterSkillMenu::PrevCharacterTab() 
{
	if (m_CurCharIconIndex <= 0)
		return false;

	if (nullptr != m_ActiveIcon[m_CurCharIconIndex])
		m_ActiveIcon[m_CurCharIconIndex]->SelectTab(false);

	--m_CurCharIconIndex;

	if (nullptr != m_ActiveIcon[m_CurCharIconIndex])
	{
		m_ActiveIcon[m_CurCharIconIndex]->SelectTab(true);
	}

	return true;
}
const bool CCharacterSkillMenu::NextCharacterTab()
{
	if (m_CurCharIconIndex >= m_ActiveIcon.size() - 1)
		return false;

	if (nullptr != m_ActiveIcon[m_CurCharIconIndex])
		m_ActiveIcon[m_CurCharIconIndex]->SelectTab(false);

	++m_CurCharIconIndex;

	if (nullptr != m_ActiveIcon[m_CurCharIconIndex])
	{
		m_ActiveIcon[m_CurCharIconIndex]->SelectTab(true);
		
	}

	return true;
}
const bool CCharacterSkillMenu::PrevSkillTab(const int& _JumpIndex /*= 1*/)
{
	if (m_CurSkillUIIndex - _JumpIndex < 0)
	{
		if (false == CheckChangePage(PAGE_UP, _JumpIndex))
			return false;

		ChangeSkillPage(PAGE_UP, _JumpIndex);
		return true;
	}

	int RealIndex = m_CurSkillUIPage * (int)m_MaxUICount + m_CurSkillUIIndex;
	
	if (nullptr != m_ActiveSkillUI[RealIndex])
		m_ActiveSkillUI[RealIndex]->SelectUI(false);
	
	RealIndex -= _JumpIndex;
	m_CurSkillUIIndex -= _JumpIndex;

	if (nullptr != m_ActiveSkillUI[RealIndex])
		m_ActiveSkillUI[RealIndex]->SelectUI(true);


	return true;
}
const bool CCharacterSkillMenu::NextSkillTab(const int& _JumpIndex /*= 1*/)
{
	if (m_CurSkillUIIndex + _JumpIndex >= m_MaxUICount)
	{
		if (false == CheckChangePage(PAGE_DOWN, _JumpIndex))
			return false;

		ChangeSkillPage(PAGE_DOWN, _JumpIndex);
		return true;
	}

	int RealIndex = m_CurSkillUIPage * (int)m_MaxUICount + m_CurSkillUIIndex;

	if (RealIndex + _JumpIndex >= m_ActiveSkillUI.size())
		return false;

	if (nullptr != m_ActiveSkillUI[RealIndex])
		m_ActiveSkillUI[RealIndex]->SelectUI(false);

	RealIndex += _JumpIndex;
	m_CurSkillUIIndex += _JumpIndex;

	if (nullptr != m_ActiveSkillUI[RealIndex])
		m_ActiveSkillUI[RealIndex]->SelectUI(true);

	return true;
}
void CCharacterSkillMenu::HideSkillMenu(const bool& _Hide)
{
	int RealIndex;

	if (true == _Hide) 
	{
		for (size_t i = 0; i < m_ActiveIcon.size(); i++)
			m_ActiveIcon[i]->TabOff();
		
		for (int i = 0 ; i < (int)m_MaxUICount ; i++)
		{
			RealIndex = m_CurSkillUIPage * (int)m_MaxUICount + i;
			
			if (RealIndex >= m_ActiveSkillUI.size())
				break;

			if (nullptr != m_ActiveSkillUI[RealIndex])
				m_ActiveSkillUI[RealIndex]->SkillUIOff();

		}

		m_SkillDetailUI->UIOff();
		MenuActorOff();
		m_bUpdate = false;
	}
	else 
	{
		for (size_t i = 0; i < m_ActiveIcon.size(); i++)
			m_ActiveIcon[i]->TabOn();

		for (int i = 0; i < (int)m_MaxUICount; i++)
		{
			RealIndex = m_CurSkillUIPage * (int)m_MaxUICount + i;
			
			if (RealIndex >= m_ActiveSkillUI.size())
				break;

			if (nullptr != m_ActiveSkillUI[RealIndex])
				m_ActiveSkillUI[RealIndex]->SkillUIOn();

		}
		m_SkillDetailUI->UIOn();
		MenuActorOn();
		m_bUpdate = true;
	}
}
void CCharacterSkillMenu::CurSkillLevelUpEvent(const bool& _LevelUp)
{
	if (false == _LevelUp)
		return;

	int RealIndex = m_CurSkillUIPage * (int)m_MaxUICount + m_CurSkillUIIndex;

	if (nullptr == m_ActiveSkillUI[RealIndex] )
		return;

	size_t SkillNo = m_ActiveSkillUI[RealIndex]->GetSkillNo();

	if (SkillNo >= CClientGlobal::vecSkill.size())
		return;

	m_ActiveSkillUI[RealIndex]->SetLevel(CClientGlobal::vecSkill[SkillNo].CurSkillLevel);

	m_SkillDetailUI->SetSkillDetail(SkillNo);
}
void CCharacterSkillMenu::MenuActorOn()
{
	ACTOR->On();
}
void CCharacterSkillMenu::MenuActorOff()
{
	ACTOR->Off();
}
const size_t CCharacterSkillMenu::GetLineIndex(SPTR<CMenuTab> _Tab)
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