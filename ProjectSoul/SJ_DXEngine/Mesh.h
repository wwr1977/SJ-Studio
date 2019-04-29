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
	UINT									m_VtxSize;	// ���ؽ��� ������ sizeof(VERTEX)
	UINT									m_VtxCount; // ���ؽ��� ���� (2D ���� 4��)

	ID3D11Buffer*							m_IB;
	UINT									m_IdxSize;	// �ε��� ������ �ε����Ѱ��� ũ��(Idx.MemSize())
	UINT									m_IdxCount; // �ε��� ������ �ε��� ����(6��, �ε����� ǥ���� �������� ����)
	DXGI_FORMAT								m_IBFormat;	// �ε��� ���� ���� sizeof(WORD) 


	D3D11_PRIMITIVE_TOPOLOGY				m_DrawMode; // �޽��� �׸��� ���(�� or ������)


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