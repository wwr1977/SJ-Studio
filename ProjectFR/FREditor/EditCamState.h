#pragma once
#include <BaseState.h>

class CEditCameraMen;
class CEditCamState : public CBaseState
{
public:
	CEditCameraMen*						m_PairObject;

	void Init(void* _pLogic);

public:
	CEditCamState();
	~CEditCamState();
};

