#pragma once
#include"DXHeader.h"
#include<string>
#include<crtdbg.h>


typedef class CSJ_DXCore
{
public:
	class CCoreBuilder 
	{
		friend class CSJ_DXCore;

	protected:
		virtual const BOOL Build() = 0;
		virtual void BuilderProc(HWND _hWnd, UINT _Msg, WPARAM _wParam, LPARAM _lParam) = 0;
		virtual const BOOL Release() = 0;;


	protected:
		CCoreBuilder();
		virtual ~CCoreBuilder();
	};

private:
	static	bool				m_bCoreProgress;
	static  CCoreBuilder*		m_pBuilder;


private:
	static BOOL CoreInit(CCoreBuilder* _Builder);
	static void CoreLoop();
	static void CoreRelease();

public:
	static void CoreOperation(CCoreBuilder* _Builder);
	static void QuitCore();
	static LRESULT CALLBACK WndProc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam);

//public:
//	static void CreateDefaultResource();
//private:
//	static void CreateMesh();
//	static void CreateShaderResource();
//	static void CreateState();

	// Window ���� �Լ�
public:
	static BOOL CreateMainWindow(const HINSTANCE _hInst, const wchar_t* _Title);
	static BOOL CreateMainWindow(const HINSTANCE _hInst, const wchar_t* _Title, const POINT& _Size, const POINT& _Pos, const bool& _Show = true);
	static BOOL CreateMainWindow(const HINSTANCE _hInst, const wchar_t* _Title, const Vec2& _Size, const Vec2& _Pos, const bool& _Show = true);
	static BOOL CreateMainWindow(const HINSTANCE _hInst, const HWND& _hWnd, const wchar_t* _Title);



	// Window ���� �Լ�
public:
	static bool SetWindowSize(const POINT& _Size);
	static bool SetWindowSize(const POINT& _Size, const POINT& _Pos);
	static BOOL ChangeWindowTitle(std::wstring _Title);
	static BOOL ChangeMouseCursor(std::wstring _Path);
	static BOOL ChangeIcon(std::wstring _Path);
	static BOOL ChangeSmallIcon(std::wstring _Path);

public:
	static const POINT WolrdPosToWndPos(const POINT& _WorldPos);


public:
	static const POINT GetWindowSize();
	static const LONG GetWindowWidth();
	static const LONG GetWindowHeight();
	static const float GetWindowHypotenuse();	// ������ �밢�� ����
	static const float GetWindowAspect();
	static const Vec3 GetScreenMousePos();
	static const HINSTANCE GetWindowInstance();
	static const HWND GetWindowWnd();

public:
	static void ShowGameWindow(const bool& _Show);


public:
	CSJ_DXCore();
	~CSJ_DXCore();

}CORE, Core;


#define  CORESTART( BuilderType , Title , Size , Pos) \
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, \
	_In_opt_ HINSTANCE hPrevInstance, \
	_In_ LPWSTR    lpCmdLine, \
	_In_ int       nCmdShow) \
{ \
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); \
	new int; \
	BuilderType Temp = BuilderType(); \
	if (!CSJ_DXCore::CreateMainWindow( hInstance,Title , Size, Pos) )\
		return -1;\
	CSJ_DXCore::CoreOperation(&Temp); \
	return 0; \
}

#define  CORESTART_HIDE( BuilderType , Title , Size , Pos) \
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, \
	_In_opt_ HINSTANCE hPrevInstance, \
	_In_ LPWSTR    lpCmdLine, \
	_In_ int       nCmdShow) \
{ \
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); \
	new int; \
	BuilderType Temp = BuilderType(); \
	if (!CSJ_DXCore::CreateMainWindow( hInstance, Title , Size, Pos , false) )\
		return -1;\
	CSJ_DXCore::CoreOperation(&Temp); \
	return 0; \
}

#define  SIMPLECORESTART( BuilderType , Title) \
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, \
	_In_opt_ HINSTANCE hPrevInstance, \
	_In_ LPWSTR    lpCmdLine, \
	_In_ int       nCmdShow) \
{ \
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); \
	new int; \
	BuilderType Temp = BuilderType(); \
	if (!CSJ_DXCore::CreateMainWindow(hInstance,Title))\
		return -1;\
	CSJ_DXCore::CoreOperation(&Temp); \
	return 0; \
}

