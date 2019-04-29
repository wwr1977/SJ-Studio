#include "stdafx.h"
#include "FontRenderer.h"
#include "GameFont.h"

CFontRenderer::CFontRenderer()
	:m_FontName(_T("±¼¸²")),m_RenderString(_T("")), m_LeftStartPivot(POINT{0,0})
	, m_MiddleStartPivot(Vec3(0.0f,0.0f,0.0f)), m_CurMode(START_LEFT)
	, m_fFontMag(2.f), m_FontColor(D3DCOLOR_ARGB(255,255,255,255))
	, m_FontSize(Vec2{0.f,0.f})
{
}


CFontRenderer::~CFontRenderer()
{
}
void CFontRenderer::Init()
{
	if (nullptr == COMRESMGR->FindGameFont(m_FontName))
		m_FontSize = COMRESMGR->FindGameFont(m_FontName)->GetFontSize();


}
void CFontRenderer::Render()
{
	Vec3 ActorPos = TRANS->GetPos() + Vec3(m_RenPivot.x, m_RenPivot.y,0.0f);

	switch (m_CurMode)
	{
	case CFontRenderer::START_LEFT:
	{
		POINT Pos = POINT({ (long)ActorPos.x + m_LeftStartPivot.x , (long)ActorPos.y + m_LeftStartPivot.y });
		COMRESMGR->DrawFont(m_FontName, m_RenderString, Pos, m_fFontMag, m_FontColor);
	}
		break;
	case CFontRenderer::START_MIDDLE:
		COMRESMGR->DrawFont(m_FontName, m_RenderString, ActorPos + m_MiddleStartPivot, m_fFontMag, m_FontColor);
		break;
	case CFontRenderer::START_RIGHT: 
	{
		POINT Pos = POINT({ (long)ActorPos.x + m_LeftStartPivot.x , (long)ActorPos.y + m_LeftStartPivot.y });
		COMRESMGR->DrawFont_RightStart(m_FontName, m_RenderString, Pos, m_fFontMag, m_FontColor);
	}
		break;
	}
}
void CFontRenderer::SetFontStyle(const tstring& _FontStyle)
{
	if (FALSE == COMRESMGR->ExistGameFont(_FontStyle)) 
		return;
	

	m_FontName = _FontStyle;
	m_FontSize = COMRESMGR->FindGameFont(_FontStyle)->GetFontSize();
}
void CFontRenderer::SetString(const tstring& _String) 
{
	m_RenderString = _String;
}

void CFontRenderer::SetLeftStartPivot(const POINT& _StartPivot)
{
	m_CurMode = START_LEFT;
	m_LeftStartPivot = _StartPivot;
}
void CFontRenderer::SetMiddleStartPivot(const Vec3& _MiddlePivot) 
{
	m_CurMode = START_MIDDLE;
	m_MiddleStartPivot = _MiddlePivot;
}
void CFontRenderer::SetRightStartPivot(const POINT& _StartPivot)
{
	m_CurMode = START_RIGHT;
	m_LeftStartPivot = _StartPivot;
}
void CFontRenderer::SetFontMag(const float& _FontMag) 
{
	if (1.0f >= _FontMag) 
	{
		return;
	}
	m_fFontMag = _FontMag;
}
void CFontRenderer::SetFontColor(const DWORD& _FontColor) 
{
	m_FontColor = _FontColor;
}
void CFontRenderer::SetFontRenderMode(const STARTPOS& _Mode)
{
	if (_Mode >= MAXSTARTPOS)
		return;
	m_CurMode = _Mode;
}
void CFontRenderer::SetFontRenderMode(const size_t& _Mode)
{
	return  SetFontRenderMode((STARTPOS)_Mode);
}

const tstring CFontRenderer::GetFontString()
{
	return m_RenderString;
}
const float CFontRenderer::GetFontXLength()
{
	size_t Len = m_RenderString.length() ;

	return (float)Len * m_FontSize.x * m_fFontMag;
}