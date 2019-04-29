#include"GameWindow.h"
#include"SJ_DXCore.h"
#include"Device.h"
#include"Scene.h"
#include"TimerMgr.h"
#include<atlstr.h>


bool						CGameWindow::bCreateWindow = false;
bool						CGameWindow::bFullScreen = false;
POINT						CGameWindow::MonitorSize = POINT{ GetSystemMetrics(SM_CXSCREEN) ,GetSystemMetrics(SM_CYSCREEN) };



HINSTANCE					CGameWindow::hInst = NULL;
HWND						CGameWindow::hWnd = NULL;
HDC							CGameWindow::hDC = NULL;
POINT						CGameWindow::WndPos;
POINT						CGameWindow::WndSize;
HCURSOR						CGameWindow::hCursor;
std::wstring				CGameWindow::WindowTitle = L"";

CGameWindow::CGameWindow()
{

}
CGameWindow::CGameWindow(HWND _hWnd)
{

}

ATOM CGameWindow::RegistWindowClass(HINSTANCE hInstance)
{
	hInst = hInstance;

	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = CSJ_DXCore::WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = nullptr;
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = DXWNDCLASSNAME;
	wcex.hIconSm = nullptr;

	return RegisterClassExW(&wcex);
}

const BOOL CGameWindow::CreateGameWindow(const wchar_t* _Title)
{
	hWnd = CreateWindowW(DXWNDCLASSNAME, _Title, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInst, nullptr);

	if (!hWnd)
		return FALSE;

	hDC = GetDC(hWnd);
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	bCreateWindow = true;

	return TRUE;
}
const BOOL CGameWindow::CreateGameWindow(const wchar_t* _Title, const POINT& _Size, const POINT& _Pos, const bool& _Show)
{
	hWnd = CreateWindowW(DXWNDCLASSNAME, _Title, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInst, nullptr);

	if (!hWnd)
		return FALSE;

	hDC = GetDC(hWnd);
	SetWindowSize(_Size, _Pos);
	CDevice::Init();
	
	if (_Show) 
	{
		ShowWindow(hWnd, SW_SHOW);
		UpdateWindow(hWnd);
	}

	bCreateWindow = true;
	return TRUE;
}
const BOOL CGameWindow::CreateGameWindow(const HWND& _hWnd, const wchar_t* _Title)
{
	hWnd = _hWnd;
	SetWindowTextW(_hWnd, _Title);

	hDC = GetDC(hWnd);
	CDevice::Init();

	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	bCreateWindow = true;
	return TRUE;
}
const POINT CGameWindow::MonitorPosToWndPos(const POINT& _WorldPoint)
{
	POINT ChangePos;

	ChangePos.x = (_WorldPoint.x + MonitorSize.x / 2);
	ChangePos.y = -(_WorldPoint.y - MonitorSize.y / 2);

	return ChangePos;
}
const POINT CGameWindow::WndPosToWorldPoint(const POINT& _WndPos)
{
	POINT ChangePos;

	ChangePos.x = (_WndPos.x - WndSize.x/ 2);
	ChangePos.y = -(_WndPos.y - WndSize.y / 2);

	return ChangePos;
}
const POINT CGameWindow::WorldPointToWnd(const POINT& _WorldPoint)
{
	POINT ChangePos;

	ChangePos.x = (_WorldPoint.x + WndSize.x / 2);
	ChangePos.y = -(_WorldPoint.y - WndSize.y / 2);

	return ChangePos;
}
bool CGameWindow::SetWindowSize(const POINT& _Size, const POINT& _Pos /*= POINT{MAXLONG,MAXLONG}*/)
{
	RECT Rc = { 0,0, _Size.x, _Size.y };

	if (_Pos.x == MAXLONG || _Pos.y == MAXLONG)
	{
		WndPos.x = MonitorSize.x / 2;
		WndPos.y = MonitorSize.y / 2;
	}
	else
		WndPos = MonitorPosToWndPos(_Pos);

	AdjustWindowRect(&Rc, WS_OVERLAPPEDWINDOW, false);

	//윈도우 틀을 포함한 윈도우 크기가 홀수인경우 Client 영역의 크기를 보정

	int biasX = 0;
	int biasY = 0;

	if (1 == (Rc.right - Rc.left) % 2)
	{
		biasX = 1;
	}
	if (1 == (Rc.bottom - Rc.top) % 2)
	{
		biasY = 1;
	}

	int SizeHX = (Rc.right - Rc.left) / 2;
	int SizeHY = (Rc.bottom - Rc.top) / 2;


	bool Check = SetWindowPos(hWnd, nullptr
		, WndPos.x - SizeHX - biasX
		, WndPos.y - SizeHY - biasY
		, Rc.right - Rc.left
		, Rc.bottom - Rc.top
		, SWP_NOZORDER
	);

	GetClientRect(hWnd, &Rc);

	WndSize = { Rc.right - Rc.left, Rc.bottom - Rc.top };

	return Check;
}

BOOL CGameWindow::ChangeBaseCursor(const std::wstring& _Path)
{
	HCURSOR Cursor = nullptr;

	Cursor = LoadCursorFromFileW(_Path.c_str());

	if (nullptr == Cursor)
		return FALSE;

	hCursor = Cursor;
	return TRUE;
}
BOOL CGameWindow::ChangeIcon(const std::wstring& _Path)
{
	HICON Icon = (HICON)LoadImageW(
		hInst
		, _Path.c_str()
		, IMAGE_ICON
		, 0, 0
		, LR_LOADFROMFILE
	);

	if (nullptr == Icon)
		return FALSE;

#ifdef X64
	SetClassLongPtr(hWnd, GCLP_HICON, (LONG_PTR)Icon);
#else
	SetClassLong(hWnd, GCL_HICON, (LONG)Icon);
#endif
	SetWindowTextW(hWnd, WindowTitle.c_str());
	return TRUE;
}
BOOL CGameWindow::ChangeSmallIcon(const std::wstring& _Path)
{
	HICON Icon = (HICON)LoadImageW(
		hInst
		, _Path.c_str()
		, IMAGE_ICON
		, 0, 0
		, LR_LOADFROMFILE
	);

	if (nullptr == Icon)
		return FALSE;

#ifdef X64
	SetClassLongPtr(hWnd, GCLP_HICONSM, (LONG_PTR)Icon);
#else
	SetClassLong(hWnd, GCL_HICONSM, (LONG)Icon);
#endif

	SetWindowTextW(hWnd, WindowTitle.c_str());
	return TRUE;
}
BOOL CGameWindow::ChangeWndTitle(const std::wstring& _Title)
{
	if (FALSE == SetWindowTextW(hWnd, _Title.c_str()))
		return FALSE;

	WindowTitle = _Title;
	return TRUE;
}

const POINT CGameWindow::GetMousePoint()
{
	POINT MousePos;
	GetCursorPos(&MousePos);
	ScreenToClient(hWnd, &MousePos);
	return MousePos;
}

const bool CGameWindow::IsFullScreen() 
{
	return bFullScreen;
}
const bool CGameWindow::IsWindowMode() 
{
	return !bFullScreen;
}
void CGameWindow::Progress(const float& _DeltaTime)
{
	CScene::SceneProgress(_DeltaTime);
	
	//std::wstring Title = L"TITAN SOUL  Fps : ";
	//Title += std::to_wstring(CTimerMgr::FPS());
	//ChangeWndTitle(Title);
}