#include "Precom.h"
#include "BattleSceneBuilder.h"
#include "BackGround.h"
#include "UnitStatusBar.h"
#include "MouseCursor.h"
#include "BattleUnit.h"
#include "GameSprite.h"
#include "TurnOrderUI.h"
#include "TurnMgr.h"
#include "TestBox.h"
#include "TestEffect.h"
#include "DemageFontGroup.h"



CBattleSceneBuilder::CBattleSceneBuilder()
	:m_MouseObject(nullptr),m_TurnMgr(nullptr)
{
	m_TurnMgr = new CTurnMgr();
	CDemageFontGroup::SetTurnMgr(m_TurnMgr.p);
}


CBattleSceneBuilder::~CBattleSceneBuilder()
{
	
}

bool CBattleSceneBuilder::SceneBuild()
{
	CreateMainCamera();
	CreateMainLight();

	m_MouseObject = CreateActObject(_T("BattleMouse"));
	m_MouseObject->CreateCom<CMouseCursor>();

	CreateCollisionGroup(_T("UI"), _T("Mouse"));
	
	m_TurnMgr->Init(GetScene(),this);
	//CClientGlobal::CreateTestSkill();
	m_TurnMgr->InitSkillUIMgr();

	return true;
}
void CBattleSceneBuilder::SceneUpdate()
{
	CSceneBuilder::SceneUpdate();

	if (TRUE == KEYDOWN("ChangeScene"))
	{
		m_TurnMgr->ResultWindowOff();
		CClientGlobal::PrevScene = CClientGlobal::BATTLESCENE;
		CClientGlobal::MainWindow->ChangeScene(CClientGlobal::SceneName[CClientGlobal::MAINSCENE]);
		m_TurnMgr->BgmOff();
	}
	if (TRUE == KEYDOWN("SceneEffectTest"))
	{
		
		if (false == IsSceneEffectOn()) 
		{
			//지진 발생
			//SceneEffect(SCENE_EFFECT_EARTHQUAKE | EARTHQUAKE_RECTSPACE | EARTHQUAKE_CONSISTENT, 0.5f);
			//SetEarthquakeData(Vec2{ 50.0f,50.0f }, 0.01f);
			

			// 암전,눈부심
			//SceneEffect(SCENE_EFFECT_FLASH , 0.5f);

			// Fade In & Out
			//SceneEffect(SCENE_EFFECT_FADE_IN | FADE_COLOR_BLACK , 1.5f);
			//SetFadeData(0.7f);

			// Zoom In & Out
			/*SceneEffect(SCENE_EFFECT_ZOOM_OUT , 2.0f);
			SetZoomData(Vec3{ 200.0f,100.0f,0.0f }, { 1.5f,0.5f }, 1.0f);*/

			//m_TurnMgr->PopDemageFont(Random, Vec3{0.0f,0.0f,0.0f},5,_T("Number_Blue"));
			//m_TurnMgr->PopMiddleDemageFont(Random, Vec3{ 0.0f,0.0f,0.0f }, 5, DEMAGE_RED);

		}
	}

	if (true == KEYDOWN("ChangeDebugMode")) 
		GetScene()->GetDebugMgr()->ChangeDebugMode();

	if (true == KEYDOWN("SoundMode"))
		m_TurnMgr->ReverseBgmPlay();
	


	m_TurnMgr->Update();
}
void CBattleSceneBuilder::WakeUpScene()
{
	CClientGlobal::MouseObeject = m_MouseObject; 
	CClientGlobal::CurNoneZoomProj = GetScene()->GetMainCamCom()->GetProjNoneRatio();
	CClientGlobal::CurSceneProj = GetScene()->GetMainCamCom()->GetSceneProj();
	CClientGlobal::CurSceneView = GetScene()->GetMainCamCom()->GetSceneView();

	//CClientGlobal::CurBattleEndRound = CSJRandom::RandomInt(0, MAXROUND - 1);
	/*
	CClientGlobal::CurBattleEndRound = 0;
	

	for (size_t j = 0; j <= CClientGlobal::CurBattleEndRound; ++j)
	{
			CClientGlobal::strMonsterTeam[j][0] = _T("DarkCommander3");
			CClientGlobal::strMonsterTeam[j][2] = _T("DarkCommander2");
	}
	*/
	// 0번은 StandBy Phase

	m_TurnMgr->BattleStartEvent();
	m_TurnMgr->ChangePhase(PHASE_STANDBY);

#ifdef _DEBUG
	MessageBeep(MB_OK);
#else
#endif

}

void CBattleSceneBuilder::ZoomCheck() 
{
	SPTR<CCamera>	MainCam = GetMainCamCom();

	// 휠을 뒤로 돌리면(카메라가 앞으로 간다, 이미지가 커보인다, 카메라 전체 화면을 늘린다)
	float CurCameraRatio = MainCam->GetZoomRatio();

	if (true == KEYPRESS("ZoomIn"))
	{
		if (CurCameraRatio <= 0.15f)
		{
			MainCam->SetZoomRatio(0.15f);
		}
		else
		{
			MainCam->SetZoomRatio(CurCameraRatio - 0.05f);
		}
		return;
	}
	// 휠을 앞으로 돌리면(카메라는 뒤로 간다)
	else if(true == KEYPRESS("ZoomOut"))
	{
		if (CurCameraRatio >= 2.0f)
		{
			MainCam->SetZoomRatio(2.0f);
		}
		else
		{
			MainCam->SetZoomRatio(CurCameraRatio + 0.05f);
		}
		return;
	}
}
void CBattleSceneBuilder::CameraMoveCheck()
{
	Vec3 Dir = GAMEVEC::ZERO;

	if (true == KEY("UP")) 
	{
		Dir += GAMEVEC::UP;
	}
	if (true == KEY("DOWN"))
	{
		Dir += GAMEVEC::DOWN;
	}
	if (true == KEY("LEFT"))
	{
		Dir += GAMEVEC::LEFT;
	}
	if (true == KEY("RIGHT"))
	{
		Dir += GAMEVEC::RIGHT;
	}

	D3DXVec3Normalize(&Dir, &Dir);

	if (GAMEVEC::ZERO != Dir) 
	{
		GetScene()->GetMainCamera()->Move(Dir*DELTATIME*100.0f);
	}
}
SPTR<CCamera> CBattleSceneBuilder::GetMainCamCom()
{
	return GetScene()->GetMainCamCom();

}
void CBattleSceneBuilder::SceneRender()
{
	TCHAR FPS[MAXKEYLENGTH];

	if (TRUE == m_pScene->GetDebugMgr()->IsDebugMode())
	{
		_stprintf_s(FPS, _T("FPS : %f"), CTimerMgr::FPS());
		ScenenCurStateMsg();
		RESMGR->DrawFont(_T("큰궁서"), FPS, { -450.0f,350.0f,10.0f }, 1.3f, D3DCOLOR_ARGB(255, 255, 255, 255));

#ifdef _WIN64
		_stprintf_s(FPS, _T("Ready : %I64u  Return : %I64u "), m_TurnMgr->GetReadyListSize(), m_TurnMgr->GetReturnListSize());
#else
		_stprintf_s(FPS, _T("Ready : %d  Return : %d "), m_TurnMgr->GetReadyListSize(), m_TurnMgr->GetReturnListSize());
#endif

		RESMGR->DrawFont(_T("큰궁서"), FPS, { 230.0f,300.0f,10.0f }, 1.3f, D3DCOLOR_ARGB(255, 255, 255, 255));

		if (true == CKeyMgr::IsUpdate())
		{
			RESMGR->DrawFont(_T("큰궁서"), _T("On"), { -400.0f,300.0f,10.0f }, 1.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
		}
		else
		{
			RESMGR->DrawFont(_T("큰궁서"), _T("Off"), { -400.0f,300.0f,10.0f }, 1.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
		}
	}
}