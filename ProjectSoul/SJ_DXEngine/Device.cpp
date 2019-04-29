#include"Device.h"
#include"GameWindow.h"
#include"DXMacro.h"


bool							CDevice::bDeviceInit = false;

CDevice::CDeviceDestroyer		CDevice::DeviceDes;

ID3D11Device*					CDevice::pDevice = nullptr;
ID3D11DeviceContext*			CDevice::pContext = nullptr;
ID3D11RenderTargetView*			CDevice::pTargetView = nullptr;
ID3D11DepthStencilView*			CDevice::pDepthStancilView = nullptr;
ID3D11Texture2D*				CDevice::pDepthStancilTex = nullptr;
IDXGISwapChain*					CDevice::pSwapChain = nullptr;
UINT							CDevice::uiMsLv = 4;
UINT							CDevice::uiMsaaQuailty = 0;
FColor							CDevice::ClearColor = {0.f,0.f,0.f,1.f};

BOOL CDevice::Init() 
{
	int iFlag = 0;

#ifdef _DEBUG
	iFlag = D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL eLv = D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0;

	if (S_OK != D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_HARDWARE
		, nullptr, iFlag, nullptr, 0, D3D11_SDK_VERSION, &pDevice, &eLv, &pContext))
	{
		Release();
		return FALSE;
	}

	HRESULT Check = E_FAIL;

	// MSAA(MultiSampling Anti-Aliasing) x4,x2,x1 ������ �ϵ���� ���ɿ� �´� ���� ����
	while (uiMsLv >= 1)
	{
		Check =pDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, uiMsLv, &uiMsaaQuailty);
	
		if (Check == S_OK)
			break;

		uiMsLv /= 2;
	}
	
	if (FALSE == CreateSwapChain()) 
	{
		Release();
		return FALSE;
	}

	if (FALSE == CreateDepthStancilView())
	{
		Release();
		return FALSE;
	}

	CreateViewPort();

	bDeviceInit = true;
	return TRUE;
}
void CDevice::Release() 
{
	if (false == bDeviceInit)
		return;


	if (pDepthStancilTex) pDepthStancilTex->Release();
	if (pDepthStancilView) pDepthStancilView->Release();
	if (pSwapChain)pSwapChain->Release();
	if (pTargetView)	pTargetView->Release();
	if (pContext) pContext->Release();
	if (pDevice) pDevice->Release();
}

void CDevice::Clear() 
{
	pContext->ClearRenderTargetView(pTargetView, ClearColor.pf);
	pContext->ClearDepthStencilView(pDepthStancilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
}
void CDevice::Present() 
{
	pSwapChain->Present(0, 0);
}
BOOL CDevice::CreateSwapChain() 
{
	// ����ü�ο� ���� �ʱ�ȭ ������ ����
	DXGI_SWAP_CHAIN_DESC tDesc = {};

	tDesc.BufferDesc.Width  =CGameWindow::WndSize.x;
	tDesc.BufferDesc.Height = CGameWindow::WndSize.y;

	// ����� �ֻ����� ���� ���� ���� Ƚ���� �ǹ��Ѵ�.
	tDesc.BufferDesc.RefreshRate.Numerator = 60;
	tDesc.BufferDesc.RefreshRate.Denominator = 1;

	tDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	tDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	tDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// ������� �뵵�� ����.
	tDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	// ����ü�� ��Ƽ���ø��� ������ ���
	//if (uiMsLv >= 2) 
	//{
	//	tDesc.SampleDesc.Count = uiMsLv;
	//	tDesc.SampleDesc.Quality = uiMsaaQuailty - 1;
	//}
	//else // ��Ƽ ���ø� ������� �ʴ� ����ü��
	//{
	//	tDesc.SampleDesc.Count = 1;
	//	tDesc.SampleDesc.Quality = 0;
	//}

	// ����ü�ο����� ��Ƽ���ø��� ������� �ʵ��� ����.
	tDesc.SampleDesc.Count = 1;
	tDesc.SampleDesc.Quality = 0;

	tDesc.OutputWindow = CGameWindow::hWnd;
	tDesc.BufferCount = 1;
	
	// â��带 �����Ұ����� 
	tDesc.Windowed = CGameWindow::IsWindowMode();
	// ���� ����Ʈ�� ����, Discard�� �����ϸ� ���÷��� �����Ⱑ �˾Ƽ� ����
	tDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	//����ü���� �߰����� �÷���
	tDesc.Flags = 0;

	// ����ü�� ����
	IDXGIDevice* pIdxDevice = nullptr;
	IDXGIAdapter* pIdxAdapter = nullptr;
	IDXGIFactory* pIdxFactory = nullptr; // ����ü�� ��ü�� ������� ���丮��ü

	pDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&pIdxDevice);
	if (nullptr == pIdxDevice)
		return FALSE;
	
	// pIdxDevice�� ���ؼ� ����ü���� ������ ���丮�� ����� �ִ� �����
	pIdxDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&pIdxAdapter);
	if (nullptr == pIdxAdapter)
		return FALSE;
	
	HRESULT Check;

	pIdxAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&pIdxFactory);
	Check = pIdxFactory->CreateSwapChain(pDevice, &tDesc, &pSwapChain);
	if (nullptr == pIdxFactory && S_OK != (Check = pIdxFactory->CreateSwapChain(pDevice, &tDesc, &pSwapChain)))
		return FALSE;

	if (nullptr == pSwapChain)
		return FALSE;


	pIdxDevice->Release();
	pIdxAdapter->Release();
	pIdxFactory->Release();

	//DXCONTEXT->PSSetTextrie(1, 1, pDepthStancilTex);
	
	return TRUE;
}
BOOL CDevice::CreateDepthStancilView()
{
	ID3D11Texture2D* pBackBufferTex = nullptr;

	if (S_OK != pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBufferTex))
		return FALSE;
	

	// ����Ÿ�ٺ�� ������ ���ؼ� �ؽ�ó���� ���� ���ϴ� ��� �����Ҽ� �ְ� ���ִ� ��ü�̴�.
	if (S_OK != pDevice->CreateRenderTargetView(pBackBufferTex, 0, &pTargetView))
		return FALSE;
	

	pBackBufferTex->Release();

	// ����ü�κ䰡 ����� �ý�ó�� �����.
	// �׸��� �̳༮�� ���� ���� ������ ���Ľ� �ؽ�ó�� �ȴ�.
	D3D11_TEXTURE2D_DESC tDecs = {};
	// 
	tDecs.ArraySize = 1;
	tDecs.Width = (UINT)CGameWindow::WndSize.x;
	tDecs.Height = (UINT)CGameWindow::WndSize.y;
	

	// �ܼ��� ���� ���� ���� �񱳸� �ʿ��ϹǷ� ��ġ�� ������ �������� ����
	tDecs.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	// ��Ƽ���ø� ������ ǰ������
	// ���� Ÿ�ٰ� ���� ǰ���� ������ �����ؾ��Ѵ�.
	tDecs.SampleDesc.Count = 1;
	tDecs.SampleDesc.Quality = 0;

	tDecs.MipLevels = 1;

	tDecs.Usage = D3D11_USAGE_DEFAULT;
	// ���� ���Ľǿ� �ؽ������� ǥ��
	tDecs.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	pDevice->CreateTexture2D(&tDecs, 0, &pDepthStancilTex);

	if (nullptr == pDepthStancilTex)
		return FALSE;
	

	// ���� ���Ľ� ����
	if (S_OK != pDevice->CreateDepthStencilView(pDepthStancilTex, 0, &pDepthStancilView))
		return FALSE;
	

	// Output Merger���� ����� ����� ����Ÿ�� �䰴ü�� �𽺽��Ľ� �䰴ü�� ����
	pContext->OMSetRenderTargets(1, &pTargetView, pDepthStancilView);
	return TRUE;
}


BOOL CDevice::CreateViewPort()
{
	D3D11_VIEWPORT VP_Data = {};
	VP_Data.TopLeftX = 0;
	VP_Data.TopLeftY = 0;
	VP_Data.Width = (float)CGameWindow::WndSize.x;
	VP_Data.Height = (float)CGameWindow::WndSize.y;
	VP_Data.MinDepth = 0.f;
	VP_Data.MaxDepth = 1.f;

	pContext->RSSetViewports(1, &VP_Data);

	return TRUE;
}
