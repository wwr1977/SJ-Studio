#pragma once
#include <Logic.h>

class CGameMultiSprite;
class CTestBox : public CLogic
{
private:
	SPTR<CFixRenderer>						m_Renderer;
	SPTR<CFixRenderer>						m_BackRenderer;
	SPTR<CFixRenderer>						m_Renderer3;
	SPTR<CAniRenderer>						m_Ani;
	float									m_fAccTime;
	float									m_fURot;
	float									m_fCurWaveSize;
	SPTR<CGameMultiSprite>					m_TestSkillIcon;

public:
	void Init();
	void PreUpdate();
	void Update();
	void FontRender();

	void TraceCamDir();


public:
	CTestBox();
	~CTestBox();
};

