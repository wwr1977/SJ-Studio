#pragma once
#include <CoreBuilder.h>

class CDX9CoreBuilder :public CCoreBuilder
{
private:
	SPTR<CGameWindow>					m_pMainWindow;
	//SPTR<CSplashWindow>				m_pSplashWindow;
	
public:
	bool CoreBuild();
	void SettingPath();
	void SettingMainWindow();
	void SettingKey();
	
public:
	void SplashWindowOn();


public:
	CDX9CoreBuilder();
	~CDX9CoreBuilder();
};

