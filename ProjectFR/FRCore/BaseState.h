#pragma once
#include "ObjBase.h"

class CStateMgr;
class CBaseState : public CObjBase
{
public:
	virtual void Init(void* _pLogic = nullptr) = 0;
	virtual void StateReset() = 0;
	virtual void Update() = 0;

	

public:
	CBaseState();
	~CBaseState();
};

