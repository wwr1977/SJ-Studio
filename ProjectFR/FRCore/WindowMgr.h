#pragma once
#include"UserHeader.h"
#include"GameWindow.h"


class CWindowMgr
{
public:
	static CWindowMgr& Inst() 
	{
		static CWindowMgr tempInst = CWindowMgr();
		return tempInst;
	}

private:
	CWindowMgr():m_bIgnoreMsg(false){}
	~CWindowMgr() 
	{
		m_maphWndWindow.clear();
		m_mapNameWindow.clear();
	}


private:
	HINSTANCE									m_hInst;
	map<tstring, SPTR<CGameWindow>>				m_mapNameWindow;
	map<tstring, SPTR<CGameWindow>>::iterator	m_FindNameIter;

	map<HWND, SPTR<CGameWindow>>				m_maphWndWindow;
	map<HWND, SPTR<CGameWindow>>::iterator		m_FindhWndIter;
	map<HWND, SPTR<CGameWindow>>::iterator		m_StarthWndIter;
	map<HWND, SPTR<CGameWindow>>::iterator		m_EndhWndIter;

	POINT										m_DefaultWndPos;
	POINT										m_DefaultWndSize;

	bool										m_bIgnoreMsg;
public:
	void SetDefaultWndSize(POINT _Size) { m_DefaultWndSize = _Size; }
	void SetDefaultWndPos(POINT _Pos) { m_DefaultWndPos = _Pos; }
public:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

public:
	bool Init(HINSTANCE _hInst);
	int Start();
	int ReduceStart();
	int Update();
	SPTR<CGameWindow> FindGameWindow(tstring _Name);
	SPTR<CGameWindow> FindGameWindow(HWND _hWnd);
	bool DestroyWindow(tstring _Name);
	bool DestroyWindow(HWND _hWnd);
	void SetIgnoreMsg(const bool& _Ignore);
	const bool IsIgnore();
public:
	friend bool CDX9Core::Init(HINSTANCE _hInst, SPTR<CCoreBuilder> _pBuilder);
	friend bool CDX9Core::Progress();
	friend CGameWindow* CGameWindow::CreateGameWindow();
	friend CGameWindow* CGameWindow::CreateSplashWindow();

public:
	SPTR<CGameWindow> CreateGameWindow(tstring _Name, tstring _Title = _T(""));
	SPTR<CGameWindow> CreateGameWindow(HWND _hWnd,tstring _Name, tstring _Title = _T(""));
	SPTR<class CSplashWindow> CreateSplashWindow(tstring _Name, tstring _Title = _T(""));
	
	bool SetWindowSize(HWND _hWnd, POINT _Size = {-1,-1}, POINT _Pos = { -1,-1 });
	bool SetWindowSize(HWND _hWnd, int _Left,int _Top,int _Right,int _Bottom);
	bool SetWindowSize(tstring _Name, POINT _Size = { -1,-1 }, POINT _Pos = { -1,-1 });
	bool SetWindowSize(tstring _Name, int _Left, int _Top, int _Right, int _Bottom);
	bool ChangeMouseCursor(HWND _hWnd,tstring _Path);
	bool ChangeMouseCursor(tstring _Name, tstring _Path);
	bool ChangeIcon(HWND _hWnd, tstring _Path);
	bool ChangeIcon(tstring _Name, tstring _Path);
	bool ChangeSmallIcon(HWND _hWnd, tstring _Path);
	bool ChangeSmallIcon(tstring _Name, tstring _Path);

};

