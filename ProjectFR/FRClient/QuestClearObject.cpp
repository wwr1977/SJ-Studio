#include "Precom.h"
#include "QuestClearObject.h"
#include "EventMgr.h"
#include "Player.h"
#include "GameMaster.h"

CQuestClearObject::CQuestClearObject()
	:m_BackFrameRen(nullptr), m_QuestTitleRen(nullptr)
	, m_fAccTime(0.f), m_EffectSoundPlayer(nullptr)
	, m_BackRen(nullptr), m_ClearQuestTitle(_T(""))
	, m_pEventMgr(nullptr), m_bLastQuestClear(false)
{
}


CQuestClearObject::~CQuestClearObject()
{
}
void CQuestClearObject::SetEventMgr(CEventMgr* _Mgr)
{
	if (nullptr == _Mgr)
		return;

	m_pEventMgr = _Mgr;
}
void CQuestClearObject::Init() 
{
	m_BackFrameRen = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_UIBACK, 1.f, true));
	m_BackFrameRen->SetSprite(_T("QuestFrame"));
	
	m_BackRen = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_UIBACK, 1.f, true));
	m_BackRen->SetSprite(_T("ColorBase"));
	m_BackRen->SetCustomSize(Vec2{ 926.f,85.f });
	m_BackRen->SetRenPivot(Vec2{ -2.f,5.f });

	m_BackRen->SetAlpha(0.7f);
	m_BackRen->SetMaterial(D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.f));
	m_BackRen->MaterialOn();

	m_QuestTitleRen = ACTOR->CreateCom<CFontRenderer>(RENDATA(LAYER_UI, 1.f, true));
	m_QuestTitleRen->SetFontColor(D3DCOLOR_ARGB(255, 255, 135, 0));
	m_QuestTitleRen->SetMiddleStartPivot(Vec3{0.f,-12.f,0.f});
	m_QuestTitleRen->SetString(_T("테스트 중입니다"));
	m_QuestTitleRen->SetFontStyle(_T("더큰볼드굴림"));

	m_EffectSoundPlayer = ACTOR->CreateCom<CSoundPlayer>();
	
	ACTOR->Off();
}	
void CQuestClearObject::Update() 
{
	if (m_fAccTime >= 4.5f) 
	{
		ACTOR->Off();
		
		if (m_bLastQuestClear) 
		{
			m_bLastQuestClear = false;
			CClientGlobal::GameMaster->FinishLastQuestEvent();
		}

		return;
	}

	
	m_fAccTime += DELTATIME;
}
void CQuestClearObject::StandBy(const tstring& _QuestTitle) 
{
	m_fAccTime = 0.f;
	m_ClearQuestTitle = _QuestTitle;

	if(nullptr != m_pEventMgr)
	{
		tstring LastQuestTitle = m_pEventMgr->GetQuestTitle(_T("Quest3"));
		
		if (m_ClearQuestTitle == LastQuestTitle) 
			m_bLastQuestClear = true;
	}
	
	m_QuestTitleRen->SetString(_T("퀘스트 완료 : ") + m_ClearQuestTitle);
	m_EffectSoundPlayer->PlaySound(_T("Questcomplete"));
	m_EffectSoundPlayer->SetVolume(0.7f);
	ACTOR->On();
}