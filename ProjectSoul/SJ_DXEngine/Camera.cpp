#include "Camera.h"
#include"TransForm.h"
#include"SJ_DXCore.h"
#include"Device.h"
#include"Actor.h"
#include"TransForm.h"
#include"KeyMgr.h"
#include"Scene.h"
#include"TimerMgr.h"
#include"DXMath.h"
#include"CameraScript.h"
#include<algorithm>
#include"SJ_DXCore.h"

// 한 엑터 안에 카메라 컴포넌트는 2개가 들어올 수 없다.

const bool CCamera::IsMultiComponentCheck(CActor* _pActor) 
{
	if (nullptr == _pActor)
		return false;

	// 엑터 안에 카메라가 있으므로 트랜스폼 컴포넌트가 들어갈수 없다.
	if (nullptr != _pActor->FindCom<CCamera>())
		return false;

	return true;
}

CCamera::CCamera()
	:m_eProjMode(ORTHO_CAMERA), m_fNear(0.f), m_fFar(1000.f),m_fFov(DirectX::XM_PIDIV2)
	, m_eOrthoMode(ORTHO_WINDOW), m_CameraGroup(CAMERA_GROUP(0)), m_bWheelZoom(false)
	, m_CurZoomMag(1.f)
{
	m_vecViewRenderLayer.clear();
	m_listEyePivot.clear();
}
CCamera::CCamera(CActor* _Actor)
	: CBaseComponent(_Actor), m_eProjMode(ORTHO_CAMERA), m_fNear(0.f), m_fFar(1000.f), m_fFov(DirectX::XM_PIDIV2)
	, m_eOrthoMode(ORTHO_WINDOW), m_CameraGroup(CAMERA_GROUP(0)), m_bWheelZoom(false)
	, m_CurZoomMag(1.f)
{
	m_vecViewRenderLayer.clear();
	m_listEyePivot.clear();
}

CCamera::~CCamera()
{
	m_vecViewRenderLayer.clear();
	m_listCameraScript.clear();
	m_listEyePivot.clear();
}

void CCamera::PushViewLayer(RENDER_LAYER _Layer)
{
	for (auto Value : m_vecViewRenderLayer)
	{
		if (Value == _Layer)
			return;
	}

	m_vecViewRenderLayer.push_back(_Layer);
	//std::sort(m_vecViewRenderLayer.begin(), m_vecViewRenderLayer.end());
}
void CCamera::SetCamData(const CAMDATA& _Data)
{
	m_fFov = _Data.Fov;
	m_fNear = _Data.Near;
	m_fFar = _Data.Far;

	m_matView.SetIdentityMat();
	m_matProj.SetIdentityMat();
}

const bool CCamera::Init(const CAMDATA& _InitData)
{
	m_CameraGroup = _InitData.CamGroup;
	
	m_fFov = _InitData.Fov;
	m_fNear = _InitData.Near;
	m_fFar = _InitData.Far;

	m_matView.SetIdentityMat();
	m_matProj.SetIdentityMat();

	m_pBelongScene->GetRenderMgr()->PushCamera(this);
	return true;
}
void CCamera::PreUpdate(const float& _DeltaTime)
{
	m_listEyePivot.clear();

	for (SPTR<CCameraScript> CamScript : m_listCameraScript) 
	{
		if (nullptr != CamScript && true == CamScript->IsActive())
			CamScript->PreUpdate(_DeltaTime);
	}

}
void CCamera::Update(const float& _DeltaTime)
{
	for (SPTR<CCameraScript> CamScript : m_listCameraScript) 
	{
		if (nullptr != CamScript && true == CamScript->IsActive())
			CamScript->Update(_DeltaTime);
	}

	if (true == m_bWheelZoom &&  true == CKeyMgr::GetWheelEvent())
	{
		// 앞으로 밀면 휠제타 값이 양수가 나온다
		if (0 < CKeyMgr::GetWheelSign())
			m_CurZoomMag -= 0.01f;
		// 뒤로 땡기면 휠 제타 값이 음수가 나온다
		else
			m_CurZoomMag += 0.01f;
	}

}
void CCamera::PostUpdate(const float& _DeltaTime)
{
	for (SPTR<CCameraScript> CamScript : m_listCameraScript) 
	{
		if(nullptr != CamScript && true == CamScript->IsActive())
			CamScript->PostUpdate(_DeltaTime);
	}


	Vec3 Eye = TRANS->GetLocalPos();

	for (Vec3& Pivot :m_listEyePivot)
		Eye += Pivot;
	
	Vec3 AtDir = TRANS->GetWorldAxis(AXIS_Z);
	Vec3 UpDir = TRANS->GetWorldAxis(AXIS_Y);

	m_matView.SetViewToMat(Eye, AtDir, UpDir);

	switch (m_eProjMode)
	{
	case CCamera::ORTHO_CAMERA:
		switch (m_eOrthoMode)
		{
		case CCamera::ORTHO_WINDOW:
			m_matProj.SetOrthoProjMat((float)Core::GetWindowWidth()*m_CurZoomMag, (float)Core::GetWindowHeight()*m_CurZoomMag, m_fNear, m_fFar);
			break;
		case CCamera::ORTHO_FREE:
			m_matProj.SetOrthoProjMat(m_OrthoCamSize.x*m_CurZoomMag, m_OrthoCamSize.y*m_CurZoomMag, m_fNear, m_fFar);
			break;
		default:
			m_matProj.SetIdentityMat();
		}
		break;
	case CCamera::PERSPECTIVE_CAMERA:
		m_matProj.SetFovPersProjMat(m_fFov, Core::GetWindowAspect(), m_fNear, m_fFar);
		break;
	default:
		m_matProj.SetIdentityMat();
		break;
	}
}
void CCamera::LastUpdate(const float& _DeltaTime) 
{
	for (SPTR<CCameraScript> CamScript : m_listCameraScript)
		CamScript->LastUpdate(_DeltaTime);
}
void CCamera::Release()
{
	if (m_listCameraScript.empty() )
		return;

	m_listCameraScript.erase(std::remove_if(m_listCameraScript.begin(), m_listCameraScript.end(), [](SPTR<CCameraScript> _Script)->bool 
	{
		if (true == _Script->IsDeath())
			_Script->Release();

		return _Script->IsDeath();
	}), m_listCameraScript.end());

}


void CCamera::SetPos(const Vec3& _Pos)
{
	TRANS->SetPos(_Pos);
}

void CCamera::SetPos(const Vec2& _Pos)
{
	TRANS->SetPos(_Pos);
}

const Vec2 CCamera::GetOrthoViewSize()
{
	if (m_eOrthoMode == ORTHO_WINDOW)
		return Vec2((float)Core::GetWindowWidth()*m_CurZoomMag, (float)Core::GetWindowHeight()*m_CurZoomMag);
	else if (m_eOrthoMode == ORTHO_FREE)
		return Vec2(m_OrthoCamSize.x*m_CurZoomMag, m_OrthoCamSize.y*m_CurZoomMag);

	return Vec2(0.f, 0.f);
}
const bool CCamera::IsCameraMove()
{
	return this->IsTransMove();
	//return TRANS->ChangeTransform();
}
const Vec3 CCamera::GetPos()
{
	return TRANS->GetLocalPos();
}
const Vec3 CCamera::GetMousePos()
{
	Vec3 MousePos;
	MousePos = Core::GetScreenMousePos();

	switch (m_eOrthoMode)
	{
	case CCamera::ORTHO_WINDOW:
	{
		MousePos.x = (MousePos.x - Core::GetWindowWidth() * 0.5f) * m_CurZoomMag;
		MousePos.y = (-(MousePos.y - Core::GetWindowHeight() * 0.5f)) * m_CurZoomMag;
		MousePos.vec2 = MousePos.vec2 + TRANS->GetWorldPos().vec2 ;
	}
	return MousePos ;
	case CCamera::ORTHO_FREE:
	{
		MousePos.x = ( MousePos.x - m_OrthoCamSize.x * 0.5f) * m_CurZoomMag;
		MousePos.y = (-(MousePos.y - m_OrthoCamSize.y * m_CurZoomMag * 0.5f)) * m_CurZoomMag;
		MousePos.vec2 = MousePos.vec2 + TRANS->GetWorldPos().vec2 ;
	}
	return MousePos;
	}

	return Vec3();
}
const float CCamera::GetViewMinDepth()
{
	return TRANS->GetWorldPos().z + m_fNear;
}
const bool CCamera::PushCameraScript(CBaseComponent* _CamScript)
{
	if (nullptr == _CamScript)
		return false;

	((CCameraScript*)_CamScript)->SetCamera(this);
	m_listCameraScript.push_back((CCameraScript*)_CamScript);
	return true;
}

void CCamera::SetCameraPivot(const Vec3& _Pivot)
{
	m_listEyePivot.push_back(_Pivot);
}
void CCamera::SetCameraZoom(const float& _ZoomMag)
{
	m_CurZoomMag = _ZoomMag;
}
void CCamera::ClearPivotList()
{
	m_listEyePivot.clear();
}



