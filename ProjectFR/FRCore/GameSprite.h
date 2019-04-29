#pragma once
#include "ObjBase.h"
#include"GameMesh.h"

//class CGameMesh;
class CGameTexture;
class CGameSprite : public CObjBase
{

private:
	LPDIRECT3DDEVICE9			m_pDevice;
	SPTR<CGameMesh>				m_Mesh;
	SPTR<CGameTexture>			m_Texture;
	Vec2						m_SpriteSize;
	Vec2						m_SpritePivot;
	FOLDER						m_FolderIndex;

public:
	void SetGameTexture(SPTR<CGameTexture> _pTexture);
	SPTR<CGameMesh>	GetGameMesh();
	void SetSpriteSize(const Vec2& _Size) 
	{
		m_SpriteSize = _Size;
	}
	const Vec2 GetSpriteSize() const 
	{
		return m_SpriteSize;
	}
	void SetSpritePivot(const Vec2& _Pivot) 
	{
		m_SpritePivot = _Pivot;
	}

	const Vec2 GetSpritePivot() const 
	{
		return m_SpritePivot;
	}

	const float GetSpriteAspect() 
	{
		return m_SpriteSize.x / m_SpriteSize.y;
	}
	const float GetLongestSide() 
	{
		if (m_SpriteSize.x >= m_SpriteSize.y) 
		{
			return m_SpriteSize.x;
		}
		else 
		{
			return m_SpriteSize.y;
		}
	}

public:
	FOLDER GetFolderIndex();
	SPTR<CGameSprite> ArticleSprite(const Vec2& _StartPos, const  Vec2& _Size);
	SPTR<CGameMesh> GetMesh();

public:
	static CGameSprite* CreateGameSprite(LPDIRECT3DDEVICE9 _pDevice);
	bool ChangeVertex(VERTEX* _VtxBuf);
	
public:
	void Render();
	

public:
	CGameSprite(LPDIRECT3DDEVICE9 _pDevice,SPTR<CGameMesh> _pMesh);
	~CGameSprite();
};

