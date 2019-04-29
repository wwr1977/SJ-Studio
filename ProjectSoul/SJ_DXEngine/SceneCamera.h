#pragma once
#include "Actor.h"
#include"Camera.h"

class ASceneCamera : public CActor
{
private:
	SPTR<CCamera>						m_pCamera;
	bool								m_bUpdate;

	Vec3								m_Eye;
	Vec3								m_AtDir;
	Vec3								m_UpDir;



public:
	void Init() override;

private:


public:
	SPTR<CCamera> GetCamera() { return m_pCamera; }
	


public:
	ASceneCamera();
	ASceneCamera(CScene* _Scene);
	~ASceneCamera();
};

