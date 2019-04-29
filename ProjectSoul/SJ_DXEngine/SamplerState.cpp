#include "SamplerState.h"
#include"Device.h"


CSamplerState::CSamplerState()
	:m_pSamplerState(nullptr)
{
}


CSamplerState::~CSamplerState()
{
	SAFE_RELEASE(m_pSamplerState)
}


void CSamplerState::Update(const int& _SlotNum)
{
	DXCONTEXT->VSSetSamplers(_SlotNum,1,&m_pSamplerState);
	DXCONTEXT->PSSetSamplers(_SlotNum, 1, &m_pSamplerState);
}

const bool CSamplerState::Create(const CREATE_DESC& _Desc)
{
	HRESULT Check;

	Check = DXDEVICE->CreateSamplerState(&_Desc.SampleDesc, &m_pSamplerState);

	if (S_OK != Check)
		return false;

	return true;
}

