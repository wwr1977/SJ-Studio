#pragma once
#include"Shader.h"

class CVtxShader : public CShader
{
private:
	ID3D11VertexShader*				m_pVtxShader;

public:
	ID3D11VertexShader * GetShader() { return m_pVtxShader; }

public:
	const bool Load(const LOAD_DESC& _Desc) override;
	
	void Update() override;

public:
	CVtxShader();
	~CVtxShader();
};

