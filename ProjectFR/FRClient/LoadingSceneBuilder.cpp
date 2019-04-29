#include "Precom.h"
#include "LoadingSceneBuilder.h"
#include <SceneCollisionMgr.h>
#include<GameSprite.h>
#include<GameMultiSprite.h>
#include "PlayerLogic.h"
#include"Monster.h"
#include"BackGround.h"
#include"TestMonster.h"
#include"MouseCursor.h"
#include"UnitStatusBar.h"
#include"NumberUI.h"
#include"TestBox.h"
#include"Effect.h"
#include<ScriptRenderer.h>


CLoadingSceneBuilder::CLoadingSceneBuilder()
	:m_iCurHandleIndex(0)
{
	memset(m_pHandle, 0, sizeof(HANDLE)*MAXHANDLE);
}


CLoadingSceneBuilder::~CLoadingSceneBuilder()
{

}
bool CLoadingSceneBuilder::SceneBuild()
{
	CClientGlobal::LoadItemData();
	SceneThreadLoading();

	CreateMainCamera();
	CreateMainLight();
	//SceneSerialLoading();
	
	return TRUE;
}
void CLoadingSceneBuilder::LoadingTexture()
{

	RESMGR->LoadTextureInFolder(_T("PreLoad"));
	RESMGR->LoadGameTexture(CFilePathMgr::FilePath(_T("Texture"), _T("Map_Rain.png")));
	RESMGR->LoadGameTexture(CFilePathMgr::FilePath(_T("Texture"), _T("Map_Lid.png")));


	//m_pHandle[m_iCurHandleIndex++] = CThreadMgr::CreateGlobvalThread(AllTextureLoading);

	/*for (size_t i = 0; i < CClientGlobal::FolderName.size(); i++)
	{
	m_pHandle[m_iCurHandleIndex++] = CThreadMgr::CreateGlobvalThread(TexThreadLoading, &CClientGlobal::FolderName[i]);
	}*/

	m_pHandle[m_iCurHandleIndex++] = CThreadMgr::CreateGlobvalThread(TexThreadLoading, &CClientGlobal::FolderName[EFFECT]);

	ThreadWaiting();

	m_pHandle[m_iCurHandleIndex++] = CThreadMgr::CreateGlobvalThread(TexThreadLoading, &CClientGlobal::FolderName[CHARACTER]);
	m_pHandle[m_iCurHandleIndex++] = CThreadMgr::CreateGlobvalThread(TexThreadLoading, &CClientGlobal::FolderName[MONSTER]);
	m_pHandle[m_iCurHandleIndex++] = CThreadMgr::CreateGlobvalThread(TexThreadLoading, &CClientGlobal::FolderName[UI]);
	m_pHandle[m_iCurHandleIndex++] = CThreadMgr::CreateGlobvalThread(TexThreadLoading, &CClientGlobal::FolderName[MAP]);


}
void CLoadingSceneBuilder::CreateSprite()
{
	set<tstring>::iterator Start = CClientGlobal::MonsterName.begin();
	set<tstring>::iterator End = CClientGlobal::MonsterName.end();

	for (; Start != End; ++Start)
	{
		RESMGR->CreateGameSprite(Icon(*Start));
		RESMGR->CreateGameSprite(Frame(*Start));
	}

	for (size_t i = 0; i < CClientGlobal::CharacterName.size(); i++)
	{
		RESMGR->CreateGameSprite(Icon(CClientGlobal::CharacterName[i].UnitName));
		RESMGR->CreateGameSprite(Frame(CClientGlobal::CharacterName[i].UnitName));
		RESMGR->CreateGameSprite(Illust(CClientGlobal::CharacterName[i].UnitName));
	}


	SPTR<CGameSprite> Mouse = RESMGR->CreateGameSprite(_T("BlueCursor"));
	Mouse->SetSpritePivot({ Mouse->GetSpriteSize().x * -0.5f, Mouse->GetSpriteSize().y*0.5f });
	RESMGR->CreateGameSprite(_T("BarBack"));


	for (int i = 0; i < 13; i++)
	{
		TCHAR BackName[128];
		_stprintf_s(BackName, _T("BattleBack%d"), i);
		RESMGR->CreateGameSprite(BackName);
	}


	//// 검은색 숫자 Sprite를 만듦(한 Texture에 크기가 서로다른 Sprite라서,나중에 MultiSprite 합성)
	vector<SPTR<CGameSprite>> BlackNumber;
	BlackNumber.reserve(10);

	BlackNumber.push_back(RESMGR->CreateGameSprite(_T("Number_Black"), _T("Number_Black0"), { 0.0f,0.0f }, { 50.75f,64.0f }));
	BlackNumber.push_back(RESMGR->CreateGameSprite(_T("Number_Black"), _T("Number_Black1"), { 50.75f,0.0f }, { 34.22f,64.0f }));
	BlackNumber.push_back(RESMGR->CreateGameSprite(_T("Number_Black"), _T("Number_Black2"), { 84.97f,0.0f }, { 55.1f,64.0f }));
	BlackNumber.push_back(RESMGR->CreateGameSprite(_T("Number_Black"), _T("Number_Black3"), { 140.07f,0.0f }, { 51.62f,64.0f }));
	BlackNumber.push_back(RESMGR->CreateGameSprite(_T("Number_Black"), _T("Number_Black4"), { 191.69f,0.0f }, { 53.36f,64.0f }));
	BlackNumber.push_back(RESMGR->CreateGameSprite(_T("Number_Black"), _T("Number_Black5"), { 245.05f,0.0f }, { 53.36f,64.0f }));
	BlackNumber.push_back(RESMGR->CreateGameSprite(_T("Number_Black"), _T("Number_Black6"), { 298.41f,0.0f }, { 51.62f,64.0f }));
	BlackNumber.push_back(RESMGR->CreateGameSprite(_T("Number_Black"), _T("Number_Black7"), { 350.03f,0.0f }, { 53.36f,64.0f }));
	BlackNumber.push_back(RESMGR->CreateGameSprite(_T("Number_Black"), _T("Number_Black8"), { 403.39f,0.0f }, { 52.2f,64.0f }));
	BlackNumber.push_back(RESMGR->CreateGameSprite(_T("Number_Black"), _T("Number_Black9"), { 455.59f,0.0f }, { 51.62f,64.0f }));

	vector<SPTR<CGameSprite>> RedNumber;
	RedNumber.push_back(RESMGR->CreateGameSprite(_T("Number_Red"), _T("Number_Red0"), { 0.0f,0.0f }, { 94.0f,113.0f }));
	RedNumber.push_back(RESMGR->CreateGameSprite(_T("Number_Red"), _T("Number_Red1"), { 98.0f,0.0f }, { 77.0f,113.0f }));
	RedNumber.push_back(RESMGR->CreateGameSprite(_T("Number_Red"), _T("Number_Red2"), { 175.0f,0.0f }, { 92.0f,113.0f }));
	RedNumber.push_back(RESMGR->CreateGameSprite(_T("Number_Red"), _T("Number_Red3"), { 268.0f,0.0f }, { 94.0f,113.0f }));
	RedNumber.push_back(RESMGR->CreateGameSprite(_T("Number_Red"), _T("Number_Red4"), { 363.f,0.0f }, { 89.0f,113.0f }));
	RedNumber.push_back(RESMGR->CreateGameSprite(_T("Number_Red"), _T("Number_Red5"), { 452.0f,0.0f }, { 95.0f,113.0f }));
	RedNumber.push_back(RESMGR->CreateGameSprite(_T("Number_Red"), _T("Number_Red6"), { 547.0f,0.0f }, { 90.0f,113.0f }));
	RedNumber.push_back(RESMGR->CreateGameSprite(_T("Number_Red"), _T("Number_Red7"), { 637.0f,0.0f }, { 96.0f,113.0f }));
	RedNumber.push_back(RESMGR->CreateGameSprite(_T("Number_Red"), _T("Number_Red8"), { 734.0f,0.0f }, { 95.0f,113.0f }));
	RedNumber.push_back(RESMGR->CreateGameSprite(_T("Number_Red"), _T("Number_Red9"), { 829.0f,0.0f }, { 92.0f,113.0f }));

	vector<SPTR<CGameSprite>> BlueNumber;
	BlueNumber.push_back(RESMGR->CreateGameSprite(_T("Number_Blue"), _T("Number_Blue0"), { 0.0f,0.0f }, { 93.0f,111.0f }));
	BlueNumber.push_back(RESMGR->CreateGameSprite(_T("Number_Blue"), _T("Number_Blue1"), { 94.0f,0.0f }, { 79.0f,111.0f }));
	BlueNumber.push_back(RESMGR->CreateGameSprite(_T("Number_Blue"), _T("Number_Blue2"), { 172.0f,0.0f }, { 91.0f,111.0f }));
	BlueNumber.push_back(RESMGR->CreateGameSprite(_T("Number_Blue"), _T("Number_Blue3"), { 263.0f,0.0f }, { 94.0f,111.0f }));
	BlueNumber.push_back(RESMGR->CreateGameSprite(_T("Number_Blue"), _T("Number_Blue4"), { 358.0f,0.0f }, { 86.0f,111.0f }));
	BlueNumber.push_back(RESMGR->CreateGameSprite(_T("Number_Blue"), _T("Number_Blue5"), { 444.0f,0.0f }, { 96.0f,111.0f }));
	BlueNumber.push_back(RESMGR->CreateGameSprite(_T("Number_Blue"), _T("Number_Blue6"), { 540.0f,0.0f }, { 90.0f,111.0f }));
	BlueNumber.push_back(RESMGR->CreateGameSprite(_T("Number_Blue"), _T("Number_Blue7"), { 630.0f,0.0f }, { 96.0f,111.0f }));
	BlueNumber.push_back(RESMGR->CreateGameSprite(_T("Number_Blue"), _T("Number_Blue8"), { 727.0f,0.0f }, { 90.0f,111.0f }));
	BlueNumber.push_back(RESMGR->CreateGameSprite(_T("Number_Blue"), _T("Number_Blue9"), { 818.0f,0.0f }, { 90.0f,111.0f }));

	vector<SPTR<CGameSprite>> YellowNumber;
	YellowNumber.push_back(RESMGR->CreateGameSprite(_T("Number_Yellow"), _T("Number_Yellow0"), { 0.0f,0.0f },   { 47.0f,59.0f }));
	YellowNumber.push_back(RESMGR->CreateGameSprite(_T("Number_Yellow"), _T("Number_Yellow1"), { 51.0f,0.0f },  { 38.0f,59.0f }));
	YellowNumber.push_back(RESMGR->CreateGameSprite(_T("Number_Yellow"), _T("Number_Yellow2"), { 95.0f,0.0f },  { 47.0f,59.0f }));
	YellowNumber.push_back(RESMGR->CreateGameSprite(_T("Number_Yellow"), _T("Number_Yellow3"), { 144.0f,0.0f }, { 46.0f,59.0f }));
	YellowNumber.push_back(RESMGR->CreateGameSprite(_T("Number_Yellow"), _T("Number_Yellow4"), { 191.0f,0.0f }, { 47.0f,59.0f }));
	YellowNumber.push_back(RESMGR->CreateGameSprite(_T("Number_Yellow"), _T("Number_Yellow5"), { 239.0f,0.0f }, { 47.0f,59.0f }));
	YellowNumber.push_back(RESMGR->CreateGameSprite(_T("Number_Yellow"), _T("Number_Yellow6"), { 287.0f,0.0f }, { 47.0f,59.0f }));
	YellowNumber.push_back(RESMGR->CreateGameSprite(_T("Number_Yellow"), _T("Number_Yellow7"), { 335.0f,0.0f }, { 47.0f,59.0f }));
	YellowNumber.push_back(RESMGR->CreateGameSprite(_T("Number_Yellow"), _T("Number_Yellow8"), { 383.0f,0.0f }, { 48.0f,59.0f }));
	YellowNumber.push_back(RESMGR->CreateGameSprite(_T("Number_Yellow"), _T("Number_Yellow9"), { 431.0f,0.0f }, { 47.0f,59.0f }));



	RESMGR->CreateMultiSprite(_T("Number_Black"), &BlackNumber);
	RESMGR->CreateMultiSprite(_T("Number_Red"), &RedNumber);
	RESMGR->CreateMultiSprite(_T("Number_Blue"), &BlueNumber);
	RESMGR->CreateMultiSprite(_T("Number_Yellow"), &YellowNumber);

	BlackNumber.clear();
	RedNumber.clear();
	BlueNumber.clear();

	RESMGR->CreateGameSprite(_T("LevelBox"));


	RESMGR->CreateGameSprite(_T("SelectCircle"));
	RESMGR->CreateGameSprite(_T("CharBarBack"));
	RESMGR->CreateGameSprite(_T("MonBarBack"));
	RESMGR->CreateGameSprite(_T("GaugeBase"));
	RESMGR->CreateGameSprite(_T("GaugeFrame"));
	RESMGR->CreateGameSprite(_T("PortraitFrame"));
	RESMGR->CreateGameSprite(_T("ActionIconBack"));
	RESMGR->CreateGameSprite(_T("TabBase"));
	RESMGR->CreateGameSprite(_T("SkillUIBack"));
	RESMGR->CreateGameSprite(_T("SkillUIFrame"));
	RESMGR->CreateGameSprite(_T("TileBase"));
	RESMGR->CreateGameSprite(_T("TileXBase"));
	RESMGR->CreateGameSprite(_T("Town"));
	RESMGR->CreateGameSprite(_T("Ruins"));
	RESMGR->CreateGameSprite(_T("Forest"));
	RESMGR->CreateGameSprite(_T("DevilCastle"));
	RESMGR->CreateGameSprite(_T("WeaponStore"));
	RESMGR->CreateGameSprite(_T("ItemStore"));
	RESMGR->CreateGameSprite(_T("WorldMap"));
	RESMGR->CreateGameSprite(_T("WorldNoFocus"));
	RESMGR->CreateGameSprite(_T("WorldFocus"));
	RESMGR->CreateGameSprite(_T("WorldNoActive"));
	RESMGR->CreateGameSprite(_T("ColorBase"));
	RESMGR->CreateGameSprite(_T("ResultBackFrame"));
	RESMGR->CreateGameSprite(_T("HPFrame"));
	RESMGR->CreateGameSprite(_T("MPFrame"));
	RESMGR->CreateGameSprite(_T("XPFrame"));
	RESMGR->CreateGameSprite(_T("LevelUpArrow"));
	RESMGR->CreateGameSprite(_T("LevelUpFont"));
	RESMGR->CreateGameSprite(_T("ObjectDebug"));
	RESMGR->CreateGameSprite(_T("MenuMainBack"));
	RESMGR->CreateGameSprite(_T("MenuSubBack"));
	RESMGR->CreateGameSprite(_T("RootGoldBack"));
	RESMGR->CreateGameSprite(_T("StatusMainBack"));
	RESMGR->CreateGameSprite(_T("StatusSubBack"));
	RESMGR->CreateGameSprite(_T("ParameterBack"));
	RESMGR->CreateGameSprite(_T("StatGaugeFrame"));
	RESMGR->CreateGameSprite(_T("XPFrameXL"));
	RESMGR->CreateGameSprite(_T("SkillXPFrame"));
	RESMGR->CreateGameSprite(_T("ActionDetail"));
	RESMGR->CreateGameSprite(_T("AbilityDetail"));
	RESMGR->CreateGameSprite(_T("BurstDetail"));
	RESMGR->CreateGameSprite(_T("SkillLevelUpFrame"));
	RESMGR->CreateGameSprite(_T("SkillLevelUpBack"));
	RESMGR->CreateGameSprite(_T("SkillLevelUpGuideFrame"));
	RESMGR->CreateGameSprite(_T("EquipTab"));
	RESMGR->CreateGameSprite(_T("SimpleEquipTabFrame"));
	RESMGR->CreateGameSprite(_T("EquipMenuBack"));
	RESMGR->CreateGameSprite(_T("EquipMenuBack2"));
	RESMGR->CreateGameSprite(_T("NoItem"));
	RESMGR->CreateGameSprite(_T("MountItemFail"));
	RESMGR->CreateGameSprite(_T("QuestFrame"));
	RESMGR->CreateGameSprite(_T("Studio"));
	RESMGR->CreateGameSprite(_T("Project"));
	RESMGR->CreateGameSprite(_T("GameTitle"));
	RESMGR->CreateGameSprite(_T("GameStart"));
	RESMGR->CreateGameSprite(_T("GameStartFrame"));
	RESMGR->CreateGameSprite(_T("GameOver"));
	RESMGR->CreateGameSprite(_T("BossGaugeFrame"));
	RESMGR->CreateGameSprite(_T("QuestTitleBack"));
	RESMGR->CreateGameSprite(_T("QuestBack"));
	RESMGR->CreateGameSprite(_T("StoryQuest"));
	RESMGR->CreateGameSprite(_T("EventQuest"));
	
	RESMGR->CreateGameSprite(_T("LidPoster"), Vec2{ 0.12f,0.f }, Vec2{ 256.f,255.5f });
	RESMGR->CreateGameSprite(_T("RainPoster"), Vec2{ 0.12f,0.f }, Vec2{ 256.f,255.5f });
	RESMGR->CreateGameSprite(_T("GolbezPoster"), Vec2{ 0.1f,0.f }, Vec2{ 256.f,255.5f });

	RESMGR->CreateGameSprite(_T("NPCSprite"), _T("Girls_Idle_Down")	, Vec2{ 48.0f,0.f }, Vec2{ 48.f,48.f });
	RESMGR->CreateGameSprite(_T("NPCSprite"), _T("Girls_Idle_Left")	, Vec2{ 48.0f,48.f }, Vec2{ 48.f,48.f });
	RESMGR->CreateGameSprite(_T("NPCSprite"), _T("Girls_Idle_Right"), Vec2{ 48.0f,96.f }, Vec2{ 48.f,48.f });
	RESMGR->CreateGameSprite(_T("NPCSprite"), _T("Girls_Idle_Up")	, Vec2{ 48.0f,144.f }, Vec2{ 48.f,48.f });
	RESMGR->CreateGameSprite(_T("NPCPoster"), _T("GirlsPoster"), Vec2{ 0.f,0.f }, Vec2{ 256.f,256.f });


	RESMGR->CreateGameSprite(_T("NPCSprite"), _T("Elder_Idle_Down")	, Vec2{ 192.f,0.f }, Vec2{ 48.f,48.f });
	RESMGR->CreateGameSprite(_T("NPCSprite"), _T("Elder_Idle_Left")	, Vec2{ 192.f,48.f }, Vec2{ 48.f,48.f });
	RESMGR->CreateGameSprite(_T("NPCSprite"), _T("Elder_Idle_Right"), Vec2{ 192.f,96.f }, Vec2{ 48.f,48.f });
	RESMGR->CreateGameSprite(_T("NPCSprite"), _T("Elder_Idle_Up")	, Vec2{ 192.f,144.f }, Vec2{ 48.f,48.f });
	RESMGR->CreateGameSprite(_T("NPCPoster"), _T("ElderPoster"), Vec2{ 256.f,0.f }, Vec2{ 256.f,256.f });

	RESMGR->CreateGameSprite(_T("NPCSprite"), _T("WeaponStoreNPC_Idle_Down"),	Vec2{ 336.f,0.f }, Vec2{ 48.f,48.f });
	RESMGR->CreateGameSprite(_T("NPCSprite"), _T("WeaponStoreNPC_Idle_Left"),	Vec2{ 336.f,48.f }, Vec2{ 48.f,48.f });
	RESMGR->CreateGameSprite(_T("NPCSprite"), _T("WeaponStoreNPC_Idle_Right"),	Vec2{ 336.f,96.f }, Vec2{ 48.f,48.f });
	RESMGR->CreateGameSprite(_T("NPCSprite"), _T("WeaponStoreNPC_Idle_Up"),		Vec2{ 336.f,144.f }, Vec2{ 48.f,48.f });

	RESMGR->CreateGameSprite(_T("NPCSprite"), _T("ArmorStoreNPC_Idle_Down"),	Vec2{ 48.0f,192.f }, Vec2{ 48.f,48.f });
	RESMGR->CreateGameSprite(_T("NPCSprite"), _T("ArmorStoreNPC_Idle_Left"),	Vec2{ 48.0f,240.f }, Vec2{ 48.f,48.f });
	RESMGR->CreateGameSprite(_T("NPCSprite"), _T("ArmorStoreNPC_Idle_Right"),	Vec2{ 48.0f,288.f }, Vec2{ 48.f,48.f });
	RESMGR->CreateGameSprite(_T("NPCSprite"), _T("ArmorStoreNPC_Idle_Up"),		Vec2{ 48.0f,336.f }, Vec2{ 48.f,48.f });

	/*
	RESMGR->CreateGameSprite(_T("NPCSprite"), _T("ArmorStoreNPC_Idle_Down"),	Vec2{ 336.f,192.f }, Vec2{ 48.f,48.f });
	RESMGR->CreateGameSprite(_T("NPCSprite"), _T("ArmorStoreNPC_Idle_Left"),	Vec2{ 336.f,240.f }, Vec2{ 48.f,48.f });
	RESMGR->CreateGameSprite(_T("NPCSprite"), _T("ArmorStoreNPC_Idle_Right"),	Vec2{ 336.f,288.f }, Vec2{ 48.f,48.f });
	RESMGR->CreateGameSprite(_T("NPCSprite"), _T("ArmorStoreNPC_Idle_Up"),		Vec2{ 336.f,336.f }, Vec2{ 48.f,48.f });
	*/
	RESMGR->CreateGameSprite(_T("NPCSprite"), _T("AccessoriesStoreNPC_Idle_Down"),	Vec2{ 192.f,192.f }, Vec2{ 48.f,48.f });
	RESMGR->CreateGameSprite(_T("NPCSprite"), _T("AccessoriesStoreNPC_Idle_Left"),	Vec2{ 192.f,240.f }, Vec2{ 48.f,48.f });
	RESMGR->CreateGameSprite(_T("NPCSprite"), _T("AccessoriesStoreNPC_Idle_Right"), Vec2{ 192.f,288.f }, Vec2{ 48.f,48.f });
	RESMGR->CreateGameSprite(_T("NPCSprite"), _T("AccessoriesStoreNPC_Idle_Up"),	Vec2{ 192.f,336.f }, Vec2{ 48.f,48.f });



	RESMGR->CreateGameSprite(_T("TargetCircle"), _T("UpTargetCircle"), Vec2{ 0.0f,0.0f }, Vec2{ 512.0f,256.0f });
	RESMGR->CreateGameSprite(_T("TargetCircle"), _T("DownTargetCircle"), Vec2{ 0.0f,256.0f }, Vec2{ 512.0f,256.0f });
	
	RESMGR->CreateGameSprite(_T("Object"), _T("BigWood"), Vec2{ 128.f,128.f }, Vec2{ 128.f,128.f });
	RESMGR->CreateGameSprite(_T("Object"), _T("BigWood_UpSide"), Vec2{ 128.f,128.f }, Vec2{ 128.f,64.f });
	RESMGR->CreateGameSprite(_T("Object"), _T("SmallWood"), Vec2{ 256.f,128.f }, Vec2{ 64.f,128.f });
	RESMGR->CreateGameSprite(_T("Object"), _T("BlueRoof"), Vec2{ 0.f,256.f }, Vec2{ 128.f,128.f });
	RESMGR->CreateGameSprite(_T("Object"), _T("BigBlueRoof"), Vec2{ 640.f,0.f }, Vec2{ 192.f,192.f });
	RESMGR->CreateGameSprite(_T("Object"), _T("DoubleBlueRoof"), Vec2{ 704.f,192.f }, Vec2{ 319.5f,320.f });
	RESMGR->CreateGameSprite(_T("Object"), _T("YellowRoof"), Vec2{ 832.f,0.f }, Vec2{ 192.f,192.f });
	RESMGR->CreateGameSprite(_T("Object"), _T("StreetLamp"), Vec2{ 576.f,0.f }, Vec2{ 64.f,192.f });
	RESMGR->CreateGameSprite(_T("Object"), _T("StonePillar"), Vec2{ 384.f,128.f }, Vec2{ 64.f,128.f });
	RESMGR->CreateGameSprite(_T("Object"), _T("GlassStatue"), Vec2{ 448.f,129.f }, Vec2{ 64.f,128.f });
	RESMGR->CreateGameSprite(_T("Object"), _T("GoldCoin"), Vec2{ 0.f,0.f }, Vec2{ 128.f,128.f });
	RESMGR->CreateGameSprite(_T("Object"), _T("SilverCoin"), Vec2{ 128.f,0.f }, Vec2{ 128.f,128.f });
	RESMGR->CreateGameSprite(_T("Object"), _T("GoldCoinBox"), Vec2{ 256.f,0.f }, Vec2{ 64.f,64.f });
	RESMGR->CreateGameSprite(_T("Object"), _T("SilverCoinBox"), Vec2{ 256.f,64.f }, Vec2{ 64.f,64.f });
	RESMGR->CreateGameSprite(_T("Object"), _T("GoldBar"), Vec2{ 320.f,0.f }, Vec2{ 64.f,64.f });
	RESMGR->CreateGameSprite(_T("Object"), _T("SilverBar"), Vec2{ 320.f,64.f }, Vec2{ 64.f,64.f });
	RESMGR->CreateGameSprite(_T("Object"), _T("DragonStatue"), Vec2{ 448.f,0.f }, Vec2{ 128.f,128.f });
	RESMGR->CreateGameSprite(_T("Object"), _T("HumanSkull"), Vec2{ 512.f,128.f }, Vec2{ 64.f,64.f });
	RESMGR->CreateGameSprite(_T("Object"), _T("BeastSkull"), Vec2{ 512.f,192.f }, Vec2{ 64.f,64.f });
	RESMGR->CreateGameSprite(_T("Object"), _T("Bush2"), Vec2{ 128.f,256.f }, Vec2{ 64.f,128.f });
	RESMGR->CreateGameSprite(_T("Object"), _T("DevilStatue"), Vec2{ 384.f,0.f }, Vec2{ 64.f,128.f });
	RESMGR->CreateGameSprite(_T("Object"), _T("weed1"), Vec2{ 192.f,256.f }, Vec2{ 64.f,64.f });
	RESMGR->CreateGameSprite(_T("Object"), _T("weed2"), Vec2{ 256,256.f }, Vec2{ 64.f,64.f });
	RESMGR->CreateGameSprite(_T("Object"), _T("BlueFlower"), Vec2{ 320.f,256.f }, Vec2{ 64.f,64.f });
	RESMGR->CreateGameSprite(_T("Object"), _T("Stub"), Vec2{ 384.f,256.f }, Vec2{ 64.f,64.f });
	RESMGR->CreateGameSprite(_T("Object"), _T("Bush1"), Vec2{ 448.f,256.f }, Vec2{ 64.f,64.f });
	RESMGR->CreateGameSprite(_T("Object"), _T("WhiteFlower"), Vec2{ 192.f,320.f }, Vec2{ 64.f,64.f });
	RESMGR->CreateGameSprite(_T("Object"), _T("PinkFlower"), Vec2{ 256.f,320.f }, Vec2{ 64.f,64.f });
	RESMGR->CreateGameSprite(_T("Object"), _T("OrangeFlower"), Vec2{ 320.0f,320.f }, Vec2{ 64.f,64.f });
	RESMGR->CreateGameSprite(_T("Object"), _T("Log"), Vec2{ 384.f,320.f }, Vec2{ 64.f,64.f });

}

void CLoadingSceneBuilder::CreateMultiSprite()
{
	RESMGR->CreateMultiSprite(_T("Map_Rain"), _T("MapRain"), Vec2{ 0.0f,0.0f }, Vec2{ 448.0f,1536.0f }, 168, 7, 24);

	RESMGR->CreateMultiSprite(_T("Map_Lid"), _T("Lid_Idle_Down"), Vec2{ 0.0f,0.0f }, Vec2{ 448.0f,64.0f }, 7, 7, 1);
	RESMGR->CreateMultiSprite(_T("Map_Lid"), _T("Lid_Idle_Up"), Vec2{ 0.0f,64.0f }, Vec2{ 448.0f,64.0f }, 7, 7, 1);
	RESMGR->CreateMultiSprite(_T("Map_Lid"), _T("Lid_Idle_Left"), Vec2{ 0.0f,128.0f }, Vec2{ 448.0f,64.0f }, 7, 7, 1);
	RESMGR->CreateMultiSprite(_T("Map_Lid"), _T("Lid_Idle_Right"), Vec2{ 0.0f,192.0f }, Vec2{ 448.0f,64.0f }, 7, 7, 1);

	RESMGR->CreateMultiSprite(_T("Beramode_Idle"), _T("Beramode_Idle_Down"), Vec2{ 0.0f,0.0f }, Vec2{ 375.0f,192.0f }, 4, 3, 2);
	RESMGR->CreateMultiSprite(_T("Carc_Idle"), _T("Carc_Idle_Down"), Vec2{ 0.0f,0.0f }, Vec2{ 333.0f,330.0f }, 8, 3, 3);
	RESMGR->CreateMultiSprite(_T("Mamon_Idle"), _T("Mamon_Idle_Down"), Vec2{ 0.0f,0.0f }, Vec2{ 300.0f,244.0f }, 4, 3, 2);
	RESMGR->CreateMultiSprite(_T("Merus_Idle"), _T("Merus_Idle_Down"), Vec2{ 0.0f,0.0f }, Vec2{ 312.0f,176.0f }, 4, 3, 2);
	RESMGR->CreateMultiSprite(_T("Golbez_Idle"), _T("Golbez_Idle_Down"), Vec2{ 0.0f,0.0f }, Vec2{ 339.0f,351.0f }, 8, 3, 3);


	RESMGR->CreateMultiSprite(_T("Destroyer_idle"), _T("Destroyer_Idle_Down"), Vec2{ 0.0f,0.0f }, Vec2{ 480.f,450.f }, 6, 3, 2);

	RESMGR->CreateMultiSprite(_T("Golbez_Dead"), _T("Golbez_Dead"), Vec2{ 0.0f,0.0f }, Vec2{ 96.f,42.f }, 1, 1, 1);
	RESMGR->CreateMultiSprite(_T("Beramode_Dead"), _T("Beramode_Dead"), Vec2{ 0.0f,0.0f }, Vec2{ 100.f,74.f }, 1, 1, 1);
	RESMGR->CreateMultiSprite(_T("Carc_Dead"), _T("Carc_Dead"), Vec2{ 0.0f,0.0f }, Vec2{ 98.f,118.f }, 1, 1, 1);
	RESMGR->CreateMultiSprite(_T("Merus_Dead"), _T("Merus_Dead"), Vec2{ 0.0f,0.0f }, Vec2{ 101.f,38.f }, 1, 1, 1);
	RESMGR->CreateMultiSprite(_T("Mamon_Dead"), _T("Mamon_Dead"), Vec2{ 0.0f,0.0f }, Vec2{ 101.f,57.f }, 1, 1, 1);

	RESMGR->CreateMultiSprite(_T("Golbez_Dying"), _T("Golbez_Dying"), Vec2{ 0.0f,0.0f }, Vec2{ 91.f,82.f }, 1, 1, 1);
	RESMGR->CreateMultiSprite(_T("Beramode_Dying"), _T("Beramode_Dying"), Vec2{ 0.0f,0.0f }, Vec2{ 99.f,73.f }, 1, 1, 1);
	RESMGR->CreateMultiSprite(_T("Carc_Dying"), _T("Carc_Dying"), Vec2{ 0.0f,0.0f }, Vec2{ 75.f,109.f }, 1, 1, 1);
	RESMGR->CreateMultiSprite(_T("Merus_Dying"), _T("Merus_Dying"), Vec2{ 0.0f,0.0f }, Vec2{ 65.f,105.f }, 1, 1, 1);
	RESMGR->CreateMultiSprite(_T("Mamon_Dying"), _T("Mamon_Dying"), Vec2{ 0.0f,0.0f }, Vec2{ 89.f,74.f }, 1, 1, 1);

	RESMGR->CreateMultiSprite(_T("Number_White"), _T("Number_White"), Vec2{ 0.0f,0.0f }, Vec2{ 256.0f,32.0f }, 10, 10, 1);
	RESMGR->CreateMultiSprite(_T("Number_Sp"), _T("Number_Sp"), Vec2{ 0.0f,0.0f }, Vec2{ 512.0f,64.0f }, 10, 10, 1);
	RESMGR->CreateMultiSprite(_T("ActionIcon"), _T("ActionIcon"), Vec2{ 0.0f,0.0f }, Vec2{ 320.0f,64.0f }, 5, 5, 1);
	RESMGR->CreateMultiSprite(_T("ArmorIcon"), _T("ArmorIcon"), Vec2{ 0.0f,0.0f }, Vec2{ 250.0f,64.0f }, 5, 5, 1);
	RESMGR->CreateMultiSprite(_T("HitIcon"), _T("HitIcon"), Vec2{ 0.0f,0.0f }, Vec2{ 250.0f,64.0f }, 5, 5, 1);
	RESMGR->CreateMultiSprite(_T("ShieldIcon"), _T("ShieldIcon"), Vec2{ 0.0f,0.0f }, Vec2{ 250.0f,64.0f }, 5, 5, 1);
	RESMGR->CreateMultiSprite(_T("SkillIcon"), _T("SkillIcon"), Vec2{ 0.0f,0.0f }, Vec2{ 250.0f,64.0f }, 5, 5, 1);
	RESMGR->CreateMultiSprite(_T("SelectCircle2"), _T("SelectCircle2"), Vec2{ 0.0f,0.0f }, Vec2{ 512.f,512.f }, 16, 4, 4);
	RESMGR->CreateMultiSprite(_T("TargetSelect"), _T("TargetSelect"), Vec2{ 0.0f,0.0f }, Vec2{ 512.f,256.f }, 8, 4, 2);
	
	RESMGR->CreateMultiSprite(_T("ScreenAni"), _T("ScreenAni"), Vec2{ 0.f,0.f }, Vec2{ 2048.f,3328.f }, 50, 4, 13);
	RESMGR->CreateMultiSprite(_T("EndingCredit"), _T("EndingCredit"), Vec2{ 0.f,0.f }, Vec2{ 4096.f,512.f }, 7, 8, 1);
	RESMGR->CreateMultiSprite(_T("Ending"), _T("Ending"), Vec2{ 0.f,0.f }, Vec2{ 4096.f,512.f }, 7, 8, 1);

	CClientGlobal::ItemIconMultiSprite = RESMGR->CreateMultiSprite(_T("Item"), _T("Item"), Vec2{ 0.0f,0.0f }, Vec2{ 256.f,128.f }, 25, 8, 4);


	map<tstring, MULTISPRITEDATA*>::iterator	Start = CClientGlobal::MultiSpriteData.begin();
	map<tstring, MULTISPRITEDATA*>::iterator	End = CClientGlobal::MultiSpriteData.end();

	for (; Start != End; ++Start)
	{
		RESMGR->CreateMultiSprite(Start->second);
	}

	CreateSkillIcon();
}
void CLoadingSceneBuilder::CreateSkillIcon()
{
	CGameMultiSprite* SkillIcon = RESMGR->CreateMultiSprite(_T("SkillMainIcon"), _T("SkillMainIcon"), Vec2{ 1.0f,0.0f }, Vec2{ 1022.0f,124.5f }, 8, 8, 1);

	for (size_t i = 0; i < 32; i++)
	{
		TCHAR MultiKey[MAXKEYLENGTH] = _T("");

#ifdef _WIN64
		_stprintf_s(MultiKey, _T("SkillMainIcon_%I64u"), i + 1);
#else 
		_stprintf_s(MultiKey, _T("SkillMainIcon_%u"), i + 1);
#endif 
		CGameMultiSprite* NewMulti = RESMGR->CreateMultiSprite(_T("SkillMainIcon"), MultiKey, Vec2{ 1.0f ,128.0f * (i + 1) }, Vec2{ 1022.0f,124.5f }, 8, 8, 1);

		SkillIcon->MergeMultiSprtie(NewMulti);
		RESMGR->EraseMultiSprite(MultiKey);
	}

	CClientGlobal::SkillIconMultiSprite = SkillIcon;

}
void CLoadingSceneBuilder::CreateGameSound()
{
	m_pHandle[m_iCurHandleIndex++] = CThreadMgr::CreateGlobvalThread(SoundThreadLoading, RESMGR.p);

}
void CLoadingSceneBuilder::SceneUpdate()
{

	//if (true == GetScene()->CheckSubMgr()) 
	//{
	//	size_t RC = GetScene()->RenderComCount();
	//	size_t CC = GetScene()->CollisionComCount();
	//	size_t AC = GetScene()->ActObjectCount();
	//	printf("Render ComCount : %i\t\tCollision ComCount : %i\t\tActObject Count : %i\n", (int)RC, (int)CC,(int)AC);
	//
	//}

	if (TRUE == KEYDOWN("ChangeScene"))
	{
		CClientGlobal::MainWindow->ChangeScene(CClientGlobal::SceneName[CClientGlobal::BATTLESCENE]);
	}

	if (TRUE == KEYDOUBLE("ChangeDebugMode"))
	{
		GetScene()->GetDebugMgr()->ChangeDebugMode();
	}
}

void CLoadingSceneBuilder::LoadAnimationData()
{
	m_pHandle[m_iCurHandleIndex++] = CThreadMgr::CreateGlobvalThread(AniThreadLoading);
	//AniThreadLoading(nullptr);
}
void CLoadingSceneBuilder::LoadMultiSpriteData()
{
	m_pHandle[m_iCurHandleIndex++] = CThreadMgr::CreateGlobvalThread(MultiDataThreadLoading);
	//MultiDataThreadLoading(nullptr);
}
void CLoadingSceneBuilder::LoadStatData()
{
	m_pHandle[m_iCurHandleIndex++] = CThreadMgr::CreateGlobvalThread(StatDataThreadLoading);
	//StatDataThreadLoading(nullptr);
}
void CLoadingSceneBuilder::LoadMonsterData()
{
	m_pHandle[m_iCurHandleIndex++] = CThreadMgr::CreateGlobvalThread(MonsterDataThreadLoading);
}
void CLoadingSceneBuilder::LoadEffectData()
{
	m_pHandle[m_iCurHandleIndex++] = CThreadMgr::CreateGlobvalThread(EffectDataThreadLoading);
	//EffectDataThreadLoading(nullptr);
}
void CLoadingSceneBuilder::LoadSkillData()
{
	m_pHandle[m_iCurHandleIndex++] = CThreadMgr::CreateGlobvalThread(SkillDataThreadLoading);
}
void CLoadingSceneBuilder::LoadItemData()
{
	m_pHandle[m_iCurHandleIndex++] = CThreadMgr::CreateGlobvalThread(ItemDataThreadLoading);
}
void CLoadingSceneBuilder::WakeUpScene()
{
	CClientGlobal::MouseObeject = m_MouseObject;
	CClientGlobal::CurNoneZoomProj = GetScene()->GetMainCamCom()->GetProjNoneRatio();
	CClientGlobal::CurSceneProj = GetScene()->GetMainCamCom()->GetSceneProj();
	CClientGlobal::CurSceneView = GetScene()->GetMainCamCom()->GetSceneView();
}
void CLoadingSceneBuilder::ThreadWaiting()
{
	WaitForMultipleObjects(m_iCurHandleIndex, m_pHandle, TRUE, INFINITE);
	memset(m_pHandle, 0, sizeof(HANDLE)*MAXHANDLE);
	m_iCurHandleIndex = 0;
}
void CLoadingSceneBuilder::SceneSerialLoading()
{
	CClientGlobal::LoadMonsterName();
	SoundThreadLoading(RESMGR.p);
	MultiDataThreadLoading(nullptr);

	RESMGR->LoadTextureInFolder(_T("PreLoad"));
	RESMGR->LoadGameTexture(CFilePathMgr::FilePath(_T("Texture"), _T("Map_Rain.png")));


	for (size_t i = CHARACTER; i < MAP; i++)
	{
		TexThreadLoading(&CClientGlobal::FolderName[i]);
	}

	CClientGlobal::LoadEffectData();
	CClientGlobal::LoadAnimationData(ANI_CHARACTER);
	CClientGlobal::LoadAnimationData(ANI_MONSTER);
	CClientGlobal::LoadAnimationData(ANI_EFFECT);
	CClientGlobal::LoadSkillData();
	CClientGlobal::LoadStatData();
	CClientGlobal::LoadMonsterData();
	CClientGlobal::LoadItemData();
	/*EffectDataThreadLoading(nullptr);
	AniThreadLoading(nullptr);
	StatDataThreadLoading(nullptr);*/

	CreateSprite();
	CreateMultiSprite();
}
void CLoadingSceneBuilder::SceneThreadLoading()
{
	CClientGlobal::LoadMonsterName();
	LoadingTexture();
	CreateGameSound();
	LoadMultiSpriteData();

	ThreadWaiting();

	LoadEffectData();
	LoadAnimationData();
	LoadStatData();
	LoadSkillData();
	LoadMonsterData();
	LoadItemData();

	CreateSprite();
	CreateMultiSprite();

	ThreadWaiting();
}
