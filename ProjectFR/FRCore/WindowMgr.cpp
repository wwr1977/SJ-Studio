#include "stdafx.h"
#include "WindowMgr.h"



bool CWindowMgr::Init(HINSTANCE _hInst)
{
	assert(_hInst);

	m_hInst = _hInst;

	if (nullptr == m_hInst) 
	{
		return FALSE;
	}

	int MiddleX = GetSystemMetrics(SM_CXSCREEN) / 2;
	int MiddleY = GetSystemMetrics(SM_CYSCREEN) / 2;

	m_DefaultWndPos = { MiddleX,MiddleY };
	m_DefaultWndSize = { 500,500 };

	CGDevice::SetDeviceInfomation();

	return TRUE;
}

LRESULT CALLBACK CWindowMgr::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) 
{
	SPTR<CGameWindow> GWin = CWindowMgr::Inst().FindGameWindow(hWnd);
	
	if (nullptr != GWin) 
	{
		if (TRUE == GWin->WndProc(message, wParam, lParam))
			return 0;
	}

	switch (message)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다.
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
	{
		CWindowMgr::Inst().DestroyWindow(hWnd);
		PostQuitMessage(0);
		break;
	}
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
SPTR<CGameWindow> CWindowMgr::CreateGameWindow(tstring _Name, tstring _Title /* = _T("")*/)
{
	SPTR<CGameWindow> GWin = FindGameWindow(_Name);

	if (nullptr != GWin) 
	{
		return nullptr;
	}

	CGameWindow* NewWin = new CGameWindow(_Name, _Title);

	if (nullptr == NewWin->hWnd())
	{
		SAFE_DELETE(NewWin);
		return nullptr;
	}

	m_mapNameWindow.insert(map<tstring, SPTR<CGameWindow>>::value_type(NewWin->Name(), NewWin));
	m_maphWndWindow.insert(map<HWND, SPTR<CGameWindow>>::value_type(NewWin->hWnd(), NewWin));


	return NewWin;
}
SPTR<CSplashWindow> CWindowMgr::CreateSplashWindow(tstring _Name, tstring _Title /*= _T("")*/)
{
	SPTR<CGameWindow> GWin = FindGameWindow(_Name);

	if (nullptr != GWin)
		return nullptr;
	

	CSplashWindow* NewWin = new CSplashWindow(_Name, _Title);

	if (nullptr == NewWin->hWnd())
	{
		SAFE_DELETE(NewWin);
		return nullptr;
	}

	m_mapNameWindow.insert(map<tstring, SPTR<CGameWindow>>::value_type(NewWin->Name(), NewWin));
	m_maphWndWindow.insert(map<HWND, SPTR<CGameWindow>>::value_type(NewWin->hWnd(), NewWin));


	return NewWin;
}
SPTR<CGameWindow> CWindowMgr::CreateGameWindow(HWND _hWnd, tstring _Name, tstring _Title/* = _T("")*/)
{
	assert(_hWnd);

	SPTR<CGameWindow> GWin = FindGameWindow(_Name);

	if (nullptr != GWin)
	{
		return nullptr;
	}

	GWin = FindGameWindow(_hWnd);

	if (nullptr != GWin)
	{
		return nullptr;
	}

	CGameWindow* NewWin = new CGameWindow(_hWnd, _Name, _Title);

	if (nullptr == NewWin->hWnd())
	{
		SAFE_DELETE(NewWin);
		return nullptr;
	}

	m_mapNameWindow.insert(map<tstring, SPTR<CGameWindow>>::value_type(NewWin->Name(), NewWin));
	m_maphWndWindow.insert(map<HWND, SPTR<CGameWindow>>::value_type(NewWin->hWnd(), NewWin));


	return NewWin;

}
SPTR<CGameWindow> CWindowMgr::FindGameWindow(tstring _Name)
{
	return FINDMAP_SPTR<CGameWindow>(_Name, m_mapNameWindow);
}
SPTR<CGameWindow>CWindowMgr::FindGameWindow(HWND _hWnd) 
{
	return FINDMAP_SPTR<CGameWindow>(_hWnd, m_maphWndWindow);
}
bool CWindowMgr::DestroyWindow(tstring _Name) 
{
	HWND hWnd = nullptr;

	m_FindNameIter = m_mapNameWindow.find(_Name);

	if (m_FindNameIter == m_mapNameWindow.end()) 
	{
		return FALSE;
	}

	hWnd = m_FindNameIter->second->hWnd();
	m_FindhWndIter = m_maphWndWindow.find(hWnd);

	if (m_FindhWndIter == m_maphWndWindow.end()) 
	{
		return FALSE;
	}

	m_mapNameWindow.erase(m_FindNameIter);
	m_maphWndWindow.erase(m_FindhWndIter);
	
	return TRUE;

}
bool CWindowMgr::DestroyWindow(HWND _hWnd)
{
	tstring Name = _T("");

	m_FindhWndIter = m_maphWndWindow.find(_hWnd);

	if (m_FindhWndIter == m_maphWndWindow.end())
	{
		return FALSE;
	}

	Name = m_FindhWndIter->second->Name();
	m_FindNameIter = m_mapNameWindow.find(Name);

	if (m_FindNameIter == m_mapNameWindow.end())
	{
		return FALSE;
	}

	m_mapNameWindow.erase(m_FindNameIter);
	m_maphWndWindow.erase(m_FindhWndIter);

	return TRUE;
}
void CWindowMgr::SetIgnoreMsg(const bool& _Ignore)
{
	m_bIgnoreMsg = _Ignore;
}
const bool CWindowMgr::IsIgnore()
{
	return m_bIgnoreMsg;
}
int CWindowMgr::Start()
{
	MSG msg;

	while (m_maphWndWindow.size() > 0)
	{
		if (CDX9Core::m_bCoreDeath) 
		{
			return (int)0;
		}

		if (PeekMessage(&msg, nullptr, 0, 0,PM_REMOVE))
		{
				TranslateMessage(&msg);
				DispatchMessage(&msg);	
		}
		else 
		{
			CTimerMgr::Update();
			CKeyMgr::Update();
			CThreadMgr::Update();
			Update();
			CKeyMgr::ResetWheelValue();
			CResourceMgr::SoundUpdate();
		}
	}

	return 0;
}
int CWindowMgr::ReduceStart()
{
	CTimerMgr::Update();
	CKeyMgr::Update();
	CThreadMgr::Update();
	Update();
	CResourceMgr::SoundUpdate();
	CKeyMgr::ResetWheelValue();

	
	return 0;
}
int CWindowMgr::Update() 
{
	m_StarthWndIter = m_maphWndWindow.begin();
	m_EndhWndIter = m_maphWndWindow.end();

	for (;m_StarthWndIter != m_EndhWndIter; ++m_StarthWndIter)
	{
		m_StarthWndIter->second->Progress();
	}

	return 0;
}
bool CWindowMgr::SetWindowSize(HWND _hWnd, POINT _Size , POINT _Pos)
{
	// 2,3번째 매개변수에 입력이 들어오지 않을 경우 DefaultSize,Pos 로 자동 설정
	POINT Size = _Size;
	POINT Pos = _Pos;
	
	if (Size.x <= -1 || Size.y <= -1) 
	{
		Size = m_DefaultWndSize;
	}

	if (Pos.x <= -1 || Pos.y <= -1)
	{
		Pos = m_DefaultWndPos;
	}

	SPTR<CGameWindow> GWin = FindGameWindow(_hWnd);

	if (nullptr == GWin) 
	{
		return FALSE;
	}
	
	return GWin->SetWindowSize(Size, Pos);

}
bool CWindowMgr::SetWindowSize(HWND _hWnd, int _Left, int _Top, int _Right, int _Bottom)
{
	SPTR<CGameWindow> GWin = FindGameWindow(_hWnd);

	if (nullptr == GWin)
	{
		return FALSE;
	}

	
	POINT Pos = {(_Right +  _Left) / 2 ,(_Bottom+_Top) / 2 };
	POINT Size = {_Right - _Left , _Bottom - _Top};

	return GWin->SetWindowSize(Size, Pos);
}
bool CWindowMgr::SetWindowSize(tstring _Name, POINT _Size /*= Default*/, POINT _Pos /* = Default*/)
{
	// 2,3번째 매개변수에 입력이 들어오지 않을 경우 DefaultSize,Pos 로 자동 설정
	POINT Size = _Size;
	POINT Pos = _Pos;

	if (Size.x <= -1 || Size.y <= -1)
	{
		Size = m_DefaultWndSize;
	}

	if (Pos.x <= -1 || Pos.y <= -1)
	{
		Pos = m_DefaultWndPos;
	}

	SPTR<CGameWindow> GWin = FindGameWindow(_Name);

	if (nullptr == GWin)
	{
		return FALSE;
	}

	return GWin->SetWindowSize(Size, Pos);
}
bool CWindowMgr::SetWindowSize(tstring _Name, int _Left, int _Top, int _Right, int _Bottom)
{
	SPTR<CGameWindow> GWin = FindGameWindow(_Name);

	if (nullptr == GWin)
	{
		return FALSE;
	}

	POINT Pos = { (_Right + _Left) / 2 ,(_Bottom + _Top) / 2 };
	POINT Size = { _Right - _Left , _Bottom - _Top };

	return GWin->SetWindowSize(Size, Pos);

}
bool CWindowMgr::ChangeMouseCursor(HWND _hWnd, tstring _Path)
{
	SPTR<CGameWindow> GWin = FindGameWindow(_hWnd);

	if (nullptr == GWin) 
	{
		return FALSE;
	}

	return GWin->ChangeBaseCursor(_Path);

}
bool CWindowMgr::ChangeMouseCursor(tstring _Name, tstring _Path) 
{
	SPTR<CGameWindow> GWin = FindGameWindow(_Name);

	if (nullptr == GWin)
	{
		return FALSE;
	}

	return GWin->ChangeBaseCursor(_Path);
	
}
bool CWindowMgr::ChangeIcon(tstring _Name ,tstring _Path) 
{
	SPTR<CGameWindow> GWin = FindGameWindow(_Name);

	if (nullptr == GWin) 
	{
		return FALSE;
	}

	HICON Icon = (HICON)LoadImage(
		CDX9Core::m_hInst
		, _Path.c_str()
		, IMAGE_ICON
		, 0, 0
		, LR_LOADFROMFILE
	);

	if (nullptr == Icon) 
	{
		return FALSE;
	}

	return GWin->ChangeIcon(Icon);

}
bool CWindowMgr::ChangeIcon(HWND _hWnd, tstring _Path) 
{
	SPTR<CGameWindow> GWin = FindGameWindow(_hWnd);

	if (nullptr == GWin)
	{
		return FALSE;
	}

	HICON Icon = (HICON)LoadImage(
		CDX9Core::m_hInst
		, _Path.c_str()
		, IMAGE_ICON
		, 0, 0
		, LR_LOADFROMFILE
	);

	if (nullptr == Icon)
	{
		return FALSE;
	}

	return GWin->ChangeIcon(Icon);
}
bool CWindowMgr::ChangeSmallIcon(tstring _Name, tstring _Path)
{
	SPTR<CGameWindow> GWin = FindGameWindow(_Name);

	if (nullptr == GWin)
	{
		return FALSE;
	}

	HICON Icon = (HICON)LoadImage(
		CDX9Core::m_hInst
		, _Path.c_str()
		, IMAGE_ICON
		, 0, 0
		, LR_LOADFROMFILE
	);

	if (nullptr == Icon)
	{
		return FALSE;
	}

	return GWin->ChangeSmallIcon(Icon);
}
bool CWindowMgr::ChangeSmallIcon(HWND _hWnd, tstring _Path)
{
	SPTR<CGameWindow> GWin = FindGameWindow(_hWnd);

	if (nullptr == GWin)
	{
		return FALSE;
	}

	HICON Icon = (HICON)LoadImage(
		CDX9Core::m_hInst
		, _Path.c_str()
		, IMAGE_ICON
		, 0, 0
		, LR_LOADFROMFILE
	);

	if (nullptr == Icon)
	{
		return FALSE;
	}

	return GWin->ChangeSmallIcon(Icon);
}