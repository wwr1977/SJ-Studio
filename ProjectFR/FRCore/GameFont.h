#pragma once
#include "ObjBase.h"
class CGameFont : public CObjBase
{
private:
	LPD3DXFONT			m_pFont;
	Vec2				m_FontSize;

public:
	BOOL IsUseFont();

public:
	static CGameFont* CreateGameFont(LPDIRECT3DDEVICE9 _pDevice, const Vec2& _FontSize, const tstring& _FontName,const UINT& _Bold = FW_NORMAL);
	void DrawGameText(LPDIRECT3DDEVICE9 _pDevice, const tstring& _Text, Vec2 _Pos, float _FontMag = 1.0f, DWORD Color = D3DCOLOR_XRGB(0, 0, 0));
	void DrawGameText(LPDIRECT3DDEVICE9 _pDevice, const tstring& _Text, POINT _LeftPos, float _FontMag = 1.0f, DWORD Color = D3DCOLOR_XRGB(0, 0, 0));
	void DrawGameText_RightStart(LPDIRECT3DDEVICE9 _pDevice, const tstring& _Text, POINT _LeftPos, float _FontMag = 1.0f, DWORD Color = D3DCOLOR_XRGB(0, 0, 0));

	const Vec2 GetFontSize();
public:
	CGameFont(LPD3DXFONT _pFont,Vec2 _FontSize);
	~CGameFont();
};

