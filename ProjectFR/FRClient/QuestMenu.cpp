#include "Precom.h"
#include "QuestMenu.h"
#include "MainMenuMgr.h"
#include "GameMaster.h"
#include "EventMgr.h"
#include "QuestTab.h"
#include<algorithm>

CQuestMenu::CQuestMenu()
	:m_QuestTitleBackRen(nullptr), m_QuestBackRen(nullptr)
	, m_CurQuestIndex(0), m_QuestContentsFont(nullptr), m_QuestRewardFont(nullptr)
{
	m_vecQuestTab.clear();
	m_vecActiveQuestTab.clear();
	m_vecMenuFont.clear();
	m_vecQuestMission.clear();
}


CQuestMenu::~CQuestMenu()
{
	m_vecQuestTab.clear();
	m_vecActiveQuestTab.clear();
	m_vecMenuFont.clear();
	m_vecQuestMission.clear();
}
void CQuestMenu::Init()
{
	MenuOff();

	m_QuestTitleBackRen = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_UIBACK, 0.9f, true));
	m_QuestTitleBackRen->SetSprite(_T("QuestTitleBack"));
	Vec2 Pivot = m_QuestTitleBackRen->GetCurSpriteSize();
	m_QuestTitleBackRen->SetRenPivot(Vec2{ -Pivot.x*0.6f ,0.0f });

	m_QuestBackRen = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_UIBACK, 0.9f, true));
	m_QuestBackRen->SetSprite(_T("QuestBack"));
	Pivot = m_QuestBackRen->GetCurSpriteSize();
	m_QuestBackRen->SetRenPivot(Vec2{ Pivot.x*0.43f,0.f });


	for (size_t i = 0; i < MAXQUESTMENUTEXT; i++)
		m_vecMenuFont.push_back(ACTOR->CreateCom<CFontRenderer>(RENDATA(LAYER_UI, 1.f, true)));
	
	m_vecMenuFont[QUESTNAME]->SetFontStyle(_T("´õÅ«º¼µå±¼¸²"));
	m_vecMenuFont[QUESTLIST]->SetFontStyle(_T("Äù½ºÆ®±¼¸²"));
	m_vecMenuFont[QUESTDETAIL]->SetFontStyle(_T("Äù½ºÆ®±¼¸²"));
	m_vecMenuFont[QUESTREWARD]->SetFontStyle(_T("Äù½ºÆ®±¼¸²"));

	m_vecMenuFont[QUESTNAME]->SetString(_T("Äù½ºÆ® ÀÌ¸§"));
	m_vecMenuFont[QUESTLIST]->SetString(_T("ÀÓ¹«"));
	m_vecMenuFont[QUESTDETAIL]->SetString(_T("³»¿ë"));
	m_vecMenuFont[QUESTREWARD]->SetString(_T("º¸»ó"));


	m_vecMenuFont[QUESTNAME]->SetLeftStartPivot(POINT{-10,300});
	m_vecMenuFont[QUESTLIST]->SetLeftStartPivot(POINT{ 0,200});
	m_vecMenuFont[QUESTDETAIL]->SetLeftStartPivot(POINT{ 320,200 });
	m_vecMenuFont[QUESTREWARD]->SetLeftStartPivot(POINT{ 320,-100 });

	size_t QuestCount = CClientGlobal::GameMaster->GetEventMgr()->GetQuestCount();

	for (size_t i = 0; i < 16; i++)
		m_vecQuestTabPos.push_back(Vec3{ -287.f,320.f - 50 * i,7.f });

	for (size_t i = 0; i < QuestCount; i++)
	{
		m_vecQuestTab.push_back(CActObject::CreateActObject(_T("QuestTab"), ACTOR->GetScene())->CreateCom<CQuestTab>());
		m_vecQuestTab[i]->SetQuestNo(i+1);
		m_vecQuestTab[i]->SetActorPos(m_vecQuestTabPos[i]);
		m_vecQuestTab[i]->SelectTab(false);
		m_vecQuestTab[i]->ActorOff();
	}

	m_QuestContentsFont = ACTOR->CreateCom<CScriptRenderer>(RENDATA(LAYER_UI, 0.9f, true));
	m_QuestRewardFont = ACTOR->CreateCom<CScriptRenderer>(RENDATA(LAYER_UI, 0.9f, true));
	
	m_QuestContentsFont->SetFontStyle(_T("Á»Å«º¼µå±¼¸²"));
	m_QuestContentsFont->SetLeftStartPivot(POINT{ 310, 160});
	m_QuestContentsFont->SetLineInterval(25);
	

	m_QuestRewardFont->SetFontStyle(_T("Á»Å«º¼µå±¼¸²"));
	m_QuestRewardFont->SetLeftStartPivot(POINT{ 310,-140 });
	m_QuestRewardFont->SetLineInterval(25);

	for (size_t i = 0; i < MAXMISSION; i++)
	{
		m_vecQuestMission.push_back(ACTOR->CreateCom<CScriptRenderer>(RENDATA(LAYER_UI, 0.9f, true)));
		m_vecQuestMission[i]->SetFontStyle(_T("Á»Å«º¼µå±¼¸²"));
		m_vecQuestMission[i]->SetLineInterval(25);
		m_vecQuestMission[i]->SetLeftStartPivot(POINT{ -10, 160 - (60*(int)i) });
	}
}
void CQuestMenu::Update()
{
	if (TRUE == KEYDOWN("MenuKey")) 
	{
		m_pMenuMgr->ChangeRootMenu();
		ClearActiveQuestTab();
		return;
	}


	if (TRUE == KEYDOWN("UP") && CheckPrevQuest())
		ChangeQuestPage();
	else if (TRUE == KEYDOWN("DOWN")&& CheckNextQuest())
		ChangeQuestPage();

}
void CQuestMenu::MenuOnEvent()
{
	m_pMenuMgr->BattlePositionUIAllOff();

	PushActiveQuest();
	
	if (m_vecActiveQuestTab.empty()) 
	{
		m_pMenuMgr->ChangeRootMenu();
		return;
	}

	for (size_t i = 0; i < min(m_vecActiveQuestTab.size(), MAXQUESTTAB); i++) 
	{
		m_vecActiveQuestTab[i]->SetActorPos(m_vecQuestTabPos[i]);
		m_vecActiveQuestTab[i]->ActorOn();
		m_vecActiveQuestTab[i]->SelectTab(false);
	}

	m_CurQuestIndex = 0;
	m_vecActiveQuestTab[m_CurQuestIndex]->SelectTab(true);

	ChangeQuestPage();
}
const bool CQuestMenu::CheckPrevQuest()
{
	if (0 == m_CurQuestIndex)
		return false;

	if (nullptr != m_vecActiveQuestTab[m_CurQuestIndex])
		m_vecActiveQuestTab[m_CurQuestIndex--]->SelectTab(false);

	if (nullptr != m_vecActiveQuestTab[m_CurQuestIndex])
		m_vecActiveQuestTab[m_CurQuestIndex]->SelectTab(true);

	return true;
}
const bool CQuestMenu::CheckNextQuest() 
{
	if (m_vecActiveQuestTab.size() - 1 <= m_CurQuestIndex)
		return false;

	if (nullptr != m_vecActiveQuestTab[m_CurQuestIndex])
		m_vecActiveQuestTab[m_CurQuestIndex++]->SelectTab(false);

	if (nullptr != m_vecActiveQuestTab[m_CurQuestIndex])
		m_vecActiveQuestTab[m_CurQuestIndex]->SelectTab(true);

	return true;
}
void CQuestMenu::ChangeQuestPage()
{
	if (m_CurQuestIndex > m_vecActiveQuestTab.size() - 1 || nullptr == m_vecActiveQuestTab[m_CurQuestIndex])
		return;


	tstring QuestKey = m_vecActiveQuestTab[m_CurQuestIndex]->GetQuestKey();
	tstring QuestTitle = CClientGlobal::GameMaster->GetEventMgr()->GetQuestSubTitle(QuestKey);
	QUESTTYPE Type = m_vecActiveQuestTab[m_CurQuestIndex]->GetQuestType();
	QUESTTEXT Text = CClientGlobal::GameMaster->GetEventMgr()->GetQuestText(QuestKey);

	m_vecMenuFont[QUESTNAME]->SetString(QuestTitle);

	switch (Type)
	{
	case QUEST_STORY:
		m_vecMenuFont[QUESTNAME]->SetFontColor(D3DCOLOR_ARGB(255, 255, 135, 0));
		break;
	case QUEST_EVENT:
	case QUEST_ETC:
		m_vecMenuFont[QUESTNAME]->SetFontColor(D3DCOLOR_ARGB(255, 30, 210, 30));
		break;

	}
	m_QuestContentsFont->SetScriptString(Text.QuestContents);
	m_QuestRewardFont->SetScriptString(Text.QuestReward);
	
	for (size_t i = 0; i < MAXMISSION; i++)
	{
		m_vecQuestMission[i]->SetScriptString(_T(""));
		m_vecQuestMission[i]->SetFontColor(D3DCOLOR_ARGB(255, 100, 100, 100));
		m_vecQuestMission[i]->Off();
	}

	POINT Pivot = POINT{ -10, 160 };
	UINT Count = 0;

	for (size_t i = 0; i <= min(Text.CurMissionIndex, MAXMISSION - 1); i++) 
	{
		m_vecQuestMission[i]->SetScriptString(Text.QuestMission[i].MissionText);
		m_vecQuestMission[i]->On();
		m_vecQuestMission[i]->SetLeftStartPivot(Pivot);
		Count += (UINT)m_vecQuestMission[i]->GetCurScriptCount();
		Pivot.y = 160 - (Count * 25) - (10 * (UINT)i);
	}

	m_vecQuestMission[min(Text.CurMissionIndex, MAXMISSION - 1)]->SetFontColor(D3DCOLOR_ARGB(255, 255, 255, 255));
}
void CQuestMenu::PushActiveQuest()
{

	stable_sort(m_vecQuestTab.begin(), m_vecQuestTab.end(), [](SPTR<CQuestTab> _Left, SPTR<CQuestTab> _Right)->bool
	{
		return _Left->GetQuestType() < _Right->GetQuestType();
	});

	for (size_t i = 0; i < m_vecQuestTab.size(); i++)
	{
		bool Check = CClientGlobal::GameMaster->GetEventMgr()->IsQuestStart(m_vecQuestTab[i]->GetQuestKey());

		if (Check)
			m_vecActiveQuestTab.push_back(m_vecQuestTab[i]);
	}

}
void CQuestMenu::ClearActiveQuestTab()
{
	for (size_t i = 0; i < m_vecActiveQuestTab.size(); i++)
	{
		m_vecActiveQuestTab[i]->SelectTab(false);
		m_vecActiveQuestTab[i]->ActorOff();
	}

	m_vecActiveQuestTab.clear();
}
void CQuestMenu::AllQuestTabOff()
{
	for (size_t i = 0; i < m_vecQuestTab.size(); i++)
		m_vecQuestTab[i]->ActorOff();
	
}