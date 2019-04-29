#pragma once
#include"Resource.h"
#include"DXHeader.h"
#include"DXEnum.h"
#include"DXTemplate.h"
#include"ConstantBuffer.h"
#include<vector>
#include<unordered_map>

#define MAXINPUTSLOT	8

class CShader :public CResource
{
public:
	SHADER_TYPE										m_eShaderType;


protected:
	static UINT SizeofFmt(const DXGI_FORMAT& _Fmt);

	class CInputLayout  : public CRefBase
	{
	private:
		CShader*									m_pParentShader;
		UINT										m_Offset[MAXINPUTSLOT];
		std::vector<D3D11_INPUT_ELEMENT_DESC>		m_vecInputDesc;
		ID3D11InputLayout*							m_pInputLayout;


	public:
		void PushInputDesc(const char* _SemanticName, const UINT& _Index
			, const DXGI_FORMAT& _Fmt, const UINT& _InputSlot
			, const UINT& _IDSR, D3D11_INPUT_CLASSIFICATION _eInput
			= D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA);

		void CreateInputLayout();
		void Update();
		


		CInputLayout() 
			:m_pParentShader(nullptr), m_pInputLayout(nullptr)
		{
			memset(m_Offset, 0, sizeof(m_Offset));
		}
		CInputLayout(CShader* _Shader)
			:m_pParentShader(_Shader), m_pInputLayout(nullptr)
		{
			memset(m_Offset, 0, sizeof(m_Offset));
		}
		~CInputLayout()
		{
			SAFE_RELEASE(m_pInputLayout)
		}

	};


protected:
	ID3DBlob*					m_pBlob;			// ���̴� �ڵ带 �������ؼ� ����̽����� �Ѱ����� ����̽��� �ִ� ������
	ID3DBlob*					m_pErrBlob;			// ���̴� �ڵ� �������� ���������� ����̽��� ������ ������ �Ѱ��ִ� ������
	UINT						m_VH;				// ���ý� ���̴� HLSL ���� ���� (5)
	UINT						m_VL;				// ���ý� ���̴� HLSL ���� ���� (0)	-> 5.0
	SPTR<CInputLayout>			m_pInputLayout;


public:
	void PushInputDesc(const char* _SemanticName, const UINT& _Index
		, const DXGI_FORMAT& _Fmt, const UINT& _InputSlot
		, const UINT& _IDSR, D3D11_INPUT_CLASSIFICATION _eInput
		= D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA);
	void PushEndInputDesc(const char* _SemanticName, const UINT& _Index
		, const DXGI_FORMAT& _Fmt, const UINT& _InputSlot
		, const UINT& _IDSR, D3D11_INPUT_CLASSIFICATION _eInput
		= D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA);

public:
	ID3DBlob* GetBIob() { return m_pBlob; }
	ID3DBlob* GetErrBIob() { return m_pErrBlob; }


	// Constant Buffer 
protected:
	std::unordered_map<UINT, SPTR<CConstantBuffer>>				m_mapConstBuffer;

public:
	const BOOL PushConstantBuffer(const UINT& _RegisterNum, const std::wstring& _BufferKey);

	template<typename T>
	void BufferUpdate(const UINT& _SlotIndex, T* _BuffData)
	{
		SPTR<CConstantBuffer> Buffer = UMAPFINDSPTR<SPTR<CConstantBuffer>>(m_mapConstBuffer, _SlotIndex);

		if (nullptr != Buffer)
			Buffer->Update<T>(_SlotIndex, _BuffData, m_eShaderType);
	}



public:
	CShader(const SHADER_TYPE& _eType);
	~CShader();

public:
	NONE_CREATE_RESOURCE(CShader)

	LOAD_DESC_BODY( UINT	iVH;
		UINT	iVL;
		std::string   FuncName;
		bool		bCreateShader;
	)

	virtual const bool Load(const LOAD_DESC& _Desc) = 0;
	virtual void Update() = 0;

};

