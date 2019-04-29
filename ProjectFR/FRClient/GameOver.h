#pragma once
#include <Logic.h>
class CGameOver : public CLogic
{
	enum 
	{
		FADE_IN,
		FADE_OUT,
		MAXFADE,
	}m_CurFade;

private:
	class CEndingSceneBuilder*			m_pEndingBuilder;
	SPTR<CFixRenderer>					m_GameOverRen;
	SPTR<CSoundPlayer>					m_GameOverSpeaker;

	float								m_fFadeTime;
	float								m_fAccTime;
	bool								m_bFadeUpdate;
	float								m_fMaxVol;
	

	bool								m_bKeyCheck;

	
	
public:
	void Init();
	void InitData(void* _pEndingBuilder);
	void Update();

public:
	void FadeInUpdate();
	void FadeOutUpdate();
	void FadeEvent(const bool& _FadeIn);
	void KeyCheck();

public:
	CGameOver();
	~CGameOver();
};

