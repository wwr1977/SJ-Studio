#include "stdafx.h"
#include "Camera.h"
#include"FixRenderer.h"

CCamera::CCamera()
	:m_fNear(0.3f),m_fFar(1000.0f), m_fZoomRatio(1.0f)
	, m_Pivot({0.0f,0.0f,-100.0f}), m_bVibration(false)
	,m_fMaxAmp(0.0f),m_fMinAmp(0.0f),m_fPulseTime(0.0f)
	, m_fAccTime(0.0f), m_VibPivot(0.0f,0.0f,0.0f)
	, m_CurEffectFlag(0), m_VibSpaceSize(Vec2{0.0f,0.0f})
	, m_CameraShield(nullptr)
	, m_bCustomViewSize(false)
	, m_CustomViewSize(Vec2{0.0f,0.0f})
	, m_CamMode(CAM_MODE_FIX)
	, m_TrackingTarget(nullptr)
	, m_TrackingRange(Vec2{0.0f,0.0f})
	, m_bAutoMove(false)
	, m_StartPoint(Vec3{ 0.f,0.f,0.f }), m_EndPoint(Vec3{ 0.f,0.f,0.f })
	, m_AutoDir(Vec3{ 0.f,0.f,0.f }), m_AutoDistance(0.f)
	, m_fAutoSpeed(150.f)
{

}


CCamera::~CCamera()
{
	m_TrackingTarget = nullptr;
}
void CCamera::SetCamMode(const int& _Mode)
{
	m_CamMode = (CAMERAMODE)_Mode;
}
void CCamera::SetCamMode(const int& _Mode, CTransForm* _Target)
{
	if (nullptr == _Target)
	{
		assert(nullptr);
		return;
	}

	m_CamMode = (CAMERAMODE)_Mode;
	m_TrackingTarget = _Target;
}
void CCamera::Init()
{
	SetTypeInfo(this);
	m_pGameWnd = ParentActObject()->GetScene()->GetWndPointer();
	m_pDevice = ParentActObject()->GetScene()->GetWindow()->GetLPDevice();

	if (nullptr == m_pDevice) 
	{
		assert(nullptr);
		return;
	}

	D3DXMatrixIdentity(&m_matProj);
	D3DXMatrixIdentity(&m_matView);

	COMRESMGR->CreateGameSprite(_T("ColorBase"));
	m_CameraShield = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_CAMERASHIELD, 1.0f, true));
	m_CameraShield->SetCustomSize(Vec2{ LARGEFLOAT,LARGEFLOAT });
	m_CameraShield->SetSprite(_T("ColorBase"));
	m_CameraShield->SetMaterial(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	m_CameraShield->Off();
}
void CCamera::InitPos()
{
	TRANS->SetPos(Vec3{ 0.0f,0.0f,5.0f });
}
void CCamera::Update()
{
	if (true == m_bVibration) 
	{
		VibrationCamera();
	}


	switch (m_CamMode)
	{
	case CAM_MODE_FIX: break;
	case CAM_MODE_TRACKING: TrackingUpdate(); break;
	case CAM_MODE_AREA_TRACKING: AreaTrackingUpdate(); break;
	}

	AutoMoveUpdate();
}
void CCamera::PostUpdate()
{
	if (true == m_bCustomViewSize)
	{
		D3DXMatrixOrthoLH(
			&m_matProj
			, m_CustomViewSize.x* m_fZoomRatio
			, m_CustomViewSize.y* m_fZoomRatio
			, m_fNear
			, m_fFar
		);

		D3DXMatrixOrthoLH(
			&m_matProjNoneRatio
			,m_CustomViewSize.x
			, m_CustomViewSize.y
			, m_fNear
			, m_fFar
		);

	}
	else 
	{
		D3DXMatrixOrthoLH(
			&m_matProj
			, m_pGameWnd->WndFWidth() * m_fZoomRatio
			, m_pGameWnd->WndFHeight() * m_fZoomRatio
			, m_fNear
			, m_fFar
		);

		D3DXMatrixOrthoLH(
			&m_matProjNoneRatio
			, m_pGameWnd->WndFWidth()
			, m_pGameWnd->WndFHeight()
			, m_fNear
			, m_fFar
		);

	}

	m_Eye = TransForm()->GetPos() + m_Pivot + m_VibPivot;
	m_At = TransForm()->GetPos() + m_VibPivot;
	m_Up = Vec3{ 0.0f,1.0f,0.0f };

	D3DXMatrixLookAtLH(&m_matView, &m_Eye, &m_At, &m_Up);


	m_pDevice->SetTransform(D3DTS_PROJECTION, &m_matProj);
	m_pDevice->SetTransform(D3DTS_VIEW, &m_matView);
}
void CCamera::SetCustomViewSize(const Vec2& _ViewSize)
{
	m_bCustomViewSize = true;
	m_CustomViewSize = _ViewSize;
}
void CCamera::SetTrackingRange(const Vec2& _Range)
{
	m_TrackingRange = _Range;
	
	if (0.0f > m_TrackingRange.x)
		m_TrackingRange.x = 0.0f;
	
	if (0.0f > m_TrackingRange.y)
		m_TrackingRange.y = 0.0f;
}
void CCamera::ZPosInit()
{
	Vec3 Pos = TRANS->GetPos();
	Pos.z = 5.f;
	TRANS->SetPos(Pos);
}
void CCamera::NoneRatioSetting() 
{
	m_pDevice->SetTransform(D3DTS_PROJECTION, &m_matProjNoneRatio);
}

void CCamera::ResetSetting() {
	m_pDevice->SetTransform(D3DTS_PROJECTION, &m_matProj);
}

void CCamera::SetPivot(const Vec3& _Pivot)
{
	m_Pivot = _Pivot;
}
Mat CCamera::GetProjNoneRatio()
{
	return m_matProjNoneRatio;
}
Mat CCamera::GetSceneProj() 
{
	return m_matProj;
}
Mat CCamera::GetSceneView()
{
	return m_matView;
}
Vec3 CCamera::GetCameraPos()
{
	return TRANS->GetPos();
}
void CCamera::VibrationCamera()
{
	/*if (true == CTimerMgr::Pulse(m_fPulseTime)) 
	{
		float Radius = CSJRandom::RandomFloat(m_fMinAmp, m_fMaxAmp);
		float Angle = CSJRandom::RandomFloat(0.0f, 2.0f * D3DX_PI);

		m_VibPivot = Vec3(Radius*cosf(Angle), Radius*cosf(Angle), 0.0f);
	}*/

	if (true == CTimerMgr::Pulse(m_fPulseTime)) 
	{
		m_VibPivot = Vec3(m_VibSpaceSize.x, m_VibSpaceSize.y, 0.0f);
	}

}
void CCamera::SetVibration(const float& _PulseTime, float _MaxAmp, float _MinAmp/* = 0.0f*/)
{
	m_fPulseTime = _PulseTime;
	m_fMaxAmp = _MaxAmp;
	m_fMinAmp = _MinAmp;

	if (m_fMinAmp < 0.0f)
	{
		m_fMinAmp = 0.0f;
	}
	
	m_fAccTime = 0.0f;
	m_bVibration = true;
}
void CCamera::InitVibration(const float& _PulseTime)
{
	m_fPulseTime = _PulseTime;

	m_fAccTime = 0.0f;
	m_bVibration = true;
}
void CCamera::SetVibSpaceSize(const Vec2& _VibSpaceSize)
{
	m_VibSpaceSize = _VibSpaceSize;
}
void CCamera::VibrationOff()
{
	m_bVibration = false;
	m_VibPivot = Vec3(0.0f,0.0f,0.0f);
}
void CCamera::BackOut()
{
	switch (m_CurEffectFlag)
	{
		case SCENE_EFFECT_BLACKOUT:
			ACTOR->GetScene()->GetWindow()->GetWndDevice()->SetColorFactor({ 0,0,0 });
			m_VibPivot = Vec3(LARGEFLOAT, LARGEFLOAT, 0.0f);
		break;
		case SCENE_EFFECT_FLASH: 
		{
			/*m_CameraShield->MaterialOn();
			m_CameraShield->On();*/
			ACTOR->GetScene()->GetWindow()->GetWndDevice()->SetColorFactor({ 255,255,255 });
			m_VibPivot = Vec3(LARGEFLOAT, LARGEFLOAT, 0.0f);
		}
		break;
	}
}
void CCamera::BackOutOff()
{
	switch (m_CurEffectFlag)
	{
	case SCENE_EFFECT_FLASH:
	{
		ACTOR->GetScene()->GetWindow()->GetWndDevice()->SetColorFactor({ 0,0,0 });
		/*
		m_CameraShield->MaterialOff();
		m_CameraShield->Off();
		*/
	}
	break;
	}
	m_VibPivot = Vec3(0.0f, 0.0f, 0.0f);
}
void CCamera::SetEffectFlag(DWORD _Flag)
{
	m_CurEffectFlag = _Flag;
}
void CCamera::InitFade(const D3DXCOLOR& _Color,const float& _InitAlpha)
{
	m_CameraShield->SetMaterial(_Color);
	m_CameraShield->SetAlpha(_InitAlpha);

	m_CameraShield->MaterialOn();
	m_CameraShield->On();
	
}
void CCamera::SetFade(const float& _Alpha)
{
	m_CameraShield->SetAlpha(_Alpha);
}
void CCamera::FadeEffectEnd() 
{
	m_CameraShield->Off();
}
void CCamera::InitZoomEffect(const Vec3& _InitPos, const float& _InitZoomRatio)
{
	m_VibPivot = _InitPos;
	m_fZoomRatio = _InitZoomRatio;
}
void CCamera::SetZoomEffect(const Vec3& _ZoomPos, const float& _ZoomRatio)
{
	m_VibPivot = _ZoomPos;
	m_fZoomRatio = _ZoomRatio;

}
void CCamera::ZoomEffectEnd()
{
	m_fZoomRatio = 1.0f;
	m_VibPivot = Vec3{ 0.0f,0.0f,0.0f };
}
void CCamera::TrackingUpdate()
{
	if (nullptr == m_TrackingTarget)
		return;

	if (false == m_TrackingTarget->IsUpdate()) 
		return;
	
	if (false == m_TrackingTarget->ChangeTranspose())
		return;

	Vec3 TPos = GetTrackingPos(m_TrackingTarget->GetPos());
	TPos.z = 5.f;

	TRANS->SetPos(TPos);

	return;
}
const Vec3 CCamera::GetTrackingPos(const Vec3& _TargetPos)
{
	float XPos = 0.0f, YPos = 0.0f;

	if (_TargetPos.x < 0.0f)
		XPos = max(_TargetPos.x, -m_TrackingRange.x);
	else 
		XPos = min(_TargetPos.x, m_TrackingRange.x);

	if (_TargetPos.y < 0.0f)
		YPos = max(_TargetPos.y, -m_TrackingRange.y);
	else
		YPos = min(_TargetPos.y, m_TrackingRange.y);

	return Vec3{ XPos,YPos,_TargetPos.z };
}
void CCamera::AreaTrackingUpdate()
{
	if (nullptr == m_TrackingTarget)
		return;

	if (false == m_TrackingTarget->IsUpdate())
		return;


	if (false == m_TrackingTarget->ChangeTranspose())
		return;
}
void CCamera::SetAutoMoveData(const Vec3& _EndPoint, const Vec3& _Dir, const float& _Speed)
{
	m_bAutoMove = true;

	m_StartPoint = TRANS->GetPos();
	m_EndPoint = _EndPoint;
	m_EndPoint.z = m_StartPoint.z;

	m_AutoDir = _Dir;
	m_AutoDir.z = 0.f;

	Vec3 Dis = m_EndPoint - m_StartPoint;
	m_AutoDistance = D3DXVec3Length(&Dis);

	m_fAutoSpeed = _Speed;
	
}
void CCamera::AutoMoveUpdate()
{
	if (!m_bAutoMove)
		return;

	Vec3 Dir = m_AutoDir * DELTATIME * m_fAutoSpeed;
	Vec3 Dis = TRANS->GetPos() + Dir - m_StartPoint;
	Dis.z = 0.f;
	float CurDis = D3DXVec3Length(&Dis);

	if (CurDis >= m_AutoDistance)
	{
		TRANS->SetPos(m_EndPoint);
		m_bAutoMove = false;
	}
	else
		TRANS->Move(Dir);
}
void CCamera::RamdomMode()
{
	m_CamMode = (CAMERAMODE)((m_CamMode + 1) / 2);
}
void CCamera::SetCameraPos(const Vec3& _Pos)
{
	Vec3 Pos = _Pos;
	Pos.z = 5.f;

	TRANS->SetPos(Pos);
}
void CCamera::MoveCamera(const Vec3& _Dir)
{
	TRANS->Move(_Dir);
}