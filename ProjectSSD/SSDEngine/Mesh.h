#pragma once
#include "Resource.h"
#include "Engine.h"
#include "DXHeader.h"
#include "DXBuffer.h"
#include <vector>
#include <list>



class CMesh : public CResource
{
	template<typename Res, typename Key>
	friend class CResourceMgr;

private:

	class CVertexBuffer : public CDXBuffer
	{
	private:
		friend CMesh;
		friend class CFBXMesh;

	private:
		// ���ؽ��� ������ sizeof(VERTEX),IA ����Լ� ȣ��� 4��° �Ű����� ����
		UINT									m_VtxSize;
		// ���ؽ��� ���� 
		UINT									m_VtxCount;

	public:
		CVertexBuffer();
		CVertexBuffer(const UINT& _iVtxCount, const UINT& _iVtxSize, const D3D11_USAGE& _VtxUsage);
		~CVertexBuffer();

	};

	class CIndexBuffer : public CDXBuffer
	{
	private:
		friend CMesh;
		friend class CFBXMesh;

	private:
		UINT									m_VtxIndex;
		UINT									m_IdxSize;	// �ε��� ������ �ε����Ѱ��� ũ��(Idx.MemSize())
		UINT									m_IdxCount; // �ε��� ������ �ε��� ����(6��, �ε����� ǥ���� �������� ����)
		DXGI_FORMAT								m_IBFormat;	// �ε��� ���� ���� sizeof(WORD) 

	public:
		CIndexBuffer();
		CIndexBuffer(const UINT& _iVtxIndex, const UINT& _iIdxCount, const UINT& _iIdxSize, const D3D11_USAGE& _IdxUsage, const DXGI_FORMAT& _IdxFm);
		~CIndexBuffer();
	};

protected:
	bool									m_bDraw;
	std::vector<SPTR<CVertexBuffer>>		m_vecVB;
	std::vector<SPTR<CIndexBuffer>>			m_vecIB;

	D3D11_PRIMITIVE_TOPOLOGY				m_DrawMode; // �޽��� �׸��� ���(�� or ������)

public:
	void Update();
	void VBUpdate(const int& _Index, void* _pData);
	void VtxRender(const int& _Index, const UINT& _DrawCount = -1);
	void VtxRender(const int& _Index, const UINT& _DrawCount ,const UINT& OffSet );
	//void VtxRender(const int& _Index);
	void Render(int _Idx);
	virtual void Render();
	virtual void InstancingRender();

public:
	UINT VtxBufferSize()
	{
		return (UINT)m_vecVB.size();
	}
	UINT IdxBufferSize()
	{
		return (UINT)m_vecIB.size();
	}

	const D3D11_PRIMITIVE_TOPOLOGY GetDrawMode() { return m_DrawMode; }


public:
	CVertexBuffer*  CreateVertexBuffer(const UINT _iVtxCount, UINT _iVtxSize, D3D11_USAGE _VtxUsage, void* _VtxMem);
	CIndexBuffer*  CreateIndexBuffer(UINT _iVtxUdx, UINT _iIdxCount, UINT _iIdxSize, D3D11_USAGE _IdxUsage, void* _IdxMem, DXGI_FORMAT _IdxFm);


public:
	CVertexBuffer* GetVtxBuffer(const int& _Index);

public:
	void Draw(const bool& _bDraw) { m_bDraw = _bDraw; }
	void DrawOn() { m_bDraw = true; }
	void DrawOff() { m_bDraw = false; }
	const bool IsDraw() { return m_bDraw; }

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



class CFBXMesh : public CMesh
{
	friend class CFbx;
	friend class CBoneMesh;

private:
	// �ڽŰ� ������ FBX BoneMesh�� Idx
	CFbx*											m_Fbx;
	unsigned int									m_MeshIdx;
	std::vector<std::list<SPTR<class CMaterial>>>	m_vecMaterialList;


public:
	void Render() override;
	void InstancingRender() override;

public:
	CFBXMesh* Clone();

public:
	void AllocateMeshMaterial();

public:
	UINT GetMeshIdx()
	{
		return m_MeshIdx;
	}
	const std::list<SPTR<class CMaterial>>& GetMatList(UINT _Idx)
	{
		return m_vecMaterialList[_Idx];
	}
	CMaterial* GetMatStart(UINT _Idx)
	{
		std::list<SPTR<class CMaterial>>::const_iterator StartIter = GetMatList(_Idx).begin();
		return (*StartIter);
	}
	UINT GetMatSize()
	{
		return (UINT)m_vecMaterialList.size();
	}

	void SetRasterizerState(const wchar_t* _RSKey);

public:
	CFBXMesh();
	CFBXMesh(const CFBXMesh& _Other);
	~CFBXMesh();
};

class COBJMesh : public CMesh
{
public:
	std::vector<COLORVTX>							m_vecColorVtx;
	std::vector<IDX32>								m_vecIdx;

public:
	COBJMesh() {}
	~COBJMesh() {}
};
