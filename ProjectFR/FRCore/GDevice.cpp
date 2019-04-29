#include"stdafx.h"
#include "GDevice.h"


LPDIRECT3D9					CGDevice::m_pD3D = nullptr;
D3DCAPS9					CGDevice::m_Caps;
DWORD						CGDevice::m_dwVtProcessing = 0;

CGDevice::CGDevice(CGameWindow* _Window)
	: m_DevicePara()
	, m_pDevice(nullptr)
	, m_pPairWindow(_Window)
	, m_bRender(false)
	, m_ColorFactor(0, 0, 0)
{
}


CGDevice::~CGDevice()
{
	if (nullptr == m_pDevice) 
	{
		return;
	}
	m_pDevice->Release();

}

////////////////////////////////////// static Function		//////////////////////

void CGDevice::SetDeviceInfomation() 
{
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (nullptr == m_pD3D) 
	{
		return;
	}

	HRESULT hr =m_pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &m_Caps);

	if (FAILED(hr)) 
	{
		return;
	}

	if (m_Caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) 
	{
		m_dwVtProcessing = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	}
	else 
	{
		m_dwVtProcessing = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}

	return;
}
LPDIRECT3DDEVICE9 CGDevice::InitDevice()
{
	SettingParameter();

	HRESULT hr = m_pD3D->CreateDevice(
		D3DADAPTER_DEFAULT
		, D3DDEVTYPE_HAL
		, m_pPairWindow->hWnd()
		, m_dwVtProcessing | D3DCREATE_MULTITHREADED
		, &m_DevicePara
		, &m_pDevice
	);

	if (FAILED(hr)) 
	{
		return nullptr;
	}

	return m_pDevice;
}
void CGDevice::SettingParameter() 
{
	memset(&m_DevicePara, 0, sizeof(m_DevicePara));


	// Device를 사용할 윈도우 지정
	m_DevicePara.hDeviceWindow = m_pPairWindow->hWnd();

	//BackBuffer의 크기와 Image Type을 지정하는 부분
	m_DevicePara.BackBufferWidth = m_pPairWindow->WndSize().x;
	m_DevicePara.BackBufferHeight = m_pPairWindow->WndSize().y;
	m_DevicePara.BackBufferCount = 1;
	m_DevicePara.BackBufferFormat = D3DFMT_A8R8G8B8;

	// 스텐실 버퍼를 사용한다는 것인데.
	// 스텐실 버퍼란 일반적으로 3D에서 거울효과 물등을 만들때 사용하는데.
	// 역시나 우리는 2D게임을 만들것이므로 3D에서 정말 진력나게 배울것이다.
	// 스텐실을 사용안하면 최종 화면에서 
	// 스탠실 버퍼를 통한 백버퍼 출력이 안된다는걸 깜빡함.


	m_DevicePara.AutoDepthStencilFormat = D3DFMT_D24S8;

	// Z - Buffer 사용여부
	m_DevicePara.EnableAutoDepthStencil = false;

	m_DevicePara.Flags = 0;

	//모니터 화면 주사율
	m_DevicePara.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;

	//멀티 샘플링 사용여부와 사용시 멀티샘플링의 강도
	m_DevicePara.MultiSampleType = D3DMULTISAMPLE_NONE;
	m_DevicePara.MultiSampleQuality = 0;

	// 스왑 체인의 속력
	// 게임이 같는 FPS의 최대값(화면 갱신만 했을때의 스왑 속력)
	m_DevicePara.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;


	// 윈도우 모드 , 전체화면 모드 설정
	// 전체화면 모드시 포커싱전환으로 인한 디바이스 장치 손실 OR 기존의 로드한 Image 손실 우려
	// 포커싱이 다시 맞춰졌을 경우 디바이스 재생성 및 이미지 재로딩 or 메모리 Check와 같은 연산이 필요

	// false = 전체모드
	// true = 윈도우 모드
	m_DevicePara.Windowed = true;

	//스왑 이팩트 설정
	m_DevicePara.SwapEffect = D3DSWAPEFFECT_DISCARD;
}
void CGDevice::RenderStart()
{
	assert(m_pDevice);

	if (nullptr == m_pDevice)
		return;

	m_pDevice->Clear(0, nullptr, D3DCLEAR_TARGET, D3DCOLOR_ARGB(255, m_ColorFactor.Red, m_ColorFactor.Green, m_ColorFactor.Blue), 1.0f, 0);

	m_bRender = false;
	// 백버퍼에 그리기를 시작하겠다.

	HRESULT hr = m_pDevice->BeginScene();

	/*if (FAILED(hr))
		m_bRender = true;*/

	if (S_OK == hr)
		m_bRender = true;
}
void CGDevice::RenderEnd() 
{
	assert(m_pDevice);

	if (nullptr == m_pDevice)
	{
		return;
	}

	if (m_bRender) 
	{
		HRESULT hr;
		hr = m_pDevice->EndScene();
		hr = m_pDevice->Present(nullptr, nullptr, nullptr, nullptr);
	}


}
void CGDevice::SetColorFactor(COLORFACTOR _ColorFactor)
{
	m_ColorFactor = _ColorFactor;
}
COLORFACTOR CGDevice::GetColorFactor() 
{
	return m_ColorFactor;
}
LPDIRECT3DDEVICE9 CGDevice::GetDevice()
{
	return m_pDevice;
}