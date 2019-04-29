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


	// Device�� ����� ������ ����
	m_DevicePara.hDeviceWindow = m_pPairWindow->hWnd();

	//BackBuffer�� ũ��� Image Type�� �����ϴ� �κ�
	m_DevicePara.BackBufferWidth = m_pPairWindow->WndSize().x;
	m_DevicePara.BackBufferHeight = m_pPairWindow->WndSize().y;
	m_DevicePara.BackBufferCount = 1;
	m_DevicePara.BackBufferFormat = D3DFMT_A8R8G8B8;

	// ���ٽ� ���۸� ����Ѵٴ� ���ε�.
	// ���ٽ� ���۶� �Ϲ������� 3D���� �ſ�ȿ�� ������ ���鶧 ����ϴµ�.
	// ���ó� �츮�� 2D������ ������̹Ƿ� 3D���� ���� ���³��� �����̴�.
	// ���ٽ��� �����ϸ� ���� ȭ�鿡�� 
	// ���Ľ� ���۸� ���� ����� ����� �ȵȴٴ°� ������.


	m_DevicePara.AutoDepthStencilFormat = D3DFMT_D24S8;

	// Z - Buffer ��뿩��
	m_DevicePara.EnableAutoDepthStencil = false;

	m_DevicePara.Flags = 0;

	//����� ȭ�� �ֻ���
	m_DevicePara.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;

	//��Ƽ ���ø� ��뿩�ο� ���� ��Ƽ���ø��� ����
	m_DevicePara.MultiSampleType = D3DMULTISAMPLE_NONE;
	m_DevicePara.MultiSampleQuality = 0;

	// ���� ü���� �ӷ�
	// ������ ���� FPS�� �ִ밪(ȭ�� ���Ÿ� �������� ���� �ӷ�)
	m_DevicePara.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;


	// ������ ��� , ��üȭ�� ��� ����
	// ��üȭ�� ���� ��Ŀ����ȯ���� ���� ����̽� ��ġ �ս� OR ������ �ε��� Image �ս� ���
	// ��Ŀ���� �ٽ� �������� ��� ����̽� ����� �� �̹��� ��ε� or �޸� Check�� ���� ������ �ʿ�

	// false = ��ü���
	// true = ������ ���
	m_DevicePara.Windowed = true;

	//���� ����Ʈ ����
	m_DevicePara.SwapEffect = D3DSWAPEFFECT_DISCARD;
}
void CGDevice::RenderStart()
{
	assert(m_pDevice);

	if (nullptr == m_pDevice)
		return;

	m_pDevice->Clear(0, nullptr, D3DCLEAR_TARGET, D3DCOLOR_ARGB(255, m_ColorFactor.Red, m_ColorFactor.Green, m_ColorFactor.Blue), 1.0f, 0);

	m_bRender = false;
	// ����ۿ� �׸��⸦ �����ϰڴ�.

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