#pragma once
#include"Resource.h"
#include"DXHeader.h"

#define VS_SET 0x00000001
#define HS_SET 0x00000002
#define DS_SET 0x00000004
#define GS_SET 0x00000008
#define PS_SET 0x00000010

class CTexture : public CResource
{
private:
	DirectX::ScratchImage						m_Image;
	DirectX::TexMetadata						m_MetaData;
	ID3D11Resource*								m_pTexture;
	ID3D11ShaderResourceView*					m_pTexSRView;

public:
	CTexture();
	~CTexture();

private:
	const BOOL LoadDDSFile();
	const BOOL LoadHDRFile();
	const BOOL LoadTGAFile();
	const BOOL LoadWICFile();


public:
	ID3D11Resource* GetResource() const { return m_pTexture; }
	ID3D11ShaderResourceView* GetShaderResourceView() const { return m_pTexSRView; }
	const Vec2 GetTextureSize();

public:
	void Update(const UINT& _SlotNum, const int& _ShaderFlag = VS_SET | PS_SET);
	//void Update(const UINT& _SlotNum);
	
public:
	NONE_CREATE_RESOURCE(CTexture)
	LOAD_DESC_BODY(int a;)


public:
	const bool Load(const LOAD_DESC& _Desc);
	
};

