#include "Precom.h"
#include "BattleResultWindow.h"
#include "CharResultUI.h"
#include "ExpGauge.h"


CBattleResultWindow::CBattleResultWindow()
	:m_ResultWindowRen(nullptr), m_WindowState(STATE_NOACTION)
	, m_fAccTime(0.f), m_PopUpPivot(Vec3{0.f,-80.f,0.f})
	, m_bChangeScene(false), m_bBackRen(false)

{
	m_ResultCharUI.assign(MAXTEAM, nullptr);
}


CBattleResultWindow::~CBattleResultWindow()
{
	m_ResultCharUI.clear();
}
void CBattleResultWindow::Init() 
{
	POINT WndSize = CClientGlobal::MainWindow->WndSize();

	m_ResultCharUIPos.push_back(CClientGlobal::MainWindow->WndPosToWorldPos(0.375f*(float)WndSize.x, 0.625f*(float)WndSize.y));
	m_ResultCharUIPos.push_back(CClientGlobal::MainWindow->WndPosToWorldPos(0.625f* (float)WndSize.x, 0.625f*(float)WndSize.y));
	m_ResultCharUIPos.push_back(CClientGlobal::MainWindow->WndPosToWorldPos(0.25f* (float)WndSize.x, 0.375f*(float)WndSize.y));
	m_ResultCharUIPos.push_back(CClientGlobal::MainWindow->WndPosToWorldPos(0.5f* (float)WndSize.x, 0.375f*(float)WndSize.y));
	m_ResultCharUIPos.push_back(CClientGlobal::MainWindow->WndPosToWorldPos(0.75f* (float)WndSize.x, 0.375f*(float)WndSize.y));

	m_ResultWindowRen = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_UIBACK,0.65f));
	m_ResultWindowRen->SetSprite(_T("ColorBase"));
	m_ResultWindowRen->SetMaterial(D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f));
	m_ResultWindowRen->MaterialOn();
	m_ResultWindowRen->SetCustomSize(Vec2{ 1500.0f,1000.0f });
	m_ResultWindowRen->Off();
	ACTOR->Off();
}
void CBattleResultWindow::Update()
{
	switch (m_WindowState)
	{
	case CBattleResultWindow::STATE_WAIT:
		WaitUpdate();
		return;
	case CBattleResultWindow::STATE_POPUI:
		PopUIUpdate();
		return;
	case CBattleResultWindow::STATE_UIUPDATE_WAIT:
		UIUpdateWait();
		return;
	case CBattleResultWindow::STATE_RESULTWINDOW:
		ResultWindowUpdate();
		return;
	}
}
void CBattleResultWindow::WaitUpdate()
{
	if (m_fAccTime >= 6.f)
	{
		SetAlpha(1.f);
		m_WindowState = STATE_POPUI;
		m_fAccTime = 0.f;
		
		for (size_t i = 0; i < m_ResultCharUI.size(); i++)
		{
			if (nullptr == m_ResultCharUI[i])
				continue;

			m_ResultCharUI[i]->UIOn();
		}

		return;
	}

	float t = m_fAccTime / 6.0f;

	if (t >= 0.7f && t < 1.0f)
	{
		if (false == m_bBackRen)
		{
			m_bBackRen = true;
			m_ResultWindowRen->On();
		}

		SetAlpha(t);
	}

	m_fAccTime += DELTATIME;
}
void CBattleResultWindow::PopUIUpdate() 
{
	float t = m_fAccTime / 0.5f;

	Vec3 Start = Vec3{0.f,0.f,0.f};
	Vec3 End = Vec3{ 0.f,0.f,0.f };

	for (size_t i = 0; i < m_ResultCharUI.size(); i++)
	{
		if (nullptr == m_ResultCharUI[i])
			continue;


		Start = m_ResultCharUIPos[i] + m_PopUpPivot;
		End = m_ResultCharUIPos[i];
		m_ResultCharUI[i]->SetPos(CSJMath::Vec3Lerp(&Start, &End, t));
	}

	if (t >= 1.f) 
	{
		m_fAccTime = 0.f;
		m_WindowState = STATE_UIUPDATE_WAIT;
		CExpGauge::StartGaugeUpdate();
	}


	m_fAccTime += DELTATIME;
}
void CBattleResultWindow::UIUpdateWait()
{
	if (m_fAccTime >= 2.f)
	{
		m_WindowState = STATE_RESULTWINDOW;
		m_fAccTime = 0.f;
		
		return;
	}


	if (true == CheckUIUpdateEnd())
		m_fAccTime += DELTATIME;
}
void CBattleResultWindow::ResultWindowUpdate() 
{
	if (m_fAccTime >= 5.0f)
	{
		m_bChangeScene = true;
		m_WindowState = STATE_NOACTION;
		return;
	}

	if (true == KEYDOWN("Space")) 
	{
		m_bChangeScene = true;
		m_WindowState = STATE_NOACTION;
		return;
	}

	m_fAccTime += DELTATIME;
}
void CBattleResultWindow::SetAlpha(const float& _Alpha)
{
	m_ResultWindowRen->SetAlpha(_Alpha* 0.65f);
}
void CBattleResultWindow::PushResultUI(const UINT& _Index, SPTR<CCharResultUI> _UI)
{
	if (_Index >= MAXTEAM)
		return;

	m_ResultCharUI[_Index] = _UI;

}
void CBattleResultWindow::ResultWindowOn(UINT& _TotalExp, const UINT& _CharCount)
{
	ACTOR->On();

	m_bChangeScene = false;
	m_bBackRen = false;

	m_fAccTime = 0.f;
	m_WindowState = STATE_WAIT;
	
	if (0 == _CharCount)
		return;

	UINT AddExp = (UINT)ceilf((float)(_TotalExp / _CharCount));

	for (size_t i = 0; i < m_ResultCharUI.size(); i++)
	{
		if (nullptr == m_ResultCharUI[i])
			continue;

		m_ResultCharUI[i]->StandBy(AddExp);
	}

	_TotalExp = AddExp * _CharCount;
	
	m_ResultWindowRen->Off();
}
void CBattleResultWindow::ResultWindowOff() 
{
	ACTOR->Off();

	for (size_t i = 0; i < m_ResultCharUI.size(); i++)
	{
		if (nullptr == m_ResultCharUI[i])
			continue;

		m_ResultCharUI[i]->UIOff();
	}

	m_ResultCharUI.assign(MAXTEAM, nullptr);
}

const bool CBattleResultWindow::CheckUIUpdateEnd()
{
	for (size_t i = 0; i < m_ResultCharUI.size(); i++)
	{
		if (nullptr == m_ResultCharUI[i])
			continue;

		if (false == m_ResultCharUI[i]->IsGaugeUpdateEnd())
			return false;
	}

	return true;
}
const bool CBattleResultWindow::CheckChangeScene()
{
	return m_bChangeScene;
}