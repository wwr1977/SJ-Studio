#pragma once
#include "Renderer.h"
class CFontRenderer :public CRenderer
{
public:
	enum STARTPOS
	{
		START_LEFT,
		START_MIDDLE,
		START_RIGHT,
		MAXSTARTPOS
	} m_CurMode;

private:
	tstring					m_RenderString;

protected:
	tstring					m_FontName;
	Vec2					m_FontSize;
	POINT					m_LeftStartPivot;
	Vec3					m_MiddleStartPivot;
	float					m_fFontMag;
	DWORD					m_FontColor;



public:
	void SetFontStyle(const tstring& _FontStyle);
	void SetString(const tstring& _String);
	void SetLeftStartPivot(const POINT& _StartPos);
	void SetMiddleStartPivot(const Vec3& _MiddlePos);
	void SetRightStartPivot(const POINT& _StartPos);
	void SetFontMag(const float& _FontMag);
	void SetFontColor(const DWORD& _FontColor);
	void SetFontRenderMode(const STARTPOS& _Mode);
	void SetFontRenderMode(const size_t& _Mode);

public:
	const tstring GetFontString();
	const float GetFontXLength();


public:
	virtual void Init();
	virtual void Render();


public:
	CFontRenderer();
	~CFontRenderer();
};

