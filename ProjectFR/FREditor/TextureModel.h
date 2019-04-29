#pragma once
#include <Logic.h>

class CTextureModel : public CLogic
{
private:
	SPTR<CFixRenderer>					m_TexRenderer;

public:
	void Init();
	void DebugRender();


public:
	CTextureModel();
	~CTextureModel();
};

