#include "Precom.h"
#include "MainMapMgr.h"
#include "TileMap.h"
#include"Tile.h"
#include "Player.h"
#include "GameMaster.h"
#include "WorldMap.h"
#include "MapObject.h"
#include "NPCBase.h"
#include "MainSceneBuilder.h"
#include <DebugMgr.h>
#include <GameScene.h>
#include <ResourceMgr.h>

CMainMapMgr::CMainMapMgr()
	: m_pGameMaster(nullptr), m_CurTileMap(nullptr)
	, m_pMainScene(nullptr), m_pMainSceneBuilder(nullptr)
	, m_WorldMap(nullptr)
	, m_MapSpeaker(nullptr)
	, m_bMusicUpdate(false)
	, m_fAccMusicTime(0.f)
	, m_CurMusicOp(MUSIC_START)
	, m_CurBgmName(_T(""))
	, m_fSpeakerVol(0.3f)
	, m_fMusicFadeTime(2.f)
	, m_bBGMPause(false)
{
	m_MainTileMap.clear();
	//m_WorldMapInitPoint.clear();

	m_ObjectName.push_back(_T("BigWood"));
	m_ObjectName.push_back(_T("BigWood_UpSide"));
	m_ObjectName.push_back(_T("SmallWood"));
	m_ObjectName.push_back(_T("BlueRoof"));
	m_ObjectName.push_back(_T("BigBlueRoof"));
	m_ObjectName.push_back(_T("DoubleBlueRoof"));
	m_ObjectName.push_back(_T("YellowRoof"));
	m_ObjectName.push_back(_T("StreetLamp"));
	m_ObjectName.push_back(_T("StonePillar"));
	m_ObjectName.push_back(_T("GlassStatue"));
	m_ObjectName.push_back(_T("weed1"));
	m_ObjectName.push_back(_T("weed2"));
	m_ObjectName.push_back(_T("BlueFlower"));
	m_ObjectName.push_back(_T("Stub"));
	m_ObjectName.push_back(_T("Bush1"));
	m_ObjectName.push_back(_T("WhiteFlower"));
	m_ObjectName.push_back(_T("PinkFlower"));
	m_ObjectName.push_back(_T("OrangeFlower"));
	m_ObjectName.push_back(_T("Log"));
	m_ObjectName.push_back(_T("GoldCoin"));
	m_ObjectName.push_back(_T("SilverCoin"));
	m_ObjectName.push_back(_T("GoldCoinBox"));
	m_ObjectName.push_back(_T("SilverCoinBox"));
	m_ObjectName.push_back(_T("GoldBar"));
	m_ObjectName.push_back(_T("SilverBar"));
	m_ObjectName.push_back(_T("DragonStatue"));
	m_ObjectName.push_back(_T("HumanSkull"));
	m_ObjectName.push_back(_T("BeastSkull"));
	m_ObjectName.push_back(_T("Bush2"));
	m_ObjectName.push_back(_T("DevilStatue"));

	CMapObject::SetMapMgr(this);
}
CMainMapMgr::CMainMapMgr(CGameMaster* _Master)
	: m_pGameMaster(_Master), m_CurTileMap(nullptr)
	, m_pMainScene(nullptr), m_pMainSceneBuilder(nullptr)
	, m_WorldMap(nullptr)
	, m_MapSpeaker(nullptr)
	, m_bMusicUpdate(false)
	, m_fAccMusicTime(0.f)
	, m_CurMusicOp(MUSIC_START)
	, m_CurBgmName(_T(""))
	, m_fSpeakerVol(0.3f)
	, m_fMusicFadeTime(3.f)
	, m_bBGMPause(false)
{
	m_MainTileMap.clear();
	//m_WorldMapInitPoint.clear();

	m_ObjectName.push_back(_T("BigWood"));			//0
	m_ObjectName.push_back(_T("BigWood_UpSide"));	//1
	m_ObjectName.push_back(_T("SmallWood"));		//2	
	m_ObjectName.push_back(_T("BlueRoof"));			//3
	m_ObjectName.push_back(_T("BigBlueRoof"));		//4
	m_ObjectName.push_back(_T("DoubleBlueRoof"));	//5
	m_ObjectName.push_back(_T("YellowRoof"));		//6
	m_ObjectName.push_back(_T("StreetLamp"));		//7
	m_ObjectName.push_back(_T("StonePillar"));		//8
	m_ObjectName.push_back(_T("GlassStatue"));		//9
	m_ObjectName.push_back(_T("weed1"));			//10
	m_ObjectName.push_back(_T("weed2"));			//11
	m_ObjectName.push_back(_T("BlueFlower"));		//12
	m_ObjectName.push_back(_T("Stub"));				//13
	m_ObjectName.push_back(_T("Bush1"));			//14
	m_ObjectName.push_back(_T("WhiteFlower"));		//15
	m_ObjectName.push_back(_T("PinkFlower"));		//16
	m_ObjectName.push_back(_T("OrangeFlower"));		//17
	m_ObjectName.push_back(_T("Log"));				//18
	m_ObjectName.push_back(_T("GoldCoin"));			//19
	m_ObjectName.push_back(_T("SilverCoin"));		//20
	m_ObjectName.push_back(_T("GoldCoinBox"));		//21
	m_ObjectName.push_back(_T("SilverCoinBox"));	//22
	m_ObjectName.push_back(_T("GoldBar"));			//23
	m_ObjectName.push_back(_T("SilverBar"));		//24
	m_ObjectName.push_back(_T("DragonStatue"));		//25
	m_ObjectName.push_back(_T("HumanSkull"));		//26
	m_ObjectName.push_back(_T("BeastSkull"));		//27
	m_ObjectName.push_back(_T("Bush2"));			//28
	m_ObjectName.push_back(_T("DevilStatue"));		//29

	CMapObject::SetMapMgr(this);
}

CMainMapMgr::~CMainMapMgr()
{
	m_MainTileMap.clear();
	//m_WorldMapInitPoint.clear();
}

void CMainMapMgr::Init(CGameScene* _MainScene, CSceneBuilder* _SceneBuilder)
{
	if (nullptr == _MainScene || nullptr == _SceneBuilder) 
	{
		assert(nullptr);
		return;
	}

	m_pMainScene = _MainScene;
	m_pMainSceneBuilder = _SceneBuilder;

	CTileMap::SetMainSceneCamera(m_pMainScene->GetMainCamCom());
	CTile::TileClassInit(this);
	
	CreateTileMap(TILEMAPDATA(TOWN, _T("Town")));
	CreateTileMap(TILEMAPDATA(RUINS, _T("Ruins")));
	CreateTileMap(TILEMAPDATA(FOREST, _T("Forest")));
	CreateTileMap(TILEMAPDATA(DEVILCASTLE, _T("DevilCastle")));
	CreateTileMap(TILEMAPDATA(WEANPONSTORE, _T("WeaponStore")));
	CreateTileMap(TILEMAPDATA(ITEMSTORE, _T("ItemStore")));

	m_WorldMap = m_pGameMaster->CreateActObject(_T("WorldMap"))->CreateCom<CWorldMap>(this);
	InitializeTileMap();


	m_MapSpeaker = m_pGameMaster->CreateActObject(_T("MapBgmPlayer"))->CreateCom<CSoundPlayer>();
}
void CMainMapMgr::Update()
{
	if (!m_bMusicUpdate)
		return;

	switch (m_CurMusicOp)
	{
	case CMainMapMgr::MUSIC_START:
		MapMusicStart();
		break;
	case CMainMapMgr::MUSIC_END:
		MapMusicEnd();
		break;
	}
}
void CMainMapMgr::ResetMgr()
{
	map<UINT, SPTR<CTileMap>>::iterator Start = m_MainTileMap.begin();
	map<UINT, SPTR<CTileMap>>::iterator End = m_MainTileMap.end();

	for (;Start!=End; ++Start)
	{
		if(nullptr != Start->second)
			Start->second->ResetTileMap();
	}
}
CTileMap* CMainMapMgr::CreateTileMap(const TILEMAPDATA& _Data)
{
	CTileMap* NewTileMap = m_pGameMaster->CreateActObject(_Data.TileMapName)->CreateCom<CTileMap>(_Data);

	m_MainTileMap.insert(map<UINT, SPTR<CTileMap>>::value_type(_Data.TileMapNo, NewTileMap));
	
	return NewTileMap;
}
SPTR<CTileMap> CMainMapMgr::FindTileMap(const UINT& _TileMapNo)
{
	map<UINT, SPTR<CTileMap>>::iterator Find = m_MainTileMap.find(_TileMapNo);

	if (Find != m_MainTileMap.end())
		return Find->second;

	return nullptr;
}
void CMainMapMgr::InitializeTileMap()
{
	map<UINT, SPTR<CTileMap>>::iterator Start = m_MainTileMap.begin();
	map<UINT, SPTR<CTileMap>>::iterator End = m_MainTileMap.end();

	for (;Start!= End;++Start)
		Start->second->CreateTile();
	
	LoadTileData();
	LoadMapObjectData();
	return;
}
SPTR<CTileMap> CMainMapMgr::ChangeTileMap(const UINT& _TileMapNo)
{
	map<UINT, SPTR<CTileMap>>::iterator Find = m_MainTileMap.find(_TileMapNo);

	if (Find == m_MainTileMap.end())
		return nullptr;

	if (nullptr == Find->second) 
	{
		assert(nullptr);
		return nullptr;
	}

	if(nullptr != m_CurTileMap)
		m_CurTileMap->TileMapOff();

	
	m_CurTileMap = Find->second;
	CClientGlobal::CurTileMap = m_CurTileMap;
	m_CurTileMap->ChangeTileMapEvent();
	m_CurTileMap->TileMapOn();
	CClientGlobal::MainPlayer->InitCurTile();
	m_CurTileMap->SetDebugMode(m_pMainScene->GetDebugMgr()->IsDebugMode());

	m_MapSpeaker->CurPlaySoundStop();

	m_CurBgmName = m_CurTileMap->GetTileMapName() + _T("Bgm");
	
	if (true == m_MapSpeaker->PlaySound(m_CurBgmName, FMOD_LOOP_NORMAL)) 
		MusicOnEvent(true);
	

	return m_CurTileMap;
}
const size_t CMainMapMgr::GetTileMapCount()
{
	return  m_MainTileMap.size();
}

void CMainMapMgr::ChangeCurTileMapDebugMode()
{
	if (nullptr != m_CurTileMap)
		m_CurTileMap->ChangeDebugMode();
}
void CMainMapMgr::SetCurTileMapDeugMode(const bool& _Mode)
{
	if (nullptr != m_CurTileMap)
		m_CurTileMap->SetDebugMode(_Mode);
}
void CMainMapMgr::ShowWolrdMapEvent(SPTR<CTile> _TrigerTile)
{
	m_CurTileMap->TileMapOff();
	CClientGlobal::MainPlayer->InitPlayerMoveState();
	CClientGlobal::MainPlayer->Off();
	CClientGlobal::GameMaster->InitCameraPos();
	m_WorldMap->On(_TrigerTile);
}
void CMainMapMgr::SelectTileMapEvent(SPTR<CTile> _TrigerTile , const MAINMAPNAME& _MapNo)
{
	if (nullptr == _TrigerTile)
		return;

	if (_MapNo >= MAXMAINMAPNAME)
		return;

	m_pGameMaster->MapChangeEvent(_TrigerTile, _MapNo);

	/*POINT InitPosIndex = _TrigerTile->GetMapChangeInitPoint(_MapNo);
	
	if (InitPosIndex == POINT{ -1,-1 })
		return;


	CClientGlobal::MainPlayer->SetInitTileIndex(InitPosIndex);
	CClientGlobal::MainPlayer->On();

	if (nullptr != ChangeTileMap(_MapNo))
		m_WorldMap->Off();*/

}
void CMainMapMgr::LoadTileData()
{
	CGameFile LoadTileFile(_T("Data"), _T("TileData.bin"));

	if (FALSE == LoadTileFile.Open(_T("rb")))
	{
		assert(nullptr);
		return;
	}
	FILE* File = LoadTileFile.GetFile();

	map<UINT, SPTR<CTileMap>>::iterator Find;

	while (!feof(File))
	{
		TILEMAPDATA MapData;
		MapData.ReadTileMapData(File);

		Find = m_MainTileMap.find(MapData.TileMapNo);

		if (Find == m_MainTileMap.end())
			continue;

		Find->second->LoadTileMapData(File, MapData);
	}


	/*size_t ReadCount = m_mapTile[0].size()*m_mapTile.size();
	size_t BufSize = sizeof(TILEDATA)*ReadCount;

	TILEDATA* Buf = (TILEDATA*)malloc(BufSize);
	memset(Buf, -1, BufSize);

	TILEDATA::ReadTileMapData(_LoadFile, Buf, ReadCount);

	size_t Offset;
	for (size_t j = 0; j < m_mapTile.size(); ++j)
	{
		Offset = m_mapTile[j].size();
		for (size_t i = 0; i < Offset; i++)
			m_mapTile[j][i]->SetTileData(*(Buf + j*Offset + i));
	}

	free(Buf);*/

}
const size_t CMainMapMgr::GetObjectCount()
{
	return m_ObjectName.size();
}
const tstring CMainMapMgr::GetObjectName(const UINT& _ObjectNo)
{
	if (_ObjectNo >= m_ObjectName.size())
		return _T("");

	return m_ObjectName[_ObjectNo];
}
void CMainMapMgr::LoadMapObjectData()
{
	CGameFile LoadFile(_T("Data"), _T("MapObjectData.bin"));

	if (FALSE == LoadFile.Open(_T("rb")))
	{
		assert(nullptr);
		return;
	}

	size_t TotalCount = LoadFile.GetFileSize() / sizeof(MAPOBJECTDATA);
	size_t BufSize = sizeof(MAPOBJECTDATA) * TotalCount;

	MAPOBJECTDATA* Buf = (MAPOBJECTDATA*)malloc(BufSize);
	memset(Buf, -1, BufSize);

	MAPOBJECTDATA::ReadData(LoadFile.GetFile(), Buf, BufSize, (UINT)TotalCount);

	for (size_t i = 0; i < TotalCount; i++)
	{
		SPTR<CTileMap> TileMap = FindTileMap(Buf[i].TileMapNo);

		if (nullptr != TileMap)
		{
			SPTR<CMapObject> NewObject = m_pMainSceneBuilder->CreateActObject(_T("MapObject"))->CreateCom<CMapObject>(Buf[i].MapObjectNo);
			NewObject->SetTileMap(TileMap.p);
			NewObject->SetTileLBIndex(POINT{ (LONG)Buf[i].LeftIndex,(LONG)Buf[i].BottomIndex });
			TileMap->PushMapObject(NewObject);
		}
	}

	free(Buf);

}
void CMainMapMgr::PushMapNPC(const MAINMAPNAME& _MapName, SPTR<CNPCBase> _NPC)
{
	if (_MapName >= MAXMAINMAPNAME || nullptr == _NPC)
		return;

	m_MainTileMap[_MapName]->PushMapNPC(_NPC);
}
void CMainMapMgr::CurTileMapMusicOn()
{
	m_MapSpeaker->CurPlaySoundStop();

	m_CurBgmName = m_CurTileMap->GetTileMapName() + _T("Bgm");

	if (true == m_MapSpeaker->PlaySound(m_CurBgmName, FMOD_LOOP_NORMAL))
		MusicOnEvent(true);
}
void CMainMapMgr::MapMusicStart()
{
	if (m_fAccMusicTime >= m_fMusicFadeTime)
	{
		m_bMusicUpdate = false;
		m_MapSpeaker->SetVolume(m_fSpeakerVol);
		return;
	}

	float t = powf((m_fAccMusicTime / m_fMusicFadeTime), 2.f);
	float Vol = t * m_fSpeakerVol;
	m_MapSpeaker->SetVolume(Vol);

	m_fAccMusicTime += DELTATIME;
}
void CMainMapMgr::MapMusicEnd()
{
	if (m_fAccMusicTime >= m_fMusicFadeTime)
	{
		m_bMusicUpdate = false;
		m_MapSpeaker->SetVolume(0.f);
		MapMusicPause(true);
		return;
	}

	float t = powf((m_fAccMusicTime / m_fMusicFadeTime), 2.f);
	float Vol = (1.f - t )* m_fSpeakerVol;
	m_MapSpeaker->SetVolume(Vol);

	m_fAccMusicTime += DELTATIME;
}
void CMainMapMgr::MapMusicPause(const bool& _Pause)
{
	if (m_bBGMPause == _Pause)
		return;

	if(nullptr != m_MapSpeaker)
		m_MapSpeaker->CurPlaySoundPaused(_Pause);
	
	m_bBGMPause = _Pause;
}
void CMainMapMgr::MusicOnEvent(const bool& _MusicOn)
{
	m_bMusicUpdate = true;
	m_fAccMusicTime = 0.f;

	if (_MusicOn) 
	{
		m_CurMusicOp = MUSIC_START;
		m_MapSpeaker->SetVolume(0.f);
		MapMusicPause(false);
	}
	else 
	{
		m_CurMusicOp = MUSIC_END;
		m_MapSpeaker->SetVolume(m_fSpeakerVol);
	}
}
const bool CMainMapMgr::IsMapBGMPause()
{
	return m_bBGMPause;
}
void CMainMapMgr::WorldMapActive(const bool& _Enable)
{
	m_WorldMap->ActorActive(_Enable);
}
void CMainMapMgr::SetEventMgr(class CEventMgr* _Mgr)
{
	m_WorldMap->SetEventMgr(_Mgr);
}