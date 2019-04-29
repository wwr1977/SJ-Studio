#include "stdafx.h"
#include "GameMesh.h"


CGameMesh::CGameMesh()
	:m_pDevice(nullptr),m_pVBuf(nullptr),m_pIBuf(nullptr)
{
}

CGameMesh::CGameMesh(LPDIRECT3DDEVICE9 _pDevice, LPDIRECT3DVERTEXBUFFER9 _pVtxBuf)
	: m_pDevice(_pDevice), m_pVBuf(_pVtxBuf),m_pIBuf(nullptr)
{

}
CGameMesh::CGameMesh(LPDIRECT3DDEVICE9 _pDevice, LPDIRECT3DVERTEXBUFFER9 _pVtxBuf, LPDIRECT3DINDEXBUFFER9 _pIndBuf)
	:m_pDevice(_pDevice),m_pVBuf(_pVtxBuf),m_pIBuf(_pIndBuf)
{

}

CGameMesh::~CGameMesh()
{
	if (nullptr != m_pVBuf) 
	{
		m_pVBuf->Release();	
	}
	if (nullptr != m_pIBuf)
	{
		m_pIBuf->Release();
	}
}
void CGameMesh::Render()
{
	HRESULT hr;
	hr = m_pDevice->SetStreamSource(0, m_pVBuf, 0, m_iVtxSize);
	hr = m_pDevice->SetFVF(m_iFVF);
	hr = m_pDevice->SetIndices(m_pIBuf);
	hr = m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST
		, 0, 0
		, m_iVtxCount
		, 0
		, m_iTriCount);
}
void CGameMesh::Render(LPDIRECT3DDEVICE9 _Device, D3DPRIMITIVETYPE _eType)
{
	_Device->SetStreamSource(0, m_pVBuf, 0, m_iVtxSize);
	_Device->SetFVF(m_iFVF);
	_Device->DrawPrimitive(_eType, 0, m_iVtxCount - 1);
}