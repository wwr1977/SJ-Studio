#pragma once
#include<Logic.h>

class CMapPortal;
class CTile;
class CTileMap : public CLogic
{

private :
	friend class CTile;
	friend class CMapObject;

	static SPTR<CCamera> MapCamera;
	static const Vec3 TileSize;

public:
	static void SetMainSceneCamera(SPTR<CCamera> _Camera);
	static void ReleaseMainSceneCamera();
	
private:
	UINT							m_TileMapNo;
	tstring							m_TileMapName;
	vector<vector<SPTR<CTile>>>		m_mapTile;
	
	Vec2							m_TileMapSize;
	POINT							m_MapMaxIndex;

	SPTR<CGameSprite>				m_TileBackground;
	SPTR<CFixRenderer>				m_BackgroundRen;

	//list<ObjectData>				m_listObjList
	list<SPTR<CMapObject>>			m_listMapObject;
	list<SPTR<class CNPCBase>>		m_listMapNPC;

	// 최근 맵 컬링을 수행하였을때의 카메라의 위치를 TileMap Index로 저장
	// 이 값이 바뀌었을때만 컬링 수행
	POINT							m_PrevCamPoint;
	bool							m_bOptionRen;
	
	
	


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
	const tstring GetTileMapName();
	SPTR<CGameSprite> GetBackgroundSprite();
	const Vec2 GetTileSize();

	// 매개변수로 받은 포스를 타일 인덱스로 변환(카메라 컬링용, 피봇적용)
	const POINT WolrdPosToTilePoint(const Vec3& _WorldPos);
	// 매개변수로 받은 포스를 타일 인덱스로 변환(타일 위치 찾기용, 피봇미적용)
	const POINT WolrdPosToTileIndex(const Vec3& _Player);
	// 매개변수로 받은 타일 인텍스를 월드 좌표로 변환(캐릭터 이동용, 피봇미적용)
	Vec2 TileIndexToWolrdPos2(const POINT& _TileIndex);
	SPTR<CTile> GetTile(const Vec3& _WorldPos);
	SPTR<CTile> GetTile(const POINT& _TileIndex);
	bool ChangeCameraPoint();
	void TileMapCurling();
	void TileMapOn();
	void TileMapOff();
	
	void AllTileCurling();
	void AllOptionTileRenOn();
	void AllOptionTileRenOff();

	const POINT GetTileMapSize();
	const bool ExistTileIndex(const POINT& _TileIndex);

	
public:
	bool IsEqual(const TILEMAPDATA& _Data);
	void LoadTileMapData(FILE* _LoadFile, const TILEMAPDATA& _Data);

public:
	void PushMapObject(SPTR<CMapObject> _Object);
	void PushMapNPC(SPTR<CNPCBase> _NPC);
	void SetDebugMode(const bool& _Mode);
	void ChangeDebugMode();

public:
	void ResetTileMap();

public:
	void TestTileSetting();

public:
	CTileMap();
	CTileMap(SPTR<CGameSprite> _Background);
	~CTileMap();
};

