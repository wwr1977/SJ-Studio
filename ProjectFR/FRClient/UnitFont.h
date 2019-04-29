#pragma once
#include <Logic.h>

class CBattleUnit;
class CUnitFont : public CLogic
{
private:
	CBattleUnit*					m_PairUnit;
	SPTR<CFontRenderer>				m_UnitFontRen;
	
	float							m_fAccTime;
	Vec3							m_StartPos;
	Vec3							m_EndPos;
public:
	void InitData(void* _Unit);
	void Update();
	
	void SetFontColor(const DWORD& _Color);
	void SetFontString(const tstring& _Str);
	void SetFontStyle(const tstring& _Style);
	
	void FontOn(const Vec3& _StartPos);
	void FontOff();
public:
	CUnitFont();
	virtual ~CUnitFont();
};

