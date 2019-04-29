#include "stdafx.h"
#include "GameWindow.h"


CGameWindow::CGameWindow()
	:CObjBase(_T("")), m_Title(_T("")), m_hWnd(nullptr), m_hDC(nullptr), m_CurScene(nullptr)
	, m_PairDevice(nullptr), m_PairResMgr(nullptr), m_PairDebugMgr(nullptr)
	, m_ColorRef(RGB(0,0,0))
{

}
CGameWindow::CGameWindow(tstring _Name, tstring _Title /* = _Name*/)
	:CObjBase(_Name), m_Title(_Title),m_hWnd(nullptr),m_hDC(nullptr),m_CurScene(nullptr)
	, m_PairDevice(nullptr),m_PairResMgr(nullptr),m_PairDebugMgr(nullptr)
	, m_ColorRef(RGB(0, 0, 0))
{
	if (_T("") == m_Title) 
	{
		m_Title = Name();
	}

	CreateGameWindow();
	ShowGameWindow();

	RECT Rc;
	GetClientRect(m_hWnd, &Rc);

	m_WndPos = { (Rc.right + Rc.left) / 2,  (Rc.bottom + Rc.top) / 2};
	m_WndSize = { Rc.right - Rc.left,  Rc.bottom - Rc.top };

	m_hCursor = LoadCursor(0, IDC_ARROW);
}

CGameWindow::CGameWindow(HWND _hWnd, tstring _Name, tstring _Title /*= _T("")*/)
	:CObjBase(_Name), m_Title(_Title), m_hWnd(_hWnd), m_hDC(nullptr), m_CurScene(nullptr)
	, m_PairDevice(nullptr), m_PairResMgr(nullptr)
	, m_ColorRef(RGB(0, 0, 0))
{
	assert(m_hWnd);
	if (_T("") == m_Title)
	{
		m_Title = Name();
	}

	SetWindowText(m_hWnd, m_Title.c_str());
	m_hDC = GetDC(m_hWnd);
	
	RECT WndRect;
	GetClientRect(m_hWnd, &WndRect);
	m_WndSize = { WndRect.right - WndRect.left,WndRect.bottom - WndRect.top };
	m_WndPos = { WndRect.left + (m_WndSize.x / 2),WndRect.top + (m_WndSize.y / 2) };

}

CGameWindow::~CGameWindow()
{
	m_mapScene.clear();
}
//////////////////////////////////////////////// 3D Device ////////////////////////////////

bool CGameWindow::CreateDevice()
{
	SPTR<CGDevice> NewDevice = new CGDevice(this);

	if (nullptr == NewDevice->InitDevice())
	{
		return FALSE;
	}


	m_PairDevice = NewDevice;
	m_PairResMgr = new CResourceMgr(this, m_PairDevice->m_pDevice);
	m_PairDebugMgr = new CDebugMgr(this, m_PairDevice->m_pDevice);
	return TRUE;

}
void CGameWindow::DeviceRenderStart()
{
	m_PairDevice->RenderStart();
}
void CGameWindow::DeviceRenderEnd()
{
	m_PairDevice->RenderEnd();
}
LPDIRECT3DDEVICE9	CGameWindow::GetLPDevice()
{
	return m_PairDevice->m_pDevice;
}
SPTR<CGDevice>	CGameWindow::GetWndDevice()
{
	return m_PairDevice;
}

////////////////////////////////////////////////Game Logic////////////////////////////////
bool CGameWindow::Progress()
{
	assert(m_CurScene);

	if (nullptr == m_CurScene) 
	{
		return FALSE;
	}
	m_CurScene->Progress();

	return TRUE;
}


///////////////////////////////////////////////Scene Function/////////////////////////////////

SPTR<CGameScene> CGameWindow::FindGameScene(const tstring& _Name) 
{
	SPTR<CGameScene> Scene = FINDMAP_SPTR<CGameScene>(_Name, m_mapScene);

	if (nullptr == Scene) 
	{
		return  nullptr;
	}

	return Scene;
}
bool CGameWindow::ChangeScene(const tstring& _Name)
{
	SPTR<CGameScene> ChangeScene = FindGameScene(_Name);

	if (nullptr == ChangeScene)
	{
		return FALSE;
	}

	m_CurScene = ChangeScene;
	CActObject::m_pCurScene = m_CurScene.p;
	m_CurScene->Loading();

	m_CurScene->WakeUp();
	return TRUE;
}

////////////////////////////////////////////////Window Create & Setting////////////////////////////////
CGameWindow* CGameWindow::CreateGameWindow()
{
	HINSTANCE hInst = CWindowMgr::Inst().m_hInst;

	ATOM Check = MyRegisterClass(hInst);

	m_hWnd = CreateWindowW(Name_str(), Title_str(), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInst, nullptr);

	if (nullptr == m_hWnd)
	{
		return nullptr;
	}

	m_hDC = GetDC(m_hWnd);

	return this;
}
CGameWindow* CGameWindow::CreateSplashWindow()
{
	HINSTANCE hInst = CWindowMgr::Inst().m_hInst;

	ATOM Check = MyRegisterClass(hInst, (HBRUSH)(COLOR_WINDOW+2));

	m_hWnd = CreateWindowW(Name_str(), Title_str(), WS_EX_LAYERED,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInst, nullptr);
	

	/*m_hWnd = CreateWindowEx(WS_EX_LAYERED, Name_str(), Title_str()
		, WS_POPUPWINDOW ^ WS_BORDER
		, CW_USEDEFAULT, 0
		, CW_USEDEFAULT, 0
		, nullptr, nullptr
		, hInst
		, nullptr
	);*/

	if (nullptr == m_hWnd)
		return nullptr;
	

	m_hDC = GetDC(m_hWnd);

	return this;
}
ATOM CGameWindow::MyRegisterClass(HINSTANCE _hInst, const HBRUSH& BackColor /*= (HBRUSH)(COLOR_WINDOW + 1)*/)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = CWindowMgr::Inst().WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = _hInst;
	wcex.hIcon = nullptr;
	wcex.hCursor =nullptr;
	wcex.hbrBackground = BackColor;
	wcex.lpszMenuName  = nullptr ;
	wcex.lpszClassName = Name_str();
	wcex.hIconSm = nullptr;

	return RegisterClassExW(&wcex);
}
bool CGameWindow::ShowGameWindow(int _ShowMode /* = SW_HIDE*/)
{
	if (NULL == m_hWnd)
	{
		return FALSE;
	}

	ShowWindow(m_hWnd, _ShowMode);
	UpdateWindow(m_hWnd);

	return TRUE;
}

bool CGameWindow::WndProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	switch (message)
	{
	case WM_MOUSEWHEEL:
	{
		CKeyMgr::MouseWheelEvent((int)wParam);
	}
	break;
	case WM_MOUSEMOVE:
	case WM_SETCURSOR:
	{
		return UpdateCursor();
	}
	break;
	case WM_KILLFOCUS: 
	{
		CKeyMgr::UpdateOff();
	}
	break;
	case WM_SETFOCUS: 
	{
		CKeyMgr::UpdateOn();
	}
	break;

	}


	return FALSE;

}
bool CGameWindow::SetWindowSize(POINT _Size, POINT _Pos)
{
	RECT Rc = { 0,0, _Size.x, _Size.y };
	m_WndPos = _Pos;

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


	bool Check = SetWindowPos(m_hWnd, nullptr
		, m_WndPos.x - SizeHX - biasX
		, m_WndPos.y - SizeHY - biasY
		, Rc.right - Rc.left
		, Rc.bottom - Rc.top
		, SWP_NOZORDER
	);


	GetClientRect(m_hWnd, &Rc);

	m_WndSize = { Rc.right - Rc.left, Rc.bottom - Rc.top };

	return Check;
}

bool CGameWindow::UpdateCursor()
{
	/*if (NULL == m_hCursor) 
	{
		SetCursor(NULL);
	}*/
	SetCursor(m_hCursor);
	return TRUE;
}
bool CGameWindow::ChangeBaseCursor(tstring _Path)
{
	HCURSOR Cursor = nullptr;


	Cursor = LoadCursorFromFile(_Path.c_str());


	if (nullptr == Cursor) 
		return FALSE;
	

	m_hCursor = Cursor;
	return TRUE;

}
bool CGameWindow::ChangeIcon(HICON _hIcon)
{
#ifdef X32
	SetClassLong(m_hWnd, GCL_HICON , (LONG)_hIcon);
#else
	SetClassLongPtr(m_hWnd, GCLP_HICON, (LONG_PTR)_hIcon);
#endif
	SetWindowText(m_hWnd, _T("ProjectFR"));
	return TRUE;
}
bool CGameWindow::ChangeSmallIcon(HICON _hIcon)
{
#ifdef X32
	SetClassLong(m_hWnd, GCL_HICONSM, (LONG)_hIcon);
#else
	SetClassLongPtr(m_hWnd, GCLP_HICONSM, (LONG_PTR)_hIcon);
#endif
	SetWindowText(m_hWnd, _T("ProjectFR"));
	return TRUE;
}
bool CGameWindow::ChangeWndTitle(const tstring& _Title)
{
	bool Check = false;

	Check = SetWindowText(m_hWnd, _Title.c_str());

	if (false == Check)
		return Check;

	m_Title = _Title;
	return Check;
}
const BOOL CGameWindow::SetGameWindowColorKey(const COLORREF& _ColorKey)
{
	m_ColorRef = _ColorKey;
	return SetLayeredWindowAttributes(m_hWnd, m_ColorRef, NULL, LWA_COLORKEY);
}
const BOOL CGameWindow::SetGameWindowAlpha(const BYTE& _Alpha)
{
	return SetLayeredWindowAttributes(m_hWnd, NULL, _Alpha, LWA_ALPHA);
	//return SetLayeredWindowAttributes(m_hWnd, RGB(0, 0, 0), 100, LWA_COLORKEY);
}
POINT CGameWindow::GetMousePoint()
{
	POINT MousePos;
	GetCursorPos(&MousePos);
	ScreenToClient(m_hWnd, &MousePos);
	return MousePos;
}
Vec2 CGameWindow::GetMousePos()
{
	POINT CurMousePoint = GetMousePoint();
	Vec2 WorldPos = { (float)CurMousePoint.x,(float)CurMousePoint.y };

	WorldPos.x = WorldPos.x - (m_WndSize.x * 0.5f);
	WorldPos.y = -(WorldPos.y - (m_WndSize.y * 0.5f));

	return WorldPos;
}
bool CGameWindow::InvalidateWndCursor()
{
	m_hCursor = NULL;
	return true;
}
Vec3 CGameWindow::WndPosToWorldPos(const POINT& _WndPos)
{
	Vec3 WorldPos = { (float)_WndPos.x,(float)_WndPos.y ,100.0f};

	WorldPos.x = WorldPos.x - (m_WndSize.x * 0.5f);
	WorldPos.y = -(WorldPos.y - (m_WndSize.y * 0.5f));

	return WorldPos;
}
Vec3 CGameWindow::WndPosToWorldPos(float _WndX, float _WndY)
{

	Vec3 WorldPos = { _WndX,_WndY ,100.0f };

	WorldPos.x = WorldPos.x - (m_WndSize.x * 0.5f);
	WorldPos.y = -(WorldPos.y - (m_WndSize.y * 0.5f));

	return WorldPos;
}