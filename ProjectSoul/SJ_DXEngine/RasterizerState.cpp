#include "RasterizerState.h"
#include "Device.h"



CRasterizerState::CRasterizerState()
	:m_pRSState(nullptr)
{
}


CRasterizerState::~CRasterizerState()
{
}
void CRasterizerState::Update()
{
	if (nullptr == m_pRSState)
		return;

	DXCONTEXT->RSSetState(m_pRSState);
}
const bool CRasterizerState::Create(const CREATE_DESC& _Desc)
{
	HRESULT Check = DXDEVICE->CreateRasterizerState(&_Desc.ResterizerDesc, &m_pRSState);

	if (S_OK != Check)
	{
		TASSERT(true);
		return false;
	}

	return true;
}