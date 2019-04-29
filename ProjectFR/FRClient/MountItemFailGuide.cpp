#include "Precom.h"
#include "MountItemFailGuide.h"


CMountItemFailGuide::CMountItemFailGuide()
	:m_fAccTime(0.f), m_GuideBackRen(nullptr)
	, m_FailRen(nullptr)
{
}


CMountItemFailGuide::~CMountItemFailGuide()
{
}
void CMountItemFailGuide::Init() 
{
	m_GuideBackRen = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_UIBACK, 0.9f, true));
	m_GuideBackRen->SetSprite(_T("ColorBase"));
	m_GuideBackRen->SetMaterial(D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.f));
	m_GuideBackRen->MaterialOn();
	m_GuideBackRen->SetCustomSize(Vec2{ 1024.f,64.f });

	m_FailRen = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_UI, 1.f, true));
	m_FailRen->SetSprite(_T("MountItemFail"));
	m_FailRen->SetMaterial(D3DXCOLOR(1.0f, 0.1f, 0.1f, 1.f));
	m_FailRen->MaterialOn();

	ACTOR->Off();
}
void CMountItemFailGuide::Update() 
{
	if (m_fAccTime >= 1.f)
	{
		ACTOR->Off();
		m_GuideBackRen->SetAlpha(0.9f);
		m_FailRen->SetAlpha(1.f);
		return;
	}

	float w = powf(m_fAccTime / 1.f, 3.f);
	float Alpha = 1.f - w;

	m_GuideBackRen->SetAlpha(0.9f*Alpha);
	m_FailRen->SetAlpha(Alpha);

	m_fAccTime += DELTATIME;
}
void CMountItemFailGuide::StandBy()
{
	ACTOR->On();
	m_fAccTime = 0.f;
}