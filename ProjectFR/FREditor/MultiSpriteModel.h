#pragma once
#include <Logic.h>

class CMultiSpriteModel : public CLogic
{
private:
	SPTR<CFixRenderer>					m_MultiSpriteRenderer;

public:
	void Init();
	void DebugRender();


public:
	CMultiSpriteModel();
	~CMultiSpriteModel();
};

