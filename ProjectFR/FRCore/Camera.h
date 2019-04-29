#pragma once
#include "GameCom.h"
class CCamera : public CGameCom
{
	

private:
	LPDIRECT3DDEVICE9		m_pDevice;
	CGameWindow*			m_pGameWnd;

	Mat						m_matView;
	Mat						m_matProj;
	Mat						m_matProjNoneRatio;

	Vec3					m_Eye;
	Vec3					m_At;
	Vec3					m_Up;
	Vec3					m_Pivot;
	// 진동시 피벗과 Actor에 같이 영향을 주는 위치값
	Vec3					m_VibPivot;

	float					m_fNear;
	float					m_fFar;


	float					m_fZoomRatio;

	bool					m_bVibration;
	float					m_fAccTime;
	float					m_fMaxAmp;
	float					m_fMinAmp;
	Vec2					m_VibSpaceSize;
	float					m_fPulseTime;


	DWORD					m_CurEffectFlag;
	SPTR<CFixRenderer>		m_CameraShield;
	

	bool					m_bCustomViewSize;
	Vec2					m_CustomViewSize;

	CAMERAMODE				m_CamMode;
	CTransForm*				m_TrackingTarget;
	
	Vec2					m_TrackingRange;

private:
	bool							m_bAutoMove;
	Vec3							m_StartPoint;
	Vec3							m_EndPoint;
	float 							m_AutoDistance;
	Vec3							m_AutoDir;
	float							m_fAutoSpeed;

public:
	void SetZoomRatio(const float& _Ratio) 
	{
		m_fZoomRatio = _Ratio;
	}
	const float GetZoomRatio() 
	{
		return  m_fZoomRatio;
	}

	void SetPivot(const Vec3& _Pivot);
	void SetCamMode(const int& _Mode);
	void SetCamMode(const int& _Mode, CTransForm* _Target);
	void RamdomMode();

	Mat GetProjNoneRatio();
	Mat GetSceneProj();
	Mat GetSceneView();
	Vec3 GetCameraPos();
public:
	void SetEffectFlag(DWORD _Flag);
	
	// 지진 효과
	void VibrationCamera();
	void SetVibration(const float& _PulseTime, float _MaxAmp,float _MinAmp = 0.0f);
	void InitVibration(const float& _PulseTime);
	void SetVibSpaceSize(const Vec2& _VibSpaceSize);
	void VibrationOff();
	
	// 암전,섬광 효과
	void BackOut();
	void BackOutOff();

	//Fade In,Out
	void InitFade(const D3DXCOLOR& _Color, const float& _InitAlpha);
	void SetFade(const float& Alpha);
	void FadeEffectEnd();

	//Zoom In,Out
	void InitZoomEffect(const Vec3& _InitPos, const float& _InitZoomRatio);
	void SetZoomEffect(const Vec3& _ZoomPos, const float& _ZoomRatio);
	void ZoomEffectEnd();

public:
	void NoneRatioSetting();
	void ResetSetting();
	void Init();
	void InitPos();
	void Update();
	void PostUpdate();
	void SetCustomViewSize(const Vec2& _ViewSize);
	void SetTrackingRange(const Vec2& _Range);
	void ZPosInit();

	// 중심 타겟팅 모드일때의 세부 업데이트 함수
	void TrackingUpdate();
	const Vec3 GetTrackingPos(const Vec3& _TargetPos);
	// 영역 타겟팅 모드일때의 세부 업테이트 함수
	void AreaTrackingUpdate();

public:
	void SetAutoMoveData(const Vec3& _EndPoint,const Vec3& _Dir,const float& _Speed);
	void AutoMoveUpdate();
public:
	void SetCameraPos(const Vec3& _Pos);
	void MoveCamera(const Vec3& _Dir);

public:
	CCamera();
	virtual ~CCamera();
};

