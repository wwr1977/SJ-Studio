#pragma once
#include"Resource.h"
#include"DXHeader.h"
#include"DXMacro.h"
#include"DXEnum.h"
#include"Device.h"


class CConstantBuffer : public CResource 
{
private:
	D3D11_BUFFER_DESC								m_BufferDesc;
	const type_info*								m_pBufferType;
	
	ID3D11Buffer*									m_CB;
	D3D11_MAPPED_SUBRESOURCE						m_MappedSub;



public:
	template<typename BuffType>
	void SetBuffDataType() 
	{
		m_pBufferType = &typeid(BuffType);
	}

	template<typename BuffType>
	void Update(UINT _RegisterNum, BuffType* _pMapping,const SHADER_TYPE& _ShaderType)
	{
		if (&typeid(BuffType) != m_pBufferType)
			return;


		// Map,Unmap 함수의 2번째 인자값 : subresource의 인덱스 번호
		// Map 함수의 4번째 인자값 : GPU가 Busy 할 경우 CPU의 행동에 대한 옵션(0 : D3D11_MAP_FLAG_DO_NOT_WAIT
	
		CDevice::GetContext()->Map(m_CB, 0, D3D11_MAP_WRITE_DISCARD, 0, &m_MappedSub);
		memcpy_s(m_MappedSub.pData, m_BufferDesc.ByteWidth, _pMapping, sizeof(BuffType));
		CDevice::GetContext()->Unmap(m_CB, 0);
		
		switch (_ShaderType)
		{
		case SHADER_VERTEX:
			CDevice::GetContext()->VSSetConstantBuffers(_RegisterNum, 1, &m_CB);
			break;
		case SHADER_HULL:
			CDevice::GetContext()->HSSetConstantBuffers(_RegisterNum, 1, &m_CB);
			break;
		case SHADER_DOMAIN:
			CDevice::GetContext()->DSSetConstantBuffers(_RegisterNum, 1, &m_CB);
			break;
		case SHADER_GEOMETRY:
			CDevice::GetContext()->GSSetConstantBuffers(_RegisterNum, 1, &m_CB);
			break;
		case SHADER_PIXEL:
			CDevice::GetContext()->PSSetConstantBuffers(_RegisterNum, 1, &m_CB);
			break;
		default:
			break;
		}

	}

public:
	void* GetSubDataPointer() { return m_MappedSub.pData; }

public:
	CConstantBuffer();
	~CConstantBuffer();


	NONE_LOAD_RESOURCE(CConstantBuffer)
	
	CREATE_DESC_BODY(UINT ByteWidth;
	D3D11_USAGE Usage;
	UINT CPUAccessFlags;
	)


	const bool Create(const CREATE_DESC& _Desc);
};

