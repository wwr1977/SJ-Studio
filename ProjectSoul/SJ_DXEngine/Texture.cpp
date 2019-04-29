#include "Texture.h"
#include<DirectXTex.h>
#include"Device.h"

CTexture::CTexture()
	:m_MetaData{}, m_Image{}, m_pTexture(nullptr), m_pTexSRView(nullptr)
{
}


CTexture::~CTexture()
{
	SAFE_RELEASE(m_pTexture);
	SAFE_RELEASE(m_pTexSRView);

}
const BOOL CTexture::LoadDDSFile()
{
	if (S_OK != DirectX::LoadFromDDSFile(GetPath().c_str(), DirectX::DDS_FLAGS_NONE, &m_MetaData, m_Image))
	{
		TASSERT(TRUE);
		return FALSE;
	}
	return TRUE;
}
const BOOL CTexture::LoadHDRFile()
{
	if (S_OK != DirectX::LoadFromHDRFile(GetPath().c_str(), &m_MetaData, m_Image))
	{
		TASSERT(TRUE);
		return FALSE;
	}
	return TRUE;
}
const BOOL CTexture::LoadTGAFile()
{
	if (S_OK != DirectX::LoadFromTGAFile(GetPath().c_str(), &m_MetaData, m_Image))
	{
		TASSERT(TRUE);
		return FALSE;
	}
	return TRUE;
}
const BOOL CTexture::LoadWICFile()
{
	//DirectX::LoadFromWICFile(GetPath().c_str(), 0, &m_MetaData, m_Image)
	if (S_OK != DirectX::LoadFromWICFile(GetPath().c_str(), DirectX::WIC_FLAGS_NONE, &m_MetaData, m_Image))
	{
		TASSERT(TRUE);
		return FALSE;
	}

	return TRUE;
}

const bool CTexture::Load(const LOAD_DESC& _Desc)
{
	std::wstring Ext = GetExtension();

	if ((Ext == L".dds" || Ext == L".DDs") && FALSE == LoadDDSFile())
		return false;
	else if ((Ext == L".tga" || Ext == L".TGA") && FALSE == LoadTGAFile())
		return false;
	else if (Ext == L".hdr" && FALSE == LoadHDRFile())
		return false;
	else if(FALSE ==  LoadWICFile())
		return false;
	


	HRESULT Check = DirectX::CreateTexture(DXDEVICE,m_Image.GetImage(0,0,0), m_Image.GetImageCount(), m_Image.GetMetadata(), &m_pTexture);
	
	if (Check != S_OK)
	{
		TASSERT(true);
		return false;
	}

	Check = DirectX::CreateShaderResourceView(DXDEVICE, m_Image.GetImage(0, 0, 0), m_Image.GetImageCount(), m_Image.GetMetadata(), &m_pTexSRView);

	if (S_OK != Check) 
	{
		TASSERT(true);
		return false;
	}

	m_MetaData =  m_Image.GetMetadata();

	return true;
}

const Vec2 CTexture::GetTextureSize()
{
	return Vec2((float)m_MetaData.width, (float)m_MetaData.height);
}

//void CTexture::Update(const UINT& _SlotNum)
//{
//	DXCONTEXT->VSSetShaderResources(_SlotNum, 1, &m_pTexSRView);
//	DXCONTEXT->PSSetShaderResources(_SlotNum, 1, &m_pTexSRView);
//}

void CTexture::Update(const UINT& _SlotNum, const int& _ShaderFlag /*= VS_SET | PS_SET*/)
{
	if (VS_SET == (_ShaderFlag & VS_SET))
		DXCONTEXT->VSSetShaderResources(_SlotNum, 1, &m_pTexSRView);

	if (HS_SET == (_ShaderFlag & HS_SET))
		DXCONTEXT->HSSetShaderResources(_SlotNum, 1, &m_pTexSRView);

	if (DS_SET == (_ShaderFlag & DS_SET))
		DXCONTEXT->DSSetShaderResources(_SlotNum, 1, &m_pTexSRView);

	if (GS_SET == (_ShaderFlag & GS_SET))
		DXCONTEXT->GSSetShaderResources(_SlotNum, 1, &m_pTexSRView);

	if (PS_SET == (_ShaderFlag & PS_SET))
		DXCONTEXT->PSSetShaderResources(_SlotNum, 1, &m_pTexSRView);
}