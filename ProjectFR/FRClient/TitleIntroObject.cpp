#include "Precom.h"
#include "TitleIntroObject.h"
#include "TitleSceneBuilder.h"


CTitleIntroObject::CTitleIntroObject()
	: m_StudioRen(nullptr)
	, m_ScreenTransAni(nullptr)
	, m_ProjectRen(nullptr)
	, m_fAccTime(0.f)
	, m_bEndIntro(true)
	, m_pTitleSceneBuilder(nullptr)
{
}


CTitleIntroObject::~CTitleIntroObject()
{
}
void CTitleIntroObject::Init()
{
	TRANS->SetPos(Vec3{ 0.f,0.f,5.f });

	m_StudioRen = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_MAPOBJECT,1.f,true));
	m_StudioRen->SetSprite(_T("Studio"));
	m_StudioRen->Off();

	m_ProjectRen = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_MAPOBJECT, 1.f, true));
	m_ProjectRen->SetSprite(_T("Project"));
	m_ProjectRen->Off();

	m_ScreenTransAni = ACTOR->CreateCom<CAniRenderer>(RENDATA(LAYER_MAPOBJECT, 1.f, true));
	m_ScreenTransAni->CreateAnimation(_T("ScreenAni"),ANI_LOOP);
	m_ScreenTransAni->Off();

	POINT WndSize = CClientGlobal::MainWindow->WndSize();
	m_ScreenTransAni->SetCustomSize(Vec2{ (float)WndSize.x * 0.8f,(float)WndSize.y * 0.8f });

}
void CTitleIntroObject::InitData(void* _pTitleBuilder)
{
	if (nullptr == _pTitleBuilder)
		return;;

	m_pTitleSceneBuilder = (CTitleSceneBuilder*)_pTitleBuilder;
}
void CTitleIntroObject::Update()
{
	if (true == m_bEndIntro)
		return;

	switch (m_CurUpdate)
	{
	case CTitleIntroObject::STUDIO:
		StudioUpdate();
		return;
	case CTitleIntroObject::SCREENANI:
		ScreenAniUpdate();
		return;
	case CTitleIntroObject::PROJECT:
		ProjectUpdate();
		return;
	}

}
void CTitleIntroObject::StudioUpdate()
{
	if (m_fAccTime >= 2.f) 
	{
		m_CurUpdate = SCREENANI;
		m_fAccTime = 0.f;
		m_StudioRen->Off();
		m_ScreenTransAni->ChangeAni(_T("ScreenAni"));
		m_ScreenTransAni->SetInterval(0.05f);
		m_ScreenTransAni->On();
		return;
	}

	if (m_fAccTime < 0.5f) 
	{
		float Alpha = m_fAccTime / 0.5f;
		m_StudioRen->SetAlpha(Alpha);
	}
	else if (m_fAccTime >= 0.5f && m_fAccTime < 1.5f) 
		m_StudioRen->SetAlpha(1.f);
	else if (m_fAccTime >= 1.5f && m_fAccTime < 2.f) 
	{
		float Alpha = (2.f - m_fAccTime) / 0.5f;
		m_StudioRen->SetAlpha(Alpha);
	}

	m_fAccTime += DELTATIME;
}
void CTitleIntroObject::ScreenAniUpdate()
{
	if (m_fAccTime >= 1.f) 
	{
		m_CurUpdate = PROJECT;
		m_fAccTime = 0.f;
		m_ScreenTransAni->Off();
	}

	if (1 <= m_ScreenTransAni->GetCurAniCount()) 
	{
		if(true == m_ScreenTransAni->IsActive())
			m_ScreenTransAni->Off();

		m_fAccTime += DELTATIME;
	}
}
void CTitleIntroObject::ProjectUpdate()
{
	if (m_fAccTime >= 2.5f)
	{
		m_bEndIntro = true;
		m_ProjectRen->Off();

		if (nullptr != m_pTitleSceneBuilder)
			m_pTitleSceneBuilder->IntroEndEvent();
	
		ACTOR->Off();
		return;
	}

	if (m_fAccTime < 0.5f)
	{
		float Alpha = m_fAccTime / 0.5f;
		m_ProjectRen->SetAlpha(Alpha);
	}
	else if (m_fAccTime >= 0.5f && m_fAccTime < 1.5f)
		m_ProjectRen->SetAlpha(1.f);
	else if (m_fAccTime >= 1.5f && m_fAccTime < 2.f)
	{
		float Alpha = (2.f - m_fAccTime) / 0.5f;
		m_ProjectRen->SetAlpha(Alpha);
	}

	m_fAccTime += DELTATIME;
}
void CTitleIntroObject::Standby()
{
	m_fAccTime = 0.f;
	m_bEndIntro = false;
	m_CurUpdate = STUDIO;

	m_StudioRen->On();
	m_StudioRen->SetAlpha(0.f);

	m_ScreenTransAni->Off();

	m_ProjectRen->On();
	m_ProjectRen->SetAlpha(0.f);
}
const bool CTitleIntroObject::IsIntroEnd()
{
	return m_bEndIntro;
}