#include "Precom.h"
#include "QuestTab.h"
#include "GameMaster.h"
#include "EventMgr.h"


CQuestTab::CQuestTab()
	:m_QuestTypeRen(nullptr), m_QuestNo(-1)
	, m_QuestKey(_T("")), m_QuestType(MAXQUESTTYPE)
{
	
}


CQuestTab::~CQuestTab()
{

}

void CQuestTab::Init()
{
	CMenuTab::Init();
	m_QuestTypeRen = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_UI, 0.95f, true));
	m_QuestTypeRen->SetRenPivot(Vec2{ -180.f,0.f });
	m_QuestTypeRen->SetCustomSize(Vec2{ 65.f,25.f });

	SetTabStyle(_T("좀큰볼드굴림"));
	SetTabFontColor(D3DCOLOR_ARGB(255,236,230,204));
	SetTabSize(Vec2{ 450.f,40.f });
	SetPivot(POINT{ -135,-7 });
	
}
void CQuestTab::SetQuestNo(const size_t& _QuestNo)
{
	m_QuestNo = _QuestNo;
	m_QuestKey = _T("Quest") + to_wstring(m_QuestNo);
	QUESTDATA Data = CClientGlobal::GameMaster->GetEventMgr()->GetCurQuestData(m_QuestKey);
	SetTabName(Data.QuestSubTitle);
	
	m_QuestType = Data.QuestType;

	switch (m_QuestType)
	{
	case QUEST_STORY:
		m_QuestTypeRen->SetSprite(_T("StoryQuest"));
		break;
	case QUEST_EVENT:
		m_QuestTypeRen->SetSprite(_T("EventQuest"));
		break;
	case QUEST_ETC:
		m_QuestTypeRen->SetSprite(_T("ActionDetail"));
		break;
	default:
		break;
	}

}
const tstring  CQuestTab::GetQuestKey()
{
	return m_QuestKey;
}
const QUESTTYPE CQuestTab::GetQuestType()
{
	return m_QuestType;
}

