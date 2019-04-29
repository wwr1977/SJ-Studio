// SSDGameServer.cpp : ���� ���α׷��� ���� �������� �����մϴ�.
//
#include <SSDServerCore.h>
#include <SSDCore.h>
#include <DeadByDaylightGlobal.h>
#include <TimerMgr.h>
#include <DBDServerManager.h>
#include "SSDGameServer.h"
#include <IOCPServer.h>
#include <IOCPServerMgr.h>
#include <Device.h>

#define MAX_LOADSTRING 100


// ���� ����:
HINSTANCE hInst;                                // ���� �ν��Ͻ��Դϴ�.
WCHAR szTitle[MAX_LOADSTRING];                  // ���� ǥ���� �ؽ�Ʈ�Դϴ�.
WCHAR szWindowClass[MAX_LOADSTRING];            // �⺻ â Ŭ���� �̸��Դϴ�.

// �� �ڵ� ��⿡ ��� �ִ� �Լ��� ������ �����Դϴ�.
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

SSDServerCore			ServerCore;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{

    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: ���⿡ �ڵ带 �Է��մϴ�.

    // ���� ���ڿ��� �ʱ�ȭ�մϴ�.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_SSDGAMESERVER, szWindowClass, MAX_LOADSTRING);

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); 
	//new int;

	//SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);

    // �⺻ �޽��� �����Դϴ�.
	//ServerCore.pServerManager = new DBDServerManager();
	//ServerCore.ServerOperation<CIOCPServer>(LOGICPORT, true);
	
	//CDevice::Init();
	CFilePathMgr::Init();
	CFilePathMgr::AddRootFolder(L"Scene");
	CTimerMgr::Init();

	DBD::ReadScene(CFilePathMgr::GetPath(L"Scene", L"060-0408-Final.scene").c_str());

	PacketProcessing::Init(ServerCore.ServerOperation<IOCPServerMgr>(LOGICPORT, false));
	CRoom::Init(reinterpret_cast<IOCPServerMgr*>(ServerCore.ServerUpdator));

	
	float dT = 0.f;

	while (false == ServerCore.ServerUpdator->IsInitComplete());

	while (true) 
	{
		CTimerMgr::Update();
		dT += CTimerMgr::DeltaTime();
		ServerCore.ServerUpdator->PrevUpdate(CTimerMgr::DeltaTime());
		ServerCore.ServerUpdator->Update(CTimerMgr::DeltaTime());


		//// ���� ������Ʈ�� 1�ʿ� 60���������� ����
		//if (dT >= GAMEUPDATETIME)
		//{
		//	// ������Ʈ ������
		//	ServerCore.ServerUpdator->Update(dT);
		//	dT = 0.f;
		//}

		//ServerCore.ServerUpdator->PostUpdate(CTimerMgr::DeltaTime());

		Sleep(1);
	}

	CDevice::Release();
	//DELETE_POINTER(ServerCore.pServerManager);

	return 0;
}



//
//  �Լ�: MyRegisterClass()
//
//  ����: â Ŭ������ ����մϴ�.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SSDGAMESERVER));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = nullptr;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}


BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // �ν��Ͻ� �ڵ��� ���� ������ �����մϴ�.

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // �޴� ������ ���� �м��մϴ�.
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: ���⿡ hdc�� ����ϴ� �׸��� �ڵ带 �߰��մϴ�.
            EndPaint(hWnd, &ps);
        }
        break;
	case WM_DESTROY: 
	{
        PostQuitMessage(0);
		ServerCore.ServerQuit();
	}
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// ���� ��ȭ ������ �޽��� ó�����Դϴ�.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
