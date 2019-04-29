#include "SJ_SplashWindow.h"


bool CDeviceCriticalSection::m_bInit = false;
CRITICAL_SECTION CDeviceCriticalSection::CS;

CSplashDevice* CSplashDevice::Device = nullptr ;

CSplashDevice::CSplashDevice()
	:m_hProcessHandle(nullptr), m_hProcessInst(nullptr)
	, m_hSplashWnd(nullptr), m_hSplashDC(nullptr)
	, m_ClassName(_T("")), m_Title(_T(""))
	, m_hCursor(nullptr)
	, m_WndPos(POINT{0,0}), m_WndSize(POINT{ 0,0 })
	, m_bProcess(true)
	, m_ImagePath(_T(""))
	, m_ImageSize(POINT{0,0})
	, m_BackBuffColor(COLORREF())
	, m_BackMemDC(nullptr)
{

}


CSplashDevice::~CSplashDevice()
{
	SelectObject(m_BackMemDC, m_hOldBmp);
	DeleteObject(m_hBmp);
	DeleteDC(m_BackMemDC);
	ReleaseDC(m_hSplashWnd, m_hSplashDC);
	CloseHandle(m_hProcessHandle);
}
void CSplashDevice::InitDevice(const SPLASHDEVICEINFO& _Info)
{
	SetDeviceInfo(_Info);
	m_hProcessHandle = (HANDLE)_beginthreadex(NULL, 0, DeviceProcess, this, 0, nullptr);

}
void CSplashDevice::SetDeviceProcess(const bool& _Process)
{ 
	CDeviceCriticalSection();
	m_bProcess = _Process; 
	
}
void CSplashDevice::EnableSplashWindow(const bool& _Enable)
{
	if (nullptr == m_hSplashWnd)
		return;

	if (_Enable)
		SendMessage(m_hSplashWnd, WM_SHOWWINDOW, NULL, NULL);
	else
		SendMessage(m_hSplashWnd, SWP_HIDEWINDOW, NULL, NULL);
}
bool CSplashDevice::SetWindowSize()
{
	RECT Rc = { 0,0, m_WndSize.x, m_WndSize.y };

	AdjustWindowRect(&Rc, WS_POPUPWINDOW, false);

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


	bool Check = SetWindowPos(Device->m_hSplashWnd, nullptr
		, m_WndPos.x - SizeHX - biasX
		, m_WndPos.y - SizeHY - biasY
		, Rc.right - Rc.left
		, Rc.bottom - Rc.top
		, SWP_NOZORDER
	);


	GetClientRect(Device->m_hSplashWnd, &Rc);
	m_WndSize = { Rc.right - Rc.left, Rc.bottom - Rc.top };

	return Check;
}
BOOL CSplashDevice::SetImageBackColor(const COLORREF& _BackColor)
{
	m_BackBuffColor = _BackColor;
	return TRUE;
}
BOOL CSplashDevice::SetColorKeyAlpha()
{
	return SetLayeredWindowAttributes(m_hSplashWnd, m_BackBuffColor, NULL, LWA_COLORKEY);
}
void CSplashDevice::SplashImageRender()
{
	if (_T("") == m_ImagePath || nullptr == m_BackMemDC || nullptr == m_hSplashDC )
		return;

	CImage* SplashImage = new CImage();
	
	HRESULT hr = SplashImage->Load(m_ImagePath.c_str());

	if (FAILED(hr))
		return;

	m_ImageSize.x = (LONG)SplashImage->GetWidth();
	m_ImageSize.y = (LONG)SplashImage->GetHeight();

	SplashImage->StretchBlt(m_BackMemDC, 0, 0, m_WndSize.x, m_WndSize.y, 0, 0, m_ImageSize.x, m_ImageSize.y, SRCCOPY);
	
	BitBlt(m_hSplashDC, 0, 0, m_WndSize.x, m_WndSize.y, m_BackMemDC, 0, 0, SRCCOPY);

	delete SplashImage;
	SplashImage = nullptr;

	return;
}
void CSplashDevice::SetDeviceInfo(const SPLASHDEVICEINFO& _Info)
{
	m_ClassName = _Info.ClassName;
	m_Title = _Info.Title;
	m_ImagePath = _Info.ImagePath;
	SetWndPos(_Info.WndPos);
	SetWndSize(_Info.WndSize);
	m_BackBuffColor = _Info.BackBuffColor;

}
void CSplashDevice::SetWndPos(const POINT& _Pos)
{
	int ResolutionHX = GetSystemMetrics(SM_CXSCREEN) / 2;
	int ResolutionHY = GetSystemMetrics(SM_CYSCREEN) / 2;

	m_WndPos.x = _Pos.x + ResolutionHX;
	m_WndPos.y = -(_Pos.y - ResolutionHY);
}
unsigned int __stdcall CSplashDevice::DeviceProcess(void* _Arg)
{
	Device->m_hProcessInst = GetModuleHandle(NULL);
	
	if (nullptr == Device->CreateSplashWindow())
		return -1;

	Device->ShowSplashWindow();
	Device->SetWindowSize();
	Device->m_hCursor = LoadCursor(0, IDC_ARROW);
	BOOL Check = Device->SetColorKeyAlpha();
	Device->SplashImageRender();
	Device->EnableSplashWindow(true);
	
	MSG msg;
	
	while (true) 
	{
		CDeviceCriticalSection();
		if (!Device->m_bProcess)
			break;
		//PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		Sleep(1000);
	}

	CSplashDevice::DeviceRelease();
	return 0;
}
LRESULT CALLBACK CSplashDevice::DeviceWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if(nullptr == Device )
		return DefWindowProc(hWnd, message, wParam, lParam);

	switch (message)
	{
	case WM_PAINT:
		Device->SplashImageRender();
		break;
	case SW_SHOW:
		Device->SplashImageRender();
		break;
	default:
		break;
	}

	return TRUE;
}
ATOM CSplashDevice::MyRegisterClass(HINSTANCE _hInst)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = DeviceWndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = _hInst;
	wcex.hIcon = nullptr;
	wcex.hCursor = nullptr;
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = m_ClassName.c_str();
	wcex.hIconSm = nullptr;

	return RegisterClassExW(&wcex);
}
HWND CSplashDevice::CreateSplashWindow()
{
	HINSTANCE hInst = Device->m_hProcessInst;

	ATOM Check = MyRegisterClass(hInst);
	
	m_hSplashWnd = CreateWindowEx(WS_EX_LAYERED | WS_EX_TOOLWINDOW, m_ClassName.c_str(), m_Title.c_str()
		, WS_POPUPWINDOW ^ WS_BORDER
		, CW_USEDEFAULT, 0
		, CW_USEDEFAULT, 0
		, nullptr, nullptr
		, hInst
		, nullptr
	);

	if (nullptr == m_hSplashWnd)
		return nullptr;


	m_hSplashDC = GetDC(m_hSplashWnd);
	CreateBackBuffer();
	return m_hSplashWnd;
}
bool CSplashDevice::ShowSplashWindow(int _ShowMode /*= SW_HIDE*/)
{
	if (nullptr == m_hSplashWnd)
		return FALSE;

	ShowWindow(m_hSplashWnd, _ShowMode);
	UpdateWindow(m_hSplashWnd);

	return TRUE;
}
void CSplashDevice::CreateBackBuffer()
{
	m_BackMemDC = CreateCompatibleDC(m_hSplashDC);

	m_hBmp = CreateCompatibleBitmap(m_hSplashDC, m_WndSize.x, m_WndSize.y);

	m_hOldBmp = (HBITMAP)SelectObject(m_BackMemDC, m_hBmp);

	BITMAP ImageInfo;
	GetObject(m_hBmp, sizeof(ImageInfo), &ImageInfo);
	Rectangle(m_BackMemDC, 0, 0, m_WndSize.x, m_WndSize.y);
	
}
