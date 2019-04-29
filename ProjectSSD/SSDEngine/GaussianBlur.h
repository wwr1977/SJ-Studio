#pragma once
#include "FilterEffect.h"

class CGaussianBlur : public CFilterEffect
{
private:
	SPTR<class CMaterial>					m_GauMat;
	CBUFFER::GaussianBuffer					m_GaussianBuf;
	float									m_Sigma;

public:
	void Init() override;
	void Update(SPTR<MultiRenderTarget> _Target) override;


public:
	// Sigma = ����þ� ���� ����ũ���� ���� ���� (Sigma �� ������ �ڽ��� �ȼ��� �� ���� ����޴´�.)
	// Range = ����þ� ���� ���� �ȼ� ���� (Range x Range ���� ��ŭ) 
	void SetGaussianBuffer(const float& _Sigma, const int& _Range = 5);

	void SetSigma(float _Sigma)
	{
		SetGaussianBuffer(_Sigma, m_GaussianBuf.Range);
	}
	void SetRange(int _Range)
	{
		SetGaussianBuffer(m_Sigma, _Range);
	}
	float GetSigma()
	{
		return m_Sigma;
	}
	int GetRange()
	{
		return m_GaussianBuf.Range;
	}
	CBUFFER::GaussianBuffer GetGaussianBuffer()
	{
		return m_GaussianBuf;
	}

public:
	CGaussianBlur();
	~CGaussianBlur();

};

