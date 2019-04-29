#include "stdafx.h"
#include "GameFont.h"


CGameFont::CGameFont(LPD3DXFONT _pFont,Vec2 _FontSize)
	:m_pFont(_pFont),m_FontSize(_FontSize)
{
}


CGameFont::~CGameFont()
{
	if (nullptr != m_pFont) 
	{
		m_pFont->Release();
		m_pFont = nullptr;
	}
}
BOOL CGameFont::IsUseFont()
{
	if (nullptr == m_pFont) 
	{
		return FALSE;
	}
	return TRUE;
}
CGameFont* CGameFont::CreateGameFont(LPDIRECT3DDEVICE9 _pDevice, const Vec2& _FontSize, const tstring& _FontName, const UINT& _Bold /*= FW_NORMAL*/)
{
	LPD3DXFONT NewFont;

	HRESULT HR = D3DXCreateFont(
		_pDevice
		, (int)_FontSize.y // 20 * 2.0f 예상 한글자크기일거다.
		, (UINT)_FontSize.x // 10
		, _Bold
		, DEFAULT_QUALITY
		, FALSE
		, DEFAULT_CHARSET
		, OUT_DEFAULT_PRECIS
		, ANTIALIASED_QUALITY
		, VARIABLE_PITCH
		, _FontName.c_str()
		, &NewFont);

	if (S_OK != HR)
	{
		assert(false);
		return nullptr;
	}

	if (nullptr == NewFont)
	{
		assert(false);
		return nullptr;
	}

	return new CGameFont(NewFont, _FontSize);
}
void CGameFont::DrawGameText(LPDIRECT3DDEVICE9 _pDevice, const tstring& _Text, Vec2 _Pos, float _FontMag /*= 1.0f*/, DWORD Color /*= D3DCOLOR_XRGB(0, 0, 0)*/)
{
	// 여기에 들어온 Pos는 Resource에서 World공간상의 좌표를 
	// 윈도우 좌표로 바뀐값이 들어온다.
	RECT rc;
	Vec2 TextLength;
	TextLength.x = m_FontSize.x  * _FontMag * (float)_Text.size();
	TextLength.y = m_FontSize.y * _FontMag;

	LONG BiasX = 0;
	LONG BiasY = 0;

	if (1 == ((int)TextLength.x % 2)) 
	{
		BiasX = 1;
	}
	if (1 == ((int)TextLength.y % 2))
	{
		BiasY = 1;
	}

	rc.left = (LONG)_Pos.x - (LONG)(TextLength.x) - BiasX;
	rc.right = (LONG)_Pos.x + (LONG)(TextLength.x);
	rc.top = (LONG)_Pos.y - (LONG)(TextLength.y / 2) - BiasY;
	rc.bottom = (LONG)_Pos.y + (LONG)(TextLength.y / 2);

	m_pFont->DrawText(NULL, _Text.c_str(), (int)_Text.size(), &rc, DT_CENTER, Color);

}
void CGameFont::DrawGameText_RightStart(LPDIRECT3DDEVICE9 _pDevice, const tstring& _Text, POINT _RightPos, float _FontMag /*= 1.0f*/, DWORD Color /*= D3DCOLOR_XRGB(0, 0, 0)*/)
{
	RECT rc;
	Vec2 TextLength;
	TextLength.x = m_FontSize.x * _FontMag * (float)_Text.size();
	TextLength.y = m_FontSize.y * _FontMag;

	LONG BiasX = 0;
	LONG BiasY = 0;

	if (1 == ((int)TextLength.x % 2))
	{
		BiasX = 1;
	}
	if (1 == ((int)TextLength.y % 2))
	{
		BiasY = 1;
	}
	
	rc.left = _RightPos.x - (LONG)TextLength.x;
	rc.right = _RightPos.x;
	rc.top = _RightPos.y - (LONG)(TextLength.y / 2);
	rc.bottom = _RightPos.y + (LONG)(TextLength.y / 2);

	//m_pFont->DrawText(NULL, _Text.c_str(), (int)_Text.size(), &rc, DT_LEFT, Color);
	m_pFont->DrawText(NULL, _Text.c_str(), (int)_Text.size(), &rc, DT_RIGHT, Color);

}
void CGameFont::DrawGameText(LPDIRECT3DDEVICE9 _pDevice, const tstring& _Text, POINT _LeftPos, float _FontMag /*= 1.0f*/, DWORD Color /*= D3DCOLOR_XRGB(0, 0, 0)*/)
{
	
	RECT rc;
	Vec2 TextLength;
	TextLength.x = m_FontSize.x * _FontMag * (float)_Text.size();
	TextLength.y = m_FontSize.y * _FontMag;

	LONG BiasX = 0;
	LONG BiasY = 0;

	if (1 == ((int)TextLength.x % 2))
	{
		BiasX = 1;
	}
	if (1 == ((int)TextLength.y % 2))
	{
		BiasY = 1;
	}

	rc.left = _LeftPos.x  ;
	rc.right = _LeftPos.x + (LONG)TextLength.x ;
	rc.top = _LeftPos.y - (LONG)(TextLength.y / 2);
	rc.bottom = _LeftPos.y + (LONG)(TextLength.y / 2);

	m_pFont->DrawText(NULL, _Text.c_str(), (int)_Text.size(), &rc, DT_LEFT, Color);

}

const Vec2 CGameFont::GetFontSize()
{
	return m_FontSize;
}