#include "Precom.h"
#include "TargetUI.h"
#include "BattleUnit.h"


Vec2	CTargetUI::FixSize = Vec2{200.0f,200.0f};


CTargetUI::CTargetUI()
	:m_PairUnit(nullptr), m_UpRenderer(nullptr)
	, m_DownRenderer(nullptr), m_fAccTime(0.0f)
	, m_ScaleSign(1.0f), m_fScaleTime(1.5f)
	, m_ScaleRatio(Vec2{1.0f,1.2f})
	, m_UpSize(Vec2{ 0.0f,0.0f }), m_DownSize(Vec2{0.0f,0.0f})
	, m_NewEffectRen(nullptr)
{

}


CTargetUI::~CTargetUI()
{
}
void CTargetUI::Init()
{
	/*m_UpRenderer = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_MONBACK, 0.6f, true));
	m_DownRenderer = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_MONBACK, 0.6f, true));

	m_UpRenderer->SetSprite(_T("UpTargetCircle"));
	m_DownRenderer->SetSprite(_T("DownTargetCircle"));*/

	ACTOR->SetImageScale(2.5f);
	m_NewEffectRen = ACTOR->CreateCom<CAniRenderer>(RENDATA(LAYER_BACKEFFECT, 0.9f, { 0.0f,0.0f,0.0f }));
	m_NewEffectRen->CreateAnimation(_T("TargetSelect"), _T("TargetSelect"), ANI_LOOP);
	m_NewEffectRen->ChangeAni(_T("TargetSelect"));
	m_NewEffectRen->SetRenPivot(Vec2{ 10.f, 50.f });
}
void CTargetUI::Update()
{
	//float Weight = m_fAccTime / m_fScaleTime;

	//float Scale = m_ScaleRatio.x * (1.0f - Weight) + m_ScaleRatio.y *Weight;
	//
	//m_UpRenderer->SetCustomSize(Scale* m_UpSize);
	//m_UpRenderer->SetRenPivot(Vec2(0.0f, Scale* m_UpSize.y*0.5f));
	//
	//m_DownRenderer->SetCustomSize(Scale * m_DownSize);
	//m_DownRenderer->SetRenPivot(Vec2{ 0.0f, Scale* m_DownSize.y*-0.5f });

	//m_fAccTime += DELTATIME*m_ScaleSign;

	//if (m_fAccTime > m_fScaleTime)
	//{
	//	m_fAccTime = m_fScaleTime;
	//	m_ScaleSign = -m_ScaleSign;
	//}
	//else if (m_fAccTime < 0.0f)
	//{
	//	m_fAccTime = 0.0f;
	//	m_ScaleSign = -m_ScaleSign;
	//}
}
void CTargetUI::SetUnit(CBattleUnit* _Unit)
{
	if (nullptr == _Unit)
		return;

	m_PairUnit = _Unit;
}
void CTargetUI::SetPos(const Vec2& _Pos)
{
	TRANS->SetPos(_Pos);
}
void CTargetUI::SetPos(const Vec3& _Pos)
{
	TRANS->SetPos(_Pos);
}
void CTargetUI::SetSize(const Vec2& _Size)
{
	m_UISize = FixSize;

	m_UpSize = Vec2{ FixSize.x, FixSize.y * 0.25f };
	m_DownSize = Vec2{ FixSize.x, FixSize.y* 0.3f };

}
void CTargetUI::TargetUIOn()
{
	m_fAccTime = 0.0f;
	ACTOR->On();
	TRANS->SetPos(m_PairUnit->GetPos());
}
void CTargetUI::TargetUIOff()
{
	ACTOR->Off();
}
CBattleUnit* CTargetUI::GetPairUnit()
{
	return m_PairUnit;
}
