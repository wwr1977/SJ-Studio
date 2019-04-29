#include "stdafx.h"
#include "EditDemageFont.h"
#include "SkillMgr.h"

int	CEditDemageFont::DemageFontCount = 0;

CEditDemageFont::CEditDemageFont()
	: m_fUpSpeed(130.0f)
	, m_fAccTime(0.0f)
	, m_iRenderIndex(0)
	, m_pSkillMgr(nullptr)
	, m_fRenderStartTime(0.0f)
	, m_FontStyle(0), m_Option(0)
	, m_fExpUpdateTime(1.0f)
	, m_fStairUpdateTime(2.5f)
	, m_MiddlePos(Vec2{0.0f,0.0f})
	, m_fMaxFontMag(1.5f)
	, m_fFontMag(1.0f)
{
}


CEditDemageFont::~CEditDemageFont()
{
}

bool CEditDemageFont::RenderOrder(SPTR<CEditDemageFont> _Left, SPTR<CEditDemageFont> _Right)
{
	int Left = _Left->GetRenderIndex();
	int Right = _Right->GetRenderIndex();
	return Left < Right;
}

void CEditDemageFont::Init()
{
	m_iRenderIndex = DemageFontCount++;
}
void CEditDemageFont::PreUpdate()
{
	if (m_fAccTime >= m_fRenderStartTime) 
	{
		CEditNumberUI::OnNumberUI();
	}
}
void CEditDemageFont::Update()
{	
	CEditNumberUI::Update();

	switch (m_FontStyle)
	{
	case DEMAGESTYLE_EXPANSION:
		ExpansionUpdate();
		break;
	case DEMAGESTYLE_STAIRS:
		StairsUpdate();
		break;
	}

	/*if (m_fAccTime >= 3.0f) 
	{
		m_pTurnMgr->ReturnDemageFont(this);
		m_fRenderStartTime = 0.0f;
		Off();
		return;
	}

	if (m_fAccTime >= 2.5f) 
	{
		float Alpha = (3.0f - m_fAccTime) / 0.5f;
		SetAlpha(Alpha);
	}

	TRANS->Move(GAMEVEC::UP * DELTATIME * m_fUpSpeed);*/

	m_fAccTime += DELTATIME;
}
const int CEditDemageFont::GetRenderIndex()
{
	return m_iRenderIndex;
}
void CEditDemageFont::SetSkillMgr(CSkillMgr* _TurnMgr)
{
	if (nullptr == _TurnMgr) 
	{
		assert(nullptr);
		return;
	}

	m_pSkillMgr = _TurnMgr;
}
void CEditDemageFont::On(const int& _Number, const Vec2& _Pos)
{
	ACTOR->On();
	SetNumber(_Number);
	CEditNumberUI::SetPos(_Pos);
	m_fAccTime = 0.0f;
}
void CEditDemageFont::On(const int& _Number, const Vec2& _Pos, const tstring& _SpriteName)
{
	ACTOR->On();
	CEditNumberUI::SetPos(_Pos);
	CEditNumberUI::ChangeNumberSprite(_SpriteName);
	SetNumber(_Number);
	m_fAccTime = 0.0f;
}
void CEditDemageFont::On(const int& _Number, const Vec2& _Pos, const size_t& _DemageFontIndex)
{
	ACTOR->On();
	CEditNumberUI::SetPos(_Pos);
	CEditNumberUI::ChangeNumberSprite(CEditGlobal::GetDemageFont(_DemageFontIndex).GetString());
	SetNumber(_Number);
	m_fAccTime = 0.0f;
		
}
void CEditDemageFont::ActorOn()
{
	ACTOR->On();
	m_fAccTime = 0.0f;
}
void CEditDemageFont::MiddleOn(const int& _Number, const Vec2& _Pos, const size_t& _DemageFontIndex)
{
	ACTOR->On();
	CEditNumberUI::ChangeNumberSprite(CEditGlobal::GetDemageFont(_DemageFontIndex).GetString());
	SetNumber(_Number);
	CEditNumberUI::SetMiddlePos(_Pos);
	m_MiddlePos = _Pos;
	m_fAccTime = 0.0f;
}
void CEditDemageFont::Off()
{
	SetAlpha(1.0f);
	ACTOR->Off();
	m_fAccTime = 0.0f;
}
void CEditDemageFont::SetRenderTimer(const float& _RenderTimer)
{
	m_fRenderStartTime = _RenderTimer;
}
void CEditDemageFont::ExpansionUpdate()
{
	if (m_fAccTime >= m_fExpUpdateTime) 
	{
		m_pSkillMgr->ReturnDemageFont(this);
		m_fRenderStartTime = 0.0f;
		Off();
		return;
	}

	float w = m_fAccTime / m_fExpUpdateTime;
	float MagRate = powf(w, 4.0f);

	float ActMag = (1.0f - MagRate)*m_fFontMag + MagRate * m_fMaxFontMag ;
	ACTOR->SetImageScale(ActMag);
	SyncNumberSpacing();
	
	float Alpha = 1.0f - powf(w,2.0f);
	SetAlpha(Alpha);

	/*SetNumber(m_iCurNumber);
	CNumberUI::SetMiddlePos(m_MiddlePos);*/


}
void CEditDemageFont::StairsUpdate()
{
	if (m_fAccTime >= m_fStairUpdateTime)
	{
		m_pSkillMgr->ReturnDemageFont(this);
		m_fRenderStartTime = 0.0f;
		Off();
		return;
	}

	if (m_fAccTime >= 2.0f)
	{
		float Alpha = (m_fStairUpdateTime - m_fAccTime) / 0.5f;
		SetAlpha(Alpha);
	}

	TRANS->Move(GAMEVEC::UP * DELTATIME * m_fUpSpeed);
}
void CEditDemageFont::SetExpStyle()
{
	m_FontStyle = DEMAGESTYLE_EXPANSION;
	map<tstring, FontSizeData >::iterator Find = m_mapFontSizeData.find(m_CurFontName);

	if (Find != m_mapFontSizeData.end())
	{
		m_fFontMag = Find->second.ImageMag;
		m_fMaxFontMag = m_fFontMag * 1.4f;
	}
}
void CEditDemageFont::SetStairStyle(const int& _Option)
{
	m_FontStyle = DEMAGESTYLE_STAIRS;
	m_Option = _Option;
	FontRenderOff();
}

bool CEditDemageFont::operator>(SPTR<CEditDemageFont> _Other)
{
	if (nullptr == _Other) 
	{
		assert(nullptr);
		return false;
	}
	return this->GetRenderIndex() > _Other->GetRenderIndex();
}
bool CEditDemageFont::operator>=(SPTR<CEditDemageFont> _Other)
{

	if (nullptr == _Other)
	{
		assert(nullptr);
		return false;
	}
	return this->GetRenderIndex() >= _Other->GetRenderIndex();
}
bool CEditDemageFont::operator<(SPTR<CEditDemageFont> _Other)
{
	if (nullptr == _Other)
	{
		assert(nullptr);
		return false;
	}
	return this->GetRenderIndex() < _Other->GetRenderIndex();
}
bool CEditDemageFont::operator<=(SPTR<CEditDemageFont> _Other)
{
	if (nullptr == _Other)
	{
		assert(nullptr);
		return false;
	}
	return this->GetRenderIndex() <= _Other->GetRenderIndex();
}