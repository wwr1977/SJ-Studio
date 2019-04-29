#include "ConstantBuffer.h"
#include"DXMacro.h"
#include"Device.h"


CConstantBuffer::CConstantBuffer()
	:m_CB(nullptr), m_BufferDesc{}
	, m_MappedSub{}, m_pBufferType(nullptr)
{
}


CConstantBuffer::~CConstantBuffer()
{
	SAFE_RELEASE(m_CB)
}

const bool CConstantBuffer::Create(const CREATE_DESC& _Desc)
{
	m_BufferDesc.ByteWidth = _Desc.ByteWidth;
	m_BufferDesc.Usage = _Desc.Usage;
	m_BufferDesc.CPUAccessFlags = _Desc.CPUAccessFlags;
	m_BufferDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;

	if (S_OK != DXDEVICE->CreateBuffer(&m_BufferDesc, nullptr, &m_CB))
	{
		TASSERT(true);
		return false;
	}

	return true;
}