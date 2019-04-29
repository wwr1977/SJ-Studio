#include "VtxShader.h"
#include"Device.h"
#include"ResourceMgr.h"


CVtxShader::CVtxShader()
	:CShader(SHADER_TYPE::SHADER_VERTEX)
	, m_pVtxShader(nullptr)
{
}


CVtxShader::~CVtxShader()
{
	SAFE_RELEASE(m_pVtxShader)
	m_mapConstBuffer.clear();

}
const bool CVtxShader::Load(const LOAD_DESC& _Desc)
{
	m_VH = _Desc.iVH;
	m_VL = _Desc.iVL;

	UINT iFlag = 0;
#ifdef _DEBUG
	iFlag = D3D10_SHADER_DEBUG;
#endif

	//iFlag = D3D10_SHADER_PACK_MATRIX_ROW_MAJOR;

	// 버텍스 쉐이더(HLSL) 컴파일 버전
	char szBuffer[256] = {};
	wsprintfA(szBuffer, "vs_%d_%d", m_VH, m_VL);

	if (S_OK != D3DCompileFromFile(GetPath().c_str(), nullptr
		, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, _Desc.FuncName.c_str(), szBuffer, iFlag, 0, &m_pBlob, &m_pErrBlob))
	{
		std::string ErrorText = (char*)m_pErrBlob->GetBufferPointer();
		TASSERT(true);
		return FALSE;
	}

	if (S_OK != DXDEVICE->CreateVertexShader(
		m_pBlob->GetBufferPointer(),
		m_pBlob->GetBufferSize(),
		nullptr, &m_pVtxShader))
	{
		TASSERT(true);
		return false;
	}

	return true;
}

void CVtxShader::Update()
{
	CShader::Update();
	DXCONTEXT->VSSetShader(m_pVtxShader, 0, 0);
}

