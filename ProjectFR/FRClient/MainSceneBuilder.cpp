#include "Precom.h"
#include "MainSceneBuilder.h"
#include "GameMaster.h"
#include "MouseCursor.h"
#include "Player.h"
#include "TurnMgr.h"
#include <GameScene.h>
#include <SceneCollisionMgr.h>

 
CMainSceneBuilder::CMainSceneBuilder()
	:m_pGameMaster(nullptr), m_MouseObject(nullptr)
	, CurCameraRatio(1.0f)
{
}


CMainSceneBuilder::~CMainSceneBuilder()
{
	SAFE_DELETE(m_pGameMaster)
}
bool CMainSceneBuilder::SceneBuild( )
{
	CreateMainCamera();
	CreateMainLight();
	
	m_MouseObject = CActObject::CreateActObject(_T("MainMouse"),GetScene());
	
	GetScene()->GetCollisionMgr()->CreateCheckGroup(_T("Player"), _T("Portal"));
	GetScene()->GetCollisionMgr()->CreateCheckGroup(_T("NPC"), _T("Player"));
	GetScene()->GetCollisionMgr()->CreateCheckGroup(_T("Inventory"), _T("Mouse"));
	GetScene()->GetCollisionMgr()->CreateCheckGroup(_T("InvenSlot"), _T("Mouse"));

	m_pGameMaster = new CGameMaster(GetScene(), this);
	m_pGameMaster->SetMouseCursor(m_MouseObject->CreateCom<CMouseCursor>());

	m_pGameMaster->InitGameMaster();

	return TRUE;
}

void CMainSceneBuilder::SceneUpdate()
{
	if (true == CKeyMgr::GetWheelEvent())
		ZoomUpdate();

	if (TRUE == KEYDOWN("InitZoom"))
		InitZoom();
	
	if (TRUE == KEYDOWN("ChangeScene")) 
	{
		//int Ran = CSJRandom::RandomInt(0, CGameMaster::BATTLE_SLIME);
		m_pGameMaster->SettingBattleMonster(CGameMaster::BATTLE_SLIME);
		CClientGlobal::GameMaster->BattleSceneChangeEvent();
		CClientGlobal::PrevScene = CClientGlobal::MAINSCENE;
		CClientGlobal::TurnMgr->SetCurBattleBgmIndex(BATTLE0_SOUND);
		CClientGlobal::TurnMgr->ChangeBackGround(2);
		CClientGlobal::StoryBattle = false;
		CClientGlobal::MainWindow->ChangeScene(CClientGlobal::SceneName[CClientGlobal::BATTLESCENE]);
	}
	

	if (TRUE == KEYDOWN("CamMode")) 
		GetMainCam()->RamdomMode();
	
	CSceneBuilder::SceneUpdate();
	m_pGameMaster->Update();
}
void CMainSceneBuilder::SceneLastUpdate()
{
	m_pGameMaster->LastUpdate();
}
void CMainSceneBuilder::SceneRender()
{
	if (false == CClientGlobal::MainWindow->GetDebugMgr()->IsDebugMode())
		return;

	TCHAR MSG[MAXKEYLENGTH];

	SPTR<CPlayer> Player = CClientGlobal::MainPlayer;


	_stprintf_s(MSG, _T("FPS : %f"), CTimerMgr::FPS());
	RESMGR->DrawFont(_T("ÅÇ±¼¸²"), MSG, { -550.0f,350.0f,10.0f }, 1.3f, D3DCOLOR_ARGB(255, 255, 255, 255));

	if (nullptr != Player) 
	{
		switch (Player->GetCurStateIndex())
		{
			case PLAYER_IDLE:
				RESMGR->DrawFont(_T("ÅÇ±¼¸²"), _T("Player State : Idle"), { -520.0f,330.0f,10.0f }, 1.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
				break;
			case PLAYER_WALK:
				RESMGR->DrawFont(_T("ÅÇ±¼¸²"), _T("Player State : Walk"), { -520.0f,330.0f,10.0f }, 1.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
				break;
			case PLAYER_RUN:
				RESMGR->DrawFont(_T("ÅÇ±¼¸²"), _T("Player State : Run "), { -520.0f,330.0f,10.0f }, 1.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
				break;
		}
	
		RESMGR->DrawFont(_T("ÅÇ±¼¸²"), Player->GetCurDirString() , { -500.0f,310.0f,10.0f }, 1.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
		RESMGR->DrawFont(_T("ÅÇ±¼¸²"), Player->GetCurTileStateString(), { -500.0f,290.0f,10.0f }, 1.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
		
		if(_T("") != Player->GetCurTileColorString())
			RESMGR->DrawFont(_T("ÅÇ±¼¸²"), Player->GetCurTileColorString(), { -500.0f,270.0f,10.0f }, 1.3f, D3DCOLOR_ARGB(255, 255, 255, 255));

	}

}
void CMainSceneBuilder::WakeUpScene()
{
	if (nullptr == m_MouseObject)
		return;

	CClientGlobal::MouseObeject = m_MouseObject;
	CClientGlobal::CurNoneZoomProj = GetScene()->GetMainCamCom()->GetProjNoneRatio();
	CClientGlobal::CurSceneProj = GetScene()->GetMainCamCom()->GetSceneProj();
	CClientGlobal::CurSceneView = GetScene()->GetMainCamCom()->GetSceneView();

	m_pGameMaster->WakeUpGameMaster();
}
void CMainSceneBuilder::ZoomUpdate()
{
	if (CKeyMgr::GetWheelSign() > 0)
	{
		if (CurCameraRatio <= 0.15f) 
		{
			CurCameraRatio = 0.15f;
			GetMainCam()->SetZoomRatio(CurCameraRatio);
		}
		else 
		{
			CurCameraRatio -= 0.05f;
			GetMainCam()->SetZoomRatio(CurCameraRatio);
		}
	}
	// ÈÙÀ» ¾ÕÀ¸·Î µ¹¸®¸é(Ä«¸Þ¶ó´Â µÚ·Î °£´Ù)
	else
	{
		if (CurCameraRatio >= 2.0f) 
		{
			CurCameraRatio = 2.0f;
			GetMainCam()->SetZoomRatio(CurCameraRatio);
		}
		else
		{
			CurCameraRatio += 0.05f;
			GetMainCam()->SetZoomRatio(CurCameraRatio );
		}
	}
}
void CMainSceneBuilder::InitZoom()
{
	CurCameraRatio = 1.0f;

	GetMainCam()->SetZoomRatio(CurCameraRatio);
}