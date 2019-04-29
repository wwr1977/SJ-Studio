#include "Precom.h"
#include "EndingSceneBuilder.h"
#include "EndingCredit.h"
#include "GameOver.h"


CEndingSceneBuilder::CEndingSceneBuilder()
	:m_bCreditEnding(false)
	, m_GameOver(nullptr)
	, m_EndingCredit(nullptr)
{
	CClientGlobal::EndingBuilder = this;
}


CEndingSceneBuilder::~CEndingSceneBuilder()
{
	m_GameOver = nullptr;
	m_EndingCredit = nullptr;
}

void CEndingSceneBuilder::WakeUpScene() 
{
	if (m_bCreditEnding)
	{
		m_EndingCredit->ActorOn();
		m_EndingCredit->StandBy();
	}
	else
	{
		m_GameOver->ActorOn();
		m_GameOver->FadeEvent(true);
	}
}
bool CEndingSceneBuilder::SceneBuild() 
{
	CreateMainCamera();
	CreateMainLight();

	m_GameOver =  CreateActObject(_T("GameOver"))->CreateCom<CGameOver>(this);
	
	m_EndingCredit = CreateActObject(_T("EndingCredit"))->CreateCom<CEndingCredit>(this);
	
	return true;
}
void CEndingSceneBuilder::SceneUpdate() 
{

}
void CEndingSceneBuilder::SceneRender() 
{

}
void CEndingSceneBuilder::ChangeTitleSceneEvent()
{
	CClientGlobal::PrevScene = CClientGlobal::ENDINGSCENE;
	CClientGlobal::MainWindow->ChangeScene(CClientGlobal::SceneName[CClientGlobal::TITLESCENE]);

	m_GameOver->ActorOff();
	m_EndingCredit->ActorOff();
}
void CEndingSceneBuilder::GameOverEvent() 
{
	m_bCreditEnding = false;
}
void CEndingSceneBuilder::EndingEvent()
{
	m_bCreditEnding = true;
}