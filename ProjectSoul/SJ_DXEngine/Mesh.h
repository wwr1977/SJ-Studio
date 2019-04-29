#pragma once
#include"Resource.h"
#include"Engine.h"
#include"DXHeader.h"



class CMesh : public CResource
{ 
	template<typename Res,typename Key>
	friend class CResourceMgr;

private:
	ID3D11Buffer *							m_VB;
	UINT									m_VtxSize;	// 버텍스의 사이즈 sizeof(VERTEX)
	UINT									m_VtxCount; // 버텍스의 개수 (2D 에선 4개)

	ID3D11Buffer*							m_IB;
	UINT									m_IdxSize;	// 인덱스 버퍼의 인덱스한개의 크기(Idx.MemSize())
	UINT									m_IdxCount; // 인덱스 버퍼의 인덱스 갯수(6개, 인덱스를 표기할 데이터의 갯수)
	DXGI_FORMAT								m_IBFormat;	// 인덱스 버퍼 포멧 sizeof(WORD) 


	D3D11_PRIMITIVE_TOPOLOGY				m_DrawMode; // 메쉬를 그리는 방법(선 or 면으로)


	//static CMesh* Create(const CREATE_DESC& _Desc);

public:
	void Update();
	void UpdateVertexBuffer();
	void UpdateIndexBuffer();
	void Render();

public:
	ID3D11Buffer * GetVtxBuffer() { return m_VB; }
	ID3D11Buffer * GetIdxBuffer() { return m_IB; }
	const UINT	GetIdxCount() { return m_IdxCount; }
	const D3D11_PRIMITIVE_TOPOLOGY GetDrawMode() { return m_DrawMode; }


public:
	CMesh();
	CMesh(const CMesh& _Other);
	~CMesh();

public:
	NONE_LOAD_RESOURCE(CMesh)
	
	CREATE_DESC_BODY(void* VtxMem;
	UINT iVtxSize;
	UINT iVtxCount;
	D3D11_USAGE VtxUsage;
	void* IdxMem;
	UINT iIdxSize;
	UINT iIdxCount;
	D3D11_USAGE IdxUsage;
	DXGI_FORMAT IBFormat;
	D3D11_PRIMITIVE_TOPOLOGY DrawMode;)

	const bool Create(const CREATE_DESC& _Desc);
};

typedef CMesh::CREATE_DESC MESHDATA, MeshData;