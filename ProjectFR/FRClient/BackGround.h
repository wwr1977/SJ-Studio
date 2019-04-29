#pragma once
#include<Logic.h>

class CBackGround : public CLogic
{
	enum 
	{
		MAXBACKGROUND = 13
	};
private:
	SPTR<CFixRenderer>					m_BackRenderer;
	SPTR<CSoundPlayer>					m_BGMSpeeker;
	int									m_CurBackIndex;

public:
	void Init();
	void Update();

public:
	BOOL WindowSizeSynchro();
	void ChangePrevEvent();
	void ChangeNextEvent();

public:
	void ChangeBackGround(const UINT& _Index);


	// BGM은 기본적으로 무한반복으로 설정
	void SetVolume(const float& _Volume);
	void PlayBgm(const tstring& _BgmName);
	void BgmOff();
	void BgmPause(const bool& _Pause);
public:
	CBackGround();
	~CBackGround();
};

