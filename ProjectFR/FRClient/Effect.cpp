#include "Precom.h"
#include "Effect.h"
#include <GameSound.h>

bool					CEffect::bCurCharacterSkill = true;

void CEffect::SettingCurEffectLayer(const bool& _bChar)
{
	bCurCharacterSkill = _bChar;
}
CEffect::CEffect()
	:m_EffectRen(nullptr), m_EffectName(_T(""))
	, m_fLifeTime(0.0f), m_fAccTime(0.0f), m_fPlayRender(0.0f)
	, m_bOriginal(false), m_fImageMag(0.0f)
	, m_CustomSize(Vec2(0.0f, 0.0f))
	, m_bBlackBack(false), m_bNonCamAffect(false)
	, m_iRenLayer(0), m_bRendering(false)
	, m_bLoop(false),m_EffectIndex(-1)
	, m_PosRen(nullptr)
	, m_MonsterEffectRen(nullptr), m_CurActiveRen(nullptr)
{
	m_EffectSound.clear();
}


CEffect::~CEffect()
{
	m_EffectSound.clear();
}
void CEffect::InitData(const EFFECTDATA& _EffectData)
{
	m_EffectName = _EffectData.EffectName;

	m_EffectAniName = _EffectData.EffectAniName;


	ANIMATIONDATA* Check  = CClientGlobal::GetAniData(_EffectData.EffectAniName);

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
		m_EffectRen = ACTOR->CreateCom<CAniRenderer>(RENDATA(m_iRenLayer, 1.0f, { 0.0f,0.0f,0.0f }, true));
		m_MonsterEffectRen = ACTOR->CreateCom<CAniRenderer>(RENDATA(LAYER_FRONTEFFECT, 1.0f, { 0.0f,0.0f,0.0f }, true));
	}
	else if (true == m_bBlackBack && false == m_bNonCamAffect)
	{
		m_EffectRen = ACTOR->CreateCom<CAniRenderer>(RENDATA(m_iRenLayer, 1.0f, { 0.0f,0.0f,0.0f }));
		m_MonsterEffectRen = ACTOR->CreateCom<CAniRenderer>(RENDATA(LAYER_FRONTEFFECT, 1.0f, { 0.0f,0.0f,0.0f }));
	}
	else if (false == m_bBlackBack && true == m_bNonCamAffect)
	{
		m_EffectRen = ACTOR->CreateCom<CAniRenderer>(RENDATA(m_iRenLayer, 1.0f, true));
		m_MonsterEffectRen = ACTOR->CreateCom<CAniRenderer>(RENDATA(LAYER_FRONTEFFECT, 1.0f, true));
	}
	else
	{
		m_EffectRen = ACTOR->CreateCom<CAniRenderer>(RENDATA(m_iRenLayer, 1.0f));
		m_MonsterEffectRen = ACTOR->CreateCom<CAniRenderer>(RENDATA(LAYER_FRONTEFFECT, 1.0f));
	}

	if (m_CustomSize.x > 0.0f &&m_CustomSize.y > 0.0f)
	{
		SetCustomSize(m_CustomSize);
	}
	// 위에서 커스텀 사이즈가 셋팅 되어있을경우 이 값은 무시될것이다.
	if (m_fImageMag >0.0f)
	{
		ACTOR->SetImageScale(m_fImageMag);
	}


	m_EffectRen->CreateAnimation(CClientGlobal::GetAniData(m_EffectAniName));
	m_EffectRen->ChangeAni(m_EffectAniName);

	m_MonsterEffectRen->CreateAnimation(CClientGlobal::GetAniData(m_EffectAniName));
	m_MonsterEffectRen->ChangeAni(m_EffectAniName);

	//m_EffectRen->SetOverlapCount(2);

	m_SoundList.reserve(EFFECTDATA::MAXSOUNDCOUNT);


	for (int i = 0; i < _EffectData.SoundCount; i++)
	{
		m_SoundList.push_back(SoundData(_EffectData.SoundName[i], _EffectData.SoundStartTime[i]));
		m_EffectSound.push_back( ACTOR->CreateCom<CSoundPlayer>());
	}


	ACTOR->Off();
}
void CEffect::InitData(SPTR<CEffect> _EffectData)
{

}
void CEffect::Init()
{
	/*m_PosRen = ACTOR->CreateCom<CLineRenderer>(RENDATA(LAYER_UIBACK, 0.5f));
	m_PosRen->SetColor(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
	m_PosRen->SetLineThick(2.0f);*/
	//SyncroDebugLine();
}
void CEffect::PreUpdate()
{
	
	// 랜더링 , 사운드 플레이 시간이 되면 컴포넌트를 킨다.

	if (m_fAccTime >= m_fPlayRender && m_bRendering == false)
	{
		m_CurActiveRen->On();
		m_bRendering = true;
	}


	for (int i = 0; i < min((int)m_SoundList.size(),EFFECTDATA::MAXSOUNDCOUNT); i++)
	{
		if (true == m_SoundList[i].Playing)
			continue;
		else
		{
			if (m_SoundList[i].StartTime <= m_fAccTime)
			{
				m_SoundQueue.push(Sound(m_SoundList[i].SoundName,i));
				m_SoundList[i].Playing = true;
			}
		}
	}

}
void CEffect::Update()
{
	while (0 != m_SoundQueue.size())
	{
		m_EffectSound[m_SoundQueue.front().PlayerIndex]->PlaySound(m_SoundQueue.front().SoundKey);
		m_SoundQueue.pop();
	}

	if (false == m_bLoop && true == m_CurActiveRen->IsEndAni())
	{
		m_CurActiveRen->Off();
	}

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
void CEffect::PostUpdate()
{
	/*if (true == TRANS->ChangeTranspose())
		SyncroDebugLine();
*/
}
tstring CEffect::GetEffectName()
{
	return m_EffectName;
}
bool CEffect::IsPlay()
{
	return ACTOR->IsActive();
}
bool CEffect::IsRestEffect()
{
	// ACTOR 가 Active면 쉬는 Effect가 아니다
	return !(ACTOR->IsActive());
}
void CEffect::SetOriginal()
{
	m_bOriginal = true;
}
void CEffect::EffectOn()
{
	SetAlpha(1.0f);

	if (true == bCurCharacterSkill) 
	{
		m_CurActiveRen = m_EffectRen;
		m_MonsterEffectRen->Off();
	}
	else 
	{
		m_CurActiveRen = m_MonsterEffectRen;
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
void CEffect::EffectOff()
{
	ACTOR->Off();
	AllSoundStop();
	InvalidCustomSize();
	
	if (m_CustomSize.x > 0.0f && m_CustomSize.y > 0.0f)
	{
		SetCustomSize(m_CustomSize);
	}
	else
		ACTOR->SetImageScale(m_fImageMag);
}
void CEffect::InitSoundList()
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

EFFECTDATA  CEffect::GetEffectData()
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
const float CEffect::GetEffectPlayTime()
{
	return m_EffectRen->GetFullAnimationTime();
}
BOOL CEffect::ChangeAnimationData()
{
	ANIMATIONDATA* AniData = CClientGlobal::GetAniData(m_EffectAniName);

	if (nullptr == AniData)
	{
		assert(nullptr);
		return FALSE;
	}

	m_EffectRen->EraseAnimation(m_EffectAniName);
	m_EffectRen->CreateAnimation(AniData->GetAniData());
	m_EffectRen->ChangeAni(m_EffectAniName);
	
	m_MonsterEffectRen->EraseAnimation(m_EffectAniName);
	m_MonsterEffectRen->CreateAnimation(AniData->GetAniData());
	m_MonsterEffectRen->ChangeAni(m_EffectAniName);
	return TRUE;
}
void CEffect::AllSoundStop()
{
	for (size_t i = 0; i < m_EffectSound.size(); i++)
	{
		m_EffectSound[i]->Stop();
	}
}

void CEffect::SetSkillEffectData(const SKILLEFFECTDATA& _Data, const Vec2& _CasterPos)
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
		SetCustomSize(_Data.EffectCustomSize);
	}
	if (0 != _Data.EffectImageMag) 
	{
		ACTOR->SetImageScale(_Data.EffectImageMag);
	}

	EffectOn();
}
void CEffect::SetHitEffectData(const HITDATA& _HitData)
{
	if (0 != _HitData.HitEffectCustomSize.x && 0 != _HitData.HitEffectCustomSize.y)
	{
		m_EffectRen->SetCustomSize(_HitData.HitEffectCustomSize);
	}
	if (0 != _HitData.HitEffectImageMag)
	{	
		
		ACTOR->SetImageScale(_HitData.HitEffectImageMag);
	}

	EffectOn();
}
void CEffect::SetPos(const Vec2& _Pos)
{
	ACTOR->SetPos(_Pos);
}
void CEffect::SetPos(const Vec3& _Pos)
{
	TRANS->SetPos(_Pos);
}
unsigned int CEffect::GetEffectIndex()
{
	return (UINT)m_EffectIndex;
}
void CEffect::SetRenPivot(const Vec2& _Pivot)
{
	m_EffectRen->SetRenPivot(_Pivot);
	m_MonsterEffectRen->SetRenPivot(_Pivot);
}
void CEffect::InvalidCustomSize()
{
	m_EffectRen->InvalidCustomSize();
	m_MonsterEffectRen->InvalidCustomSize();
}
void CEffect::SetCustomSize(const Vec2& _CustomSize)
{
	m_EffectRen->SetCustomSize(_CustomSize);
	m_MonsterEffectRen->SetCustomSize(_CustomSize);
}
void CEffect::SetImageScale(const float& _ImageMag)
{
	ACTOR->SetImageScale(_ImageMag);
}
void CEffect::SetAlpha(const float& _Alpha)
{
	m_EffectRen->SetAlpha(_Alpha);
	m_MonsterEffectRen->SetAlpha(_Alpha);
}
void CEffect::SyncroDebugLine() 
{
	Vec3 StartPos = TRANS->GetPos() + Vec3{ -500.0f,0.0f,0.0f };
	Vec3 EndPos = StartPos + Vec3{ 1000.0f,0.0f,0.0f };

	if (nullptr != m_PosRen)
		m_PosRen->SettingLineData(StartPos, EndPos);
}
void CEffect::UnitPosLineOn() 
{
	if (nullptr != m_PosRen)
		m_PosRen->On();
}
void CEffect::UnitPosLineOff() 
{
	if (nullptr != m_PosRen)
		m_PosRen->Off();
}
void CEffect::ReverseLineRendering()
{
	if(nullptr!= m_PosRen)
		m_PosRen->ReverseRendering();
}