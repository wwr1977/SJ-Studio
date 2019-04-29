#include "Precom.h"
#include "GameMaster.h"
#include "MainMapMgr.h"
#include "MapBackground.h"
#include "TestBox.h"
#include "Player.h"
#include "Tile.h"
#include "TileMap.h"
#include "NPCBase.h"
#include "EventNPC.h"
#include "StoreNPC.h"
#include "MainMenuMgr.h"
#include "InventoryMgr.h"
#include "ScriptMgr.h"
#include "EventMgr.h"
#include "MainSceneBuilder.h"
#include "EndingSceneBuilder.h"
#include "BattleEventObject.h"
#include "QuestClearObject.h"
#include "Effect.h"
#include "StoryCurtain.h"
#include <GameSprite.h>
#include "BossGauge.h"
#include "StoreUI.h"
#include "MouseCursor.h"
#include"QuestRewardGuide.h"


CGameMaster::CGameMaster()
	:m_pMainScene(nullptr), m_pMainSceneBuilder(nullptr)
	, m_pMapMgr(nullptr), m_MainMenuMgr(nullptr)
	, m_pInventoryMgr(nullptr), m_pScriptMgr(nullptr)
	, m_pEventMgr(nullptr)
	, m_CurBattleQuestKey(_T("")), m_CurBattleQuestCount(0)
	, m_bBattleSceneTriger(false), m_bMainSceneStart(false)
	, m_fAccTime(0.f), m_SceneCurtain(nullptr)
	, m_QuestClearObject(nullptr)
	, m_pEventUpdateFunc(nullptr)
	, m_fAccWaitTime(0.f)
	, m_bMapChange(false)
	, m_bMapChangeUpdate(false)
	, m_ChangeTrigerTile(nullptr)
	, m_StoryCurtain(nullptr)
	, m_bStoryCurtainWait(false)
	, m_bBaseNPCTriger(false)
	, m_bGameClear(false)
	, m_StoreUI(nullptr)
	, m_MouseCursor(nullptr)
	, m_QuestResultGuide(nullptr)
{
	m_vecCurChar.clear();
	m_vecNPC.clear();
	memset(m_CurTeam, -1, MAXTEAM);
	m_listEffect.clear();
	m_vecCharacterLineOpen.clear();
}
CGameMaster::CGameMaster(CGameScene* _MainScene, CSceneBuilder* _MainBuilder)
	: m_pMainScene(_MainScene), m_pMainSceneBuilder(_MainBuilder)
	, m_pMapMgr(nullptr), m_pInventoryMgr(nullptr)
	, m_MainMenuMgr(nullptr)
	, m_CurBattleQuestKey(_T("")), m_CurBattleQuestCount(0)
	, m_bBattleSceneTriger(false), m_bMainSceneStart(true)
	, m_fAccTime(0.f), m_SceneCurtain(nullptr)
	, m_QuestClearObject(nullptr)
	, m_fAccWaitTime(0.f)
	, m_bMapChange(false)
	, m_ChangeTrigerTile(nullptr)
	, m_bMapChangeUpdate(false)
	, m_StoryCurtain(nullptr)
	, m_bStoryCurtainWait(false)
	, m_bBaseNPCTriger(false)
	, m_bGameClear(false)
	, m_StoreUI(nullptr)
	, m_MouseCursor(nullptr)
{
	if (nullptr == _MainScene || nullptr == _MainBuilder) 
	{
		assert(nullptr);
		return;
	}

	CNPCBase::InitNpcBase();
	CClientGlobal::GameMaster = this;
	m_pMapMgr = new CMainMapMgr(this);
	
	m_pScriptMgr = new CScriptMgr(this,m_pMainScene,m_pMainSceneBuilder);
	m_pScriptMgr->Init();
	
	m_pEventMgr = new CEventMgr(this, m_pMainScene, m_pMainSceneBuilder, m_pScriptMgr);
	m_pEventMgr->Init();
	

	m_vecCurChar.clear();
	m_vecNPC.clear();
	memset(m_CurTeam, -1, MAXTEAM);
	m_listEffect.clear();
	m_vecCharacterLineOpen.clear();
}

CGameMaster::~CGameMaster()
{
	ReleaseGameMaster();
	m_vecCurChar.clear();
	CClientGlobal::InventoryMgr = nullptr;
	m_MouseCursor = nullptr;
	SAFE_DELETE(m_pInventoryMgr);
	SAFE_DELETE(m_pScriptMgr);
	SAFE_DELETE(m_pEventMgr);
	m_listEffect.clear();
	m_vecCharacterLineOpen.clear();
}

void CGameMaster::InitGameMaster()
{
	SPTR<CActObject> Player = CreateActObject(_T("Player"));
	
	CClientGlobal::MainPlayer =   Player->CreateCom<CPlayer>();
	m_MainPlayer = CClientGlobal::MainPlayer;

	m_SceneCurtain = CreateActObject(_T("Curtain"))->CreateCom<CFixRenderer>(RENDATA(LAYER_CAMERASHIELD, 1.f, true));
	m_SceneCurtain->SetSprite(_T("ColorBase"));
	m_SceneCurtain->SetMaterial(D3DXCOLOR(0.f, 0.f, 0.f, 1.f));
	m_SceneCurtain->MaterialOn();
	m_SceneCurtain->SetCustomSize(Vec2{ 1500.f,1500.f });
	m_SceneCurtain->Off();

	m_pInventoryMgr = new CInventoryMgr(this);
	m_pInventoryMgr->SetMouseCursor(m_MouseCursor);

	if (nullptr != m_pMapMgr) 
	{
		m_pMapMgr->Init(m_pMainScene, m_pMainSceneBuilder);
		m_pMapMgr->SetEventMgr(m_pEventMgr);
	}

	m_vecNPCData.push_back(NPCDATA(_T("Girls"), _T("소녀"), FIXIDLE));
	m_vecNPCData.push_back(NPCDATA(_T("Elder"), _T("장로"), FIXIDLE));
	m_vecNPCData.push_back(NPCDATA(_T("Lid"), _T("리드"), ANIIDLE));
	m_vecNPCData.push_back(NPCDATA(_T("Destroyer"), _T("파괴왕"), ANIIDLE));
	m_vecNPCData.push_back(NPCDATA(_T("Beramode"), _T("베라모드"), ANIIDLE));
	m_vecNPCData.push_back(NPCDATA(_T("Carc"), _T("칼크"), ANIIDLE));
	m_vecNPCData.push_back(NPCDATA(_T("Merus"), _T("메루스"), ANIIDLE));
	m_vecNPCData.push_back(NPCDATA(_T("Mamon"), _T("마몬"), ANIIDLE));
	m_vecNPCData.push_back(NPCDATA(_T("Golbez"), _T("골베즈"), ANIIDLE));
	m_vecNPCData.push_back(NPCDATA(_T("WeaponStoreNPC"), _T("무기점 상인"), FIXIDLE));
	m_vecNPCData.push_back(NPCDATA(_T("ArmorStoreNPC"), _T("방어구점 상인"), FIXIDLE));
	m_vecNPCData.push_back(NPCDATA(_T("AccessoriesStoreNPC"), _T("악세사리점 상인"), FIXIDLE));



	m_vecNPCData[0].NpcPoster = m_pMainScene->GetWindow()->GetResourceMgr()->FindGameSprite(_T("GirlsPoster"));
	m_vecNPCData[1].NpcPoster = m_pMainScene->GetWindow()->GetResourceMgr()->FindGameSprite(_T("ElderPoster"));
	m_vecNPCData[2].NpcPoster = m_pMainScene->GetWindow()->GetResourceMgr()->FindGameSprite(_T("LidPoster"));
	m_vecNPCData[3].NpcPoster = nullptr;
	m_vecNPCData[4].NpcPoster = nullptr;
	m_vecNPCData[5].NpcPoster = nullptr;
	m_vecNPCData[6].NpcPoster = nullptr;
	m_vecNPCData[7].NpcPoster = nullptr;
	m_vecNPCData[8].NpcPoster = m_pMainScene->GetWindow()->GetResourceMgr()->FindGameSprite(_T("GolbezPoster"));
	m_vecNPCData[9].NpcPoster = nullptr;
	m_vecNPCData[10].NpcPoster = nullptr;
	m_vecNPCData[11].NpcPoster = nullptr;

	m_vecNPC.push_back(CreateActObject(_T("NPC"))->CreateCom<CNPCBase>(m_vecNPCData[0]));
	m_vecNPC.push_back(CreateActObject(_T("NPC"))->CreateCom<CNPCBase>(m_vecNPCData[1]));
	m_vecNPC.push_back(CreateActObject(_T("NPC"))->CreateCom<CNPCBase>(m_vecNPCData[2]));
	m_vecNPC.push_back(CreateActObject(_T("NPC"))->CreateCom<CEventNPC>(m_vecNPCData[3]));
	m_vecNPC.push_back(CreateActObject(_T("NPC"))->CreateCom<CNPCBase>(m_vecNPCData[4]));
	m_vecNPC.push_back(CreateActObject(_T("NPC"))->CreateCom<CNPCBase>(m_vecNPCData[5]));
	m_vecNPC.push_back(CreateActObject(_T("NPC"))->CreateCom<CNPCBase>(m_vecNPCData[6]));
	m_vecNPC.push_back(CreateActObject(_T("NPC"))->CreateCom<CNPCBase>(m_vecNPCData[7]));
	m_vecNPC.push_back(CreateActObject(_T("NPC"))->CreateCom<CEventNPC>(m_vecNPCData[8]));
	m_vecNPC.push_back(CreateActObject(_T("NPC"))->CreateCom<CNPCBase>(m_vecNPCData[9]));
	m_vecNPC.push_back(CreateActObject(_T("NPC"))->CreateCom<CNPCBase>(m_vecNPCData[10]));
	m_vecNPC.push_back(CreateActObject(_T("NPC"))->CreateCom<CNPCBase>(m_vecNPCData[11]));

	for (size_t i = 0; i < m_vecNPC.size(); i++)
	{
		if (nullptr != m_vecNPC[i])
			m_vecNPC[i]->ActorOff();
	}


	m_pMapMgr->PushMapNPC(ITEMSTORE, m_vecNPC[2]);
	m_pMapMgr->PushMapNPC(TOWN, m_vecNPC[0]);
	m_pMapMgr->PushMapNPC(TOWN, m_vecNPC[1]);
	m_pMapMgr->PushMapNPC(RUINS, m_vecNPC[3]);
	m_pMapMgr->PushMapNPC(DEVILCASTLE, m_vecNPC[4]);
	m_pMapMgr->PushMapNPC(DEVILCASTLE, m_vecNPC[5]);
	m_pMapMgr->PushMapNPC(DEVILCASTLE, m_vecNPC[6]);
	m_pMapMgr->PushMapNPC(DEVILCASTLE, m_vecNPC[7]);
	m_pMapMgr->PushMapNPC(DEVILCASTLE, m_vecNPC[8]);
	m_pMapMgr->PushMapNPC(WEANPONSTORE, m_vecNPC[9]);
	m_pMapMgr->PushMapNPC(WEANPONSTORE, m_vecNPC[10]);
	m_pMapMgr->PushMapNPC(ITEMSTORE, m_vecNPC[11]);

	m_pMainScene->GetMainCamCom()->SetCamMode(CAM_MODE_TRACKING, CClientGlobal::MainPlayer->GetTransForm());

	m_MainMenuMgr = CreateActObject(_T("MainMenuMgr"))->CreateCom<CMainMenuMgr>(m_pMainSceneBuilder);
	m_MainMenuMgr->CreateMainMenu();
	m_MainMenuMgr->MenuMgrOff();

	
	m_vecCurChar.push_back(0);
	m_vecCurChar.push_back(0);
	m_vecCurChar.push_back(0);
	m_vecCurChar.push_back(0);
	m_vecCurChar.push_back(0);
	m_vecCurChar.push_back(0);
	m_vecCurChar.push_back(0);
	m_vecCurChar.push_back(0);
	m_vecCurChar.push_back(0);
	m_vecCurChar.push_back(0);

	m_CurTeam[0] = CClientGlobal::CharacterLine[RAIN][m_vecCurChar[RAIN]].CharacterNo;
	m_CurTeam[1] = CClientGlobal::CharacterLine[CHIZURU][m_vecCurChar[CHIZURU]].CharacterNo;
	m_CurTeam[2] = CClientGlobal::CharacterLine[LUNA][m_vecCurChar[LUNA]].CharacterNo;
	m_CurTeam[3] = CClientGlobal::CharacterLine[ZYRUS][m_vecCurChar[ZYRUS]].CharacterNo;
	m_CurTeam[4] = -1;
	//m_CurTeam[4] = CClientGlobal::CharacterLine[LID][m_vecCurChar[LID]].CharacterNo;

	//m_vecNPC[0]->SetActorPos(Vec3{ 100.f,0.,7.f });
	m_vecNPC[0]->SetActorPos(Vec3{ 185.f,-140.f,7.f });
	m_vecNPC[0]->SetActorImageMag(1.5f);
	m_vecNPC[0]->SetAlpha(0.85f);
	m_vecNPC[0]->CreatePlayerCollision(COLFIGURE(Vec2{ 0.f,-7.f }, Vec2{48.f,46.f}), CT_RECT);
	m_vecNPC[0]->CreateScriptCollision(COLFIGURE(Vec2{ 0.f,-7.f }, Vec2{ 50.f,48.f }), CT_RECT);
	m_vecNPC[0]->SetHideType(CNPCBase::HIDE_EFFECT);
	
	//m_vecNPC[1]->SetActorPos(Vec3{ 200.f,0.,7.f });
	m_vecNPC[1]->SetActorPos(Vec3{ -544.f,750.f,7.f });
	m_vecNPC[1]->SetActorImageMag(1.5f);
	m_vecNPC[1]->SetAlpha(0.85f);
	m_vecNPC[1]->CreatePlayerCollision(COLFIGURE(Vec2{ 0.f,-7.f }, Vec2{ 48.f,46.f }), CT_RECT);
	m_vecNPC[1]->CreateScriptCollision(COLFIGURE(Vec2{ 0.f,-7.f }, Vec2{ 50.f,48.f }), CT_RECT);
	m_vecNPC[1]->SetHideType(CNPCBase::HIDE_EFFECT);

	m_vecNPC[2]->SetActorPos(Vec3{ -256.f,-118.f,7.f });
	m_vecNPC[2]->SetActorImageMag(1.5f);
	m_vecNPC[2]->CreatePlayerCollision(COLFIGURE(Vec2{ 0.f,-15.f }, Vec2{ 30.f,54.f }), CT_RECT);
	m_vecNPC[2]->CreateScriptCollision(COLFIGURE(Vec2{ 0.f,-15.f }, Vec2{ 32.f,56.f }), CT_RECT);


	m_vecNPC[3]->SetActorPos(Vec3{ 768.f,-384.f,7.f });
	m_vecNPC[3]->SetActorImageMag(-1.f);
	m_vecNPC[3]->CreateSubCollision(COLFIGURE(Vec2{ -350.f,-64.f }, Vec2{ 1000.f,975.f }), CT_RECT);
	m_vecNPC[3]->SetAniIntervalTime(0.25f);
	m_vecNPC[3]->SetHideType(CNPCBase::HIDE_VIBRATION);

	m_vecNPC[4]->SetActorPos(Vec3{ -108.f,684.f,7.f });
	m_vecNPC[4]->SetActorImageMag(1.5f);
	m_vecNPC[4]->SetAniIntervalTime(0.25f);
	m_vecNPC[4]->SetHideType(CNPCBase::HIDE_DEADANI);
	m_vecNPC[4]->SetDead_Dying_NPC();

	m_vecNPC[5]->SetActorPos(Vec3{ -256.f,704.f,7.f });
	m_vecNPC[5]->SetActorImageMag(1.5f);
	m_vecNPC[5]->SetAniIntervalTime(0.25f);
	m_vecNPC[5]->SetHideType(CNPCBase::HIDE_DEADANI);
	m_vecNPC[5]->SetDead_Dying_NPC();

	m_vecNPC[6]->SetActorPos(Vec3{ 93.f,684.f,7.f });
	m_vecNPC[6]->SetActorImageMag(1.5f);
	m_vecNPC[6]->SetAniIntervalTime(0.25f);
	m_vecNPC[6]->SetHideType(CNPCBase::HIDE_DEADANI);
	m_vecNPC[6]->SetDead_Dying_NPC();

	m_vecNPC[7]->SetActorPos(Vec3{ 266.f,704.f,7.f });
	m_vecNPC[7]->SetActorImageMag(1.5f);
	m_vecNPC[7]->SetAniIntervalTime(0.25f);
	m_vecNPC[7]->SetHideType(CNPCBase::HIDE_DEADANI);
	m_vecNPC[7]->SetDead_Dying_NPC();

	m_vecNPC[8]->SetActorPos(Vec3{ 0.f,846.f,7.f });
	m_vecNPC[8]->SetActorImageMag(1.5f);
	m_vecNPC[8]->SetAniIntervalTime(0.25f);
	m_vecNPC[8]->CreateSubCollision(COLFIGURE(Vec2{ 0.f,-250.f }, Vec2{ 1000.f,975.f }), CT_RECT);
	m_vecNPC[8]->SetHideType(CNPCBase::HIDE_DEADANI);
	m_vecNPC[8]->SetDead_Dying_NPC();

	
	m_vecNPC[9]->SetActorPos(Vec3{ -672.f,64.f,7.f });
	m_vecNPC[9]->SetActorImageMag(1.5f);
	m_vecNPC[9]->CreatePlayerCollision(COLFIGURE(Vec2{ 0.f,-7.f }, Vec2{ 48.f,46.f }), CT_RECT);
	m_vecNPC[9]->CreateScriptCollision(COLFIGURE(Vec2{ 0.f,-67.f }, Vec2{ 50.f,192.f }), CT_RECT); 
	m_vecNPC[9]->SetStoreNPC(CNPCBase::WEAPON_STORE_NPC);

	m_vecNPC[10]->SetActorPos(Vec3{ 672.f,64.,7.f });
	m_vecNPC[10]->SetActorImageMag(1.5f);
	m_vecNPC[10]->CreatePlayerCollision(COLFIGURE(Vec2{ 0.f,-7.f }, Vec2{ 48.f,46.f }), CT_RECT);
	m_vecNPC[10]->CreateScriptCollision(COLFIGURE(Vec2{ 0.f,-67.f }, Vec2{ 50.f,192.f }), CT_RECT);
	m_vecNPC[10]->SetStoreNPC(CNPCBase::ARMOR_STORE_NPC);

	m_vecNPC[11]->SetActorPos(Vec3{ 128.f, 64.f,7.f });
	m_vecNPC[11]->SetActorImageMag(1.5f);
	m_vecNPC[11]->CreatePlayerCollision(COLFIGURE(Vec2{ 0.f,-7.f }, Vec2{ 48.f,46.f }), CT_RECT);
	m_vecNPC[11]->CreateScriptCollision(COLFIGURE(Vec2{ 0.f,-34.f }, Vec2{ 50.f,128.f }), CT_RECT);
	m_vecNPC[11]->SetStoreNPC(CNPCBase::ACCESSORIES_STORE_NPC);

	//m_pMapMgr->ChangeTileMap(TOWN);

	m_pScriptMgr->ScriptWindowOn(false);

	m_QuestClearObject = CreateActObject(_T("QuestClearObject"))->CreateCom<CQuestClearObject>();
	m_QuestClearObject->SetActorPos(Vec3{ 0.f,200.f,7.f });
	m_QuestClearObject->SetEventMgr(m_pEventMgr);

	m_StoryCurtain = CreateActObject(_T("StoryCurtain"))->CreateCom<CStoryCurtain>();
	m_StoryCurtain->SetActorPos(Vec3{ 0.f,0.f,7.f });

	m_StoreUI = CreateActObject(_T("StoreUI"))->CreateCom<CStoreUI>();
	m_StoreUI->SetActorPos(Vec3{ 0.f,0.f,7.f });
	m_StoreUI->CloseStoreUI();

	m_vecCharacterLineOpen.assign(MAXCHARACTERLINE, true);
	
	for (size_t i = LID; i < MAXCHARACTERLINE; i++)
		m_vecCharacterLineOpen[i] = false;

	
	m_QuestResultGuide = CreateActObject(_T("RewardGuide"))->CreateCom<CQuestRewardGuide>();
}
void  CGameMaster::SetMouseCursor(CMouseCursor* _MouseCursor)
{
	if (nullptr == _MouseCursor)
		return;

	m_MouseCursor = _MouseCursor;
}
void CGameMaster::ResetGameMaster()
{
	//m_MainMenuMgr;
	//m_pInventoryMgr;
	//m_pScriptMgr;
	m_vecCharacterLineOpen.assign(MAXCHARACTERLINE, true);

	for (size_t i = LID; i < MAXCHARACTERLINE; i++)
		m_vecCharacterLineOpen[i] = false;

	m_pEventMgr->ResetMgr();
	m_pMapMgr->ResetMgr();
}
void CGameMaster::Update()
{
	m_pScriptMgr->Update();
	m_pMapMgr->Update();


	if (m_bMainSceneStart)
		SceneStartUpdate();
	else if (m_bMapChangeUpdate)
		MapChangeUpdate();
	else if (m_bStoryCurtainWait)
		StoryCurtainUpdate();
	else 
		KeyCheck();
	
	if (nullptr != m_pEventUpdateFunc)
		m_pEventUpdateFunc();

	m_pInventoryMgr->Update();
}
void CGameMaster::LastUpdate()
{
	m_pScriptMgr->LastUpdate();

	if (m_QuestClearObject->IsLastQuest()) 
		CClientGlobal::MainPlayer->PlayerPause(true);
		
	

	if (m_bGameClear)
	{
		GameClearUpdate();
		return;
	}


	if (false == m_bBattleSceneTriger)
		return;

	// EventMgr가 전투씬 전환을 감지하게 되었을때
	// Triger변수를 참으로 바꾸는데
	// 이때 전투씬 전환에 필요한 작업을 모두 수행한뒤
	// ChangeScene()을 호출한다.
	CaptureScreen();
	CClientGlobal::MainPlayer->PlayerPause(true);
	CClientGlobal::GameMaster->BattleSceneChangeEvent();
	CClientGlobal::PrevScene = CClientGlobal::MAINSCENE;
	CClientGlobal::MainWindow->ChangeScene(CClientGlobal::SceneName[CClientGlobal::BATTLESCENE]);

	m_bBattleSceneTriger = false;
	ReleaseEffect();
}
void CGameMaster::WakeUpGameMaster()
{
	if(nullptr != m_pMapMgr)
		m_pMapMgr->SetCurTileMapDeugMode(m_pMainScene->GetDebugMgr()->IsDebugMode());

	m_SceneCurtain->SetAlpha(1.f);
	m_SceneCurtain->On();
	m_bMainSceneStart = true;
	m_fAccTime = 0.f;
	
	CClientGlobal::MainPlayer->SetPos(CClientGlobal::MainPlayer->GetActorPos());
	m_pMainScene->GetMainCamCom()->ZPosInit();
	
	PrevSceneCheck();
	//m_pMainScene->GetMainCamCom()->SetCamMode(CAM_MODE_TRACKING, CClientGlobal::MainPlayer->GetTransForm());

}
void CGameMaster::PrevSceneCheck()
{
	switch (CClientGlobal::PrevScene)
	{
	case CClientGlobal::LOADINGSCENE:
		break;
	case CClientGlobal::BATTLESCENE: 
	{
		m_pEventMgr->SceneChangeEvent(m_CurBattleQuestKey, m_CurBattleQuestCount);
		
		m_pMapMgr->MapMusicPause(false);
		m_pMapMgr->MusicOnEvent(true);
		if (CClientGlobal::StoryBattle) 
		{
			m_StoryCurtain->SetCurtain(true);
			m_StoryCurtain->RenderEnable(true);
		}
	}
		break;
	case CClientGlobal::MAINSCENE:
		break;
	case CClientGlobal::TITLESCENE: 
	{
		ResetGameMaster();
		CClientGlobal::MainPlayer->GameStart();
		m_pMapMgr->ChangeTileMap(TOWN);
		CClientGlobal::MainPlayer->SetPos(Vec3{ 0.f,0.f,5.0f });
		CClientGlobal::MainPlayer->PlayerPause(false);
	}
		break;
	case CClientGlobal::ENDINGSCENE:
		break;
	}
}
void CGameMaster::SceneStartUpdate()
{
	if (m_fAccTime >= 1.6f) 
	{
		m_bMainSceneStart = false;
		m_fAccTime = 0.f;
		m_SceneCurtain->Off();
		if(CClientGlobal::BATTLESCENE == CClientGlobal::PrevScene)
			BattleWinEvent();
		return;
	}

	float Alpha =  1.5f -  m_fAccTime;

	m_SceneCurtain->SetAlpha(Alpha);

	m_fAccTime += DELTATIME;
}
void CGameMaster::KeyCheck()
{
	if (true == KEYDOUBLE("MasterKey"))
		ChangeMasterMode();

	if (true == KEYDOWN("MenuKey") && false == m_MainMenuMgr->IsMenuActive())
	{
		if (true == CMainMenuMgr::bMenuOn)
		{
			m_MainMenuMgr->MenuMgrOn();
			CNPCBase::bScriptEvent = false;
		}
	}

	if (TRUE == KEYDOWN("ChangeDebugMode"))
	{
		m_pMainScene->GetDebugMgr()->ChangeDebugMode();
		m_pMapMgr->SetCurTileMapDeugMode(m_pMainScene->GetDebugMgr()->IsDebugMode());
	}

}
void CGameMaster::MapChangeUpdate()
{
	if (m_fAccWaitTime >= 1.5f) 
	{
		m_bMapChangeUpdate = false;
		m_SceneCurtain->SetAlpha(0.f);
		m_SceneCurtain->Off();
		CClientGlobal::MainPlayer->PlayerPause(false);
	}

	if (m_fAccWaitTime < 0.5f) 
	{
		float t = m_fAccWaitTime / 0.5f;
		m_SceneCurtain->SetAlpha(min(1.f,t));
	}
	else if (m_fAccWaitTime >= 0.5f && !m_bMapChange) 
	{
		m_bMapChange = true;
		
		switch (m_TrigerType)
		{
		case CGameMaster::MAPCHANGE_PLAYERTRIGER:
		{
			if (MAPCHANGETILE == m_ChangeTrigerTile->GetTileOption())
			{
				m_MainPlayer->SetInitTileIndex(m_ChangeTrigerTile->GetInitPortalIndex());
				m_pMapMgr->ChangeTileMap(m_ChangeTrigerTile->GetChangeMapNo());
				//CClientGlobal::MainPlayer->InitPlayerMoveState();
			}
			else if (WORLDMAPTILE == m_ChangeTrigerTile->GetTileOption())
				m_pMapMgr->ShowWolrdMapEvent(m_ChangeTrigerTile);
		}
		break;
		case CGameMaster::MAPCHANGE_WORLDMAPTRIGER:
		{
			POINT InitPosIndex = m_ChangeTrigerTile->GetMapChangeInitPoint(m_ChangeMap);

			if (InitPosIndex == POINT{ -1,-1 })
				return;


			CClientGlobal::MainPlayer->SetInitTileIndex(InitPosIndex);
			CClientGlobal::MainPlayer->On();
		

			if (nullptr != m_pMapMgr->ChangeTileMap(m_ChangeMap))
				m_pMapMgr->WorldMapActive(false);
		}
			break;
		}
	}
	else if (m_fAccWaitTime >= 1.0f && m_fAccWaitTime < 1.5f) 
	{
		float t = (1.5f - m_fAccWaitTime ) / 0.5f;
		m_SceneCurtain->SetAlpha(max(0.f, t));
	}

	m_fAccWaitTime += DELTATIME;
}
void CGameMaster::StoryCurtainUpdate()
{
	if (!m_StoryCurtain->IsCurtainUpdate())
	{
		m_bStoryCurtainWait = false;
		if (m_bBaseNPCTriger) 
		{		
			if(CStoryCurtain::STORY_START ==   m_StoryCurtain->GetCurEvent())
				m_pScriptMgr->ScriptWindowOn(true);
			else
				m_pScriptMgr->ScriptWindowOn(false);
			return;
		}

		m_pInventoryMgr->ActiveInventoryPopUp(true);
	}

}
void CGameMaster::GameClearUpdate()
{
	if (m_fAccTime >= 3.5f) 
	{
		m_SceneCurtain->SetAlpha(0.f);
		m_SceneCurtain->Off();

		m_fAccTime = 0.f;
		m_bGameClear = false;

		CClientGlobal::PrevScene = CClientGlobal::MAINSCENE;
		CClientGlobal::EndingBuilder->EndingEvent();
		CClientGlobal::MainWindow->ChangeScene(CClientGlobal::SceneName[CClientGlobal::ENDINGSCENE]);
		//CClientGlobal::EndingBuilder->GameOverEvent();
		//CClientGlobal::MainWindow->ChangeScene(CClientGlobal::SceneName[CClientGlobal::TITLESCENE]);
	}

	float Alpha = m_fAccTime / 3.5f;
	m_SceneCurtain->SetAlpha(Alpha);

	m_fAccTime += DELTATIME;
}
CActObject* CGameMaster::CreateActObject(const tstring& _Name)
{
	return CActObject::CreateActObject(_Name, m_pMainScene);
}
CGameScene* CGameMaster::GetMainScene()
{
	return m_pMainScene;
}
void CGameMaster::MapChangeEvent(SPTR<CTile> _Tile)
{
	if (nullptr != _Tile)
		m_ChangeTrigerTile = _Tile;
	
	m_TrigerType = MAPCHANGE_PLAYERTRIGER;
	m_bMapChangeUpdate = true;
	m_bMapChange = false;
	m_fAccWaitTime = 0.f;
	m_SceneCurtain->On();
	CClientGlobal::MainPlayer->PlayerPause(true);

}
void CGameMaster::MapChangeEvent(SPTR<CTile> _Tile, const MAINMAPNAME& _MapNo)
{
	if (nullptr != _Tile)
		m_ChangeTrigerTile = _Tile;

	m_TrigerType = MAPCHANGE_WORLDMAPTRIGER;
	m_ChangeMap = _MapNo;
	m_bMapChangeUpdate = true;
	m_bMapChange = false;
	m_fAccWaitTime = 0.f;
	m_SceneCurtain->On();
	CClientGlobal::MainPlayer->PlayerPause(true);
}
void CGameMaster::ReleaseGameMaster()
{
	SAFE_DELETE(m_pMapMgr);
}
void CGameMaster::InitCameraPos()
{
	m_pMainScene->GetMainCamCom()->InitPos();
}
void CGameMaster::BattleSceneChangeEvent()
{
	for (size_t i = 0; i < MAXTEAM; i++) 
	{
		if (m_CurTeam[i] >= MAXBATTLECHARACTER)
			CClientGlobal::AdvanceTeam[i] = -1;
		else 
			CClientGlobal::AdvanceTeam[i] = (int)m_CurTeam[i];
	}

	m_bStoryCurtainWait = false;
	m_pMapMgr->MapMusicPause(true);
}
void CGameMaster::SetCharacterTeam(const size_t& _TeamIndex, const size_t& _CharNo)
{
	if (_TeamIndex >= MAXTEAM || _CharNo >= CClientGlobal::CharacterName.size())
		return;

	m_CurTeam[_TeamIndex] = _CharNo;
}
void CGameMaster::PopCharacterTeam(const size_t& _TeamIndex)
{
	if (_TeamIndex >= MAXTEAM)
		return;

	m_CurTeam[_TeamIndex] = -1;
}
const bool CGameMaster::CheckCurBelongTeam(const size_t& _CharNo)
{
	for (size_t i = 0; i < MAXTEAM; i++)
	{	
		if (_CharNo == m_CurTeam[i])
			return true;
	}

	return false;
}
const size_t CGameMaster::GetCurTeam(const size_t& _TeamIndex)
{
	if (_TeamIndex >= MAXTEAM)
		return -1;

	return m_CurTeam[_TeamIndex];
}
const size_t CGameMaster::GetCurCharacter(const CHARACTERLINE& _LineNo)
{
	if (_LineNo >= CClientGlobal::CharacterLine.size())
		return -1;

	return CClientGlobal::CharacterLine[_LineNo][m_vecCurChar[_LineNo]].CharacterNo;
}
const CHARACTERDATA CGameMaster::GetCurCharData(const CHARACTERLINE& _LineNo)
{
	if (_LineNo >= CClientGlobal::CharacterLine.size())
		return CHARACTERDATA();

	return CClientGlobal::CharacterLine[_LineNo][m_vecCurChar[_LineNo]];
}
void CGameMaster::MenuOffEvnet()
{
	CNPCBase::bScriptEvent = true;
}
bool CGameMaster::SceneEffect(DWORD _Flag, const float& _Time)
{
	if (nullptr != m_pMainSceneBuilder)
		return m_pMainSceneBuilder->SceneEffect(_Flag, _Time);

	return false;
}
// 지진 Data 셋팅
const Vec2 CGameMaster::GetSpaceSize(DWORD _Power) 
{
	if (nullptr != m_pMainSceneBuilder)
		return m_pMainSceneBuilder->GetSpaceSize(_Power);

	return Vec2{ 0.f,0.f };
}
void CGameMaster::SetEarthquakeData(const Vec2& _SpaceSize, const float& _PulseTime /*= 0.02f*/)
{
	if (nullptr != m_pMainSceneBuilder)
		return m_pMainSceneBuilder->SetEarthquakeData(_SpaceSize, _PulseTime);
}
// Fade Effect Data 셋팅
void CGameMaster::SetFadeData(const float& _FadeTime) 
{
	if (nullptr != m_pMainSceneBuilder)
		return m_pMainSceneBuilder->SetFadeData(_FadeTime);
}
void CGameMaster::SetFadeData(const float& _FadeTime, const float& _Pow)
{
	if (nullptr != m_pMainSceneBuilder)
		return m_pMainSceneBuilder->SetFadeData(_FadeTime, _Pow);
}
// Zoom Effect Data 셋팅
void CGameMaster::SetZoomData(const Vec3& _ZoomPos, const float& _Near, const float& _ZoomTime) 
{
	if (nullptr != m_pMainSceneBuilder)
		return m_pMainSceneBuilder->SetZoomData(_ZoomPos, _Near, _ZoomTime);
}
void CGameMaster::SetZoomData(const Vec3& _ZoomPos, const Vec2& _ZoomRatio, const float& _ZoomTime) 
{
	if (nullptr != m_pMainSceneBuilder)
		return m_pMainSceneBuilder->SetZoomData(_ZoomPos, _ZoomRatio, _ZoomTime);
}
const bool CGameMaster::IsStortCurtainWait()
{
	return m_bStoryCurtainWait;
}
SPTR<CNPCBase> CGameMaster::GetNPC(const NPC& _Index)
{
	if (_Index >= NPC::MAXNPC)
		return nullptr;

	return m_vecNPC[_Index];
}
SPTR<CNPCBase> CGameMaster::GetNPC(const tstring& _NPCName)
{
	for (size_t i = 0; i < m_vecNPCData.size(); i++)
	{
		if (m_vecNPCData[i].NpcName == _NPCName)
			return m_vecNPC[i];
	}
	return  nullptr;
}
SPTR<CGameSprite> CGameMaster::GetNpcPoster(const NPC& _Index)
{
	if (_Index >= m_vecNPCData.size())
		return nullptr;

	return m_vecNPCData[_Index].NpcPoster;
}
SPTR<CGameSprite> CGameMaster::GetNPCPoster(const tstring& _NPCName)
{
	for (size_t i = 0; i < m_vecNPCData.size(); i++)
	{
		if (m_vecNPCData[i].NpcName == _NPCName)
			return m_vecNPCData[i].NpcPoster;
	}

	if (_NPCName == _T("Rain") || _NPCName == _T("레인"))
		return m_pMainSceneBuilder->GetResourceMgr()->FindGameSprite(_T("RainPoster"));
	
	return nullptr;
}

const tstring CGameMaster::GetNPCKName(const tstring& _NpcName)
{
	if (_T("Player") == _NpcName || _T("Rain") == _NpcName)
		return _T("레인");

	if (_T("SJ") == _NpcName)
		return _NpcName;

	for (size_t i = 0; i < m_vecNPCData.size(); i++)
	{
		if (m_vecNPCData[i].NpcName == _NpcName || m_vecNPCData[i].NpcKName == _NpcName)
			return m_vecNPCData[i].NpcKName;
	}

	return _T("");
}
void CGameMaster::EnableNPC(const NPC& _Index, const bool& _Enable /*= true*/)
{
	if (_Index >= m_vecNPC.size() || nullptr == m_vecNPC[_Index])
		return;

	m_vecNPC[_Index]->ActorActive(_Enable);
	
	if (_Enable)
		m_vecNPC[_Index]->Reset();
	else
		m_vecNPC[_Index]->HideEventEnd();

	return;
}
SPTR<CStoreUI>	CGameMaster::GetStoreUI()
{
	return m_StoreUI;
}
void CGameMaster::ChangeNPCDyingAni(const UINT& _NPCNo)
{
	if (_NPCNo >= m_vecNPC.size())
		return;

	if (nullptr != m_vecNPC[_NPCNo])
		m_vecNPC[_NPCNo]->ChangeAni_Dying();

	return;
}
void CGameMaster::ChangeNPCDeadAni(const UINT& _NPCNo)
{
	if (_NPCNo >= m_vecNPC.size())
		return;

	if (nullptr != m_vecNPC[_NPCNo])
		m_vecNPC[_NPCNo]->ChangeAni_Dead();

	return;
}
void CGameMaster::ShowScriptEvent(const tstring& _NPCName, const bool& _bDialog/* = true*/)
{
	if (nullptr == m_pEventMgr || nullptr == m_pScriptMgr)
		return;

	for (size_t i = 0; i < m_vecNPCData.size(); i++)
	{
		if (_NPCName == m_vecNPCData[i].NpcName)
		{
			m_pScriptMgr->SetScriptPoster(m_vecNPCData[i].NpcPoster);
			m_pScriptMgr->SetCurNPC(m_vecNPC[i]);

			if (-1 != m_pEventMgr->CheckQuestEvent((NPC)i))
			{
				m_pEventMgr->SetNPCName(m_vecNPCData[i].NpcKName);
				m_pEventMgr->SettingQuestScript();
				m_bBaseNPCTriger = _bDialog;
				if (m_bBaseNPCTriger) 
				{
					m_bStoryCurtainWait = true;
					m_StoryCurtain->StoryEvent(true);
				}
				else 
					m_pScriptMgr->ScriptWindowOn(true);
				
			}
			else 
			{
				m_pScriptMgr->SetBaseScript(m_vecNPCData[i].NpcName, m_vecNPCData[i].NpcKName);
				m_pScriptMgr->ScriptWindowOn(true);
			}

			return;
		}
	
	}
}
void CGameMaster::ShowScript()
{
	m_pScriptMgr->ScriptWindowOn(true);
}
void CGameMaster::MenuMgrEventOn(const bool& _On)
{
	CMainMenuMgr::bMenuOn = _On;
}
void CGameMaster::FinishScriptEvent()
{
	MenuMgrEventOn(true);

	if (true == m_pMapMgr->IsMapBGMPause())
		m_pMapMgr->MusicOnEvent(true);

	if (nullptr != m_pEventMgr && true == m_pEventMgr->QuestScriptEndEvent())
	{
		m_StoryCurtain->StoryEvent(false);
		m_bStoryCurtainWait = true;
	}

}
void CGameMaster::FinishQuestEvent(const tstring& _QuestName)
{
	if (nullptr == m_QuestClearObject)
		return;

	m_QuestClearObject->StandBy(_QuestName);

}
void CGameMaster::FinishLastQuestEvent()
{
	m_bGameClear = true;
	m_SceneCurtain->SetAlpha(0.f);
	m_SceneCurtain->On();
	m_pMapMgr->MusicOnEvent(false);
}
void CGameMaster::EventTriger(const tstring& _NPCName)
{
	MenuMgrEventOn(false);

	if (nullptr == m_pEventMgr)
		return;

	SetEventUpdateFunc(_NPCName);

	CClientGlobal::MainPlayer->PlayerPause(true);
	m_pInventoryMgr->ActiveInventoryPopUp(false);
	// 메인씬의 카메라 움직임 모드를 추적하지 않는 상태로 만든다.
	m_pMainScene->GetMainCamCom()->SetCamMode(CAM_MODE_FIX);
	m_bStoryCurtainWait = true;
	m_StoryCurtain->StoryEvent(true);
	m_bBaseNPCTriger = false;
}
void CGameMaster::SetEventUpdateFunc(const tstring& _NPCName)
{
	m_pEventUpdateFunc = m_pEventMgr->GetEventUpdateFunc(_NPCName);
	m_pEventMgr->QuestUpdateStandBy(_NPCName);
}
void CGameMaster::FinishEventUpdate(const tstring& _NPCName)
{
	m_pEventUpdateFunc = nullptr;
	CClientGlobal::GameMaster->ShowScriptEvent(_NPCName,false);
}
void CGameMaster::BattleSceneChangeEvent(const tstring& _QuestKey, const UINT& _QuestCount)
{
	m_CurBattleQuestKey = _QuestKey;
	m_CurBattleQuestCount = _QuestCount;
	m_StoryCurtain->RenderEnable(false);

	m_bBattleSceneTriger = true;
}
void CGameMaster::SettingBattleMonster(const BATTLECOUNT& _Count)
{
	switch (_Count)
	{
	case CGameMaster::BATTLE_THIEF: 
	{
		CClientGlobal::CurBattleEndRound = 2;

		CClientGlobal::strMonsterTeam[0][0] = _T("");
		CClientGlobal::strMonsterTeam[0][1] = _T("Thief1");
		CClientGlobal::strMonsterTeam[0][2] = _T("Thief2");

		CClientGlobal::strMonsterTeam[1][0] = _T("Thief3");
		CClientGlobal::strMonsterTeam[1][1] = _T("Thief1");
		CClientGlobal::strMonsterTeam[1][2] = _T("Thief2");

		CClientGlobal::strMonsterTeam[2][0] = _T("Destroyer");
		CClientGlobal::strMonsterTeam[2][1] = _T("");
		CClientGlobal::strMonsterTeam[2][2] = _T("");
	}
		break;
	case CGameMaster::BATTLE_DARKCOMMANDER: 
	{
		CClientGlobal::CurBattleEndRound = 2;

		CClientGlobal::strMonsterTeam[0][0] = _T("DarkCommander4");
		CClientGlobal::strMonsterTeam[0][1] = _T("");
		CClientGlobal::strMonsterTeam[0][2] = _T("");

		CClientGlobal::strMonsterTeam[1][0] = _T("DarkCommander2");
		CClientGlobal::strMonsterTeam[1][1] = _T("");
		CClientGlobal::strMonsterTeam[1][2] = _T("");

		CClientGlobal::strMonsterTeam[2][0] = _T("DarkCommander3");
		CClientGlobal::strMonsterTeam[2][1] = _T("");
		CClientGlobal::strMonsterTeam[2][2] = _T("DarkCommander1");
	}
		break;
	case CGameMaster::BATTLE_BOSS: 
	{
		CClientGlobal::CurBattleEndRound = 0;

		CClientGlobal::strMonsterTeam[0][0] = _T("FinalBoss");
		CClientGlobal::strMonsterTeam[0][1] = _T("");
		CClientGlobal::strMonsterTeam[0][2] = _T("");
	}
		break;
	case CGameMaster::BATTLE_SLIME:
	{
		CClientGlobal::CurBattleEndRound = 0;

		CClientGlobal::strMonsterTeam[0][0] = _T("AzhiDahaka");
		CClientGlobal::strMonsterTeam[0][1] = _T("");
		CClientGlobal::strMonsterTeam[0][2] = _T("");
	}
	break;
	}
}
void CGameMaster::CaptureScreen()
{
	if (nullptr == CClientGlobal::MainWindow)
		return;

	LPDIRECT3DDEVICE9 Device = CClientGlobal::MainWindow->GetLPDevice();
	if (nullptr == Device)
		return;


	POINT WndSize = CClientGlobal::MainWindow->WndSize();
	LPDIRECT3DSURFACE9 Surf;

	Device->CreateOffscreenPlainSurface(WndSize.x, WndSize.y
		, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT
		, &Surf, NULL);

	Device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &Surf);
	Device->GetFrontBufferData(NULL, Surf);
	
	tstring Path = CFilePathMgr::GetPath(_T("Texture"));
	Path += _T("TempScreen.bmp");

	D3DXSaveSurfaceToFile(Path.c_str(), D3DXIFF_BMP, Surf, NULL, NULL);
	
	return;
}
void CGameMaster::BattleWin(const bool& _Win)
{
	m_bBattleWin = _Win;
}
void CGameMaster::BattleWinEvent()
{
	if (false == m_bBattleWin)
		return;

	if (nullptr == m_pEventMgr)
		return;

	m_pEventMgr->BattleWinEvent(m_CurBattleQuestKey, m_CurBattleQuestCount);
}
void CGameMaster::EffectOn(const tstring& _EffectKey, const Vec3& _Pos, const bool& _CharBack /*= true*/)
{
	SPTR<CEffect> NewEffect = CreateActObject(_T("Effect"))->CreateCom<CEffect>(CClientGlobal::GetEffectData(_EffectKey));

	if (-1 == NewEffect->GetEffectIndex())
		return;
 
	bool Check = CEffect::bCurCharacterSkill;

	CEffect::SettingCurEffectLayer(_CharBack);
	NewEffect->EffectOn();
	NewEffect->SetPos(_Pos);

	m_listEffect.push_back(CreateActObject(_T("Effect"))->CreateCom<CEffect>(CClientGlobal::GetEffectData(_EffectKey)));

}
void CGameMaster::ReleaseEffect()
{
	list<SPTR<CEffect>>::iterator Start = m_listEffect.begin();
	list<SPTR<CEffect>>::iterator End = m_listEffect.end();

	for (; Start!= End; )
	{
		if (nullptr != (*Start) && !(*Start)->IsPlay())
			Start = m_listEffect.erase(Start);
		else
			++Start;
	}
	return;
}
void CGameMaster::StoreOpenEvent(const STORETYPE& _Type)
{
	if (_Type >= MAXSTORE)
		return;

	if (m_StoreUI->IsActorOn())
		return;

	m_StoreUI->OpenStoreUI();
	m_StoreUI->StandBy(_Type);
	m_MainPlayer->PlayerPause(true);
	m_MainMenuMgr->bMenuOn = false;
	m_pInventoryMgr->ActiveInventoryPopUp(false);
}
void CGameMaster::StoreCloseEvent()
{
	m_StoreUI->CloseStoreUI();
	m_MainPlayer->PlayerPause(false);
	m_MainMenuMgr->bMenuOn = true;
	m_pInventoryMgr->ActiveInventoryPopUp(true);
}
void CGameMaster::InventoryEvent(const bool& _Open)
{
	if (_Open) 
	{
		//m_MainPlayer->PlayerPause(true);
		m_MainMenuMgr->bMenuOn = false;
	}
	else 
	{
		//m_MainPlayer->PlayerPause(false);
		m_MainMenuMgr->bMenuOn = true;
	}
}
SPTR<CMouseCursor> CGameMaster::GetMouseCursor()
{
	return m_MouseCursor;
}
CEventMgr* CGameMaster::GetEventMgr()
{
	return m_pEventMgr;
}
void CGameMaster::OpenCharacterLine(const CHARACTERLINE& _LineNo, const bool& _Open /*true*/)
{
	if (_LineNo >= m_vecCharacterLineOpen.size())
		return;

	m_vecCharacterLineOpen[_LineNo] = _Open;
	m_QuestResultGuide->SetGuideMode(LID);
	m_QuestResultGuide->StandBy();
}
void CGameMaster::MapSoundPauseEvent()
{
	if (nullptr == m_pMapMgr)
		return;

	m_pMapMgr->MapMusicPause(true);
}
const bool CGameMaster::IsOpenCharacterLine(const CharacterIndex& _CharIndex)
{
	CHARACTERLINE No = (CHARACTERLINE)CClientGlobal::FindCharacterLineNo(_CharIndex);

	if (No >= MAXCHARACTERLINE)
		return  false;

	return m_vecCharacterLineOpen[No];
}
const bool CGameMaster::IsOpenCharacterLine(const tstring& _CharName)
{
	CHARACTERLINE No = (CHARACTERLINE)CClientGlobal::FindCharacterLineNoForLineName(_CharName);

	if (No >= MAXCHARACTERLINE)
		return  false;

	return m_vecCharacterLineOpen[No];
}
const bool CGameMaster::IsOpenCharacterLine(const CHARACTERLINE& _LineNo)
{
	if (_LineNo >= MAXCHARACTERLINE)
		return  false;

	return m_vecCharacterLineOpen[_LineNo];
}