#pragma once
#include <Logic.h>
class CTitleBackGround : public CLogic
{
	enum COLORINDEX
	{
		NOCOLOR,
		COLOR,
		MAXCOLOR,
	}m_CurColor;

	enum 
	{
		FADE_OUT,
		FADE_IN,
		MAXFADEEFFECT,
	}m_CurFade;

	class CTitleSceneBuilder*		m_pTitleSceneBuilder;
	SPTR<CFixRenderer>				m_TitleBackRen;
	SPTR<CFixRenderer>				m_MainTitleRen;

	SPTR<CFixRenderer>				m_GameStartRen;
	SPTR<CFixRenderer>				m_StartFrameRen;
	SPTR<CFixRenderer>				m_StartBackFrameRen;

	D3DXCOLOR						m_SelectColor[MAXCOLOR];
	SPTR<CSoundPlayer>				m_TitleSpeaker;

	bool							m_bFade;
	float							m_fAccTime;
	
	bool							m_bGameStart;
public:
	void Init();
	void InitData(void* _pBuilder);
	void Update();

public:
	// 점점 어둡게 하는 효과
	void FadeOutEvent();
	// 점점 밝게게 하는 효과
	void FadeInEvent();
	// 매개변수가 true이면 FadeOut , false 일때 FadeIn 
	void FadeEvent(const bool _FadeOut);

public:
	void GameStartEvent();
	void StartUpdate();
	const bool IsStartUpdateEnd();

public:
	CTitleBackGround();
	~CTitleBackGround();
};

