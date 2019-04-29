#include "Precom.h"
#include "EndingCredit.h"
#include "EndingSceneBuilder.h"
#include <GameMultiSprite.h>


CEndingCredit::CEndingCredit()
	:m_pEndingBuilder(nullptr), m_EndingRen(nullptr)
	, m_CreditRen(nullptr), m_EndingSpeaker(nullptr)
	, m_fAccTime(0.f), m_fFrameTime(14.f), m_CurIndex(0)
	, m_bEndingUpdate(false), m_fMaxVol(0.4f)
	, m_CurUpdate(ENDING_UPDATE)
{
}


CEndingCredit::~CEndingCredit()
{
}
void CEndingCredit::Init() 
{

}
void CEndingCredit::InitData(void* _pEndingBuilder) 
{
	if (nullptr == _pEndingBuilder)
		return;

	m_pEndingBuilder = (CEndingSceneBuilder*)_pEndingBuilder;

	m_EndingRen = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_MAPOBJECT, 1.f, true));
	m_EndingRen->SetRenPivot(Vec2{ -300.f ,0.});
	m_EndingSprite = COMRESMGR->FindMultiSprite(_T("Ending"));

	m_CreditRen = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_MAPOBJECT, 1.f, true));
	m_CreditRen->SetRenPivot(Vec2{ 400.f ,100. });
	m_CreditSprite = COMRESMGR->FindMultiSprite(_T("EndingCredit"));

	m_EndingSpeaker = ACTOR->CreateCom<CSoundPlayer>();
	return;
}
void CEndingCredit::Update()
{
	if (!m_bEndingUpdate)
		return;

	switch (m_CurUpdate)
	{
	case CEndingCredit::ENDING_UPDATE:
		EndingCreditUpdate();
		break;
	case CEndingCredit::ENDING_WAIT:
		EndingWaitUpdate();
		break;
	}

	m_fAccTime += DELTATIME;
}
void CEndingCredit::EndingCreditUpdate() 
{
	if (m_fAccTime >= m_fFrameTime) 
	{
		++m_CurIndex;

		if (m_CurIndex >= 8)
			m_CurUpdate = ENDING_WAIT;
		else if( 7 == m_CurIndex)
		{
			m_EndingRen->SetSprite(m_EndingSprite->GetSprite(m_CurIndex - 1));
			m_EndingRen->SetRenPivot(Vec2{ 0.f,0.f });
			m_EndingRen->On();
			m_EndingRen->SetAlpha(0.f);
			
			m_CreditRen->Off();
			m_CreditRen->SetAlpha(0.f);
		}
		else
		{
			m_EndingRen->SetSprite(m_EndingSprite->GetSprite(m_CurIndex - 1));
			m_EndingRen->SetRenPivot(Vec2{ -300.f ,0. });

			m_CreditRen->SetSprite(m_CreditSprite->GetSprite(m_CurIndex));
			m_CreditRen->SetRenPivot(Vec2{ 400.f ,100. });

			m_EndingRen->On();
			m_EndingRen->SetAlpha(0.f);
			m_CreditRen->On();
			m_CreditRen->SetAlpha(0.f);
		}
		m_fAccTime = 0.f;
		return;
	}

	float Alpha;

	if (m_fAccTime <= 2.f) 
	{
		Alpha = m_fAccTime/ 2.f;
		m_EndingRen->SetAlpha(Alpha);
		m_CreditRen->SetAlpha(Alpha);
	}
	else if (m_fAccTime >= 12.f) 
	{
		Alpha = (m_fFrameTime - m_fAccTime) *0.5f ;
		m_EndingRen->SetAlpha(Alpha);
		m_CreditRen->SetAlpha(Alpha);
	}

}
void CEndingCredit::EndingWaitUpdate()
{
	if (m_fAccTime >= 5.f) 
	{
		m_bEndingUpdate = false;
		m_EndingSpeaker->CurPlaySoundStop();
		m_pEndingBuilder->ChangeTitleSceneEvent();
		return;
	}

	float Vol = 3.5f - m_fAccTime;
	m_EndingSpeaker->SetVolume(max(0.f, min(Vol, 1.f)*m_fMaxVol));
}
void CEndingCredit::StandBy()
{
	m_fAccTime = 0.f;
	m_CurIndex = 0;
	m_bEndingUpdate = true;
	m_CurUpdate = ENDING_UPDATE;
	
	m_EndingSpeaker->PlaySound(_T("EndingBgm"),true);
	m_EndingSpeaker->SetVolume(m_fMaxVol);
	
	m_CreditRen->SetSprite(m_CreditSprite->GetSprite(m_CurIndex));
	m_CreditRen->SetRenPivot(Vec2{ 0.f,0.f });
	m_CreditRen->On();
	m_EndingRen->Off();

	m_EndingRen->SetAlpha(0.f);
	m_CreditRen->SetAlpha(0.f);
}