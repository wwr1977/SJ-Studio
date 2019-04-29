#pragma once
#include <Logic.h>
class CMiddlePoint : public CLogic
{
	
public:
	virtual void Init();
	virtual void PreUpdate();
	virtual void Update();
	virtual void PostUpdate();
	virtual void DebugRender();

public:
	CMiddlePoint();
	~CMiddlePoint();
};

