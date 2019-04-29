#include "stdafx.h"
#include "GameScene.h"
#include "SceneBuilder.h"
#include "SceneCollisionMgr.h"


CSceneBuilder::CSceneBuilder()
	:m_pScene(nullptr), m_pResMgr(nullptr)
	, m_EffectFlag(0), m_SceneEffectFunc(nullptr), m_bSceneEffect(false)
	, m_fSceneEffectTime(0.0f), m_fAccSceneEffectTime(0.0f)
	, m_SpaceSize(Vec2{ 0.0f,0.0f })
	, m_ZoomPos(Vec3{0.0f,0.0f,0.0f}),m_fZoomRatio(0.0f)
	, m_ZoomRatio(Vec2{0.0f,0.0f})
	, m_fPow(1.f)
{
}


CSceneBuilder::~CSceneBuilder()
{
}
void CSceneBuilder::SceneUpdate()
{
	
	if (nullptr != m_SceneEffectFunc && true == m_bSceneEffect) 
	{
		m_SceneEffectFunc();

		m_fAccSceneEffectTime += DELTATIME;
		m_fAccSubTime += DELTATIME;
	}

}
void CSceneBuilder::SceneLastUpdate()
{

}
void CSceneBuilder::CreateMainCamera()
{
	if (nullptr == m_pScene)
	{
		exit(1);
		assert(nullptr);
		return;
	}

	m_pScene->CreateMainCamera();
}
void CSceneBuilder::CreateMainLight()
{
	if (nullptr == m_pScene) 
	{
		assert(nullptr);
		return;
	}
	m_pScene->CreateMainLight();
}
SPTR<CCamera>	CSceneBuilder::GetMainCam()
{
	return GetScene()->GetMainCamCom();
}
CGameScene* CSceneBuilder::GetScene()
{
	return m_pScene;
}
CActObject* CSceneBuilder::CreateActObject(const tstring& _ActName/* = _T("")*/)
{
	return CActObject::CreateActObject(_ActName, m_pScene);
}
void CSceneBuilder::CreateCollisionGroup(const tstring& _Src, const tstring& _Dest)
{
	m_pScene->GetCollisionMgr()->CreateCheckGroup(_Src, _Dest);
}
void CSceneBuilder::SceneRender()
{

}
void CSceneBuilder::ScenenCurStateMsg()
{
	int Count = GetScene()->GetRenderOnComponent();
	TCHAR msg[MAXKEYLENGTH] = _T("");
	_stprintf_s(msg, _T("Render On : %d"), Count);
	RESMGR->DrawFont(_T("Å«±Ã¼­"), msg, { 100.0f,350.0f,10.0f }, 1.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
}
bool CSceneBuilder::SceneEffect(DWORD _Flag, const float& _Time)
{
	DWORD EffectMode = _Flag & 0xFFFF0000L;

	switch (EffectMode)
	{
	case SCENE_EFFECT_EARTHQUAKE | EARTHQUAKE_UPDOWN :
		SetSceneEffect(this, &CSceneBuilder::YEarthquake); break;
	case SCENE_EFFECT_EARTHQUAKE | EARTHQUAKE_LEFTRIGHT:
		SetSceneEffect(this, &CSceneBuilder::XEarthquake); break;
	case SCENE_EFFECT_EARTHQUAKE | EARTHQUAKE_CIRCLESPACE:
		SetSceneEffect(this, &CSceneBuilder::CircleEarthquake); break;
	case SCENE_EFFECT_EARTHQUAKE | EARTHQUAKE_RECTSPACE:
		SetSceneEffect(this, &CSceneBuilder::RectEarthquake); break;
	case SCENE_EFFECT_BLACKOUT:
	case SCENE_EFFECT_FLASH:
	{
		SetSceneEffect(this, &CSceneBuilder::BlackOut);
		InitBlackOut();
	}
	break;
	case SCENE_EFFECT_FADE_IN:
	case SCENE_EFFECT_FADE_OUT:
		SetSceneEffect(this, &CSceneBuilder::FadeEffect);
		break;
	case SCENE_EFFECT_ZOOM_IN:
	case SCENE_EFFECT_ZOOM_OUT:
		SetSceneEffect(this, &CSceneBuilder::ZoomEffect); 
		break;
	default:
		return false;
	}

	m_EffectFlag = _Flag;
	m_fSceneEffectTime = _Time;
	m_fAccSceneEffectTime = 0.0f;
	m_bSceneEffect = true;
	return true;
}
void CSceneBuilder::SetEarthquakeData(const Vec2& _SpaceSize, const float& _PulseTime /*= 0.1f*/)
{
	m_SpaceSize = _SpaceSize;
	GetScene()->GetMainCamCom()->InitVibration(_PulseTime);
}
const Vec2 CSceneBuilder::GetSpaceSize(DWORD _Power)
{
	float weight = m_fAccSceneEffectTime / m_fSceneEffectTime;
	switch (_Power)
	{
	case EARTHQUAKE_CONSISTENT: 
		return m_SpaceSize;
	case EARTHQUAKE_INCREASE:
		return CSJMath::Vec2Lerp(&Vec2({ 0.0f,0.0f }), &m_SpaceSize, weight);
	case EARTHQUAKE_DECREASE:
		return CSJMath::Vec2Lerp(&m_SpaceSize, &Vec2({ 0.0f,0.0f }), weight);
	}
	return Vec2{ 0.0f,0.0f };
}
void CSceneBuilder::YEarthquake()
{
	if (m_fAccSceneEffectTime >= m_fSceneEffectTime) 
	{
		GetMainCam()->VibrationOff();
		m_bSceneEffect = false;
		m_SceneEffectFunc = nullptr;
		return;
	}

	DWORD Power = m_EffectFlag & 0x0000FF00L;
	Vec2  Weight = Vec2{ 0.0f ,CSJRandom::RandomFloat(-1.0f,1.0f) };
	Vec2  MaxRadius = GetSpaceSize(Power);


	GetScene()->GetMainCamCom()->SetVibSpaceSize(Vec2(Weight.x* MaxRadius.x, Weight.y* MaxRadius.y));
}
void CSceneBuilder::XEarthquake() 
{
	if (m_fAccSceneEffectTime >= m_fSceneEffectTime)
	{
		GetMainCam()->VibrationOff();
		m_bSceneEffect = false;
		m_SceneEffectFunc = nullptr;
		return;
	}

	DWORD Power = m_EffectFlag & 0x0000FF00L;
	Vec2  Weight = Vec2{ CSJRandom::RandomFloat(-1.0f,1.0f) ,0.0f};
	Vec2  MaxRadius = GetSpaceSize(Power);

	GetScene()->GetMainCamCom()->SetVibSpaceSize(Vec2(Weight.x* MaxRadius.x, Weight.y* MaxRadius.y));
}
void CSceneBuilder::CircleEarthquake() 
{
	if (m_fAccSceneEffectTime >= m_fSceneEffectTime)
	{
		GetMainCam()->VibrationOff();
		m_bSceneEffect = false;
		m_SceneEffectFunc = nullptr;
		return;
	}

	DWORD	Power = m_EffectFlag & 0x0000FF00L;
	float	Seta = CSJRandom::RandomFloat(0.0f, D3DX_PI * 2.0f);
	Vec2	MaxRadius = GetSpaceSize(Power);
	Vec2	Radius = Vec2{ MaxRadius.x* cosf(Seta),MaxRadius.y* sinf(Seta) };

	GetScene()->GetMainCamCom()->SetVibSpaceSize(Radius);
}
void CSceneBuilder::RectEarthquake() 
{
	if (m_fAccSceneEffectTime >= m_fSceneEffectTime)
	{
		GetMainCam()->VibrationOff();
		m_bSceneEffect = false;
		m_SceneEffectFunc = nullptr;
		return;
	}

	DWORD	Power = m_EffectFlag & 0x0000FF00L;

	Vec2 Weight = Vec2{ CSJRandom::RandomFloat(-0.5f,0.5f),CSJRandom::RandomFloat(-0.5f,0.5f) };
	Vec2 MaxRadius = GetSpaceSize(Power);
	
	GetScene()->GetMainCamCom()->SetVibSpaceSize(Vec2(Weight.x* MaxRadius.x, Weight.y* MaxRadius.y));
}
void CSceneBuilder::InitBlackOut()
{
	DWORD Mode = m_EffectFlag & 0xFF000000L;
	GetMainCam()->SetEffectFlag(Mode);
	GetMainCam()->BackOut();

	
}
void CSceneBuilder::EndBlackOut()
{
	GetMainCam()->BackOutOff();
}
void CSceneBuilder::BlackOut() 
{
	if (m_fAccSceneEffectTime >= m_fSceneEffectTime) 
	{
		EndBlackOutEffect();
		return;
	}
}

// Fade in,out 
void CSceneBuilder::SetFadeData(const float& _FadeTime)
{
	m_fAccSubTime = 0.0f;
	m_fPow = 1.f;

	if (_FadeTime <= 0.f) 
	{
		return;
	}

	m_fSubTime = _FadeTime;

	DWORD FadeMode = m_EffectFlag & 0xFF000000L;
	float InitAlpha = 0.0f;

	switch (FadeMode)
	{
	case SCENE_EFFECT_FADE_IN:
		m_fSceneEffectTime = _FadeTime;
		InitAlpha = 1.0f;
		break;
	case SCENE_EFFECT_FADE_OUT:
		InitAlpha = 0.0f;
		break;
	}
	
	DWORD ColorMode = m_EffectFlag & 0x0000FF00L;

	switch (ColorMode)
	{
	case  FADE_COLOR_BLACK:
		GetMainCam()->InitFade(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), InitAlpha);
		break;
	case FADE_COLOR_WHITE:
		GetMainCam()->InitFade(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), InitAlpha);
		break;
	case FADE_COLOR_RED:
		GetMainCam()->InitFade(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), InitAlpha);
		break;
	case FADE_COLOR_GREEN:
		GetMainCam()->InitFade(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), InitAlpha);
		break;
	case FADE_COLOR_BLUE:
		GetMainCam()->InitFade(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), InitAlpha);
		break;
	}
	return;
}
void CSceneBuilder::SetFadeData(const float& _FadeTime, const float& _Pow)
{
	SetFadeData(_FadeTime);
	m_fPow = _Pow;
}
void CSceneBuilder::FadeEffect() 
{
	if (m_fAccSceneEffectTime >= m_fSceneEffectTime) 
	{
		EndFadeEffect();
		return;
	}
	
	DWORD Flag = m_EffectFlag & 0xFF000000L;

	float Alpha = powf( m_fAccSubTime / m_fSubTime ,m_fPow);
	
	
	switch (Flag)
	{
	case SCENE_EFFECT_FADE_IN:
		GetMainCam()->SetFade(1.0f - Alpha);
		break;
	case SCENE_EFFECT_FADE_OUT:
		GetMainCam()->SetFade(Alpha);
		break;
	}
	
}
// Zoom in,out
void CSceneBuilder::ZoomEffect() 
{
	if (m_fAccSceneEffectTime >= m_fSceneEffectTime)
	{
		EndZoomEffect();
		return;
	}

	DWORD Flag = m_EffectFlag & 0xFF000000L;
	float Ratio = min(m_fAccSubTime / m_fSubTime,1.0f);
	float IvnRatio = 1.0f - Ratio;

	switch (Flag)
	{
		// ZoomRatio.x = InitRatio , ZoomRatio.y = EndRatio
	case SCENE_EFFECT_ZOOM_IN:
		GetMainCam()->SetZoomEffect(Ratio*m_ZoomPos, IvnRatio*m_ZoomRatio.x + Ratio*m_ZoomRatio.y);
		break;
	case SCENE_EFFECT_ZOOM_OUT:
		GetMainCam()->SetZoomEffect(IvnRatio*m_ZoomPos, IvnRatio*m_ZoomRatio.x + Ratio*m_ZoomRatio.y);
		break;
	}

}
void CSceneBuilder::EndBlackOutEffect() 
{
	EndBlackOut();
	m_bSceneEffect = false;
	m_SceneEffectFunc = nullptr;
}
void CSceneBuilder::EndFadeEffect() 
{
	m_bSceneEffect = false;
	GetMainCam()->FadeEffectEnd();
	m_SceneEffectFunc = nullptr;
}
void CSceneBuilder::EndZoomEffect() 
{
	m_bSceneEffect = false;
	GetMainCam()->ZoomEffectEnd();
	m_SceneEffectFunc = nullptr;
}
void CSceneBuilder::SetZoomData(
	const Vec3& _ZoomPos,
	const float& _Near,
	const float& _ZoomTime)
{
	m_ZoomPos = _ZoomPos;
	m_fSubTime = _ZoomTime;
	m_fAccSubTime = 0.0f;

	DWORD Flag = m_EffectFlag & 0xFF000000L;

	switch (Flag)
	{
	case SCENE_EFFECT_ZOOM_IN:
	{
		// ZoomRatio.x = InitRatio , ZoomRatio.y = EndRatio
		m_ZoomRatio = Vec2{ 1.0f,_Near };
		GetMainCam()->InitZoomEffect(Vec3{0.0f,0.0f,0.0f}, m_ZoomRatio.x);
	}
		break;
	case SCENE_EFFECT_ZOOM_OUT:
	{
		m_ZoomRatio = Vec2{ _Near ,1.0f };
		m_fSceneEffectTime = m_fSubTime;
		GetMainCam()->InitZoomEffect(m_ZoomPos, m_ZoomRatio.x);
	}
	break;
	}

}

void CSceneBuilder::SetZoomData(const Vec3& _ZoomPos, const Vec2& _ZoomRatio, const float& _ZoomTime)
{
	m_ZoomPos = _ZoomPos;
	m_fSubTime = _ZoomTime;
	m_fAccSubTime = 0.0f;

	DWORD Flag = m_EffectFlag & 0xFF000000L;

	switch (Flag)
	{
	case SCENE_EFFECT_ZOOM_IN:
	{
		// ZoomRatio.x = InitRatio , ZoomRatio.y = EndRatio
		if (_ZoomRatio.x < _ZoomRatio.y)
			m_ZoomRatio = Vec2{ _ZoomRatio.y,_ZoomRatio.x };
		else
			m_ZoomRatio = _ZoomRatio;

		GetMainCam()->InitZoomEffect(Vec3{ 0.0f,0.0f,0.0f }, m_ZoomRatio.x);
	}
	break;
	case SCENE_EFFECT_ZOOM_OUT:
	{
		if (_ZoomRatio.x > _ZoomRatio.y)
			m_ZoomRatio = Vec2{ _ZoomRatio.y,_ZoomRatio.x };
		else
			m_ZoomRatio = _ZoomRatio;
		
		if(1.0f == _ZoomRatio.y )
			m_fSceneEffectTime = m_fSubTime;
		
		GetMainCam()->InitZoomEffect(m_ZoomPos, m_ZoomRatio.x);
	}
	break;
	}
}
bool CSceneBuilder::IsSceneEffectOn()
{
	return m_bSceneEffect;
}