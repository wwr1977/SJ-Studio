#include "SJ_DXCore.h"
#include"GameWindow.h"
#include"FilePathMgr.h"
#include"TimerMgr.h"
#include"ResourceMgr.h"
#include"Mesh.h"
#include"VtxShader.h"
#include"PixShader.h"
#include"ConstantBuffer.h"
#include"SamplerState.h"
#include"BlendState.h"
#include"DXVertex.h"
#include"KeyMgr.h"
#include"DXMacro.h"
#include"Sound.h"
#include"DXStruct.h"
#include"Collider.h"
#include"GlobalDebug.h"
#include"SoundPlayer.h"



bool CSJ_DXCore::m_bCoreProgress = true;
CSJ_DXCore::CCoreBuilder* CSJ_DXCore::m_pBuilder = nullptr;



CSJ_DXCore::CSJ_DXCore()
{
}


CSJ_DXCore::~CSJ_DXCore()
{
}
BOOL CSJ_DXCore::CoreInit(CCoreBuilder* _Builder)
{
	if (!_Builder)
		return FALSE;

	m_pBuilder = _Builder;


	CSound::CreateSoundSystem();
	CFilePathMgr::Init();
	CTimerMgr::Init();
	CKeyMgr::Init();
	GlobalDebug::InitGlobalDebug();

	return _Builder->Build();
}

void CSJ_DXCore::CoreLoop()
{
	MSG msg;

	while (m_bCoreProgress)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			// Window Death Time
			CSound::SoundSystemUpdate();
			CTimerMgr::Update();
			CKeyMgr::Update(CTimerMgr::DeltaTime());
			CSoundPlayer::GlobalChannelUpdate(CTimerMgr::DeltaTime());
			CGameWindow::Progress(CTimerMgr::DeltaTime());
			CKeyMgr::ResetWheelValue();
		}
	}
}
void CSJ_DXCore::CoreRelease()
{
	if (nullptr != m_pBuilder)
		m_pBuilder->Release();

	CFilePathMgr::Release();
	CResourceMgr<CSound>::Release();
	CSound::ReleaseSoundSystem();
	GlobalDebug::ReleaseGlobalDebug();
	CKeyMgr::Release();
}
void CSJ_DXCore::CoreOperation(CCoreBuilder* _Builder)
{
	if (!CoreInit(_Builder))
		return;

	CoreLoop();
	CoreRelease();
}
void CSJ_DXCore::QuitCore()
{
	m_bCoreProgress = false;
}

LRESULT CALLBACK CSJ_DXCore::WndProc(HWND _hWnd, UINT _Msg, WPARAM _wParam, LPARAM _lParam)
{
	switch (_Msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		CSJ_DXCore::QuitCore();
		break;
	case WM_MOUSEWHEEL:
		CKeyMgr::MouseWheelEvent(GET_WHEEL_DELTA_WPARAM(_wParam));
	}

	if (m_pBuilder)
		m_pBuilder->BuilderProc(_hWnd, _Msg, _wParam, _lParam);

	return DefWindowProc(_hWnd, _Msg, _wParam, _lParam);
}
//

BOOL CSJ_DXCore::CreateMainWindow(const HINSTANCE _hInst, const wchar_t* _Title)
{
	if (true == CGameWindow::bCreateWindow)
		return FALSE;

	CGameWindow::RegistWindowClass(_hInst);
	return CGameWindow::CreateGameWindow(_Title);
}
BOOL CSJ_DXCore::CreateMainWindow(const HINSTANCE _hInst, const wchar_t* _Title, const POINT& _Size, const POINT& _Pos, const bool& _Show/* = true*/)
{
	if (true == CGameWindow::bCreateWindow)
		return FALSE;

	CGameWindow::RegistWindowClass(_hInst);
	return CGameWindow::CreateGameWindow(_Title, _Size, _Pos, _Show);
}
BOOL CSJ_DXCore::CreateMainWindow(const HINSTANCE _hInst, const wchar_t* _Title, const Vec2& _Size, const Vec2& _Pos, const bool& _Show /*= true*/)
{
	if (true == CGameWindow::bCreateWindow)
		return FALSE;

	CGameWindow::RegistWindowClass(_hInst);
	return CGameWindow::CreateGameWindow(_Title, { (LONG)_Size.ix,(LONG)_Size.iy }, { (LONG)_Pos.ix,(LONG)_Pos.iy }, _Show);
}
BOOL CSJ_DXCore::CreateMainWindow(const HINSTANCE _hInst,const HWND& _hWnd ,const wchar_t* _Title)
{
	if (true == CGameWindow::bCreateWindow)
		return FALSE;

	CGameWindow::RegistWindowClass(_hInst);
	return CGameWindow::CreateGameWindow(_hWnd,_Title);
}

bool CSJ_DXCore::SetWindowSize(const POINT& _Size)
{
	return CGameWindow::SetWindowSize(_Size);
}
bool CSJ_DXCore::SetWindowSize(const POINT& _Size, const POINT& _Pos)
{
	return CGameWindow::SetWindowSize(_Size, _Pos);
}
BOOL CSJ_DXCore::ChangeWindowTitle(std::wstring _Title)
{
	return CGameWindow::ChangeWndTitle(_Title);
}
BOOL CSJ_DXCore::ChangeMouseCursor(std::wstring _Path)
{
	return CGameWindow::ChangeBaseCursor(_Path);
}
BOOL CSJ_DXCore::ChangeIcon(std::wstring _Path)
{
	return CGameWindow::ChangeIcon(_Path);
}
BOOL CSJ_DXCore::ChangeSmallIcon(std::wstring _Path)
{
	return CGameWindow::ChangeSmallIcon(_Path);
}
const POINT CSJ_DXCore::WolrdPosToWndPos(const POINT& _WorldPos)
{
	return CGameWindow::WorldPointToWnd(_WorldPos);
}
const POINT CSJ_DXCore::GetWindowSize()
{
	return CGameWindow::WndSize;
}
const LONG CSJ_DXCore::GetWindowWidth()
{
	return (LONG)CGameWindow::WndSize.x;
}
const LONG CSJ_DXCore::GetWindowHeight() 
{
	return (LONG)CGameWindow::WndSize.y;
}
const float CSJ_DXCore::GetWindowHypotenuse()
{
	float W = (float)CGameWindow::WndSize.x;
	float H = (float)CGameWindow::WndSize.y;
	return sqrtf( W * W + H * H);
}
const float CSJ_DXCore::GetWindowAspect()
{
	return (float)CGameWindow::WndSize.x / CGameWindow::WndSize.y  ;
}

const Vec3 CSJ_DXCore::GetScreenMousePos()
{
	return Vec3((float)CGameWindow::GetMousePoint().x, (float)CGameWindow::GetMousePoint().y, 0.f);
}
const HINSTANCE CSJ_DXCore::GetWindowInstance()
{
	return CGameWindow::hInst;
}

const HWND CSJ_DXCore::GetWindowWnd()
{
	return CGameWindow::hWnd;
}
void CSJ_DXCore::ShowGameWindow(const bool& _Show)
{
	if (_Show)
		ShowWindow(CGameWindow::hWnd, SW_SHOW);
	else
		ShowWindow(CGameWindow::hWnd, SW_HIDE);

	UpdateWindow(CGameWindow::hWnd);
}
////////////	CoreBuilder		//////////////////////////////
CSJ_DXCore::CCoreBuilder::CCoreBuilder()
{

}
CSJ_DXCore::CCoreBuilder::~CCoreBuilder()
{

}





//void CSJ_DXCore::CreateDefaultResource()
//{
//	CreateMesh();
//	CreateShaderResource();
//	CreateState();
//	CCollider::ColliderInit();
//}
//void CSJ_DXCore::CreateMesh() 
//{
//	CMesh::CREATE_DESC Desc{ DXVTX::ColorVtx ,sizeof(COLORVTX) ,4,D3D11_USAGE_DYNAMIC
//		, DXVTX::BasicIdx ,IDX16::MemSize() ,6,D3D11_USAGE_DYNAMIC ,IDX16::Format() ,D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST };
//
//	CResourceMgr<CMesh>::Create(L"ColorMesh", Desc);
//
//	Desc = { DXVTX::BasicVtx ,sizeof(BASICVTX) , 4,D3D11_USAGE_DYNAMIC
//		, DXVTX::BasicIdx ,IDX16::MemSize() ,6, D3D11_USAGE_DYNAMIC ,IDX16::Format() ,D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST };
//
//	CResourceMgr<CMesh>::Create(L"2DMesh", Desc);
//}
//void CSJ_DXCore::CreateShaderResource()
//{
//	// 상수 버퍼 생성
//	SPTR<CConstantBuffer> Buff = CResourceMgr<CConstantBuffer>::Create(L"TransFormBuffer",
//		{ sizeof(Mat),D3D11_USAGE::D3D11_USAGE_DYNAMIC,D3D11_CPU_ACCESS_WRITE });
//	Buff->SetBuffDataType<Mat>();
//
//	Buff = CResourceMgr<CConstantBuffer>::Create(L"SpriteUV",
//		{ sizeof(Vec4),D3D11_USAGE::D3D11_USAGE_DYNAMIC,D3D11_CPU_ACCESS_WRITE });
//	Buff->SetBuffDataType<Vec4>();
//
//	Buff = CResourceMgr<CConstantBuffer>::Create(L"Base2DBuffer",
//		{ sizeof(CBUFFER::Base2DBuffer),D3D11_USAGE::D3D11_USAGE_DYNAMIC,D3D11_CPU_ACCESS_WRITE });
//	Buff->SetBuffDataType<CBUFFER::Base2DBuffer>();
//
//
//
//	// 버텍스 쉐이더 생성
//	CVtxShader::LOAD_DESC VtxDesc{ 5,0,"VS_ColorRect" };
//
//	SPTR<CVtxShader> VS = CResourceMgr<CVtxShader>::Load(CFilePathMgr::GetPath(L"Shader", L"ColorRect.fx"), VtxDesc);
//	VS->PushInputDesc("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
//	VS->PushInputDesc("COLOR", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
//	VS->PushEndInputDesc("TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0, 0);
//	VS->PushConstantBuffer(0, L"TransFormBuffer");
//	
//	VtxDesc = { 5,0,"VS_BasicRect" };
//	SPTR<CVtxShader> TBVS = CResourceMgr<CVtxShader>::Load(CFilePathMgr::GetPath(L"Shader", L"Basic.fx"), VtxDesc);
//	TBVS->PushInputDesc("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, 0);
//	TBVS->PushInputDesc("NORMAL", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, 0);
//	TBVS->PushEndInputDesc("TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0, 0);
//	TBVS->PushConstantBuffer(0, L"TransFormBuffer");
//	TBVS->PushConstantBuffer(1, L"SpriteUV");
//
//	VtxDesc = { 5,0,"VS_Main" };
//	SPTR<CVtxShader> BVS = CResourceMgr<CVtxShader>::Load(CFilePathMgr::GetPath(L"Shader", L"Base2D.fx"), VtxDesc);
//	BVS->PushInputDesc("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, 0);
//	BVS->PushInputDesc("NORMAL", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, 0);
//	BVS->PushEndInputDesc("TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0, 0);
//	BVS->PushConstantBuffer(0, L"TransFormBuffer");
//
//	// 픽셀 쉐이더 생성
//	CPixShader::LOAD_DESC PixDesc{ 5,0,"PS_ColorRect" };
//	CResourceMgr<CPixShader>::Load(CFilePathMgr::GetPath(L"Shader", L"ColorRect.fx"), PixDesc);
//
//	PixDesc = { 5,0,"PS_BasicRect" };
//	SPTR<CPixShader> TBPS = CResourceMgr<CPixShader>::Load(CFilePathMgr::GetPath(L"Shader", L"Basic.fx"), PixDesc);
//	
//	PixDesc = { 5,0,"PS_Main" };
//	SPTR<CPixShader> BPS = CResourceMgr<CPixShader>::Load(CFilePathMgr::GetPath(L"Shader", L"Base2D.fx"), PixDesc);
//	BPS->PushConstantBuffer(0, L"Base2DBuffer");
//}
//
//void CSJ_DXCore::CreateState()
//{
//	CSamplerState::CREATE_DESC Desc;
//	
//	Desc.SampleDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
//	Desc.SampleDesc.MaxAnisotropy = 1;
//	Desc.SampleDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
//	Desc.SampleDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
//	Desc.SampleDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
//
//	Desc.SampleDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
//	for (size_t i = 0; i < 4; i++)
//		Desc.SampleDesc.BorderColor[i] = 0.f;
//
//	Desc.SampleDesc.MipLODBias = 0;
//	Desc.SampleDesc.MinLOD = -3.402823466e+38f;
//	Desc.SampleDesc.MaxLOD = 3.402823466e+38f;
//
//	CResourceMgr<CSamplerState>::Create(POINTSAMPLER, Desc);
//
//	Desc.SampleDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
//	CResourceMgr<CSamplerState>::Create(LINEARSAMPLER, Desc);
//
//
//	D3D11_BLEND_DESC	BlendDesc;
//	BlendDesc.AlphaToCoverageEnable = false;
//	BlendDesc.IndependentBlendEnable = false;
//	BlendDesc.RenderTarget[0].BlendEnable = true;
//	// RGB 색상 혼합에 관련된 블렌드 펙터를 설정
//	BlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
//	BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;
//	BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
//	// Alpha 값의 혼합에 관련된 블렌드 펙터를 설정
//	BlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;
//	BlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_ZERO;
//	BlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
//	BlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
//
//	CBlendState::CREATE_DESC BDesc{BlendDesc};
//	CResourceMgr<CBlendState>::Create(L"BasicBlend", BDesc);
//}