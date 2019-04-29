#include "Precom.h"
#include "BossGauge.h"
#include "BattleUnit.h"
#include <GameSprite.h>


CBossGauge::CBossGauge()
	:m_GaugeBack(nullptr), m_GaugeFrame(nullptr)
	, m_Gauge(nullptr), m_GaugeSprite(nullptr)
	, m_PairUnit(nullptr)
	, m_iCurHp(0)
	, m_iMaxHp(1)
	, m_fAccTime(0.f)
	, m_fFadeTime(1.f)
	, m_CurFade(FADE_IN)
{
	InitVtx();
}


CBossGauge::~CBossGauge()
{
	m_GaugeSprite = nullptr;
}
void CBossGauge::Init()
{
	Vec2 WndSize = Vec2{ (float)CClientGlobal::MainWindow->WndSize().x,(float)CClientGlobal::MainWindow->WndSize().y };
	TRANS->SetPos(Vec3{ 0.f, WndSize.y * 0.44f ,7.f });

	m_GaugeSprite = COMRESMGR->NewGameSprite(_T("BossGauge"));

	m_GaugeBack = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_UIBACK, 0.9f, true));
	m_GaugeBack->SetSprite(_T("ColorBase"));
	m_GaugeBack->SetMaterial(D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.f));
	m_GaugeBack->MaterialOn();
	m_GaugeBack->SetCustomSize(660.f, 40.f);

	m_Gauge = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_UI, 0.9f, true));
	m_Gauge->SetSprite(m_GaugeSprite);

	m_GaugeFrame = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_UI, 0.9f, true));
	m_GaugeFrame->SetSprite(_T("BossGaugeFrame"));
	

}
void CBossGauge::Update()
{
	if (!m_bFadeUpdate)
		return;

	switch (m_CurFade)
	{
	case CBossGauge::FADE_IN:
		FadeInUpdate();
		break;
	case CBossGauge::FADE_OUT:
		FadeOutUpdate();
		break;
	}

	m_fAccTime += DELTATIME;
}
void CBossGauge::FadeInUpdate()
{
	if (m_fAccTime >= m_fFadeTime)
	{
		m_bFadeUpdate = false;
		SetAlpha(1.f);
		return;
	}

	float t = m_fAccTime / m_fFadeTime;
	SetAlpha(t);
}
void CBossGauge::FadeOutUpdate()
{
	if (m_fAccTime >= m_fFadeTime)
	{
		m_bFadeUpdate = false;
		SetAlpha(0.f);
		return;
	}
	float t = 1.f - (m_fAccTime / m_fFadeTime);
	SetAlpha(t);
}
void CBossGauge::FadeEvent(const bool& _FadeIn)
{
	m_bFadeUpdate = true;
	m_fAccTime = 0.f;

	if (_FadeIn)
		m_CurFade = FADE_IN;
	else
		m_CurFade = FADE_OUT;
}
void CBossGauge::StandBy()
{
	//SetAlpha(1.f);
	m_iMaxHp = m_PairUnit->GetMaxHp();
	m_iCurHp = m_PairUnit->GetCurHp();
	UpdateGauge();
	FadeEvent(true);
}
void CBossGauge::InitVtx()
{
	m_CurVtx[0] = VERTEX({ -0.5f, 0.5f, 1.0f }, { 0.0f, 0.0f });
	m_CurVtx[1] = VERTEX({ 0.5f, 0.5f, 1.0f }, { 1.0f,0.0f });
	m_CurVtx[2] = VERTEX({ 0.5f, -0.5f, 1.0f }, { 1.0f,1.0f });
	m_CurVtx[3] = VERTEX({ -0.5f, -0.5f, 1.0f }, { 0.0f,1.0f });
}
void CBossGauge::SetPairUnit(CBattleUnit* _Unit)
{
	if (nullptr == _Unit)
		return;

	m_PairUnit = _Unit;
	m_iMaxHp = m_PairUnit->GetMaxHp();
	m_iCurHp = m_iMaxHp;
}
void CBossGauge::SetAlpha(const float& _Alpha)
{

	m_GaugeBack->SetAlpha(_Alpha*0.9f);
	m_Gauge->SetAlpha(_Alpha*0.9f);
	m_GaugeFrame->SetAlpha(_Alpha*0.9f);
}
void CBossGauge::UpdateHp(const int& _CurHp)
{
	m_iCurHp = max(_CurHp, 0);
	UpdateGauge();
}
void CBossGauge::UpdateGauge()
{
	float t = (float)m_iCurHp / (float)m_iMaxHp;

	VERTEX V1, V2;
	
	V1 = VERTEX({ 0.5f, 0.5f, 1.0f }, { 1.0f,0.0f });
	V2 = VERTEX({ 0.5f, -0.5f, 1.0f }, { 1.0f,1.0f });

	m_CurVtx[1] = LerpVertax(m_CurVtx[0], V1, t);
	m_CurVtx[2] = LerpVertax(m_CurVtx[3], V2, t);

	bool Check = m_GaugeSprite->ChangeVertex(m_CurVtx);
}
void CBossGauge::SetMaxHp(const int& _MaxHp)
{
	m_iMaxHp = max(_MaxHp, 1);
}