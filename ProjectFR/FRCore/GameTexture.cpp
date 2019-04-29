#include "stdafx.h"
#include "GameTexture.h"


CGameTexture::CGameTexture(LPDIRECT3DTEXTURE9 _pTex, D3DSURFACE_DESC _Desc, D3DXIMAGE_INFO _Info)
	:m_pTexture(_pTex),m_Desc(_Desc),m_Info(_Info),m_FolderIndex(UNSPECIFIED)
{

}


CGameTexture::~CGameTexture()
{
	if (nullptr == m_pTexture) 
	{
		return;
	}
	m_pTexture->Release();
}
CGameTexture* CGameTexture::CreateTexture(LPDIRECT3DDEVICE9 _pDevice,const tstring& _Path)
{
	if (nullptr == _pDevice) 
	{
		return nullptr;
	}

	LPDIRECT3DTEXTURE9 TempTex = nullptr;
	
	HRESULT Hr = S_FALSE;	

	CCriSectionObject<CGameTexture>();
	Hr = D3DXCreateTextureFromFile(_pDevice, _Path.c_str(), &TempTex);
	
	if (S_OK != Hr) 
	{	
		assert(false);
		int a = 0;
	}
	
	if (nullptr == TempTex) 
	{
		return nullptr;
	}
	
	D3DXIMAGE_INFO TempInfo;
	D3DXGetImageInfoFromFile(_Path.c_str(), &TempInfo);
	
	D3DSURFACE_DESC TempDesc;
	TempTex->GetLevelDesc(0, &TempDesc);
	TempTex->SetPriority(D3D9_RESOURCE_PRIORITY_MAXIMUM);

	return new CGameTexture(TempTex, TempDesc, TempInfo);
}
void CGameTexture::ReLoadTexture(LPDIRECT3DDEVICE9 _pDevice, const tstring& _Path)
{
	if (nullptr == _pDevice)
		return;
	
	LPDIRECT3DTEXTURE9 TempTex = nullptr;

	HRESULT Hr = S_FALSE;

	CCriSectionObject<CGameTexture>();
	Hr = D3DXCreateTextureFromFile(_pDevice, _Path.c_str(), &TempTex);

	if (S_OK != Hr)
		assert(false);
	
	if (nullptr == TempTex)
		return;
	
	D3DXIMAGE_INFO TempInfo;
	D3DXGetImageInfoFromFile(_Path.c_str(), &TempInfo);

	D3DSURFACE_DESC TempDesc;
	TempTex->GetLevelDesc(0, &TempDesc);
	TempTex->SetPriority(D3D9_RESOURCE_PRIORITY_MAXIMUM);

	m_pTexture = TempTex;
	m_Desc = TempDesc;
	m_Info = TempInfo;
	return;
}
void CGameTexture::CalTexCoord(Vec2 _Pos, Vec2 _Size, VERTEX* _Vtx)
{
	Vec2 ArrRectPoint[4];

	ArrRectPoint[0] = _Pos;
	ArrRectPoint[1] = Vec2{ _Pos.x + _Size.x , _Pos.y};
	ArrRectPoint[2] = Vec2{ _Pos.x + _Size.x , _Pos.y + _Size.y };
	ArrRectPoint[3] = Vec2{ _Pos.x , _Pos.y + _Size.y};

	_Vtx[0].vTexCoord = Vec2{ (float)((double)ArrRectPoint[0].x / (double)m_Info.Width), (float)((double)ArrRectPoint[0].y / (double)m_Info.Height) };
	_Vtx[1].vTexCoord = Vec2{ (float)((double)ArrRectPoint[1].x / (double)m_Info.Width), (float)((double)ArrRectPoint[1].y / (double)m_Info.Height )};
	_Vtx[2].vTexCoord = Vec2{ (float)((double)ArrRectPoint[2].x / (double)m_Info.Width), (float)((double)ArrRectPoint[2].y / (double)m_Info.Height )};
	_Vtx[3].vTexCoord = Vec2{ (float)((double)ArrRectPoint[3].x / (double)m_Info.Width), (float)((double)ArrRectPoint[3].y / (double)m_Info.Height )};
}
void CGameTexture::SetFolderIndex(FOLDER _FolderIndex)
{
	m_FolderIndex = _FolderIndex;
}
FOLDER CGameTexture::GetFolderIndex()
{
	return m_FolderIndex;
}