#pragma once
#include "GameCom.h"

class CActObject;
class CRenderer :public CGameCom
{
protected:
	static D3DMATERIAL9				BasicMaterial;
	Vec2							m_RenPivot;
	Vec2							m_CustomSize;
	bool							m_bCustom;

private:
	float							m_fAlphaFactor;
	bool							m_bAlpha;
	bool							m_bUI;
	PROJECTIONMODE					m_ProjectionMode;
	Vec3							m_TexBackColor;

	// ������ ���׸��� �������� �������� ����
	bool							m_bUseMaterial;
	D3DMATERIAL9					m_RendererMaterial;
protected:
	int								m_iOverlapCount;

public:
	static void RendererInit(LPDIRECT3DDEVICE9 _WndDevice);
	
public:
	virtual void Init();
	void InitData(void* p);
	virtual void Render() = 0 ;
	virtual void InitData(RENDATA _Data);
	void ComRender();
	// ������ ���İ� ���� ���(�⺻ ����ó�� �Լ�)
	void BaseAlpha();
	// ������ ���� ����ó��
	void BackGroundAlpha();
	// ������ �������� ���ó���� �̹��� ���� ó��
	void FilterAlpha();
	
	void SetAlpha(const float& _Alpha);

	bool isUI();
	PROJECTIONMODE GetProjMode();

	void SetMaterial(D3DXCOLOR _MatColor);
	void MaterialOff();
	void MaterialOn();

	// Ŀ���� ������ ���� �Լ�
	void CustomOff() { m_bCustom = false; }
	bool isCustomSize()
	{
		return m_bCustom;
	}
	void SetRenPivot(const Vec3& _Pivot);
	void SetRenPivot(const Vec2& _Pivot);
	void SetImageMag(const float& _Mag);
	const Vec2 GetRenPivot();
	
	void InvalidCustomSize();
	void SetCustomSize(const Vec2& _Size);
	void SetCustomSize(const float& _X, const float& _Y);
	const Vec2 GetCustomSize();

	void SetOverlapCount(const int& _Count);
	int GetOverlapCount();

	const float GetPosY();
public:
	CRenderer();
	CRenderer(CActObject* _ActObject);
	~CRenderer();
};

