#pragma once
#include "GameWindow.h"

class CSplashWindow : public CGameWindow
{
private:
	tstring					m_SplashImagePath;
	//CImage*				m_SplashImage;
	POINT					m_ImageSize;

public:
	void SetSplashImage(const tstring& _FolderKey,const tstring& _FileName);
	
	void SplashImageRender();
	//void SplashImageRender(const tstring& _Pashs);
public:
	virtual bool WndProc(UINT message, WPARAM wParam, LPARAM lParam);

public:
	CSplashWindow(const tstring& _Name,const tstring& _Title);
	~CSplashWindow();
};

