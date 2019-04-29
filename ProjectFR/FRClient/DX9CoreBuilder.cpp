#include "Precom.h"
#include "DX9CoreBuilder.h"
#include "LoadingSceneBuilder.h"
#include "MainSceneBuilder.h"
#include "BattleSceneBuilder.h"
#include "TitleSceneBuilder.h"
#include "EndingSceneBuilder.h"
#include <SJ_SplashWindow.h>


CDX9CoreBuilder::CDX9CoreBuilder()
	: m_pMainWindow(nullptr)
{

}


CDX9CoreBuilder::~CDX9CoreBuilder()
{
	
}
bool CDX9CoreBuilder::CoreBuild()
{
	SettingPath();

	SettingMainWindow();
	//CreateSplashWindow();
	SplashWindowOn();
	
	SettingKey();
	
	/*AllocConsole();
	freopen("CONOUT$", "wt", stdout);*/
	
	CClientGlobal::Init();
	

	m_pMainWindow->CreateScene<CLoadingSceneBuilder>(_T("LoadingScene"), TRUE);
	m_pMainWindow->CreateScene<CBattleSceneBuilder>(_T("BattleScene"), TRUE);
	m_pMainWindow->CreateScene<CMainSceneBuilder>(_T("MainScene"),TRUE);
	m_pMainWindow->CreateScene<CTitleSceneBuilder>(_T("TitleScene"), TRUE);
	m_pMainWindow->CreateScene<CEndingSceneBuilder>(_T("EndingScene"), TRUE);
	

	CClientGlobal::SceneName.push_back(_T("LoadingScene"));
	CClientGlobal::SceneName.push_back(_T("BattleScene"));
	CClientGlobal::SceneName.push_back(_T("MainScene"));
	CClientGlobal::SceneName.push_back(_T("TitleScene"));
	CClientGlobal::SceneName.push_back(_T("EndingScene"));


	CWindowMgr::Inst().ChangeSmallIcon(_T("MainWindow"), CFilePathMgr::GetPath(_T("Texture")) + _T("FRIcon.ico"));
	CWindowMgr::Inst().ChangeIcon(_T("MainWindow"), CFilePathMgr::GetPath(_T("Texture")) + _T("FRLogo.ico"));
	CWindowMgr::Inst().ChangeSmallIcon(_T("MainWindow"), CFilePathMgr::GetPath(_T("Texture")) + _T("FRIcon.ico"));


	m_pMainWindow->ChangeScene(_T("TitleScene"));
	//m_pMainWindow->ChangeScene(_T("MainScene"));
	
	CSplashDevice::Inst()->EnableSplashWindow(false);
	CSplashDevice::Inst()->SetDeviceProcess(false);

	m_pMainWindow->ShowGameWindow(SW_SHOW);
	
	return TRUE;
}
void CDX9CoreBuilder::SettingPath()
{
	CFilePathMgr::AddRootFolder(_T("Texture"));
	CFilePathMgr::AddRootFolder(_T("Sound"));
	CFilePathMgr::AddRootFolder(_T("Data"));

	CFilePathMgr::AddPlusFolder(_T("Texture"), _T("PreLoad"));
	CFilePathMgr::AddPlusFolder(_T("Texture"), _T("Character"));
	CFilePathMgr::AddPlusFolder(_T("Texture"), _T("Monster"));
	CFilePathMgr::AddPlusFolder(_T("Texture"), _T("Effect"));
	CFilePathMgr::AddPlusFolder(_T("Texture"), _T("UI"));
	CFilePathMgr::AddPlusFolder(_T("Texture"), _T("Map"));

	CFilePathMgr::AddPlusFolder(_T("Sound"), _T("SkillSound"));
	CFilePathMgr::AddPlusFolder(_T("Sound"), _T("Bgm"));
}

void CDX9CoreBuilder::SettingMainWindow()
{
	m_pMainWindow = CWindowMgr::Inst().CreateGameWindow(_T("MainWindow"), _T("Project FR"));
	CClientGlobal::MainWindow = m_pMainWindow;
	CWindowMgr::Inst().SetWindowSize(_T("MainWindow"), { 1360,768 });
	m_pMainWindow->InvalidateWndCursor();
	

	//Device�� ����� Render������ �����Ѵ�
	m_pMainWindow->CreateDevice();
	m_pMainWindow->GetLPDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	
	// ���� ���� ��ü�� ����.
	m_pMainWindow->GetLPDevice()->SetRenderState(D3DRS_LIGHTING, true);
	m_pMainWindow->GetLPDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	
	
	m_pMainWindow->GetLPDevice()->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pMainWindow->GetLPDevice()->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pMainWindow->GetLPDevice()->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);

	// �ؽ��� ����
	m_pMainWindow->GetLPDevice()->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pMainWindow->GetLPDevice()->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pMainWindow->GetLPDevice()->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	// ���� ���� ����
	m_pMainWindow->GetLPDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	m_pMainWindow->GetLPDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pMainWindow->GetLPDevice()->SetRenderState(D3DRS_ALPHAREF, 0);

	m_pMainWindow->GetResourceMgr()->LoadGameFont(_T("�ü�"), { 6.0f,10.0f });
	m_pMainWindow->GetResourceMgr()->LoadGameFont(_T("�ü�"),_T("XP"), { 10.0f,18.0f });
	m_pMainWindow->GetResourceMgr()->LoadGameFont(_T("�ü�"),_T("�����ü�"), { 5.0f,8.0f });
	m_pMainWindow->GetResourceMgr()->LoadGameFont(_T("�ü�"), _T("ū�ü�"), { 20.0f,35.0f });

	m_pMainWindow->GetResourceMgr()->LoadGameFont(_T("����"), { 7.0f,12.0f });
	m_pMainWindow->GetResourceMgr()->LoadGameFont(_T("����"), _T("�������屼��"), { 6.0f,14.0f }, FW_EXTRABOLD);
	m_pMainWindow->GetResourceMgr()->LoadGameFont(_T("����"), _T("���屼��"),{ 9.0f,14.0f }, FW_EXTRABOLD);
	m_pMainWindow->GetResourceMgr()->LoadGameFont(_T("����"), _T("��ū���屼��"), { 9.5f,15.0f }, FW_BOLD);
	m_pMainWindow->GetResourceMgr()->LoadGameFont(_T("����"), _T("ū���屼��"), { 14.0f,26.f }, FW_EXTRABOLD);
	m_pMainWindow->GetResourceMgr()->LoadGameFont(_T("����"), _T("����Ʈ����"), { 11.0f,19.f }, FW_EXTRABOLD);
	m_pMainWindow->GetResourceMgr()->LoadGameFont(_T("����"), _T("��ū���屼��"), { 20.0f,34.f }, FW_EXTRABOLD);
	m_pMainWindow->GetResourceMgr()->LoadGameFont(_T("����"), _T("�Ǳ���") , { 10.0f,15.0f });
	m_pMainWindow->GetResourceMgr()->LoadGameFont(_T("����"), _T("��ųŸ��Ʋ"), { 8.0f,14.0f });
	m_pMainWindow->GetResourceMgr()->LoadGameFont(_T("HYPMokGak ����"), _T("UI������Ʈ"), { 8.0f,14.0f });
	m_pMainWindow->GetResourceMgr()->LoadGameFont(_T("HYPMokGak ����"), _T("ūUI������Ʈ"), { 10.0f,16.0f }, FW_SEMIBOLD);
	m_pMainWindow->GetResourceMgr()->LoadGameFont(_T("����"), _T("ĳ��������Ʈ"), { 18.0f,28.0f }, FW_BOLD);
	m_pMainWindow->GetResourceMgr()->LoadGameFont(_T("����"), _T("�޴�������Ʈ"), { 10.0f,16.0f }, FW_HEAVY);
	m_pMainWindow->GetResourceMgr()->LoadGameFont(_T("HYPMokGak ����"), _T("MPȸ����Ʈ"), { 16.0f,22.0f });
	m_pMainWindow->GetResourceMgr()->LoadGameFont(_T("HYPMokGak ����"), _T("��ų�±���"), { 16.0f,27.0f }, FW_BOLD);
	m_pMainWindow->GetResourceMgr()->LoadGameFont(_T("�޸�����ü ����"), _T("�޴���"), { 24.0f,37.0f });
	m_pMainWindow->GetResourceMgr()->LoadGameFont(_T("����"), _T("�����޴���"), { 17.0f,26.0f });
	m_pMainWindow->GetResourceMgr()->LoadGameFont(_T("�ü�"), _T("Lv��Ʈ"), { 10.0f,18.0f });

	m_pMainWindow->GetResourceMgr()->LoadGameFont(_T("����"), _T("ĳ���ʹг�����Ʈ"), { 18.0f,27.0f });

	
}
void CDX9CoreBuilder::SettingKey()
{
	CKeyMgr::CreateKey(_T("MasterKey"), 'A','S','D');

	CKeyMgr::CreateKey(_T("A_Key"), 'A');
	CKeyMgr::CreateKey(_T("S_Key"), 'S');
	CKeyMgr::CreateKey(_T("D_Key"), 'D');
	CKeyMgr::CreateKey(_T("W_Key"), 'W');



	CKeyMgr::CreateKey(_T("ChangeDebugMode"), 'F');
	
	CKeyMgr::CreateKey(_T("CamMode"), 'C');
	CKeyMgr::CreateKey(_T("SoundMode"), 'Q');

	CKeyMgr::CreateKey(_T("InitZoom"), 'Z');

	CKeyMgr::CreateKey(_T("BattlePrev"), 'N');
	CKeyMgr::CreateKey(_T("BattleNext"), 'M');

	CKeyMgr::CreateKey(_T("UP"),  VK_UP);
	CKeyMgr::CreateKey(_T("DOWN"), VK_DOWN);
	CKeyMgr::CreateKey(_T("LEFT"), VK_LEFT);
	CKeyMgr::CreateKey(_T("RIGHT"), VK_RIGHT);
	
	CKeyMgr::CreateKey(_T("SceneEffectTest"), VK_RBUTTON);
	CKeyMgr::CreateKey(_T("LMouse"), VK_LBUTTON);

	CKeyMgr::CreateKey(_T("ChangeScene"), 'X');

	CKeyMgr::CreateKey(_T("Enter"), VK_RETURN);

	CKeyMgr::CreateKey(_T("Space"), VK_SPACE);

	CKeyMgr::CreateKey(_T("MenuKey"), VK_ESCAPE);

	CKeyMgr::CreateKey(_T("InvenKey"), 'I');

}
void CDX9CoreBuilder::SplashWindowOn()
{
	SPLASHDEVICEINFO Info;
	Info.ClassName = _T("SplashWindow");
	Info.Title = _T("Title");
	Info.WndPos = POINT{ 0,0 };
	Info.WndSize = POINT{ 810,460 };
	Info.BackBuffColor = RGB(255, 255, 255);
	Info.ImagePath = CFilePathMgr::GetPath(_T("Texture")) + _T("NewSplashImage.png");
	
	CSplashDevice::Inst()->InitDevice(Info);
	
}