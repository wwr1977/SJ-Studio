#pragma once
#include<Logic.h>
class CMenuTab : public CLogic
{
protected:
	bool							m_bSelect;
	SPTR<CFontRenderer>				m_TabFontRen;
	SPTR<CFixRenderer>				m_TabSelectRen;
	vector<SPTR<CFontRenderer>>		m_SubTabFontRen;


public:
	void Init();
	void Update();
	

public:
	const size_t CreateSubTabFont(const tstring& _Style
		, const Vec3& _MidPivot = Vec3{ 0.f,0.f,0.f }
		, const tstring _String = _T("")
		, const float _FontMag = 1.3f);

	const size_t CreateSubTabFont(const tstring& _Style, const POINT& _StartPivot
		, const FONTPIVOTOPTION& _StartOp = LEFTSTART
		, const tstring& _String = _T("")
		, const float& _FontMag = 1.3f);

public:
	void SetPos(const Vec3& _Pos);
	void SetPivot(const Vec3& _Pivot);
	void SetFontPivot(const Vec3& _Pivot);
	void SetPivot(const POINT& _Pivot,const FONTPIVOTOPTION& _Op = LEFTSTART);
	void SetTabName(const tstring& _TabName);
	void SetTabFontColor(const DWORD& _Color);
	void SetTabStyle(const tstring& _FontStyle);
	void SetSelectColor(const D3DXCOLOR& _Color);
	void SetTabSize(const Vec2& _TabSize);
	void SelectTab(const bool& _Select);
	void SetSubFontString(const size_t& _SubIndex, const tstring& _String);
	void SetSubFontStyle(const size_t& _SubIndex, const tstring& _Style);
	void SetSubFontColor(const size_t& _SubIndex, const DWORD& _Color);

public:
	void MenuTabOn();
	void MenuTabOff();
	void TabActorOn();
	void TabActorOff();

public:
	CMenuTab();
	~CMenuTab();
};

