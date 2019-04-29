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

	// DX9�� Device : ��� �׷��� �۾��� ����
	// DX11���� 9���� ����۾��� ������ Device�� ����ȭ ��Ŵ

	static bool								bDeviceInit;
	static ID3D11Device*					pDevice;

	// ���ҿ��� �����ϴ� ���̷�Ʈ11�� �׷���ī�� �������� ���� ��ü.
	static ID3D11DeviceContext*				pContext;

	// ��µ� ����� �ٰ��� �䰴ü.
	static ID3D11RenderTargetView*			pTargetView;

	// �������� ���̰��� ���� View��ü.
	static ID3D11DepthStencilView*			pDepthStancilView;
	
	// ���̰��� ������ �ؽ�ó ��ü.
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