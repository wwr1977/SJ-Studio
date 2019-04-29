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

	// �ֱ� �� �ø��� �����Ͽ������� ī�޶��� ��ġ�� TileMap Index�� ����
	// �� ���� �ٲ�������� �ø� ����
	POINT							m_PrevCamPoint;
	bool							m_bOptionRen;
	
	
	


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
	const tstring GetTileMapName();
	SPTR<CGameSprite> GetBackgroundSprite();
	const Vec2 GetTileSize();

	// �Ű������� ���� ������ Ÿ�� �ε����� ��ȯ(ī�޶� �ø���, �Ǻ�����)
	const POINT WolrdPosToTilePoint(const Vec3& _WorldPos);
	// �Ű������� ���� ������ Ÿ�� �ε����� ��ȯ(Ÿ�� ��ġ ã���, �Ǻ�������)
	const POINT WolrdPosToTileIndex(const Vec3& _Player);
	// �Ű������� ���� Ÿ�� ���ؽ��� ���� ��ǥ�� ��ȯ(ĳ���� �̵���, �Ǻ�������)
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

