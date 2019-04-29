#pragma once
#include <Logic.h>
class CTitleIntroObject : public CLogic
{
	enum 
	{
		STUDIO,
		SCREENANI,
		PROJECT,
	}m_CurUpdate;

	class CTitleSceneBuilder*			m_pTitleSceneBuilder;

	SPTR<CFixRenderer>					m_StudioRen;
	SPTR<CAniRenderer>					m_ScreenTransAni;
	SPTR<CFixRenderer>					m_ProjectRen;
	float								m_fAccTime;
	bool								m_bEndIntro;

public:
	void Init();
	void InitData(void* _pTitleBuilder);
	void Update();

public:
	void StudioUpdate();
	void ScreenAniUpdate();
	void ProjectUpdate();


public:
	void Standby();
	const bool IsIntroEnd();

public:
	CTitleIntroObject();
	~CTitleIntroObject();
};

