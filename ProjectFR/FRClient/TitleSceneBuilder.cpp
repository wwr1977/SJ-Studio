#include "Precom.h"
#include "TitleSceneBuilder.h"
#include "TitleIntroObject.h"
#include "TitleBackGround.h"


CTitleSceneBuilder::CTitleSceneBuilder()
	: m_bIntro(true)
	, m_IntroObject(nullptr)
	, m_bKeyCheck(false)
	, m_MainTitle(nullptr)
	, m_bGameStart(false)
{
}


CTitleSceneBuilder::~CTitleSceneBuilder()
{
}
void CTitleSceneBuilder::WakeUpScene() 
{
	switch (CClientGlobal::PrevScene)
	{
	case CClientGlobal::LOADINGSCENE:
		break;
	case CClientGlobal::BATTLESCENE: 
		break;
	case CClientGlobal::MAINSCENE:
		break;
	case CClientGlobal::TITLESCENE:
		break;
	case CClientGlobal::ENDINGSCENE:
		IntroEndEvent();
		break;
	}
}
bool CTitleSceneBuilder::SceneBuild() 
{
	CreateMainCamera();
	CreateMainLight();

	m_IntroObject = CreateActObject(_T("IntroObject"))->CreateCom<CTitleIntroObject>(this);
	m_IntroObject->ActorOff();

	m_MainTitle = CreateActObject(_T("TitleBackGround"))->CreateCom<CTitleBackGround>(this);
	return true;
}
void CTitleSceneBuilder::SceneUpdate() 
{
	if (m_bKeyCheck)
		KeyCheck();
	else if (m_bGameStart)
		GameStartUpdate();

}
void CTitleSceneBuilder::SceneLastUpdate()
{
	if (m_bIntro && m_IntroObject->IsIntroEnd())
	{
		m_IntroObject->ActorOn();
		m_IntroObject->Standby();
		return;
	}
}
void CTitleSceneBuilder::SceneRender()
{

}
void CTitleSceneBuilder::SetKeyCheck(const bool& _Check)
{
	m_bKeyCheck = _Check;
}
void CTitleSceneBuilder::KeyCheck() 
{
	if (TRUE == KEYDOWN("Enter")) 
	{
		m_bKeyCheck = false;
		m_bGameStart = true;
		m_MainTitle->GameStartEvent();
	}
}
void CTitleSceneBuilder::GameStartUpdate()
{
	if (true == m_MainTitle->IsStartUpdateEnd()) 
	{
		CClientGlobal::PrevScene = CClientGlobal::TITLESCENE;
		CClientGlobal::MainWindow->ChangeScene(CClientGlobal::SceneName[CClientGlobal::MAINSCENE]);
		//CClientGlobal::MainWindow->ChangeScene(CClientGlobal::SceneName[CClientGlobal::ENDINGSCENE]);
		m_bGameStart = false;
		m_bKeyCheck = false;
	}
	

}
void CTitleSceneBuilder::IntroEndEvent()
{
	m_bIntro = false;
	m_MainTitle->FadeEvent(false);
}