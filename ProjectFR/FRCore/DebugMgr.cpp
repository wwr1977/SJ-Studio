#include "stdafx.h"
#include "DebugMgr.h"
#include "GameMesh.h"

VTXLINE CDebugMgr::LineVtx[5];
CDebugMgr::CDebugMgr(CGameWindow* _pGameWnd , LPDIRECT3DDEVICE9 _pDevice)
	:m_PairWindow(_pGameWnd),m_PairDevice(_pDevice), m_bDebugMode(FALSE)
{
	LineVtx[0] = VTXLINE({ -0.5f, 0.5f,  0.0f }, D3DCOLOR_XRGB(255, 0, 0));
	LineVtx[1] = VTXLINE({ 0.5f, 0.5f, 0.0f }, D3DCOLOR_XRGB(255, 0, 0));
	LineVtx[2] = VTXLINE({ 0.5f, -0.5f,  0.0f }, D3DCOLOR_XRGB(255, 0, 0));
	LineVtx[3] = VTXLINE({ -0.5f, -0.5f, 0.0f }, D3DCOLOR_XRGB(255, 0, 0));
	LineVtx[4] = VTXLINE({ -0.5f, 0.5f,  0.0f }, D3DCOLOR_XRGB(255, 0, 0));
}


CDebugMgr::~CDebugMgr()
{
}

SPTR<CGameMesh> CDebugMgr::CreateLineMesh(DWORD _Color)
{
	m_FindLineIter = m_LineMesh.find(_Color);
	
	if (m_FindLineIter == m_LineMesh.end())
	{
		if (nullptr == m_PairDevice)
		{
			return nullptr;
		}

		LPDIRECT3DVERTEXBUFFER9 MeshVBuff = CGameMesh::CreateVtxBuffer<VTXLINE>(m_PairDevice, 5);

		if (nullptr == MeshVBuff)
		{
			return nullptr;
		}

		CGameMesh* NewMesh = new CGameMesh(m_PairDevice, MeshVBuff);

		if (nullptr == NewMesh)
		{
			assert(nullptr);
			return nullptr;
		}

		NewMesh->SetMesh<VTXLINE>(5);

		LineVtx[0].Color = _Color;
		LineVtx[1].Color = _Color;
		LineVtx[2].Color = _Color;
		LineVtx[3].Color = _Color;
		LineVtx[4].Color = _Color;

		NewMesh->FILLVTX<VTXLINE>(LineVtx);

		m_LineMesh.insert(map<DWORD, SPTR<CGameMesh>>::value_type(_Color, NewMesh));

		return NewMesh;
	}
	
	return m_FindLineIter->second;
	
}


void CDebugMgr::RectRender(DWORD _Color, const Vec3& _Pos, const Vec3& _Size)
{
	
	SPTR<CGameMesh> LineMesh = CreateLineMesh(_Color);

	if (nullptr != LineMesh) 
	{
		D3DXMatrixTranslation(&matDebugPos, _Pos.x, _Pos.y, _Pos.z);
		D3DXMatrixScaling(&matDebugSize, _Size.x, _Size.y, _Size.z);

		m_PairDevice->SetTransform(D3DTS_WORLD, &(matDebugSize * matDebugPos));
		
		m_PairDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
		LineMesh->Render(m_PairDevice, D3DPRIMITIVETYPE::D3DPT_LINESTRIP);
	}
}
void CDebugMgr::RectRender(DWORD _Color, const Mat& _Mat)
{
	
	SPTR<CGameMesh> LineMesh = CreateLineMesh(_Color);

	if (nullptr != LineMesh) 
	{
		m_PairDevice->SetTransform(D3DTS_WORLD, &_Mat);

		LineMesh->Render(m_PairDevice, D3DPRIMITIVETYPE::D3DPT_LINESTRIP);
	}
}
void CDebugMgr::ChangeDebugMode()
{
	m_bDebugMode = !m_bDebugMode;
}