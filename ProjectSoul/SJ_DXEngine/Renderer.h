#pragma once
#include"SubTrans.h"
#include"DXHeader.h"
#include"DXEnum.h"

struct RENDATA 
{
	RENDER_LAYER			RenderLayer;
};

class CRenderer : public CSubTrans
{
	friend class CActor;
	friend class CSceneRenderMgr;

protected:
	RENDER_LAYER							m_RenderLayer;
	Vec3									m_RenSize;
	Vec3									m_RenPivot;
	Vec4									m_RenColorFactor;

protected:
	RENDER_SORTING							m_eSortingType;
	RENDER_OPTION							m_eRenderOption;

protected:
	Vec4									m_StartColorFactor;
	Vec4									m_EndColorFactor;
	float									m_fFactorTime;
	float									m_fAccFactorTime;
	bool									m_bAutoFactor;

protected:
	virtual void Render(const Mat& _VP) = 0;

protected:
	virtual const bool Init(const RENDATA& _InitData);
	void PostUpdate(const float& _DeltaTime)override;


public:
	void SetRenPivot(const Vec3& _Pivot);
	void SetRenSize(const Vec3& _Size);
	void SetColorFactor(const Vec4& _Factor)
	{
		m_RenColorFactor = _Factor;
	}
	void SetColorFactor(const Vec3& _Color)
	{
		m_RenColorFactor.vec3 = _Color;
	}
	void SetAlpha(const float& _Alpha);
	void SetAutoColorFactor(const Vec4& _StartFactor, const Vec4& _EndFactor,const float& _Time) 
	{
		m_bAutoFactor = true;
		m_StartColorFactor = _StartFactor;
		m_EndColorFactor = _EndFactor;
		m_fFactorTime = _Time;
		m_fAccFactorTime = 0.f;
	}


public:
	void YSortingOn() { m_eSortingType = RENDER_YSORTING; }
	void YSortingOff() { m_eSortingType = RENDER_NONESORTING; }
	void SetRenderOption(const RENDER_OPTION& _Op) { m_eRenderOption = _Op; }
	void SetGrayScale(const bool& _Gray) 
	{
		if (true == _Gray)
			m_eRenderOption = RENDER_OPTION_GRAYSCALE;
		else
			m_eRenderOption = RENDER_OPTION_NONE;
	}
	const RENDER_SORTING GetRenSortingType() { return m_eSortingType; }
	const RENDER_OPTION GetRenderOption() { return m_eRenderOption; }
	const bool IsGrayScale() 
	{
		if (m_eRenderOption == RENDER_OPTION_GRAYSCALE)
			return true;

		return false;
	}
public:
	const Vec3 GetRenSize();
	const Vec3 GetRenPivot();


public:
	const RENDER_LAYER GetRenderLayer() {	return m_RenderLayer;	}
	const Vec4 GetColorFactor() { return m_RenColorFactor; }
public:
	CRenderer();
	~CRenderer();
};

