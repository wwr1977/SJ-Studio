#include "stdafx.h"
#include "EditMapMgr.h"
#include "EditTileMap.h"
#include"EditTile.h"
#include <GameScene.h>
#include <ResourceMgr.h>
#include "MapSceneBuilder.h"
#include "MapDlg.h"
#include "EditMapObject.h"


CEditMapMgr::CEditMapMgr()
	: m_CurTileMap(nullptr)
	, m_pMapScene(nullptr), m_pMapSceneBuilder(nullptr)
	, m_CurFocusTile(nullptr), m_PrevFocusTile(nullptr)
{
	m_MainTileMap.clear();
	//m_WorldMapInitPoint.clear();
	CEditGlobal::EditMapMgr = this;
	CEditMapObject::SetMapMgr(this);

	m_ObjectName.push_back(MapObjectName(_T("BigWood"), _T("큰 나무")) );
	m_ObjectName.push_back(MapObjectName(_T("BigWood_UpSide"), _T("큰 나무윗편")));
	m_ObjectName.push_back(MapObjectName(_T("SmallWood"), _T("작은 나무")));
	m_ObjectName.push_back(MapObjectName(_T("BlueRoof"), _T("파란지붕")));
	m_ObjectName.push_back(MapObjectName(_T("BigBlueRoof"), _T("큰 파란지붕")));
	m_ObjectName.push_back(MapObjectName(_T("DoubleBlueRoof"), _T("옥상 집")));
	m_ObjectName.push_back(MapObjectName(_T("YellowRoof"), _T("오두막지붕")));
	m_ObjectName.push_back(MapObjectName(_T("StreetLamp"), _T("가로등")));
	m_ObjectName.push_back(MapObjectName(_T("StonePillar"), _T("돌기둥")));
	m_ObjectName.push_back(MapObjectName(_T("GlassStatue"), _T("유리조각상")));
	m_ObjectName.push_back(MapObjectName(_T("weed1"), _T("잡초1")));
	m_ObjectName.push_back(MapObjectName(_T("weed2"), _T("잡초2")));
	m_ObjectName.push_back(MapObjectName(_T("BlueFlower"), _T("파란 꽃")));
	m_ObjectName.push_back(MapObjectName(_T("Stub"), _T("그루터기")));
	m_ObjectName.push_back(MapObjectName(_T("Bush1"), _T("수풀1")));
	m_ObjectName.push_back(MapObjectName(_T("WhiteFlower"), _T("하얀 꽃")));
	m_ObjectName.push_back(MapObjectName(_T("PinkFlower"), _T("분홍 꽃")));
	m_ObjectName.push_back(MapObjectName(_T("OrangeFlower"), _T("주황 꽃")));
	m_ObjectName.push_back(MapObjectName(_T("Log"), _T("통나무")));
	m_ObjectName.push_back(MapObjectName(_T("GoldCoin"), _T("금화 무더기")));
	m_ObjectName.push_back(MapObjectName(_T("SilverCoin"), _T("은화 무더기")));
	m_ObjectName.push_back(MapObjectName(_T("GoldCoinBox"), _T("금화 상자")));
	m_ObjectName.push_back(MapObjectName(_T("SilverCoinBox"), _T("은화 상자")));
	m_ObjectName.push_back(MapObjectName(_T("GoldBar"), _T("금괴")));
	m_ObjectName.push_back(MapObjectName(_T("SilverBar"), _T("은괴")));
	m_ObjectName.push_back(MapObjectName(_T("DragonStatue"), _T("드래곤 석상")));
	m_ObjectName.push_back(MapObjectName(_T("HumanSkull"), _T("인간 해골")));
	m_ObjectName.push_back(MapObjectName(_T("BeastSkull"), _T("동물 해골")));
	m_ObjectName.push_back(MapObjectName(_T("Bush2"), _T("수풀2")));
	m_ObjectName.push_back(MapObjectName(_T("DevilStatue"), _T("악마 석상")));



}


CEditMapMgr::~CEditMapMgr()
{
	m_MainTileMap.clear();
	
}

void CEditMapMgr::Init(CGameScene* _MainScene, CSceneBuilder* _SceneBuilder)
{
	if (nullptr == _MainScene || nullptr == _SceneBuilder) 
	{
		assert(nullptr);
		return;
	}

	m_pMapScene = _MainScene;
	m_pMapSceneBuilder = _SceneBuilder;

	CEditTileMap::SetMainSceneCamera(m_pMapScene->GetMainCamCom());
	CEditTile::TileClassInit(this);
	
	CreateTileMap(TILEMAPDATA(TOWN, _T("Town")));
	CreateTileMap(TILEMAPDATA(RUINS, _T("Ruins")));
	CreateTileMap(TILEMAPDATA(FOREST, _T("Forest")));
	CreateTileMap(TILEMAPDATA(DEVILCASTLE, _T("DevilCastle")));
	CreateTileMap(TILEMAPDATA(WEANPONSTORE, _T("WeaponStore")));
	CreateTileMap(TILEMAPDATA(ITEMSTORE, _T("ItemStore")));

	
	InitializeTileMap();

	m_CurMapObject =  m_pMapSceneBuilder->CreateActObject(_T("TestMapObject"))->CreateCom<CEditMapObject>(0);
	m_CurMapObject->MapObjectOff();
	m_CurMapObject->SetTestMapObject();

	ChangeTileMap(TOWN);
	
}
void CEditMapMgr::Update()
{
	if (TRUE == KEY("MouseLeft"))
	{
		EditTileMap();
		return;
	}

	if (TRUE == KEY("MouseRight")) 
	{
		InitTileMap();
		return;
	}

}

CEditTileMap* CEditMapMgr::CreateTileMap(const TILEMAPDATA& _Data)
{
	CEditTileMap* NewTileMap = m_pMapSceneBuilder->CreateActObject(_Data.TileMapName)->CreateCom<CEditTileMap>(_Data);

	m_MainTileMap.insert(map<UINT, SPTR<CEditTileMap>>::value_type(_Data.TileMapNo, NewTileMap));
	
	return NewTileMap;
}
SPTR<CEditTileMap> CEditMapMgr::FindTileMap(const UINT& _TileMapNo)
{
	map<UINT, SPTR<CEditTileMap>>::iterator Find = m_MainTileMap.find(_TileMapNo);

	if (Find != m_MainTileMap.end())
		return Find->second;

	return nullptr;
}
void CEditMapMgr::InitializeTileMap()
{
	map<UINT, SPTR<CEditTileMap>>::iterator Start = m_MainTileMap.begin();
	map<UINT, SPTR<CEditTileMap>>::iterator End = m_MainTileMap.end();

	for (;Start!= End;++Start)
		Start->second->CreateTile();
	
	return;
}
void CEditMapMgr::InitCameraPos()
{
	if (nullptr == m_pMapSceneBuilder)
		return;

	m_pMapSceneBuilder->GetMainCam()->InitPos();
}
SPTR<CEditTileMap> CEditMapMgr::ChangeTileMap(const UINT& _TileMapNo)
{
	map<UINT, SPTR<CEditTileMap>>::iterator Find = m_MainTileMap.find(_TileMapNo);

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
	CEditGlobal::CurTileMap = m_CurTileMap;
	m_CurTileMap->ChangeTileMapEvent();
	m_CurTileMap->TileMapOn();
	m_CurMapObject->SetTileMap(m_CurTileMap.p);
	return m_CurTileMap;
}
const size_t CEditMapMgr::GetTileMapCount()
{
	return  m_MainTileMap.size();
}
const POINT CEditMapMgr::GetTileMapSize(const UINT& _TileMapNo)
{
	map<UINT, SPTR<CEditTileMap>>::iterator Find = m_MainTileMap.find(_TileMapNo);

	if (Find == m_MainTileMap.end())
		return POINT{ -1,-1 };

	return Find->second->GetTileMapSize();
}
const POINT CEditMapMgr::GetTileMapMaxIndex(const UINT& _TileMapNo)
{
	map<UINT, SPTR<CEditTileMap>>::iterator Find = m_MainTileMap.find(_TileMapNo);

	if (Find == m_MainTileMap.end())
		return POINT{ -1,-1 };

	return Find->second->GetTileMapMaxIndex();
}
void CEditMapMgr::SelectTileMapEvent(SPTR<CEditTile> _TrigerTile , const MAINMAPNAME& _MapNo)
{
	if (nullptr == _TrigerTile)
		return;

	if (_MapNo >= MAXMAINMAPNAME)
		return;

	POINT InitPosIndex = _TrigerTile->GetMapChangeInitPoint(_MapNo);
	
	if (InitPosIndex == POINT{ -1,-1 })
		return;


	/*CClientGlobal::MainPlayer->SetInitTileIndex(InitPosIndex);

	CClientGlobal::MainPlayer->On();

	if (nullptr != ChangeTileMap(_MapNo))
		m_WorldMap->Off();
*/
}
void CEditMapMgr::CurTileMapOn() 
{
	m_CurTileMap->TileMapOn();
}
void CEditMapMgr::CurTileMapOff() 
{
	m_CurTileMap->TileMapOff();
}
void CEditMapMgr::FindFocusTile(const Vec2& _MousePos, const Vec2& _CameraPos, const float& _ZoomRatio)
{
	Vec2 TilePos = Vec2{ _MousePos.x * _ZoomRatio,_MousePos.y * _ZoomRatio } +_CameraPos;

	if (m_PrevFocusTile.p != m_CurTileMap->GetTile(Vec3{ TilePos.x,TilePos.y , 10.0f }).p) 
	{
		m_PrevFocusTile = m_CurFocusTile;
		m_CurFocusTile  =  m_CurTileMap->GetTile(Vec3{ TilePos.x,TilePos.y , 10.0f});

		if (nullptr != m_CurTileMap->GetTile(Vec3{ TilePos.x,TilePos.y , 10.0f }).p)
			m_CurMapObject->SetTileLBIndex(m_CurFocusTile->GetTileIndex());
		//m_CurMapObject->MapObjectOn();
	}
}
void CEditMapMgr::NoFocusTile()
{
	m_CurFocusTile = nullptr;
}
SPTR<CEditTile>	CEditMapMgr::GetCurFocusTile()
{
	return m_CurFocusTile;
}
void CEditMapMgr::EditTileMap()
{
	if (nullptr == CEditGlobal::MapDlg)
		return;

	switch (CEditGlobal::MapDlg->GetCurEditMode())
	{
	case CMapDlg::TILE_EDIT:
		TileEdit(CEditGlobal::MapDlg->GetCurTileType(), CEditGlobal::MapDlg->GetCurTileSubOption());
		break;
	case CMapDlg::OBJECT_EDIT:
		ObjectEdit(CEditGlobal::MapDlg->GetCurMapObject());
		break;
	}
}
void CEditMapMgr::TileEdit(const TILEOPTION& _TileType, const UINT& _TileOption)
{
	if (nullptr == m_CurFocusTile)
		return;

	switch (_TileType)
	{
	case STEPTILE:
		m_CurFocusTile->SetStepTile(_TileOption);
		break;
	case STAIRTILE:
	case NOENTERTILE:
		m_CurFocusTile->SetTileOption(_TileType);
		break;
	case STEPSTAIRTILE:
		m_CurFocusTile->SetStepStairTile(_TileOption);
		break;
	case MAPCHANGETILE: 
	{
		POINT Index = CEditGlobal::MapDlg->GetSpawnTileIndex();
		m_CurFocusTile->SetPortalTile(_TileOption, Index);
	}
		break;
	case WORLDMAPTILE:
		m_CurFocusTile->SetWorldMapTile(_TileOption);
		break;
	}
}
void CEditMapMgr::ObjectEdit(const UINT& _ObjectNo)
{
	if (_ObjectNo >= m_ObjectName.size())
		return;


	// 여기에선 등록
	if (nullptr == m_CurTileMap || nullptr == m_CurFocusTile)
		return;

	SPTR<CEditMapObject> CurObject = m_CurTileMap->FindMapObject(m_CurFocusTile->GetTileIndex());
	
	if (nullptr != CurObject && CurObject->GetObjectNo() == _ObjectNo)
		return;

	SPTR<CEditMapObject> NewObject = m_pMapSceneBuilder->CreateActObject(_T("MapObject"))->CreateCom<CEditMapObject>(_ObjectNo);
	
	assert(NewObject);

	NewObject->SetTileMap(m_CurTileMap.p);
	NewObject->SetTileLBIndex(m_CurFocusTile->GetTileIndex());
	m_CurTileMap->PushMapObject(NewObject);

	/*if (nullptr == m_CurFocusTile )
		return;

	m_CurMapObject->SetTileLBIndex(m_CurFocusTile->GetTileIndex());
	m_CurMapObject->MapObjectOn();*/
}
void CEditMapMgr::InitTileMap()
{
	if (nullptr == CEditGlobal::MapDlg)
		return;

	switch (CEditGlobal::MapDlg->GetCurEditMode())
	{
	case CMapDlg::TILE_EDIT:
		TileInit();
		break;
	case CMapDlg::OBJECT_EDIT:
		ObjectDelete();
		break;
	}
}
void CEditMapMgr::TileInit() 
{
	if (nullptr == m_CurFocusTile)
		return;

	m_CurFocusTile->SetStepTile(0);
}
void CEditMapMgr::ObjectDelete() 
{
	if (nullptr == m_CurFocusTile)
		return;

	m_CurTileMap->PopMapObject(m_CurFocusTile->GetTileIndex());
}
BOOL CEditMapMgr::LoadTileData(FILE* _LoadFile)
{
	if (nullptr == _LoadFile)
		return FALSE;

	map<UINT, SPTR<CEditTileMap>>::iterator Find;

	while (!feof(_LoadFile)) 
	{
		TILEMAPDATA MapData;
		MapData.ReadTileMapData(_LoadFile);
		
		Find = m_MainTileMap.find(MapData.TileMapNo);

		if (Find == m_MainTileMap.end())
			continue;

		Find->second->LoadTileMapData(_LoadFile, MapData);
	}

	return TRUE;
}
BOOL CEditMapMgr::SaveTileData(FILE* _SaveFile)
{
	if (nullptr == _SaveFile)
		return FALSE;

	map<UINT, SPTR<CEditTileMap>>::iterator Start = m_MainTileMap.begin();
	map<UINT, SPTR<CEditTileMap>>::iterator End = m_MainTileMap.end();

	for (; Start != End; ++Start)
	{
		Start->second->SaveTileMapData(_SaveFile);
	}

	return TRUE;
}
const tstring CEditMapMgr::GetObjectName(const UINT& _ObjectNo) 
{
	if (_ObjectNo >= m_ObjectName.size())
		return _T("");

	return m_ObjectName[_ObjectNo].ObjectName;
}
const tstring CEditMapMgr::GetObjectKName(const UINT& _ObjectNo) 
{
	if (_ObjectNo >= m_ObjectName.size())
		return _T("");

	return m_ObjectName[_ObjectNo].ObjectKName;
}
const size_t CEditMapMgr::GetObjectCount()
{
	return m_ObjectName.size();
}
const POINT CEditMapMgr::GetObjectSize(const UINT& _ObjectNo)
{
	if (_ObjectNo >= m_ObjectName.size())
		return POINT{ 0,0 };

	SPTR<CGameSprite> ObjectSprite = CEditGlobal::MainWindow->GetResourceMgr()->FindGameSprite(m_ObjectName[_ObjectNo].ObjectName);

	if(nullptr == ObjectSprite)
		return POINT{ 0,0 };
	
	Vec2 ObjectSize = ObjectSprite->GetSpriteSize();

	POINT Value = POINT{ (LONG)(ObjectSize.x / CEditTileMap::TileSize.x),(LONG)(ObjectSize.y / CEditTileMap::TileSize.y) };

	return Value;
}
void CEditMapMgr::CurMapObjectEnable(const BOOL& _Enable)
{
	if (nullptr != m_CurMapObject)
		m_CurMapObject->EnableMapObject(_Enable);
}
void CEditMapMgr::ChangeCurMapObject(const UINT& _ObjectNo)
{
	if (nullptr == m_CurMapObject)
		return;;

	m_CurMapObject->ChangeMapObject(_ObjectNo);

}
BOOL CEditMapMgr::LoadMapObjectData(CGameFile* _GameFile)
{
	if (nullptr == _GameFile)
		return FALSE;

	_GameFile->Open(_T("rb"));

	if (nullptr == _GameFile->GetFile())
		return FALSE;


	size_t TotalCount = _GameFile->GetFileSize() / sizeof(MAPOBJECTDATA);
	size_t BufSize = sizeof(MAPOBJECTDATA) * TotalCount;
	
	MAPOBJECTDATA* Buf = (MAPOBJECTDATA*)malloc(BufSize);
	memset(Buf, -1, BufSize);

	MAPOBJECTDATA::ReadData(_GameFile->GetFile(), Buf, BufSize, (UINT)TotalCount);

	for (size_t i = 0; i < TotalCount; i++)
	{
		SPTR<CEditTileMap> TileMap = FindTileMap(Buf[i].TileMapNo);
	
		if (nullptr != TileMap) 
		{
			SPTR<CEditMapObject> NewObject = m_pMapSceneBuilder->CreateActObject(_T("MapObject"))->CreateCom<CEditMapObject>(Buf[i].MapObjectNo);
			NewObject->SetTileMap(TileMap.p);
			NewObject->SetTileLBIndex(POINT{ (LONG)Buf[i].LeftIndex,(LONG)Buf[i].BottomIndex });
			TileMap->PushMapObject(NewObject);
			if (m_CurTileMap->GetTileMapNo() != TileMap->GetTileMapNo())
				NewObject->MapObjectOff();

		}
		
	}

	

	free(Buf);


	return TRUE;
}
BOOL CEditMapMgr::SaveMapObjectData(FILE* _SaveFile)
{
	if (nullptr == _SaveFile)
		return FALSE;

	map<UINT, SPTR<CEditTileMap>>::iterator Start = m_MainTileMap.begin();
	map<UINT, SPTR<CEditTileMap>>::iterator End = m_MainTileMap.end();

	for (;Start!= End; ++Start)
		Start->second->SaveMapObjectData(_SaveFile);
	

	return TRUE;
}