#pragma once
#include<Logic.h>

class CMapPortal;
class CEditTile;
class CEditTileMap : public CLogic
{
private :
	friend class CEditTile;
	friend class CEditMapObject;
	friend class CEditMapMgr;

	static SPTR<CCamera> MapCamera;
	static const Vec3 TileSize;

public:
	static void SetMainSceneCamera(SPTR<CCamera> _Camera);
	static void ReleaseMainSceneCamera();

private:
	UINT								m_TileMapNo;
	tstring								m_TileMapName;
	vector<vector<SPTR<CEditTile>>>		m_mapTile;
	
	Vec2								m_TileMapSize;
	POINT								m_MapMaxIndex;

	SPTR<CGameSprite>					m_TileBackground;
	SPTR<CFixRenderer>					m_BackgroundRen;

	//list<ObjectData>					m_listObjList
	list<SPTR<CEditMapObject>>			m_listMapObject;


	// 최근 맵 컬링을 수행하였을때의 카메라의 위치를 TileMap Index로 저장
	// 이 값이 바뀌었을때만 컬링 수행
	POINT								m_PrevCamPoint;
	bool								m_bOptionRen;

public:
	void InitData(const TILEMAPDATA& _Data);
	void Update();
	void PostUpdate();
	void DebugRender();
	CActObject* GetTileMapActor();
	
public:
	// 카메라의 추적 한계치를 타일맵 크기게 맞추워 조정
	void ChangeTileMapEvent();
	
	POINT CreateTile();
	UINT GetTileMapNo();
	SPTR<CGameSprite> GetBackgroundSprite();
	const Vec2 GetTileSize();

	// 매개변수로 받은 포스를 타일 인덱스로 변환(카메라 컬링용, 피봇적용)
	const POINT WolrdPosToTilePoint(const Vec3& _WorldPos);
	// 매개변수로 받은 포스를 타일 인덱스로 변환(타일 위치 찾기용, 피봇미적용)
	const POINT WolrdPosToTileIndex(const Vec3& _Player);
	// 매개변수로 받은 타일 인텍스를 월드 좌표로 변환(캐릭터 이동용, 피봇미적용)
	Vec2 TileIndexToWolrdPos2(const POINT& _TileIndex);
	SPTR<CEditTile> GetTile(const Vec3& _WorldPos);
	SPTR<CEditTile> GetTile(const POINT& _TileIndex);
	bool ChangeCameraPoint();
	void TileMapCurling();
	void TileMapOn();
	void TileMapOff();
	
	void AllTileCurling();
	void AllOptionTileRenOn();
	void AllOptionTileRenOff();

	const POINT GetTileMapSize();
	const POINT GetTileMapMaxIndex();
	const bool ExistTileIndex(const POINT& _TileIndex);

public:
	void TestTileSetting();
	
	bool IsEqual(const TILEMAPDATA& _Data);
public:
	const TILEMAPDATA GetTileMapData();
	void SaveTileMapData(FILE* _SaveFile);
	void LoadTileMapData(FILE* _LoadFile, const TILEMAPDATA& _Data);


public:
	void PushMapObject(SPTR<CEditMapObject> _Object);
	void PopMapObject(const POINT& _LBIndex);
	
	void SaveMapObjectData(FILE* _SaveFile);
	SPTR<CEditMapObject> FindMapObject(const POINT& _LBIndex);

public:
	CEditTileMap();
	CEditTileMap(SPTR<CGameSprite> _Background);
	~CEditTileMap();
};

