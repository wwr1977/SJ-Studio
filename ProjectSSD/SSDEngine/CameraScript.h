#pragma once
#include "Script.h"



class CActor;
class CCameraScript : public CScript
{
private:
	friend class CCamera;

protected:
	CCamera*								m_pPairCamera;

public:
	// ī�޶� ��ũ��Ʈ�� ī�޶� ���õ� ���Ŀ� ȣ��Ǵ� �ʱ�ȭ�� �Լ�
	virtual void MountingCameraEvent() {}
public:
	virtual void LastCamScriptUpdate(const float& _DeltaTime) {}

public:
	void SetCamera(CCamera* _Cam);

public:
	CCameraScript();
	~CCameraScript();
};





