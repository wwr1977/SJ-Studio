#include "Precom.h"
#include "TitleBackGround.h"
#include "TitleSceneBuilder.h"


CTitleBackGround::CTitleBackGround()
	: m_MainTitleRen(nullptr)
	, m_GameStartRen(nullptr)
	, m_TitleSpeaker(nullptr)
	, m_pTitleSceneBuilder(nullptr)
	, m_fAccTime(0.f)
	, m_bFade(false)
	, m_CurFade(FADE_OUT)
	, m_TitleBackRen(nullptr)
	, m_StartFrameRen(nullptr)
	, m_StartBackFrameRen(nullptr)
	, m_CurColor(NOCOLOR)
	, m_bGameStart(false)
{
	m_SelectColor[NOCOLOR] = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.f);
	m_SelectColor[COLOR] = D3DXCOLOR(1.f, 0.5f, 0.1f, 1.f);
}


CTitleBackGround::~CTitleBackGround()
{
}
void CTitleBackGround::Init()
{
	Vec2 WndSize = Vec2{ (float)CClientGlobal::MainWindow->WndSize().x,(float)CClientGlobal::MainWindow->WndSize().y };

	ACTOR->SetImageScale(1.2f);
	/*m_TitleBackRen = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_MAPOBJECT, 1.f, true));
	m_TitleBackRen->SetSprite(_T("ColorBase"));
	m_TitleBackRen->SetCustomSize(Vec2{ 1500.f,1000.f });
	m_TitleBackRen->SetAlpha(0.f);*/

	m_MainTitleRen = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_MAPOBJECT, 1.f, true));
	m_MainTitleRen->SetSprite(_T("GameTitle"));
	m_MainTitleRen->SetAlpha(0.f);
	m_MainTitleRen->SetRenPivot(Vec2{ 0.f,20.f });

	

	m_StartBackFrameRen = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_MAPOBJECT, 0.5f, true));
	m_StartBackFrameRen->SetSprite(_T("ColorBase"));
	m_StartBackFrameRen->SetRenPivot(Vec2{ 13.f,WndSize.y *-0.455f });
	m_StartBackFrameRen->SetMaterial(D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.f));
	m_StartBackFrameRen->MaterialOn();
	m_StartBackFrameRen->SetCustomSize(Vec2{ 330.f,48.f });
	m_StartBackFrameRen->SetAlpha(0.f);

	m_StartFrameRen = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_MAPOBJECT, 1.f, true));
	m_StartFrameRen->SetSprite(_T("GameStartFrame"));
	m_StartFrameRen->SetRenPivot(Vec2{ 10.f,WndSize.y *-0.455f });
	m_StartFrameRen->SetCustomSize(Vec2{ 512.f,58.f });
	m_StartFrameRen->SetAlpha(0.f);

	m_GameStartRen = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_MAPOBJECT, 1.f, true));
	m_GameStartRen->SetSprite(_T("GameStart"));
	m_GameStartRen->SetRenPivot(Vec2{ 0.f,WndSize.y *-0.455f });
	m_GameStartRen->SetCustomSize(Vec2{ 512.f,76.f });
	m_GameStartRen->SetAlpha(0.f);

	m_TitleSpeaker = ACTOR->CreateCom<CSoundPlayer>();
}
void CTitleBackGround::InitData(void* _pBuilder)
{
	

	if (nullptr == _pBuilder)
		return;

	m_pTitleSceneBuilder = (CTitleSceneBuilder*)_pBuilder;
}
void CTitleBackGround::Update()
{
	if (m_bGameStart)
	{
		StartUpdate();
		return;
	}

	if (false == m_bFade)
		return;

	switch (m_CurFade)
	{
	case CTitleBackGround::FADE_OUT:
		FadeOutEvent();
		break;
	case CTitleBackGround::FADE_IN:
		FadeInEvent();
		break;
	}

}
void CTitleBackGround::FadeOutEvent() 
{
	if (1.f <= m_fAccTime) 
	{
		m_bFade = false;
		m_MainTitleRen->SetAlpha(0.f);
		m_GameStartRen->SetAlpha(0.f);
		m_StartFrameRen->SetAlpha(0.f);
		m_StartBackFrameRen->SetAlpha(0.f);
		m_fAccTime = 0.f;
		//m_TitleBackRen->SetAlpha(0.f);
		return;
	}

	float Alpha = 1.f - m_fAccTime;
	m_MainTitleRen->SetAlpha(Alpha);
	m_GameStartRen->SetAlpha(Alpha);
	m_StartFrameRen->SetAlpha(Alpha);
	m_StartBackFrameRen->SetAlpha(Alpha*0.5f);
	//m_TitleBackRen->SetAlpha(Alpha);

	m_fAccTime += DELTATIME;
}
void CTitleBackGround::FadeInEvent()
{
	if (1.f <= m_fAccTime)
	{
		m_bFade = false;
		m_MainTitleRen->SetAlpha(1.f);
		m_GameStartRen->SetAlpha(1.f);
		m_StartFrameRen->SetAlpha(1.f);
		m_StartBackFrameRen->SetAlpha(0.5f);
		m_fAccTime = 0.f;
		m_pTitleSceneBuilder->SetKeyCheck(true);
		//m_TitleBackRen->SetAlpha(1.f);
		return;
	}

	float Alpha = m_fAccTime;
	m_MainTitleRen->SetAlpha(Alpha);
	m_GameStartRen->SetAlpha(Alpha);
	m_StartFrameRen->SetAlpha(Alpha);
	m_StartBackFrameRen->SetAlpha(Alpha*0.5f);
	//m_TitleBackRen->SetAlpha(Alpha);

	m_fAccTime += DELTATIME;
}
void CTitleBackGround::FadeEvent(const bool _FadeOut)
{	
	m_bFade = true;
	
	if (_FadeOut)
		m_CurFade = FADE_OUT;
	else
		m_CurFade = FADE_IN;

	m_fAccTime = 0.f;
}
void CTitleBackGround::GameStartEvent()
{
	m_bGameStart = true;
	m_fAccTime = 0.f;
	m_TitleSpeaker->PlaySound(_T("GameStart"));
}
void CTitleBackGround::StartUpdate()
{

	if (m_fAccTime < 0.5f) 
	{
		if (TRUE == CTimerMgr::Pulse(0.04f))
		{
			m_CurColor = (COLORINDEX)((m_CurColor + 1) % 2);
			m_StartBackFrameRen->SetMaterial(m_SelectColor[m_CurColor]);
		}
	}
	else if (m_fAccTime >=1.5f && m_fAccTime < 2.5f)
	{
		float Alpha = 2.5f - m_fAccTime;
		m_MainTitleRen->SetAlpha(Alpha);
		m_GameStartRen->SetAlpha(Alpha);
		m_StartFrameRen->SetAlpha(Alpha);
		m_StartBackFrameRen->SetAlpha(Alpha*0.5f);
	}
	else if (m_fAccTime >= 3.f) 
	{
		m_MainTitleRen->SetAlpha(0.f);
		m_GameStartRen->SetAlpha(0.f);
		m_StartFrameRen->SetAlpha(0.f);
		m_StartBackFrameRen->SetAlpha(0.f);
		m_StartBackFrameRen->SetMaterial(m_SelectColor[NOCOLOR]);
		m_bGameStart = false;
	}

	m_fAccTime += DELTATIME;
}
const bool CTitleBackGround::IsStartUpdateEnd()
{
	return !m_bGameStart;
}