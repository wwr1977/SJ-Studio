#pragma once
#include "GameCom.h"
class CLight : public CGameCom
{
private:
	D3DLIGHT9					m_Light;


public:
	void Init();
	void Update();

public:
	void SetDirectionLight();
	void SetAmbient(D3DXCOLOR _AmbColor);
	void SetDiffuse(D3DXCOLOR _AmbColor);
	void SetSpecular(D3DXCOLOR _AmbColor);

public:
	void SetLight();

public:
	CLight();
	~CLight();
};

