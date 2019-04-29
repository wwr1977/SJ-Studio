#include "Precom.h"
#include "GameOver.h"
#include "EndingSceneBuilder.h"


CGameOver::CGameOver()
	:m_GameOverRen(nullptr), m_GameOverSpeaker(nullptr)
	, m_CurFade(MAXFADE), m_fFadeTime(2.f)
	, m_fAccTime(0.f),m_bFadeUpdate(false)
	, m_bKeyCheck(false), m_fMaxVol(0.3f)
{
}


CGameOver::~CGameOver()
{
}
void CGameOver::Init()
{
	m_GameOverRen = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_MAPOBJECT, 1.f, true));
	m_GameOverRen->SetSprite(_T("GameOver"));
	m_GameOverRen->SetAlpha(0.f);

	m_GameOverSpeaker = ACTOR->CreateCom<CSoundPlayer>();
}
void CGameOver::InitData(void* _pEndingBuilder)
{
	if (nullptr == _pEndingBuilder)
		return;

	m_pEndingBuilder = (CEndingSceneBuilder*)_pEndingBuilder;
	return;
}
void CGameOver::Update()
{
	if (m_bFadeUpdate)
	{
		switch (m_CurFade)
		{
		case CGameOver::FADE_IN:
			FadeInUpdate();
			break;
		case CGameOver::FADE_OUT:
			FadeOutUpdate();
			break;
		}

		m_fAccTime += DELTATIME;
		return;
	}
	else if (m_bKeyCheck)
		KeyCheck();
	

}
void CGameOver::FadeInUpdate() 
{
	if (m_fAccTime >= m_fFadeTime)
	{
		m_bFadeUpdate = false;
		m_bKeyCheck = true;
		m_GameOverRen->SetAlpha(1.f);
		m_GameOverSpeaker->SetVolume(m_fMaxVol);
	}

	float t = m_fAccTime / m_fFadeTime;
	float Fade = t;

	m_GameOverRen->SetAlpha(Fade);
	m_GameOverSpeaker->SetVolume(Fade*m_fMaxVol);
}
void CGameOver::FadeOutUpdate() 
{
	if (m_fAccTime >= m_fFadeTime + 0.5f)
	{
		m_bFadeUpdate = false;
		m_GameOverSpeaker->CurPlaySoundStop();
		m_GameOverSpeaker->ChannelRelease();
		m_pEndingBuilder->ChangeTitleSceneEvent();
	}

	float t = m_fAccTime / m_fFadeTime;
	float Fade =  1.f - t;

	m_GameOverRen->SetAlpha(max(0.f,Fade));
	m_GameOverSpeaker->SetVolume(max(0.f, Fade*m_fMaxVol));
}
void CGameOver::FadeEvent(const bool& _FadeIn) 
{
	m_bFadeUpdate = true;
	m_bKeyCheck = false;
	m_fAccTime = 0.f;

	if (_FadeIn) 
	{
		m_GameOverSpeaker->PlaySound(_T("GameOver"));
		m_GameOverSpeaker->SetVolume(0.f);
		m_CurFade = FADE_IN;
	}
	else
		m_CurFade = FADE_OUT;
}
void CGameOver::KeyCheck()
{
	if (TRUE == KEYDOWN("Enter") || TRUE == KEYDOWN("Space")) 
	{
		m_bKeyCheck = false;
		FadeEvent(false);
	}
}