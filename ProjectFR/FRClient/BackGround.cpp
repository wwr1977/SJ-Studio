#include "Precom.h"
#include "BackGround.h"


CBackGround::CBackGround()
	:m_BackRenderer(nullptr),m_CurBackIndex(0)
	, m_BGMSpeeker(nullptr)
{
}
CBackGround::~CBackGround()
{
}
void CBackGround::Init()
{
	m_BackRenderer = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_BACKGROUND, 0.9f));
	m_BackRenderer->SetSprite(_T("BattleBack0"));
	m_BackRenderer->SetRenPivot(Vec2{ 0.f,0.f });

	TRANS->SetPos({ 0.0f,0.0f,700.0f });
	
	m_BGMSpeeker = ACTOR->CreateCom<CSoundPlayer>();
	m_BGMSpeeker->SetVolume(0.7f);
	WindowSizeSynchro();
}
void CBackGround::Update()
{
	if (TRUE == KEYDOWN("BattlePrev")) 
	{
		ChangePrevEvent();
		WindowSizeSynchro();
		return;
	}
	if (TRUE == KEYDOWN("BattleNext"))
	{
		ChangeNextEvent();
		WindowSizeSynchro();
		return;
	}
}
BOOL CBackGround::WindowSizeSynchro()
{
	m_BackRenderer->SetCustomSize(Vec2(1570.f,1000.f));
	//TRANS->SetPos(Vec3(0.0f, -50.0f, 50.0f));

	return TRUE;
}
void CBackGround::ChangePrevEvent()
{
	--m_CurBackIndex;

	if (m_CurBackIndex < 0) 
	{
		m_CurBackIndex = 12;
	}

	TCHAR ChangeBackName[128] = _T("");

	_stprintf_s(ChangeBackName, _T("BattleBack%d"), m_CurBackIndex);

	m_BackRenderer->SetSprite(ChangeBackName);
}
void CBackGround::ChangeBackGround(const UINT& _Index)
{
	if (m_CurBackIndex >= MAXBACKGROUND)
		return;

	m_CurBackIndex = _Index;

	TCHAR ChangeBackName[128] = _T("");

	_stprintf_s(ChangeBackName, _T("BattleBack%d"), m_CurBackIndex);

	m_BackRenderer->SetSprite(ChangeBackName);
}
void CBackGround::ChangeNextEvent()
{
	++m_CurBackIndex;

	if (m_CurBackIndex >= 13)
	{
		m_CurBackIndex = 0;
	}

	TCHAR ChangeBackName[128] = _T("");

	_stprintf_s(ChangeBackName, _T("BattleBack%d"), m_CurBackIndex);

	m_BackRenderer->SetSprite(ChangeBackName);
}
void CBackGround::SetVolume(const float& _Volume)
{
	if (_Volume <= 0.f) 
		m_BGMSpeeker->SetVolume(0.f);
	else 
		m_BGMSpeeker->SetVolume(0.7f * min(_Volume,1.f));
}
void CBackGround::PlayBgm(const tstring& _BgmName) 
{
	m_BGMSpeeker->PlaySound(_BgmName, FMOD_LOOP_NORMAL,-1);
	m_BGMSpeeker->SetVolume(0.7f);
}
void CBackGround::BgmOff() 
{
	m_BGMSpeeker->CurPlaySoundStop();
}
void CBackGround::BgmPause(const bool& _Pause)
{
	m_BGMSpeeker->CurPlaySoundPaused(_Pause);
}