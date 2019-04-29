#include "DeadByDaylightBuilder.h"
#include <ClientConnectMgr.h>
#include <Collider.h>
#include <Scene.h>
#include <FilePathMgr.h>
#include <DeadByDaylightGlobal.h>
#include "IntroScene.h"
#include "PlayScene.h"
#include "TestScene.h"
#include <GameWindow.h>

#include "SelectScene.h"
#include "RoomSelectScene.h"
#include "MainPlayScene.h"
#include "EndingScene.h"
#include "KillerLobbyScene.h"
#include "KillerVictoryScene.h"
#include "SurvivorLobbyScene.h"
#include "SurvivorVictoryScene.h"

#include <SJ_SplashWindow.h>

DeadByDaylightBuilder::DeadByDaylightBuilder()
	: m_bOpenServer(false)
{
}


DeadByDaylightBuilder::~DeadByDaylightBuilder()
{
}
const BOOL DeadByDaylightBuilder::PreBuild()
{
	SPLASHDEVICEINFO Info;
	Info.ClassName = L"SplashWindow";
	Info.Title = L"Title";
	Info.WndPos = POINT{ 0,0 };
	Info.WndSize = POINT{ 810,460 };
	Info.BackBuffColor = RGB(255, 0, 255);
	Info.ImagePath = CFilePathMgr::GetPath(L"Texture") + L"DBDSplash.png";

	CSplashDevice::Inst()->InitDevice(Info);
	return TRUE;
}
const BOOL DeadByDaylightBuilder::Build()
{	
	GlobalDBD::Init();
	CCollider::ColliderInit();

	// 1. ���� ������ �ִ� ���� - Ŭ��
	//DeadClientBuild();
	
	// 2. ���� ������ ������ ���� - Ŭ�� 
	//ClientBuild();
	
	// 3. ���� ������ Ŭ���̾�Ʈ  
	TestClientBuild();

	return TRUE;
}
void DeadByDaylightBuilder::DeadClientBuild()
{
	m_bOpenServer = true;
	GETSINGLE(CClientConnectMgr).Init();
	CScene::CreateScene<CSelectScene>(L"SelectScene", true);
	//CScene::CreateScene<CRoomSelectScene>(L"RoomSelectScene", true);
	//CScene::CreateScene<CMainPlayScene>(L"MainPlayScene", true);
	//CScene::CreateScene<CEndingScene>(L"EndingScene", true);
	//CScene::CreateScene<CKillerLobbyScene>(L"KillerLobbyScene", true);
	//CScene::CreateScene<CKillerVictoryScene>(L"KillerVictoryScene", true);
	//CScene::CreateScene<CSurvivorLobbyScene>(L"SurvivorLobbyScene", true);
	//CScene::CreateScene<CSurvivorVictoryScene>(L"SurvivorVictoryScene", true);

	CScene::ChangeScene(L"SelectScene");

	CSplashDevice::Inst()->EnableSplashWindow(false);
	CSplashDevice::Inst()->SetDeviceProcess(false);
	Core::ShowGameWindow(true);
}
void DeadByDaylightBuilder::ClientBuild() 
{
	// ���ε���  ���� ������ ������� ������
	// �� �۾��� ������ �κ� ������ �̵�
	// 1. ��Ʈ�� �� �ε� : �ؽ���,��������Ʈ �Ͱ��� �׷��� ���ҽ� �ε�
	// 2. �κ� �� �ε� : �κ� UI ��  �κ���� ĳ���� �̸� �����
	// 3. ����(����) �� �ε� : ������ ������ �ε� �� ������Ʈ �̸� ����
	// ������ ���� �� �� 
	
	Core::SetProcessPrioriy(REALTIME_PRIORITY_CLASS);
	m_bOpenServer = true;
	GETSINGLE(CClientConnectMgr).Init();
	CScene::CreateScene<CPlayScene>(L"PlayScene", true);
	CScene::CreateScene<CIntroScene>(L"IntroScene", true);
	CScene::CreateScene<CEndingScene>(L"EndingScene", true);

	CScene::ChangeScene(L"IntroScene");
}
void DeadByDaylightBuilder::TestClientBuild() 
{
	// ������ ���� �ȵ� ��
	m_bOpenServer = false;
	CScene::CreateScene<CTestScene>(L"TestScene", true);
	//CScene::CreateScene<CSelectScene>(L"SelectScene", true);
	//CScene::CreateScene<CRoomSelectScene>(L"RoomSelectScene", true);
	//CScene::CreateScene<CKillerLobbyScene>(L"KillerLobbyScene", true);
	//CScene::CreateScene<CSurvivorLobbyScene>(L"SurvivorLobbyScene", true);

	CScene::ChangeScene(L"TestScene");
	//CScene::ChangeScene(L"SelectScene");
	//CScene::ChangeScene(L"KillerLobbyScene");
	Core::ShowGameWindow(true);
}
void DeadByDaylightBuilder::BuilderProc(HWND _hWnd, UINT _Msg, WPARAM _wParam, LPARAM _lParam)
{
	switch (_Msg)
	{
	case WM_MOVE:
	case WM_SIZE:
	{
		CGameWindow::SyncroWindowScreen();
	}
	break;
	case WM_ACTIVATE:
	{
		Core::FocusingClient((bool)_wParam);
	}
		break;
	default:
		break;
	}
}

const BOOL DeadByDaylightBuilder::Release()
{
	GlobalDBD::Release();

	if (true == m_bOpenServer) 
	{
		GETSINGLE(ClientConnector).DisConnectLogicServer();
	}
	return TRUE;
}      

CORESTART_HIDE(DeadByDaylightBuilder, L"Client", Vec2(1600 , 900), Vec2(0, 15))
