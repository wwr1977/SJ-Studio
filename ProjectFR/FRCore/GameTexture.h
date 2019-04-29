#pragma once
#include "ObjBase.h"
class CGameTexture : public CObjBase
{
private:
	LPDIRECT3DTEXTURE9				m_pTexture;
	D3DSURFACE_DESC					m_Desc;
	D3DXIMAGE_INFO					m_Info;
	FOLDER							m_FolderIndex;

public:
	static CGameTexture* CreateTexture(LPDIRECT3DDEVICE9 _pDevice, const tstring& _Path);
	void ReLoadTexture(LPDIRECT3DDEVICE9 _pDevice, const tstring& _Path);

public:
	LPDIRECT3DTEXTURE9 GetTexture() 
	{
		return m_pTexture;
	}
	D3DXIMAGE_INFO	GetImageInfo() 
	{
		return m_Info;
	}
	void CalTexCoord(Vec2 _Pos, Vec2 _Size, VERTEX* _Vtx);
	
	void SetFolderIndex(FOLDER _FolderIndex);
	FOLDER GetFolderIndex();

public:
	CGameTexture(LPDIRECT3DTEXTURE9 _pTex, D3DSURFACE_DESC _Desc , D3DXIMAGE_INFO _Info);
	~CGameTexture();
};

