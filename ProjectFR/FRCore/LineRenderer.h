#pragma once
#include "Renderer.h"

class CGameSprite;
class CLineRenderer : public CRenderer
{
private:
	SPTR<CGameSprite>				m_LineSprite;
	Vec3							m_StartPos;
	Vec3							m_EndPos;
	float							m_LineThick;
	Mat								m_LineWorldMat;


public:
	virtual void Init();
	virtual void Render();
	
public:
	void SettingLineData(const Vec3& _Start, const Vec3& _End);
	void SetLineThick(const float& _Thick);
	void SetSprite(SPTR<CGameSprite> _pSprite);
	void SetSprite(const tstring& _Key);
	void SetSprite(const tstring& _Key, size_t _Index);
	void SetColor(const D3DXCOLOR& _Color);
	void InvailidateColor();

	// 현재의 랜더링 상태를 반전
	void ReverseRendering();

public:
	CLineRenderer();
	~CLineRenderer();
};

