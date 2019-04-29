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


	// �ֱ� �� �ø��� �����Ͽ������� ī�޶��� ��ġ�� TileMap Index�� ����
	// �� ���� �ٲ�������� �ø� ����
	POINT								m_PrevCamPoint;
	bool								m_bOptionRen;

public:
	void InitData(const TILEMAPDATA& _Data);
	void Update();
	void PostUpdate();
	void DebugRender();
	CActObject* GetTileMapActor();
	
public:
	// ī�޶��� ���� �Ѱ�ġ�� Ÿ�ϸ� ũ��� ���߿� ����
	void ChangeTileMapEvent();
	
	POINT CreateTile();
	UINT GetTileMapNo();
	SPTR<CGameSprite> GetBackgroundSprite();
	const Vec2 GetTileSize();

	// �Ű������� ���� ������ Ÿ�� �ε����� ��ȯ(ī�޶� �ø���, �Ǻ�����)
	const POINT WolrdPosToTilePoint(const Vec3& _WorldPos);
	// �Ű������� ���� ������ Ÿ�� �ε����� ��ȯ(Ÿ�� ��ġ ã���, �Ǻ�������)
	const POINT WolrdPosToTileIndex(const Vec3& _Player);
	// �Ű������� ���� Ÿ�� ���ؽ��� ���� ��ǥ�� ��ȯ(ĳ���� �̵���, �Ǻ�������)
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

