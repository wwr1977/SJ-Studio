#include "stdafx.h"
#include "LineRenderer.h"
#include "GameSprite.h"


CLineRenderer::CLineRenderer()
	:m_LineSprite(nullptr), m_StartPos(Vec3{0.0f,0.0f,0.0f})
	, m_EndPos(Vec3{0.0f,0.0f,0.0f})
	, m_LineThick(5.0f)
{
	D3DXMatrixIdentity(&m_LineWorldMat);
}


CLineRenderer::~CLineRenderer()
{
}
void CLineRenderer::Init()
{
	m_LineSprite = COMRESMGR->FindGameSprite(_T("ColorBase"));
}
void CLineRenderer::Render()
{
	if (nullptr == m_LineSprite)
		return;
	
	GetComDevice()->SetTransform(D3DTS_WORLD, &m_LineWorldMat);

	if (nullptr != m_LineSprite)
	{
		for (int i = 0; i < m_iOverlapCount; i++)
		{
			m_LineSprite->Render();
		}
	}


}
void CLineRenderer::SettingLineData(const Vec3& _Start, const Vec3& _End)
{
	if (nullptr == m_LineSprite)
		return;


	m_StartPos = _Start;
	m_EndPos = _End;

	Vec3 Mid = (m_StartPos + m_EndPos) * 0.5f;
	float XLen = CSJMath::Length(m_StartPos, m_EndPos);

	Vec3 Base = GAMEVEC::RIGHT;
	Vec3 Dir = Vec3{ m_EndPos.x - m_StartPos.x,m_EndPos.y - m_StartPos.y , 0.0f };
	D3DXVec3Normalize(&Dir, &Dir);
	Vec3 Axis;

	Mat Scale;
	Mat Rot;
	Mat Trans;
	
	D3DXMatrixScaling(&Scale, XLen, m_LineThick, 1.0f);

	float Seta = acosf(D3DXVec3Dot(&Base, &Dir));
	D3DXVec3Cross(&Axis, &Base, &Dir);
	D3DXMatrixRotationAxis(&Rot, &Axis, Seta);
	D3DXMatrixTranslation(&Trans, Mid.x, Mid.y, Mid.z);

	m_LineWorldMat = Scale* Rot * Trans;
}
void CLineRenderer::SetLineThick(const float& _Thick)
{
	m_LineThick = _Thick;
}
void CLineRenderer::SetSprite(SPTR<CGameSprite> _pSprite)
{
	assert(_pSprite);

	if (nullptr == _pSprite)
	{
		return;
	}

	m_LineSprite = _pSprite;

}
void CLineRenderer::SetSprite(const tstring& _Key)
{
	m_LineSprite = COMRESMGR->FindGameSprite(_Key);
	assert(m_LineSprite);

}

void CLineRenderer::SetSprite(const tstring& _Key, size_t _Index)
{
	m_LineSprite = COMRESMGR->FindMultiSpriteToIndex(_Key, _Index);
	assert(m_LineSprite);
}

void CLineRenderer::SetColor(const D3DXCOLOR& _Color)
{
	SetMaterial(_Color);
	MaterialOn();
}
void CLineRenderer::InvailidateColor()
{
	MaterialOff();
}
void CLineRenderer::ReverseRendering()
{
	if (true == IsActive())
		Off();
	else
		On();
}