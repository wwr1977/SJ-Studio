#pragma once
#include "GameCom.h" 

class CPersCamera :public CGameCom
{
private:
	LPDIRECT3DDEVICE9		m_pDevice;
	CGameWindow*			m_pGameWnd;

	Mat						m_matVeiw;
	Mat						m_matProj;

	Vec3					m_Eye;
	Vec3					m_At;
	Vec3					m_Up;
	Vec3					m_Pivot;

	float					m_fNear;
	float					m_fFar;

public:
	void SetPivot(const Vec3& _Pivot);

public:
	void Init();
	void PostUpdate();
	
public:
	Mat GetSceneProj();
	Mat GetSceneView();
	Vec3 GetEye();

public:
	CPersCamera();
	~CPersCamera();
};

