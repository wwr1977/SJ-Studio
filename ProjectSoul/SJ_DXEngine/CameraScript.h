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
	void SetCamera(CCamera* _Cam);

public:
	CCameraScript();
	~CCameraScript();
};





