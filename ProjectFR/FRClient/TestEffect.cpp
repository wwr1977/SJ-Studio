#include "Precom.h"
#include "TestEffect.h"
#include <GameMultiSprite.h>

CTestEffect::CTestEffect()
	:m_bRendering(false), m_bPlaySound(false)
	, m_fAccTime(0.0f)
{
}


CTestEffect::~CTestEffect()
{
}
void CTestEffect::Init() 
{

}
void CTestEffect::EffectOn()
{
	ACTOR->On();
	TRANS->SetPos(Vec3(-400.0f, 0.0f, 30.0f));

	m_Renderer->CurAniReset();

	if (m_fPlayRender > 0.0f)
	{
		m_bRendering = false;
		m_Renderer->Off();
	}

	if (m_fPlaySound > 0.0f)
	{
		m_bPlaySound = false;
		m_Sound->Off();
	}
}
void CTestEffect::PreUpdate()
{
	if (m_fAccTime >= m_fPlayRender && m_bRendering == false)
	{
		m_Renderer->On();
		m_bRendering = true;
	}

	if (m_fAccTime >= m_fPlaySound && m_bPlaySound == false)
	{
		m_Sound->On();
		m_Sound->PlaySound(m_SoundName);
		m_bPlaySound = true;
	}
}
void CTestEffect::Update() 
{
	if (1 == m_Renderer->GetCurAniCount()) 
	{
		m_Renderer->Off();
	}


	if (m_fLifeTime <= m_fAccTime)
	{
		ACTOR->Off();
		m_Sound->CurPlaySoundStop();
		m_fAccTime = 0.0f;
		m_bRendering = false;
		m_bPlaySound = false;
	}

	m_fAccTime += DELTATIME;
}
void CTestEffect::Setting(const tstring& _EffectName, const tstring& _SoundName, const float& _fLifeTime, const float& _fPlaySound, const float& _fPlayRender /*= 0.0f*/)
{
	m_SoundName = _SoundName;
	
	m_fLifeTime = _fLifeTime;
	m_fPlaySound = _fPlaySound;
	m_fPlayRender = _fPlayRender;

	m_Renderer = ACTOR->CreateCom<CAniRenderer>(RENDATA(LAYER_FRONTEFFECT, 0.9f, {0.0f,0.0f,0.0f}));

	SPTR<CGameMultiSprite> Multi = COMRESMGR->FindMultiSprite(_EffectName);

	if(nullptr ==  Multi)
	{
		return;
	}

	ANIDATA AniData(_EffectName, _EffectName);
	AniData.SettingFullRage({ 0,Multi->GetSpriteLastIndex() }, 0.1f);
	m_Renderer->CreateAnimation(AniData);
	m_Renderer->ChangeAni(_EffectName);

	if (0.0f == m_fLifeTime) 
	{
		size_t Frame = Multi->GetSpriteCount();

		m_fLifeTime = Frame*0.1f;
	}

	m_Sound = ACTOR->CreateCom<CSoundPlayer>();
	ACTOR->SetImageScale(3.0f);
	ACTOR->Off();

}
bool CTestEffect::isLife()
{

	if (true == ACTOR->IsActive() && m_fLifeTime >= m_fAccTime) 
	{
		return true;
	}

	return  false;
}