#pragma once
#include "DXHeader.h"
#include <unordered_map>
#include "ConstantBuffer.h"
#include "DXTemplate.h"

#define MAXRENDERTARGET 8


class CDevice
{
private:
	friend class Engine;

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
	friend class SSDCore;
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
	static D3D11_VIEWPORT					VP_Data;

private:
	static bool																	bChangeRS;
	static class CRasterizerState*												BaseRS;

	static bool																	bChangeDS;
	static class CDepthStencilState*											BaseDS;
	static CDepthStencilState*													DisableDS;

	static bool																	bChangeBS;
	static class CBlendState*													BaseBS;
	static class CBlendState*													AccBS;

public:
	static class CConstantBuffer*												RenBaseCB;

public:
	static ID3D11RenderTargetView*												ClearTargetView[MAXRENDERTARGET];

public:
	static ID3D11Device* GetDevice() { return pDevice; }
	static ID3D11DeviceContext* GetContext() { return pContext; }

public:
	static BOOL Init();
	static void Release();
private:
	static void Present();
public:
	static void Clear();

private:
	static BOOL CreateSwapChain();
	static BOOL CreateDepthStancilView();
	static BOOL CreateViewPort();

public:
	static void SetBackBufferView();

public:
	static void SetBaseRS();
	static void SetRS(const wchar_t * _RSName);
	static void SetBaseDS();
	static void SetDS(const wchar_t * _RSName, const UINT & _StencilRef = 0);
	static void SetDisableDS();
	static void SetBaseBlend();

public:
	static void ViewPortUpdate();

public:
	static void ResetRenderTarget();
	static void ResetShaderResource();
	static void ResetShaderResourceTexture();
	static void ResetConstantBuffer();
public:
	CDevice() {}
	virtual ~CDevice() = 0;
};




#define DXDEVICE CDevice::GetDevice()
#define DXCONTEXT CDevice::GetContext()