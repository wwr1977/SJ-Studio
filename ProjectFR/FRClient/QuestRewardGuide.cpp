#include "Precom.h"
#include "QuestRewardGuide.h"
#include "GameMaster.h"
#include <GameSprite.h>

Vec2	CQuestRewardGuide::RewardIconSize = Vec2{40.f,40.f};
CQuestRewardGuide::CQuestRewardGuide()
	:m_GuideBack(nullptr), m_IconRen(nullptr)
	, m_ResultFont(nullptr), m_fAccTime(0.f)
{
}


CQuestRewardGuide::~CQuestRewardGuide()
{
}
void CQuestRewardGuide::Init()
{
	m_GuideBack = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_UIBACK, 0.65f, true));
	m_GuideBack->SetSprite(_T("ColorBase"));
	m_GuideBack->SetMaterial(D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.f));
	m_GuideBack->MaterialOn();
	m_GuideBack->SetCustomSize(Vec2{ 800.f,40.f });

	m_ResultFont = ACTOR->CreateCom<CFontRenderer>(RENDATA(LAYER_UIBACK, 0.65f, true));
	m_ResultFont->SetFontStyle(_T("좀큰볼드굴림"));

	m_IconRen = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_UI, 0.95f, true));
	m_IconRen->SetCustomSize(RewardIconSize);
	
	ACTOR->Off();
}
void CQuestRewardGuide::Update()
{
	if (m_fAccTime >= 4.5f)
	{
		ACTOR->Off();
		//SetAlpha(1.f);
		return;
	}

	float w = powf( m_fAccTime , 3.f);
	float Alpha = 1.f - w;

	//SetAlpha(Alpha);

	m_fAccTime += DELTATIME;
	
}
void CQuestRewardGuide::StandBy()
{
	m_fAccTime = 0.f;
	ACTOR->On();
}
void CQuestRewardGuide::SetGuideMode(const CharacterIndex& _CharNo)
{
	if (_CharNo >= MAXBATTLECHARACTER)
		return;

	UNITNAME Name = CClientGlobal::CharacterName[_CharNo];
	
	m_IconRen->SetSprite(Icon(Name.UnitName));
	m_ResultFont->SetString(_T("『") + Name.UnitKName + _T("』가 동료가 되었습니다."));

	float Len = m_ResultFont->GetFontXLength() + RewardIconSize.x ;
	m_GuideBack->SetCustomSize(Vec2{ Len +100.f ,50.f });
	m_IconRen->SetRenPivot(Vec2{ Len*-0.5f ,0.f });
	m_ResultFont->SetMiddleStartPivot(Vec3{ 0.f ,-7.f,7.f });
}
void CQuestRewardGuide::SetGuideMode(const CHARACTERLINE& _LineNo)
{
	if (_LineNo >= MAXCHARACTERLINE)
		return;

	CharacterIndex InitChar = (CharacterIndex)CClientGlobal::CharacterLine[_LineNo][0].CharacterNo;

	return SetGuideMode(InitChar);
}
void CQuestRewardGuide::SetAlpha(const float& _Alpha)
{
	m_GuideBack->SetAlpha(0.65f*_Alpha);
	m_IconRen->SetAlpha(0.95f*_Alpha);
	m_ResultFont->SetAlpha(0.65f*_Alpha);
}