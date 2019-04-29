#pragma once
#include "Resource.h"
#include"DXHeader.h"

class CSamplerState :	public CResource
{
	/*template<typename Res, typename Key>
	friend class CResourceMgr;*/

	friend class Engine;
	//friend class CResourceMgr<CSamplerState>;

private:
	ID3D11SamplerState *						m_pSamplerState;

private:
	CSamplerState();
	~CSamplerState();

public:
	void Update(const int& _SlotNum);


public:
	ID3D11SamplerState** GetSamplerSate() 
	{
		return &m_pSamplerState;
	}

public:
	NONE_LOAD_RESOURCE(CSamplerState)
	CREATE_DESC_BODY(
		D3D11_SAMPLER_DESC SampleDesc;
	)

public:
	const bool Create(const CREATE_DESC& _Desc);
};

