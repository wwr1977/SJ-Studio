#pragma once
#include "BaseComponent.h"
#include"DXHeader.h"
#include"DXEnum.h"
#include<list>



#define MAXTRACKRADIUS 50.f 

struct CAMDATA 
{
	CAMERA_GROUP			CamGroup;
	float					Fov;
	float					Near;
	float					Far;

};

// 카메라 컴포넌트는 일반객체에 추가가 불가능하고
// ASceneCamera라는 카메라 전용 객체만을 통해서만 만들 수 있다.
class CCamera final : public CBaseComponent
{
	friend class ASceneCamera;
	friend class CSceneRenderMgr;
	friend class CActor;

	enum CAMERAMODE
	{
		ORTHO_CAMERA,
		PERSPECTIVE_CAMERA
	}m_eProjMode;

	enum OTTHO_MODE
	{
		ORTHO_WINDOW,
		ORTHO_FREE
	}m_eOrthoMode;

	//enum CAM_MOVE_MODE
	//{
	//	MOVE_FIX,
	//	MOVE_TRACKING,

	//}m_eMoveMode;

public:
	static const bool IsMultiComponentCheck(CActor* _pActor);

private:
	CAMERA_GROUP							m_CameraGroup;
	std::vector<RENDER_LAYER>				m_vecViewRenderLayer;

	bool									m_bWheelZoom;
	float									m_CurZoomMag;

	float									m_fFov;
	float									m_fFar;
	float									m_fNear;

	Vec2									m_OrthoCamSize;
	Mat										m_matView;
	Mat										m_matProj;

	/*SPTR<CTransForm>						m_pTrackingTrans;
	bool									m_bTracking;
	float									m_fAccelFactor;
	Vec3									m_CurPos;
	Vec3									m_TargetPos;

	float									m_fDirFactor;
	Vec3									m_TargetDir;
	Vec3									m_CurDir;*/


public:
	template<typename ...Rest>
	void SettingViewLayer(Rest ..._Arg) 
	{
		m_vecViewRenderLayer.reserve(sizeof...(_Arg));
		PushViewLayer(_Arg...);
	}

	template<typename ...Rest>
	void PushViewLayer(RENDER_LAYER _Layer , Rest ..._Arg)
	{
		for (auto Value : m_vecViewRenderLayer)
		{
			if(Value == _Layer)
				PushViewLayer(_Arg...);
		}

		m_vecViewRenderLayer.push_back(_Layer);
		PushViewLayer(_Arg...);
	}

	void PushViewLayer(RENDER_LAYER _Layer);


public:
	void SetFreeOrtho(const Vec2& _OrthoSize) 
	{
		m_eOrthoMode = ORTHO_FREE;
		m_OrthoCamSize = _OrthoSize;
	}
	void SetCamData(const CAMDATA& _Data);


public:
	std::vector<RENDER_LAYER>* GetLayerVec() { return &m_vecViewRenderLayer; }


public:
	const bool Init(const CAMDATA& _InitData);
	void PreUpdate(const float& _DeltaTime) override;
	void Update(const float& _DeltaTime) override;
	void PostUpdate(const float& _DeltaTime) override;
	void LastUpdate(const float& _DeltaTime) override;
	void Release() override;


public:
	void SetPos(const Vec3& _Pos); 
	void SetPos(const Vec2& _Pos);
	void SetWheelZoom(const bool& _Wheel) { m_bWheelZoom = _Wheel; }

public:
	const Vec2 GetOrthoViewSize();
	const Mat GetVPMatrix() {return m_matView * m_matProj;}
	const CAMERA_GROUP GetCamGroup() { return m_CameraGroup; }
	const bool IsCameraMove();
	const Vec3 GetPos();
	CActor* GetCamActor() { return ACTOR; }
	const float GetNear() { return m_fNear; }
	const float GetFar() { return m_fFar; }
	const Vec3 GetMousePos();
	const float GetZoomMag() { return m_CurZoomMag; }
	const float GetViewMinDepth();

private:
	std::list<SPTR<class CCameraScript>>					m_listCameraScript;
	std::list<Vec3>											m_listEyePivot;

private:
	const bool PushCameraScript(CBaseComponent* _CamScript);
	void ClearPivotList();
public:
	void SetCameraPivot(const Vec3& _Pivot);
	void SetCameraZoom(const float& _ZoomMag);

public:
	template<typename T>
	SPTR<T> FindCameraScript() 
	{
		if (true == m_listCameraScript.empty())
			return nullptr;

		for (SPTR<CCameraScript> Script : m_listCameraScript)
		{
			if (true == Script->EqualType<T>())
				return Script;
		}

		return nullptr;
	}

	template<typename T>
	SPTR<T> FindCameraScript(const wchar_t* _Tag)
	{
		if (true == m_listCameraScript.empty())
			return nullptr;

		for (SPTR<CCameraScript> Script : m_listCameraScript)
		{
			if (true == Script->EqualType<T>() && true == Script->ExistTag(_Tag))
				return Script;
		}

		return nullptr;
	}

public:
	CCamera();
	CCamera(CActor* _Actor);
	~CCamera();
};

