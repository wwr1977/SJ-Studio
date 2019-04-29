#pragma once
#include <Windows.h>
#include <process.h>
#include <tchar.h>
#include <string>
#include <atlimage.h>
#include <atlstr.h>


class CDeviceCriticalSection
{
private:
	static bool m_bInit;
	static CRITICAL_SECTION CS;
public:
	CDeviceCriticalSection()
	{
		if (nullptr == CS.DebugInfo)
		{
			InitializeCriticalSection(&CS);
			m_bInit = true;
		}
		EnterCriticalSection(&CS);
	}
	~CDeviceCriticalSection()
	{
		LeaveCriticalSection(&CS);
	}
};


struct SPLASHDEVICEINFO 
{
	std::wstring				ClassName;
	std::wstring				Title;
	POINT						WndPos;
	POINT						WndSize;
	std::wstring				ImagePath;
	COLORREF					BackBuffColor;


	void operator=(const SPLASHDEVICEINFO& _Other) 
	{
		ClassName = _Other.ClassName;
		Title = _Other.Title;
		WndPos = _Other.WndPos;
		WndSize = _Other.WndSize;
		ImagePath = _Other.ImagePath;
		BackBuffColor = _Other.BackBuffColor;
	}

	SPLASHDEVICEINFO() 
		:ClassName(L""), Title(L"")
		, WndPos(POINT{ 0,0 }), WndSize(POINT{0,0})
		, ImagePath(L""), BackBuffColor(COLORREF())
	{}
	SPLASHDEVICEINFO(const SPLASHDEVICEINFO& _Other) 
		:ClassName(_Other.ClassName), Title(_Other.Title)
		, WndPos(_Other.WndPos), WndSize(_Other.WndSize)
		, ImagePath(_Other.ImagePath), BackBuffColor(_Other.BackBuffColor)
	{
	}
	~SPLASHDEVICEINFO() {}
};
class CSplashDevice
{
private:
	static CSplashDevice*				Device;
private:
	// 프로세스 관련 변수
	bool								m_bProcess;
	HANDLE								m_hProcessHandle;
	HINSTANCE							m_hProcessInst;
	
private:	
	// 스플레시 윈도우 관련 변수
	HWND								m_hSplashWnd;
	HDC									m_hSplashDC;
	HCURSOR								m_hCursor;
	POINT								m_WndPos;
	POINT								m_WndSize;
	std::wstring						m_ClassName;
	std::wstring						m_Title;

private:
	// 랜더링 이미지 관련 변수
	std::wstring						m_ImagePath;
	POINT								m_ImageSize;
	COLORREF							m_BackBuffColor;
	
	HDC									m_BackMemDC;
	HBITMAP								m_hBmp;
	HBITMAP								m_hOldBmp;


public:
	static CSplashDevice* Inst()
	{
		if (nullptr == Device)
			Device = new CSplashDevice();
		
		return Device;
	}
	void InitDevice(const SPLASHDEVICEINFO& _Info);
	void SetDeviceProcess(const bool& _Process);
	void EnableSplashWindow(const bool& _Enable);



private:
	void SetDeviceInfo(const SPLASHDEVICEINFO& _Info);
	void SetWndPos(const POINT& _Pos);
	void SetWndSize(const POINT& _Size) { Device->m_WndSize = _Size; }
	bool SetWindowSize();

	void SetSplashImage(const std::wstring& _ImagePath) { m_ImagePath = _ImagePath; }
	BOOL SetImageBackColor(const COLORREF& _BackColor);
	BOOL SetColorKeyAlpha();
	void SplashImageRender();
	static void DeviceRelease()
	{
		if (nullptr != Device)
		{
			delete Device;
			Device = nullptr;
		}
	}
	static unsigned int __stdcall DeviceProcess(void* _Arg);
	static LRESULT CALLBACK DeviceWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	ATOM MyRegisterClass(HINSTANCE _hInst);
	HWND CreateSplashWindow();
	bool ShowSplashWindow(int _ShowMode = SW_SHOW);
	void CreateBackBuffer();



private:
	CSplashDevice();
	~CSplashDevice();
};

