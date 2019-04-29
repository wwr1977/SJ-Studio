#include "stdafx.h"
#include "GameSprite.h"
#include "GameTexture.h"

CGameSprite::CGameSprite(LPDIRECT3DDEVICE9 _pDevice,SPTR<CGameMesh> _pMesh)
	:m_pDevice(_pDevice), m_Mesh(_pMesh), m_Texture(nullptr), m_SpritePivot({0.0f,0.0f})
	,m_FolderIndex(UNSPECIFIED)
{
}


CGameSprite::~CGameSprite()
{
}

CGameSprite* CGameSprite::CreateGameSprite(LPDIRECT3DDEVICE9 _pDevice)
{

	if (nullptr == _pDevice) 
	{
		return nullptr;
	}

	
	LPDIRECT3DVERTEXBUFFER9 MeshVBuff = CGameMesh::CreateVtxBuffer<VERTEX>(_pDevice, 4);

	if (nullptr == MeshVBuff) 
	{
		return nullptr;
	}

	LPDIRECT3DINDEXBUFFER9 MeshIBuff = CGameMesh::CreateIdxBuffer<VTXIDX16>(_pDevice, 2);

	if (nullptr == MeshIBuff)
	{
		MeshVBuff->Release();
		return nullptr;
	}

	CGameMesh* NewMesh = new CGameMesh(_pDevice, MeshVBuff, MeshIBuff);
	
	if (nullptr == NewMesh) 
	{
		assert(nullptr);
		return nullptr;
	}

	NewMesh->SetMesh<VERTEX, VTXIDX16>(4, 2);
	
	return new CGameSprite(_pDevice, NewMesh);
}
bool CGameSprite::ChangeVertex(VERTEX* _VtxBuf)
{
	if (nullptr == _VtxBuf || nullptr == m_Mesh)
		return false;

	/*size_t a = sizeof(*_VtxBuf);
	size_t b = sizeof(VERTEX) * 4;

	if (sizeof(*_VtxBuf) != sizeof(VERTEX) * 4)
		return false;*/

	return m_Mesh->FILLVTX<VERTEX>(_VtxBuf);
}
void CGameSprite::Render() 
{
	HRESULT hr;
	
	if (nullptr != m_Texture) 
	{
		 hr = m_pDevice->SetTexture(0, m_Texture->GetTexture());
	}
	

	if (nullptr != m_Mesh) 
	{
		m_Mesh->Render();
	}
}
SPTR<CGameMesh>	CGameSprite::GetGameMesh()
{
	return m_Mesh;
}
void CGameSprite::SetGameTexture(SPTR<CGameTexture> _pTexture)
{
	m_Texture = _pTexture;
	
	m_FolderIndex = _pTexture->GetFolderIndex();
	
}
FOLDER CGameSprite::GetFolderIndex()
{
	return m_FolderIndex;
}
SPTR<CGameSprite> CGameSprite::ArticleSprite(const Vec2& _StartPos, const  Vec2& _Size)
{
	SPTR<CGameSprite> Temp = CGameSprite::CreateGameSprite(m_pDevice);

	if (nullptr == Temp->GetGameMesh())
	{
		return nullptr;
	}

	Temp->SetGameTexture(m_Texture);
	Temp->SetSpriteSize(_Size);

	CCriSectionObject<CResourceMgr>();
	m_Texture->CalTexCoord(_StartPos, _Size, CResourceMgr::GetCalSpriteVtx());
	Temp->GetGameMesh()->FILLVTX<VERTEX>(CResourceMgr::GetCalSpriteVtx());
	Temp->GetGameMesh()->FILLIDX<VTXIDX16>(CResourceMgr::GetSpriteIdx());


	return Temp;
}
SPTR<CGameMesh> CGameSprite::GetMesh()
{
	return m_Mesh;
}