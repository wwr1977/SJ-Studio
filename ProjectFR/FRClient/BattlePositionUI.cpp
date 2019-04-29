#include "Precom.h"
#include "BattlePositionUI.h"
#include "StdGauge.h"
#include <SceneBuilder.h>



CBattlePositionUI::CBattlePositionUI()
	:m_CharacterNo((CharacterIndex)-1), m_CharacterKName(_T(""))
	, m_CharNameRen(nullptr), m_CharLevelRen(nullptr)
	, m_CharacterLine(-1), m_LineIndex(-1)
	, m_AniIllustRen(nullptr)
	, m_SelectRen(nullptr)
{
}


CBattlePositionUI::~CBattlePositionUI()
{
	m_StatusGauge.clear();
}
void CBattlePositionUI::Init() 
{
	ACTOR->SetImageScale(2.0f);

	m_SelectRen = ACTOR->CreateCom<CAniRenderer>(RENDATA(LAYER_UI, 0.9f, { 0.0f,0.0f,0.0f },true));
	m_AniIllustRen = ACTOR->CreateCom<CAniRenderer>(RENDATA(LAYER_UI, 0.8f, true));
	m_CharNameRen = ACTOR->CreateCom<CFontRenderer>(RENDATA(LAYER_UI, 0.8f, true));
	m_CharLevelRen = ACTOR->CreateCom<CFontRenderer>(RENDATA(LAYER_UI, 0.8f, true));
;
	m_SelectRen->CreateAnimation(_T("SelectCircle2"), _T("SelectCircle2"), ANI_LOOP);
	m_SelectRen->ChangeAni(_T("SelectCircle2"));
	m_SelectRen->Off();

	m_CharNameRen->SetFontStyle(_T("MP회복폰트"));
	m_CharNameRen->SetFontMag(2.0f);
	m_CharNameRen->SetLeftStartPivot(POINT{ -55,168 });

	m_CharLevelRen->SetFontStyle(_T("MP회복폰트"));
	m_CharLevelRen->SetFontColor(D3DCOLOR_ARGB(200,200,150,20));
	m_CharLevelRen->SetFontMag(2.0f);

	m_CharLevelRen->SetLeftStartPivot(POINT{ 95,168 });
	
	/*
	//m_IllustRen->SetRenPivot(Vec2{-75.f,0.f});
	m_CharNameRen->SetRenPivot(Vec2{});
	*/

	m_StatusGauge.push_back(CActObject::CreateActObject(_T("HP Gauge"), ACTOR->GetScene())->CreateCom<CStdGauge>());
	m_StatusGauge.push_back(CActObject::CreateActObject(_T("MP Gauge"), ACTOR->GetScene())->CreateCom<CStdGauge>());
	m_StatusGauge.push_back(CActObject::CreateActObject(_T("XP Gauge"), ACTOR->GetScene())->CreateCom<CStdGauge>());

	for (size_t i = 0; i < m_StatusGauge.size(); i++) 
	{
		m_StatusGauge[i]->SetFrameSize(Vec2{ 150.f, 25.f });
		m_StatusGauge[i]->CreateGaugeFont(_T("UI레벨폰트"), POINT{ -20,-3 }, LEFTSTART);
		m_StatusGauge[i]->CreateGaugeFont(_T("XP"), POINT{ -65,7 }, LEFTSTART);
		m_StatusGauge[i]->SetGaugeFont(0);
		m_StatusGauge[i]->SetFontColor(1 , D3DCOLOR_ARGB(255, 200, 200, 200));
		m_StatusGauge[i]->SetGaugeSize(Vec2{ 142.f,19.f });
	}

	m_StatusGauge[GAUGE_HP]->SetGaugeFrame(_T("HPFrame"));
	m_StatusGauge[GAUGE_HP]->SetGaugeColor(D3DXCOLOR(0.2f, 1.f, 0.2f, 1.0f));
	m_StatusGauge[GAUGE_HP]->SetFontString(1,_T("HP "));

	m_StatusGauge[GAUGE_MP]->SetGaugeFrame(_T("MPFrame"));
	m_StatusGauge[GAUGE_MP]->SetGaugeColor(D3DXCOLOR(0.2f, 0.2f, 1.f, 1.0f));
	m_StatusGauge[GAUGE_MP]->SetFontString(1, _T("MP "));

	m_StatusGauge[GAUGE_XP]->SetGaugeFrame(_T("XPFrame"));
	m_StatusGauge[GAUGE_XP]->SetGaugeColor(D3DXCOLOR(0.35f, 0.2f, 0.56f, 1.0f));
	m_StatusGauge[GAUGE_XP]->SetFontString(1, _T("XP "));

}
void CBattlePositionUI::InitData(const CharacterIndex& _CharNo)
{
	if (_CharNo >= CClientGlobal::CharacterName.size())
		return;

	m_CharacterNo = _CharNo;
	m_CharacterLine = CClientGlobal::FindCharacterLineNo(m_CharacterNo);
	m_LineIndex = CClientGlobal::FindLineIndex(m_CharacterNo);

	m_AniIllustRen->CreateAnimation(Ani_Idle(CClientGlobal::CharacterName[_CharNo].UnitName));
	m_AniIllustRen->ChangeAni(Idle(CClientGlobal::CharacterName[_CharNo].UnitName));
	m_AniIllustRen->SetRenPivot( Vec2{ 0.f, 13.f});
	m_SelectRen->SetRenPivot( Vec2{ 10.f, 50.f });
	
	m_CharacterKName = CClientGlobal::CharacterName[_CharNo].UnitKName;

	m_CharNameRen->SetString(m_CharacterKName);
	
}
void CBattlePositionUI::DebugRender()
{
	
	GetComDebugMgr()->RectRender(D3DCOLOR_ARGB(255, 255, 255, 255), TRANS->GetPos(), { 1000.0f,0.0f,2.0f });
}
void CBattlePositionUI::UIOn()
{
	ACTOR->On();
	TCHAR LvStr[MAXKEYLENGTH] = _T("");

	if (m_CharacterLine < CClientGlobal::CharacterLine.size() && m_LineIndex < CClientGlobal::CharacterLine[m_CharacterLine].size()) 
	{
		CHARACTERDATA CharData = CClientGlobal::CharacterLine[m_CharacterLine][m_LineIndex];
		_stprintf_s(LvStr, _T("Lv  %3d"),(int)CharData.CharacterLv);

		m_StatusGauge[GAUGE_HP]->SetMaxGauge((int)CharData.MaxHp);
		m_StatusGauge[GAUGE_HP]->SetCurGauge((int)CharData.CurHp);

		m_StatusGauge[GAUGE_MP]->SetMaxGauge((int)CharData.MaxMp);
		m_StatusGauge[GAUGE_MP]->SetCurGauge((int)CharData.CurMp);

		m_StatusGauge[GAUGE_XP]->SetMaxGauge((int)CharData.MaxExp);
		m_StatusGauge[GAUGE_XP]->SetCurGauge((int)CharData.CurExp);
	}


	m_CharLevelRen->SetString(LvStr);
}
void CBattlePositionUI::UIOn(const Vec3& _OnPos)
{
	TRANS->SetPos(_OnPos);
	UIOn();

	for (size_t i = 0; i < m_StatusGauge.size(); i++)
	{
		if (nullptr == m_StatusGauge[i])
			continue;
	
		m_StatusGauge[i]->GaugeOn();
	}

	m_StatusGauge[GAUGE_HP]->SetPos(_OnPos + Vec3{ 165.f,108.f,0.f});
	m_StatusGauge[GAUGE_MP]->SetPos(_OnPos + Vec3{ 165.f,68.f,0.f });
	m_StatusGauge[GAUGE_XP]->SetPos(_OnPos + Vec3{ 165.f,28.f,0.f });

}
void CBattlePositionUI::UIOff() 
{
	ACTOR->Off();
	for (size_t i = 0; i < m_StatusGauge.size(); i++)
	{
		if (nullptr == m_StatusGauge[i])
			continue;

		m_StatusGauge[i]->GaugeOff();
	}
}
void CBattlePositionUI::CharNameOn()
{
	m_CharNameRen->On();
}
void CBattlePositionUI::CharNameOff()
{
	m_CharNameRen->Off();
}
void CBattlePositionUI::SelectUIOn()
{
	m_SelectRen->On();
}
void CBattlePositionUI::SelectUIOff()
{
	m_SelectRen->Off();
}
void CBattlePositionUI::ChangeRenderingVersion(const UIRENDERINGVERSION& _Ver)
{
	switch (_Ver)
	{
	case ROOTMENEVER: 
	{
		ACTOR->SetImageScale(2.0f);
		m_CharNameRen->On();
		m_CharLevelRen->On();
		for (size_t i = 0; i < m_StatusGauge.size(); i++)
			m_StatusGauge[i]->GaugeOn();
	}
		break;
	case BATTLEPOSITIONVER: 
	{
		ACTOR->SetImageScale(1.5f);
		m_CharNameRen->Off();
		m_CharLevelRen->Off();
		for (size_t i = 0; i < m_StatusGauge.size(); i++)
			m_StatusGauge[i]->GaugeOff();
	}
		break;

	}
}