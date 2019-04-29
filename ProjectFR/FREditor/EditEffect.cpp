#include "stdafx.h"
#include "EditEffect.h"
#include <GameSound.h>

bool			CEditEffect::bCurCharacterSkill = true;

void CEditEffect::SettingCurEffectLayer(const bool& _bChar)
{
	bCurCharacterSkill = _bChar;
}
CEditEffect::CEditEffect()
	:m_EffectRen(nullptr), m_EffectName(_T(""))
	, m_fLifeTime(0.0f), m_fAccTime(0.0f), m_fPlayRender(0.0f)
	, m_bOriginal(false), m_fImageMag(0.0f)
	, m_CustomSize(Vec2(0.0f, 0.0f))
	, m_bBlackBack(false), m_bNonCamAffect(false)
	, m_iRenLayer(0), m_bRendering(false)
	, m_bLoop(false), m_EffectIndex(-1)
	, m_MonTurnEffectRen(nullptr)
	, m_CurActiveRen(nullptr)
{
	m_EffectSound.clear();
}


CEditEffect::~CEditEffect()
{
	m_EffectSound.clear();
}
void CEditEffect::InitData(const EFFECTDATA& _EffectData)
{
	m_EffectName = _EffectData.EffectName;

	m_EffectAniName = _EffectData.EffectAniName;


	ANIMATIONDATA* Check = CEditGlobal::GetAniData(_EffectData.EffectAniName);

	if (nullptr == Check)
	{
		assert(Check);
		return;
	}

	m_EffectIndex = _EffectData.EffectIndex;

	m_fLifeTime = _EffectData.LifeTime;
	m_fPlayRender = _EffectData.PlayRender;

	m_fImageMag = _EffectData.ImageScale;
	m_CustomSize = _EffectData.CustomSize;

	m_iRenLayer = _EffectData.RenderLayer;

	m_bLoop = _EffectData.Loop;
	m_bBlackBack = _EffectData.BlackBack;
	m_bNonCamAffect = _EffectData.NonCameraAffect;

	// 랜더러의 배경색 OR 카메라 흔들림 여부를 랜더러에 셋팅한다.
	if (true == m_bBlackBack && true == m_bNonCamAffect)
	{
		m_EffectRen = ACTOR->CreateCom<CAniRenderer>(RENDATA(m_iRenLayer, 0.9f, { 0.0f,0.0f,0.0f }, true));
		m_MonTurnEffectRen = ACTOR->CreateCom<CAniRenderer>(RENDATA(LAYER_FRONTEFFECT, 0.9f, { 0.0f,0.0f,0.0f }, true));
	}
	else if (true == m_bBlackBack && false == m_bNonCamAffect)
	{
		m_EffectRen = ACTOR->CreateCom<CAniRenderer>(RENDATA(m_iRenLayer, 0.9f, { 0.0f,0.0f,0.0f }));
		m_MonTurnEffectRen = ACTOR->CreateCom<CAniRenderer>(RENDATA(LAYER_FRONTEFFECT, 0.9f, { 0.0f,0.0f,0.0f }));
	}
	else if (false == m_bBlackBack && true == m_bNonCamAffect)
	{
		m_EffectRen = ACTOR->CreateCom<CAniRenderer>(RENDATA(m_iRenLayer, 0.9f, true));
		m_MonTurnEffectRen = ACTOR->CreateCom<CAniRenderer>(RENDATA(LAYER_FRONTEFFECT, 0.9f, true));
	}
	else
	{
		m_EffectRen = ACTOR->CreateCom<CAniRenderer>(RENDATA(m_iRenLayer, 0.9f));
		m_MonTurnEffectRen = ACTOR->CreateCom<CAniRenderer>(RENDATA(LAYER_FRONTEFFECT, 0.9f));
	}

	if (m_CustomSize.x > 0.0f &&m_CustomSize.y > 0.0f)
	{
		m_EffectRen->SetCustomSize(m_CustomSize);
		m_MonTurnEffectRen->SetCustomSize(m_CustomSize);
	}
	// 위에서 커스텀 사이즈가 셋팅 되어있을경우 이 값은 무시될것이다.
	if (m_fImageMag >0.0f)
	{
		ACTOR->SetImageScale(m_fImageMag);
	}


	m_EffectRen->CreateAnimation(CEditGlobal::GetAniData(m_EffectAniName));
	m_EffectRen->ChangeAni(m_EffectAniName);

	m_MonTurnEffectRen->CreateAnimation(CEditGlobal::GetAniData(m_EffectAniName));
	m_MonTurnEffectRen->ChangeAni(m_EffectAniName);
	
	//m_EffectRen->SetOverlapCount(2);

	m_SoundList.reserve(EFFECTDATA::MAXSOUNDCOUNT);


	for (int i = 0; i < _EffectData.SoundCount; i++)
	{
		m_SoundList.push_back(SoundData(_EffectData.SoundName[i], _EffectData.SoundStartTime[i]));
		m_EffectSound.push_back(ACTOR->CreateCom<CSoundPlayer>());
	}


	ACTOR->Off();
}
void CEditEffect::InitData(SPTR<CEditEffect> _EffectData)
{

}
void CEditEffect::Init()
{

}
void CEditEffect::PreUpdate()
{

	// 랜더링 , 사운드 플레이 시간이 되면 컴포넌트를 킨다.

	if (m_fAccTime >= m_fPlayRender && m_bRendering == false)
	{
		m_CurActiveRen->On();
		m_bRendering = true;
	}


	for (int i = 0; i < min((int)m_SoundList.size(), EFFECTDATA::MAXSOUNDCOUNT); i++)
	{
		if (i >= 5)
		{
			int a = 0;
		}

		if (true == m_SoundList[i].Playing)
		{
			continue;
		}
		else
		{
			if (m_SoundList[i].StartTime <= m_fAccTime)
			{
				m_SoundQueue.push(Sound(m_SoundList[i].SoundName, i));
				m_SoundList[i].Playing = true;
			}
		}
	}

}
void CEditEffect::Update()
{
	while (0 != m_SoundQueue.size())
	{
		m_EffectSound[m_SoundQueue.front().PlayerIndex]->PlaySound(m_SoundQueue.front().SoundKey);
		m_SoundQueue.pop();
	}

	if (false == m_bLoop && true == m_CurActiveRen->IsEndAni())
		m_CurActiveRen->Off();
	

	if (m_fLifeTime <= m_fAccTime)
	{
		while (false == m_SoundQueue.empty())
		{
			m_SoundQueue.pop();
		}

		InitSoundList();

		AllSoundStop();
		ACTOR->Off();
		m_fAccTime = 0.0f;
		m_bRendering = false;
		return;
	}

	m_fAccTime += DELTATIME;
}
tstring CEditEffect::GetEffectName()
{
	return m_EffectName;
}
bool CEditEffect::IsPlay()
{
	return ACTOR->IsActive();
}
bool CEditEffect::IsRestEffect()
{
	// ACTOR 가 Active면 쉬는 Effect가 아니다
	return !(ACTOR->IsActive());
}
void CEditEffect::SetOriginal()
{
	m_bOriginal = true;
}
void CEditEffect::EffectOn()
{
	if (true == bCurCharacterSkill) 
	{
		m_CurActiveRen = m_EffectRen;
		m_MonTurnEffectRen->Off();
	}
	else 
	{
		m_CurActiveRen = m_MonTurnEffectRen;
		m_EffectRen->Off();
	}

	m_fAccTime = 0.0f;

	ACTOR->On();

	m_CurActiveRen->On();
	m_bRendering = true;

	if (m_fPlayRender > 0.0f)
	{
		m_bRendering = false;
		m_CurActiveRen->Off();
	}

	m_CurActiveRen->CurAniReset();

	InitSoundList();

}
void CEditEffect::EffectOff()
{
	ACTOR->Off();
	AllSoundStop();
}
void CEditEffect::InitSoundList()
{
	for (size_t i = 0; i < m_SoundList.size(); i++)
	{
		m_SoundList[i].Playing = false;
	}

	while (false == m_SoundQueue.empty())
	{
		m_SoundQueue.pop();
	}
}

EFFECTDATA  CEditEffect::GetEffectData()
{

	EFFECTDATA Data = EFFECTDATA(m_EffectName, m_EffectAniName);

	for (size_t i = 0; i < m_SoundList.size(); i++)
	{
		Data.AddSoundData(m_SoundList[i].SoundName, m_SoundList[i].StartTime);
	}
	Data.SoundCount = (int)m_SoundList.size();
	Data.LifeTime = m_fLifeTime;
	Data.ImageScale = m_fImageMag;
	Data.PlayRender = m_fPlayRender;
	Data.CustomSize = m_CustomSize;
	Data.RenderLayer = m_iRenLayer;
	Data.Loop = m_bLoop;
	Data.BlackBack = m_bBlackBack;
	Data.NonCameraAffect = m_bNonCamAffect;
	Data.EffectIndex = (UINT)m_EffectIndex;

	return Data;
}
const float CEditEffect::GetEffectPlayTime()
{
	return m_CurActiveRen->GetFullAnimationTime();
}
BOOL CEditEffect::ChangeAnimationData()
{
	ANIMATIONDATA* AniData = CEditGlobal::GetAniData(m_EffectAniName);

	if (nullptr == AniData)
	{
		assert(nullptr);
		return FALSE;
	}

	m_EffectRen->EraseAnimation(m_EffectAniName);
	m_EffectRen->CreateAnimation(AniData->GetAniData());
	m_EffectRen->ChangeAni(m_EffectAniName);

	m_MonTurnEffectRen->EraseAnimation(m_EffectAniName);
	m_MonTurnEffectRen->CreateAnimation(AniData->GetAniData());
	m_MonTurnEffectRen->ChangeAni(m_EffectAniName);

	return TRUE;
}
void CEditEffect::AllSoundStop()
{
	for (size_t i = 0; i < m_EffectSound.size(); i++)
	{
		m_EffectSound[i]->Stop();
	}
}

void CEditEffect::SetSkillEffectData(const SKILLEFFECTDATA& _Data, const Vec2& _CasterPos)
{
	//ACTOR->SetPos(_Data.InitPos);

	switch (_Data.InitPosMode)
	{
	case SKILLEFFECTDATA::INITPOS_CASTERPOS:
	{
		ACTOR->SetPos(_CasterPos + _Data.InitPos);
	}
	break;
	case SKILLEFFECTDATA::INITPOS_CUSTOM:
	default:
	{
		ACTOR->SetPos(_Data.InitPos);
	}
	break;
	}

	if (0 != _Data.EffectCustomSize.x && 0 != _Data.EffectCustomSize.y)
	{
		m_EffectRen->SetCustomSize(_Data.EffectCustomSize);
		m_MonTurnEffectRen->SetCustomSize(_Data.EffectCustomSize);
	}
	if (0 != _Data.EffectImageMag)
	{
		ACTOR->SetImageScale(_Data.EffectImageMag);
	}

	EffectOn();
}
void CEditEffect::SetHitEffectData(const HITDATA& _HitData)
{
	if (0 != _HitData.HitEffectCustomSize.x && 0 != _HitData.HitEffectCustomSize.y)
	{
		m_EffectRen->SetCustomSize(_HitData.HitEffectCustomSize);
		m_MonTurnEffectRen->SetCustomSize(_HitData.HitEffectCustomSize);
	}
	if (0 != _HitData.HitEffectImageMag)
	{

		ACTOR->SetImageScale(_HitData.HitEffectImageMag);
	}

	EffectOn();
}
void CEditEffect::SetPos(const Vec2& _Pos)
{
	ACTOR->SetPos(_Pos);
}
unsigned int CEditEffect::GetEffectIndex()
{
	return (UINT)m_EffectIndex;
}
void CEditEffect::SetRenPivot(const Vec2& _Pivot)
{
	m_EffectRen->SetRenPivot(_Pivot);
	m_MonTurnEffectRen->SetRenPivot(_Pivot);
}
void CEditEffect::SetAlpha(const float& _Alpha)
{
	m_MonTurnEffectRen->SetAlpha(_Alpha);
	m_EffectRen->SetAlpha(_Alpha);
}
void CEditEffect::SetCustomSize(const Vec2& _CustomSize)
{
	m_EffectRen->SetCustomSize(_CustomSize);
	m_MonTurnEffectRen->SetCustomSize(_CustomSize); 
}
void CEditEffect::SetImageScale(const float& _ImageMag)
{
	ACTOR->SetImageScale(_ImageMag);
}
