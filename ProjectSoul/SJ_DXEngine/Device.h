#pragma once
#include"DXHeader.h"

class CDevice
{
private:
	class CDeviceDestroyer 
	{
		friend CDevice;

	public:
		~CDeviceDestroyer() 
		{
			CDevice::Release();
		}
	} ;

	static CDeviceDestroyer			DeviceDes;

private:
	friend class CGameWindow;
	friend class CScene;

	// DX9의 Device : 모든 그래픽 작업을 제어
	// DX11에선 9에서 모든작업을 제어한 Device를 세분화 시킴

	static bool								bDeviceInit;
	static ID3D11Device*					pDevice;

	// 마소에서 지원하는 다이렉트11의 그래픽카드 랜더연산 관련 객체.
	static ID3D11DeviceContext*				pContext;

	// 출력될 백버퍼 다겟의 뷰객체.
	static ID3D11RenderTargetView*			pTargetView;

	// 랜더링시 깊이값에 대한 View객체.
	static ID3D11DepthStencilView*			pDepthStancilView;
	
	// 깊이값을 저장할 텍스처 객체.
	static ID3D11Texture2D*					pDepthStancilTex;

	static IDXGISwapChain*					pSwapChain;

	static UINT								uiMsLv;
	static UINT								uiMsaaQuailty;
	static FColor							ClearColor;


public:
	static ID3D11Device* GetDevice() { return pDevice; }
	static ID3D11DeviceContext* GetContext() { return pContext; }

private:
	static BOOL Init();
	static void Release();
	static void Present();
	static void Clear();

private:
	static BOOL CreateSwapChain();
	static BOOL CreateDepthStancilView();
	static BOOL CreateViewPort();

public:
	CDevice() {}
	virtual ~CDevice() = 0;
};




#define DXDEVICE CDevice::GetDevice()
#define DXCONTEXT CDevice::GetContext()