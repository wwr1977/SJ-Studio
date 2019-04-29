#include "Mesh.h"
#include"Engine.h"
#include"Device.h"


//CMesh* CMesh::CREATE_DESC::Create(const CMesh::CREATE_DESC& _Desc) 
//{
//	CMesh* NewMesh = Engine::CreateResourceObject<CMesh>();
//
//	NewMesh->m_VtxSize = _Desc.iVtxSize;
//	NewMesh->m_VtxCount = _Desc.iVtxCount;
//	NewMesh->m_IdxSize = _Desc.iIdxSize;
//	NewMesh->m_IdxCount = _Desc.iIdxCount;
//	NewMesh->m_IBFormat = _Desc.IBFormat;
//	NewMesh->m_DrawMode = _Desc.DrawMode;
//
//
//	D3D11_BUFFER_DESC VtxDecs = D3D11_BUFFER_DESC();
//	VtxDecs.ByteWidth = _Desc.iVtxSize * _Desc.iVtxCount;
//	VtxDecs.Usage = _Desc.VtxUsage;
//													
//	if (D3D11_USAGE::D3D11_USAGE_DYNAMIC == VtxDecs.Usage)
//		VtxDecs.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
//
//	// 이 버퍼는 버텍스 버퍼로 사용할것이다.
//	VtxDecs.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//
//	D3D11_SUBRESOURCE_DATA VtxSub = D3D11_SUBRESOURCE_DATA();
//	VtxSub.pSysMem = &_Desc.VtxMem;		// 버퍼가 복사 할 정점정보가 담긴 메모리 주소
//
//	if (S_OK != CDevice::GetDevice()->CreateBuffer(&VtxDecs, &VtxSub, &NewMesh->m_VB))
//		TASSERT(true);
//
//
//	D3D11_BUFFER_DESC IdxDecs = D3D11_BUFFER_DESC();
//	IdxDecs.ByteWidth = _Desc.iIdxSize * _Desc.iIdxCount;
//
//	// GPU 뿐만아니라  CPU에서도 메쉬 정보를 바꿀수 있다.
//	//VtxDecs.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;  //GPU에서만  메쉬 정보를 바꿀수 있다.
//	IdxDecs.Usage = _Desc.IdxUsage;					
//	if (D3D11_USAGE::D3D11_USAGE_DYNAMIC == IdxDecs.Usage)
//		IdxDecs.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
//
//	// 이 버퍼는 버텍스 버퍼로 사용할것이다.
//	IdxDecs.BindFlags = D3D11_BIND_INDEX_BUFFER;
//
//	D3D11_SUBRESOURCE_DATA IdxSub = D3D11_SUBRESOURCE_DATA();
//	IdxSub.pSysMem = &_Desc.IdxMem;		// 버퍼가 접근할 정점정보가 담긴 메모리 주소
//
//	if (S_OK != CDevice::GetDevice()->CreateBuffer(&IdxDecs, &IdxSub, &NewMesh->m_IB));
//		TASSERT(true);
//
//	return NewMesh;
//}

/////////////////////////////////////////////////////////////////////////////////

CMesh::CMesh()
	:m_VB(nullptr), m_VtxSize(0), m_VtxCount(0)
	, m_IB(nullptr),m_IdxSize(0), m_IdxCount(0), m_IBFormat(DXGI_FORMAT_R16_UINT)
	, m_DrawMode(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
{
}

CMesh::CMesh(const CMesh& _Other)
	:m_VB(_Other.m_VB), m_VtxSize(_Other.m_VtxSize), m_VtxCount(_Other.m_VtxCount)
	, m_IB(_Other.m_IB), m_IdxSize(_Other.m_IdxSize), m_IdxCount(_Other.m_IdxCount)
	, m_IBFormat(_Other.m_IBFormat), m_DrawMode(_Other.m_DrawMode)
{

}

CMesh::~CMesh()
{
	SAFE_RELEASE(m_VB)
	SAFE_RELEASE(m_IB)
}

void CMesh::Update()
{
	UINT OffSet = 0;
	// 버텍스 버퍼와
	// 인덱스 버퍼의 내용을 그려달라고 컨텍스트에 정보를 넘겨줘야 한다.
	// 버텍스 버퍼와 인덱스 버퍼의 세팅
	DXCONTEXT->IASetVertexBuffers(0, 1, &m_VB, &m_VtxSize, &OffSet);
	DXCONTEXT->IASetPrimitiveTopology(m_DrawMode);
	DXCONTEXT->IASetIndexBuffer(m_IB, m_IBFormat, 0);

}
void CMesh::UpdateVertexBuffer()
{
	UINT OffSet = 0;
	DXCONTEXT->IASetVertexBuffers(0, 1, &m_VB, &m_VtxSize, &OffSet);
}
void CMesh::UpdateIndexBuffer()
{
	DXCONTEXT->IASetPrimitiveTopology(m_DrawMode);
	DXCONTEXT->IASetIndexBuffer(m_IB, m_IBFormat, 0);
}
void CMesh::Render() 
{
	DXCONTEXT->DrawIndexed(m_IdxCount, 0, 0);
}


const bool CMesh::Create(const CREATE_DESC& _Desc)
{
	m_VtxSize = _Desc.iVtxSize;
	m_VtxCount = _Desc.iVtxCount;
	m_IdxSize = _Desc.iIdxSize;
	m_IdxCount = _Desc.iIdxCount;
	m_IBFormat = _Desc.IBFormat;
	m_DrawMode = _Desc.DrawMode;

	D3D11_BUFFER_DESC VtxDecs = D3D11_BUFFER_DESC();
	VtxDecs.ByteWidth = _Desc.iVtxSize * _Desc.iVtxCount;
	VtxDecs.Usage = _Desc.VtxUsage;

	if (D3D11_USAGE::D3D11_USAGE_DYNAMIC == VtxDecs.Usage)
		VtxDecs.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	// 이 버퍼는 버텍스 버퍼로 사용할것이다.
	VtxDecs.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA VtxSub = D3D11_SUBRESOURCE_DATA();
	VtxSub.pSysMem = _Desc.VtxMem;		// 버퍼가 복사 할 정점정보가 담긴 메모리 주소

	if (S_OK != DXDEVICE->CreateBuffer(&VtxDecs, &VtxSub, &m_VB))
		TASSERT(true);




	D3D11_BUFFER_DESC IdxDecs = D3D11_BUFFER_DESC();
	IdxDecs.ByteWidth = _Desc.iIdxSize * _Desc.iIdxCount;

	// 인덱스 버퍼에 크기가 0으로 설정되어있다면 인덱스 버퍼를 생성하지 않는다.
	if (IdxDecs.ByteWidth <= 0 || nullptr == _Desc.IdxMem)
		return true;

	// GPU 뿐만아니라  CPU에서도 메쉬 정보를 바꿀수 있다.
	//VtxDecs.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;  //GPU에서만  메쉬 정보를 바꿀수 있다.
	IdxDecs.Usage = _Desc.IdxUsage;
	if (D3D11_USAGE::D3D11_USAGE_DYNAMIC == IdxDecs.Usage)
		IdxDecs.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	// 이 버퍼는 버텍스 버퍼로 사용할것이다.
	IdxDecs.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA IdxSub = D3D11_SUBRESOURCE_DATA();
	IdxSub.pSysMem = _Desc.IdxMem;						// 버퍼가 접근할 정점정보가 담긴 메모리 주소

	if (S_OK != DXDEVICE->CreateBuffer(&IdxDecs, &IdxSub, &m_IB))
	{
		TASSERT(true);
		return false;
	}

	return true;

}