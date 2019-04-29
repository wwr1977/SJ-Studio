#pragma once
#include "GameCom.h"

class CLogic : public CGameCom
{
public:
	virtual void Init();
	virtual void PreUpdate();
	virtual void Update();
	virtual void PostUpdate();

public:
	const Vec3 GetActorPos();

public:
	void SetActorPos(const Vec3& _Pos);
	void SetActorImageMag(const float& _Mag);

public:
	CLogic();
	CLogic(CActObject* _ActObject);
	virtual ~CLogic();
};

