#pragma once
#include<ObjBase.h>

struct MapObjectName
{
	tstring					ObjectName;
	tstring					ObjectKName;

	void operator=(const MapObjectName& _Other) 
	{
		ObjectName = _Other.ObjectName;
		ObjectKName = _Other.ObjectKName;
	}

	MapObjectName() :ObjectName(_T("")),ObjectKName(_T("")) {}
	MapObjectName(const tstring& _Name, const tstring& _KName) 
		:ObjectName(_Name), ObjectKName(_KName)
	{}
	MapObjectName(const MapObjectName& _Other) 
		:ObjectName(_Other.ObjectName), ObjectKName(_Other.ObjectKName)
	{}
	~MapObjectName() {}
};

class CEditTileMap;
class CGameScene;
class CEditTile;
class CEditMapMgr : public CObjBase
{
	
private:
	CGameScene*									m_pMapScene;
	CSceneBuilder*								m_pMapSceneBuilder;
	
	SPTR<CEditTileMap>							m_CurTileMap;
	map<UINT, SPTR<CEditTileMap>>				m_MainTileMap;
	
	// 월드맵을 통한 이동이 존재할때 캐릭터의 초기 타일 위치를 저장
	//map<TileMapKey, POINT>					m_WorldMapInitPoint;
	vector<MapObjectName>						m_ObjectName;


	SPTR<CEditTile>								m_PrevFocusTile;
	SPTR<CEditTile>								m_CurFocusTile;

	SPTR<class CEditMapObject>					m_CurMapObject;

public:
	//typedef CMainMapMgr CTileMapMgr;

	void Init(CGameScene* _MainScene, CSceneBuilder* _SceneBuilder);
	void Update();

public:
	CEditTileMap*  CreateTileMap(const TILEMAPDATA& _Data);
	SPTR<CEditTileMap> FindTileMap(const UINT& _TileMapNo);
	void InitializeTileMap();
	void InitCameraPos();
	SPTR<CEditTileMap>  ChangeTileMap(const UINT& _TileMapNo);
	const size_t GetTileMapCount();
	const POINT GetTileMapSize(const UINT& _TileMapNo);
	const POINT GetTileMapMaxIndex(const UINT& _TileMapNo);

	void SelectTileMapEvent(SPTR<CEditTile> TrigerTile , const MAINMAPNAME& _MapNo);
	void CurTileMapOn();
	void CurTileMapOff();

	
public:
	void FindFocusTile(const Vec2& _MousePos, const Vec2& _CameraPos, const float& _ZoomRatio);
	void NoFocusTile();
	SPTR<CEditTile>	GetCurFocusTile();

	void EditTileMap();
	void TileEdit(const TILEOPTION& _TileType,const UINT& _TileOption);
	void ObjectEdit(const UINT& _ObjectNo);

	void InitTileMap();
	void TileInit();
	void ObjectDelete();
	BOOL LoadTileData(FILE* _LoadFile);
	BOOL SaveTileData(FILE* _SaveFile);



public:
	const tstring GetObjectName(const UINT& _ObjectNo);
	const tstring GetObjectKName(const UINT& _ObjectNo);
	const size_t GetObjectCount();
	const POINT GetObjectSize(const UINT& _ObjectNo);
	void CurMapObjectEnable(const BOOL& _Enable);
	void ChangeCurMapObject(const UINT& _ObjectNo);

	BOOL LoadMapObjectData(CGameFile* _GameFile);
	BOOL SaveMapObjectData(FILE* _SaveFile);
public:

public:
	CEditMapMgr();
	~CEditMapMgr();
};

