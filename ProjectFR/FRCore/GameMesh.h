#pragma once
#include "ObjBase.h"
class CGameMesh : public CObjBase
{
private:
	LPDIRECT3DDEVICE9					m_pDevice;
	LPDIRECT3DVERTEXBUFFER9				m_pVBuf;
	LPDIRECT3DINDEXBUFFER9				m_pIBuf;
	const type_info*					m_pVtxType;
	int									m_iVtxCount;		// 버택스의 갯수 : 4개
	int									m_iVtxSize;			// 버택스의 크기 : sizeif(VERTEX)
	int									m_iVtxCalSize;		// 버택스 버퍼의 크기 (Count * Size)
	DWORD								m_iFVF;

	const type_info*					m_pIdxType;
	int									m_iTriSize;
	int									m_iTriCount;
	int									m_iIdxCalSize;
	D3DFORMAT							m_iFmt;				//색상 포맷

public:

	template<typename T>
	static LPDIRECT3DVERTEXBUFFER9 CreateVtxBuffer(LPDIRECT3DDEVICE9 _pDevice, int _VtxCount) 
	{
		int BufSize = _VtxCount * sizeof(T);
		LPDIRECT3DVERTEXBUFFER9	TempVtxBuf = nullptr;
		
		CCriSectionObject<CGameMesh>();

		HRESULT Hr = _pDevice->CreateVertexBuffer(
			BufSize
			, 0
			, T::FVF()
			, D3DPOOL_DEFAULT
			, &TempVtxBuf
			, nullptr);


		if ( S_OK !=  Hr )
		{
			assert(nullptr);
			return nullptr;
		}

		return TempVtxBuf;
	}
	
	template<typename T>
	static LPDIRECT3DINDEXBUFFER9 CreateIdxBuffer(LPDIRECT3DDEVICE9 _pDevice, int TriCount)
	{
		int BufSize = TriCount * sizeof(T);

		LPDIRECT3DINDEXBUFFER9	TempIdxBuf = nullptr;

		CCriSectionObject<CGameMesh>();
		if (FAILED(_pDevice->CreateIndexBuffer(
			BufSize
			, 0
			, T::FMT()
			, D3DPOOL_DEFAULT
			, &TempIdxBuf
			, nullptr)))
		{
			assert(nullptr);
			return nullptr;
		}

		return TempIdxBuf;
	}

	template<typename Vtx,typename Idx>
	void SetMesh(int _VtxCount,int _TriCount) 
	{
		m_pVtxType = &typeid(Vtx);
		m_iFVF = Vtx::FVF();
		m_iVtxCount = _VtxCount;
		m_iVtxSize = sizeof(Vtx);
		m_iVtxCalSize = m_iVtxSize * m_iVtxCount;

		m_pIdxType = &typeid(Idx);
		m_iTriCount = _TriCount;
		m_iTriSize = sizeof(Idx);
		m_iFmt = Idx::FMT();
		m_iIdxCalSize = m_iTriSize * m_iTriCount;

	}
	
	template<typename Vtx>
	void SetMesh(int _VtxCount)
	{
		m_pVtxType = &typeid(Vtx);
		m_iFVF = Vtx::FVF();
		m_iVtxCount = _VtxCount;
		m_iVtxSize = sizeof(Vtx);
		m_iVtxCalSize = m_iVtxSize * m_iVtxCount;
	}
	template<typename T>
	bool FILLVTX(T* _Vtx) 
	{
		if (m_pVtxType != &typeid(T)) 
		{
			return FALSE;
		}

		CCriSectionObject<CGameMesh>();
		void* pVtx = nullptr;
		if (FAILED(m_pVBuf->Lock(0, 0, &pVtx, 0))) 
		{
			assert(nullptr);
			return FALSE;
		}

		memcpy_s(pVtx, m_iVtxCalSize, _Vtx, m_iVtxCalSize);
		m_pVBuf->Unlock();
		return TRUE;
	}

	template<typename T>
	bool FILLIDX(T* _Idx)
	{
		if (m_pIdxType != &typeid(T))
		{
			return FALSE;
		}

		void* pIdx = nullptr;

		if (FAILED(m_pIBuf->Lock(0, 0, &pIdx, 0)))
		{
			assert(nullptr);
			return FALSE;
		}

		memcpy_s(pIdx, m_iIdxCalSize, _Idx, m_iIdxCalSize);
		m_pIBuf->Unlock();
		return TRUE;
	}


public:
	void Render();
	void Render(LPDIRECT3DDEVICE9 _Device, D3DPRIMITIVETYPE _eType);
public:
	CGameMesh();
	CGameMesh(LPDIRECT3DDEVICE9 _pDevice, LPDIRECT3DVERTEXBUFFER9 _pVtxBuf);
	CGameMesh(LPDIRECT3DDEVICE9 _pDevice , LPDIRECT3DVERTEXBUFFER9 _pVtxBuf,LPDIRECT3DINDEXBUFFER9 _pIndBuf);
	~CGameMesh();
};

