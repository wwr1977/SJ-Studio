#include "Precom.h"
#include "DemageFont.h"
#include "TurnMgr.h"

int	CDemageFont::DemageFontCount = 0;

CDemageFont::CDemageFont()
	: m_fUpSpeed(130.0f)
	, m_fAccTime(0.0f)
	, m_iRenderIndex(0)
	, m_pTurnMgr(nullptr)
	, m_fRenderStartTime(0.0f)
	, m_FontStyle(0), m_Option(0)
	, m_fExpUpdateTime(1.0f)
	, m_fStairUpdateTime(2.5f)
	, m_MiddlePos(Vec2{0.0f,0.0f})
	, m_fMaxFontMag(1.5f)
	, m_fFontMag(1.0f)
{
}


CDemageFont::~CDemageFont()
{
}

bool CDemageFont::RenderOrder(SPTR<CDemageFont> _Left, SPTR<CDemageFont> _Right)
{
	int Left = _Left->GetRenderIndex();
	int Right = _Right->GetRenderIndex();
	return Left < Right;
}

void CDemageFont::Init()
{
	m_iRenderIndex = DemageFontCount++;
}
void CDemageFont::PreUpdate()
{
	if (m_fAccTime >= m_fRenderStartTime) 
	{
		CNumberUI::OnNumberUI();
	}
}
void CDemageFont::Update()
{	
	CNumberUI::Update();

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
const int CDemageFont::GetRenderIndex()
{
	return m_iRenderIndex;
}
void CDemageFont::SetTurnMgr(CTurnMgr* _TurnMgr)
{
	if (nullptr == _TurnMgr) 
	{
		assert(nullptr);
		return;
	}

	m_pTurnMgr = _TurnMgr;
}
void CDemageFont::On(const int& _Number, const Vec2& _Pos)
{
	ACTOR->On();
	SetNumber(_Number);
	CNumberUI::SetPos(_Pos);
	m_fAccTime = 0.0f;
}
void CDemageFont::On(const int& _Number, const Vec2& _Pos, const tstring& _SpriteName)
{
	ACTOR->On();
	CNumberUI::SetPos(_Pos);
	CNumberUI::ChangeNumberSprite(_SpriteName);
	SetNumber(_Number);
	m_fAccTime = 0.0f;
}
void CDemageFont::On(const int& _Number, const Vec2& _Pos, const size_t& _DemageFontIndex)
{
	ACTOR->On();
	CNumberUI::SetPos(_Pos);
	CNumberUI::ChangeNumberSprite(CClientGlobal::GetDemageFont(_DemageFontIndex));
	SetNumber(_Number);
	m_fAccTime = 0.0f;
		
}
void CDemageFont::ActorOn()
{
	ACTOR->On();
	m_fAccTime = 0.0f;
}
void CDemageFont::MiddleOn(const int& _Number, const Vec2& _Pos, const size_t& _DemageFontIndex)
{
	ACTOR->On();
	CNumberUI::ChangeNumberSprite(CClientGlobal::GetDemageFont(_DemageFontIndex));
	SetNumber(_Number);
	CNumberUI::SetMiddlePos(_Pos);
	m_MiddlePos = _Pos;
	m_fAccTime = 0.0f;
}
void CDemageFont::Off()
{
	SetAlpha(1.0f);
	ACTOR->Off();
	m_fAccTime = 0.0f;
}
void CDemageFont::SetRenderTimer(const float& _RenderTimer)
{
	m_fRenderStartTime = _RenderTimer;
}
void CDemageFont::ExpansionUpdate() 
{
	if (m_fAccTime >= m_fExpUpdateTime) 
	{
		m_pTurnMgr->ReturnDemageFont(this);
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
void CDemageFont::StairsUpdate() 
{
	if (m_fAccTime >= m_fStairUpdateTime)
	{
		m_pTurnMgr->ReturnDemageFont(this);
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
void CDemageFont::SetExpStyle()
{
	m_FontStyle = DEMAGESTYLE_EXPANSION;
	map<tstring, FontSizeData >::iterator Find = m_mapFontSizeData.find(m_CurFontName);

	if (Find != m_mapFontSizeData.end())
	{
		m_fFontMag = Find->second.ImageMag;
		m_fMaxFontMag = m_fFontMag * 1.4f;
	}
}
void CDemageFont::SetStairStyle(const int& _Option)
{
	m_FontStyle = DEMAGESTYLE_STAIRS;
	m_Option = _Option;
	FontRenderOff();
}

bool CDemageFont::operator>(SPTR<CDemageFont> _Other) 
{
	if (nullptr == _Other) 
	{
		assert(nullptr);
		return false;
	}
	return this->GetRenderIndex() > _Other->GetRenderIndex();
}
bool CDemageFont::operator>=(SPTR<CDemageFont> _Other) 
{

	if (nullptr == _Other)
	{
		assert(nullptr);
		return false;
	}
	return this->GetRenderIndex() >= _Other->GetRenderIndex();
}
bool CDemageFont::operator<(SPTR<CDemageFont> _Other) 
{
	if (nullptr == _Other)
	{
		assert(nullptr);
		return false;
	}
	return this->GetRenderIndex() < _Other->GetRenderIndex();
}
bool CDemageFont::operator<=(SPTR<CDemageFont> _Other) 
{
	if (nullptr == _Other)
	{
		assert(nullptr);
		return false;
	}
	return this->GetRenderIndex() <= _Other->GetRenderIndex();
}