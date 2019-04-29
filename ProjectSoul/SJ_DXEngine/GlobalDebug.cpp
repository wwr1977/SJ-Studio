#include "GlobalDebug.h"
#include "ResourceMgr.h"
#include "DXStruct.h"
#include "Mesh.h"
#include "VtxShader.h"
#include "PixShader.h"
#include "DXVertex.h"
#include "ConstantBuffer.h"
#include "FilePathMgr.h"



bool				GlobalDebug::bDebugMode = true;
SPTR<CMesh>			GlobalDebug::DebugMash = nullptr;
SPTR<CVtxShader>	GlobalDebug::DebugVtxShader = nullptr;
SPTR<CPixShader>	GlobalDebug::DebugPixShader = nullptr;

void GlobalDebug::InitGlobalDebug()
{
	CFilePathMgr::AddRootFolder(L"DefaultShader",L"Shader");
	CMesh::CREATE_DESC Desc = { DXVTX::BasicVtx ,sizeof(BASICVTX) , 4,D3D11_USAGE_DYNAMIC
		, DXVTX::BasicIdx ,IDX16::MemSize() ,6, D3D11_USAGE_DYNAMIC ,IDX16::Format() ,D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST };

	DebugMash = CResourceMgr<CMesh>::Create(L"DebugMesh", Desc);

	CConstantBuffer* Buff = CResourceMgr<CConstantBuffer>::Create(L"Debug2DBuffer",
		{ sizeof(CBUFFER::Base2DBuffer),D3D11_USAGE::D3D11_USAGE_DYNAMIC,D3D11_CPU_ACCESS_WRITE });

	Buff->SetBuffDataType<CBUFFER::Base2DBuffer>();

	Buff = CResourceMgr<CConstantBuffer>::Create(L"Debug2DTransBuffer",
		{ sizeof(Mat),D3D11_USAGE::D3D11_USAGE_DYNAMIC,D3D11_CPU_ACCESS_WRITE });

	Buff->SetBuffDataType<Mat>();


	CVtxShader::LOAD_DESC VtxDesc = { 5,0,"VS_Main" };
	DebugVtxShader = CResourceMgr<CVtxShader>::Load(CFilePathMgr::GetPath(L"DefaultShader", L"Debug2D.fx"), VtxDesc);
	DebugVtxShader->PushInputDesc("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, 0);
	DebugVtxShader->PushInputDesc("NORMAL", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, 0);
	DebugVtxShader->PushEndInputDesc("TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0, 0);
	DebugVtxShader->PushConstantBuffer(0, L"Debug2DTransBuffer");

	CPixShader::LOAD_DESC  PixDesc = { 5,0,"PS_Main" };
	DebugPixShader = CResourceMgr<CPixShader>::Load(CFilePathMgr::GetPath(L"DefaultShader", L"Debug2D.fx"), PixDesc);
	DebugPixShader->PushConstantBuffer(0, L"Debug2DBuffer");

}
void GlobalDebug::ReleaseGlobalDebug()
{

}
void GlobalDebug::DrowDebugRect(const Mat& _VP, const Vec3& _Pos, const Vec3& _Size, const Vec3& _Color /*= Vec3::One3*/, const float& _BorderSize /*= 2.f*/)
{

	CBUFFER::Base2DBuffer BaseBuffer;

	
	BaseBuffer = CBUFFER::Base2DBuffer(_Color, BASE2D_RECT, Vec4(1.f / _Size.x, 1.f / _Size.y, _BorderSize));
	Mat Pos, Size;
	Pos.SetPosMat(_Pos);
	Size.SetScalingMat(_Size);

	Mat WVP = Size * Pos * _VP;

	DebugMash->Update();
	DebugVtxShader->Update();
	DebugPixShader->Update();
	DebugVtxShader->BufferUpdate<Mat>(0, &WVP.GetTransposeMat());
	DebugPixShader->BufferUpdate<CBUFFER::Base2DBuffer>(0, &BaseBuffer);

	DebugMash->Render();
}

void GlobalDebug::DrowDebugCircle(const Mat& _VP, const Vec3& _Pos, const Vec3& _Size, const Vec3& _Color/* = Vec3::One3*/, const float& _BorderSize/* = 2.f*/)
{
	CBUFFER::Base2DBuffer BaseBuffer;

	BaseBuffer = CBUFFER::Base2DBuffer(_Color, BASE2D_CIRCLE, Vec4(1.f / _Size.x, 1.f / _Size.y, _BorderSize));
	Mat Pos, Size;
	Pos.SetPosMat(_Pos);
	Size.SetScalingMat(_Size);

	Mat WVP = Size * Pos * _VP;

	DebugMash->Update();
	DebugVtxShader->Update();
	DebugPixShader->Update();
	DebugVtxShader->BufferUpdate<Mat>(0, &WVP.GetTransposeMat());
	DebugPixShader->BufferUpdate<CBUFFER::Base2DBuffer>(0, &BaseBuffer);

	DebugMash->Render();
}
//void GlobalDebug::DrawString(const wchar_t* _FontKey, const wchar_t* _Text, const float& _FontSize, const Vec2& _Pos, const Vec4& Color, const FW1_TEXT_FLAG& _Align /*= FW1_TEXT_FLAG::FW1_TOP*/)
//{
//	SPTR<CDXFont> Font = CResourceMgr<CDXFont>::Find(_FontKey);
//
//	if (nullptr == Font)
//		return;
//
//	Font->DrawString(_Text, _FontSize, _Pos, Color, _Align);
//}