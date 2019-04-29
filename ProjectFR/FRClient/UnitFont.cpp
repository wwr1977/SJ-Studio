#include "Precom.h"
#include "UnitFont.h"
#include "BattleUnit.h"


CUnitFont::CUnitFont()
	:m_PairUnit(nullptr), m_fAccTime(0.0f)
	, m_EndPos(Vec3{0.0f,0.0f,0.0f})
	, m_StartPos(Vec3{0.0f,0.0f,0.0f})
{
}


CUnitFont::~CUnitFont()
{
	m_PairUnit = nullptr;
}
void CUnitFont::InitData(void* _Unit)
{
	m_PairUnit = (CBattleUnit*)_Unit;
	
	if (nullptr == m_PairUnit)
		return;

	m_UnitFontRen = ACTOR->CreateCom<CFontRenderer>(RENDATA(LAYER_UI, 1.0f,true));
	m_UnitFontRen->SetMiddleStartPivot(Vec3{ 0.0f,0.0f,0.0f });
	m_UnitFontRen->SetFontStyle(_T("MP회복폰트"));

}
void CUnitFont::Update() 
{
	if (1.0f <= m_fAccTime) 
	{
		FontOff();
		return;
	}
	

	TRANS->SetPos(CSJMath::Vec3Lerp(&m_StartPos, &m_EndPos, m_fAccTime / 1.0f));
	m_fAccTime += DELTATIME;
}

void CUnitFont::SetFontColor(const DWORD& _Color)
{
	if (nullptr == m_UnitFontRen)
		return;

	m_UnitFontRen->SetFontColor(_Color);
}
void CUnitFont::SetFontString(const tstring& _Str) 
{
	if (nullptr == m_UnitFontRen)
		return;

	m_UnitFontRen->SetString(_Str);
}
void CUnitFont::SetFontStyle(const tstring& _Style)
{
	if (nullptr == m_UnitFontRen)
		return;

	m_UnitFontRen->SetFontStyle(_Style);
}
void CUnitFont::FontOn(const Vec3& _StartPos)
{
	m_StartPos = _StartPos;
	m_EndPos = m_StartPos + Vec3{ 0.0f,150.0f,0.0f };
	ACTOR->On();
}
void CUnitFont::FontOff() 
{
	m_fAccTime = 0.0f;
	ACTOR->Off();
}