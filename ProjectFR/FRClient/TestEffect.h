#pragma once
#include <Logic.h>

// Test Effect EffectEditor 만들때 싹다 지운다 
class CTestEffect : public CLogic
{
private:
	SPTR<CAniRenderer>					m_Renderer;
	SPTR<CSoundPlayer>					m_Sound;

	tstring								m_SoundName;


	float								m_fLifeTime;
	float								m_fAccTime;
	float								m_fPlaySound;
	float								m_fPlayRender;

	bool								m_bRendering;
	bool								m_bPlaySound;


public:
	void Init();
	void PreUpdate();
	void Update();
	void EffectOn();
	bool isLife();

	
public:
	void Setting(const tstring& _EffectName, const tstring& _SoundName
		,const float& _fLifeTime, const float& _fPlaySound, const float& _fPlayRender = 0.0f);

public:
	CTestEffect();
	~CTestEffect();
};

