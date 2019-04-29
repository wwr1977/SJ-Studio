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

	// MSAA(MultiSampling Anti-Aliasing) x4,x2,x1 설정을 하드웨어 성능에 맞는 값을 저장
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
	// 스왑체인에 대한 초기화 데이터 셋팅
	DXGI_SWAP_CHAIN_DESC tDesc = {};

	tDesc.BufferDesc.Width  =CGameWindow::WndSize.x;
	tDesc.BufferDesc.Height = CGameWindow::WndSize.y;

	// 모니터 주사율에 따른 버퍼 갱신 횟수를 의미한다.
	tDesc.BufferDesc.RefreshRate.Numerator = 60;
	tDesc.BufferDesc.RefreshRate.Denominator = 1;

	tDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	tDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	tDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// 백버퍼의 용도가 뭐냐.
	tDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	// 스왑체인 멀티샘플링을 적용할 경우
	//if (uiMsLv >= 2) 
	//{
	//	tDesc.SampleDesc.Count = uiMsLv;
	//	tDesc.SampleDesc.Quality = uiMsaaQuailty - 1;
	//}
	//else // 멀티 샘플링 사용하지 않는 스왑체인
	//{
	//	tDesc.SampleDesc.Count = 1;
	//	tDesc.SampleDesc.Quality = 0;
	//}

	// 스왕체인에서는 멀티샘플링을 사용하지 않도록 설정.
	tDesc.SampleDesc.Count = 1;
	tDesc.SampleDesc.Quality = 0;

	tDesc.OutputWindow = CGameWindow::hWnd;
	tDesc.BufferCount = 1;
	
	// 창모드를 지원할것인지 
	tDesc.Windowed = CGameWindow::IsWindowMode();
	// 스왑 이펙트를 설정, Discard를 선택하면 디스플레이 구동기가 알아서 선택
	tDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	//스왑체인의 추가적인 플래그
	tDesc.Flags = 0;

	// 스왑체인 생성
	IDXGIDevice* pIdxDevice = nullptr;
	IDXGIAdapter* pIdxAdapter = nullptr;
	IDXGIFactory* pIdxFactory = nullptr; // 스왑체인 객체를 만들어줄 팩토리객체

	pDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&pIdxDevice);
	if (nullptr == pIdxDevice)
		return FALSE;
	
	// pIdxDevice를 통해서 스왑체인을 생성할 팩토리를 만들수 있는 어댑터
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
	

	// 랜더타겟뷰는 간단히 말해서 텍스처등을 내가 원하는 대로 제어할수 있게 해주는 객체이다.
	if (S_OK != pDevice->CreateRenderTargetView(pBackBufferTex, 0, &pTargetView))
		return FALSE;
	

	pBackBufferTex->Release();

	// 스왑체인뷰가 담당할 택스처를 만든다.
	// 그리고 이녀석은 깊이 값을 저장할 스탠실 텍스처가 된다.
	D3D11_TEXTURE2D_DESC tDecs = {};
	// 
	tDecs.ArraySize = 1;
	tDecs.Width = (UINT)CGameWindow::WndSize.x;
	tDecs.Height = (UINT)CGameWindow::WndSize.y;
	

	// 단순히 깊이 값에 대한 비교만 필요하므로 수치비교 가능한 포멧으로 설정
	tDecs.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	// 멀티샘플링 개수와 품질수준
	// 랜더 타겟과 같은 품질과 갯수로 셋팅해야한다.
	tDecs.SampleDesc.Count = 1;
	tDecs.SampleDesc.Quality = 0;

	tDecs.MipLevels = 1;

	tDecs.Usage = D3D11_USAGE_DEFAULT;
	// 뎁스 스탠실용 텍스쳐임을 표기
	tDecs.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	pDevice->CreateTexture2D(&tDecs, 0, &pDepthStancilTex);

	if (nullptr == pDepthStancilTex)
		return FALSE;
	

	// 뎁스 스탠실 생성
	if (S_OK != pDevice->CreateDepthStencilView(pDepthStancilTex, 0, &pDepthStancilView))
		return FALSE;
	

	// Output Merger에서 사용할 백버퍼 랜더타겟 뷰객체와 댑스스탠실 뷰객체를 설정
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
