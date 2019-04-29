#pragma once
#include <ObjBase.h>

class CMainMapMgr;
class CDebugMgr;
class CTileMap;
class CTile : public CObjBase 
{	
public:
	enum 
	{
		TYPE1,
		TYPE2,
		MAXCHANGETYPE,
	};

private:
	enum TILECOLOR
	{
		TILE_GREEN,
		TILE_BLUE,
		TILE_YELLOW,
		TILE_RED,
		TILE_BLACK,
		TILE_GRAY,
	};

	static CMainMapMgr*				MainMapMgr;
	static bool						OptionRenderMode;
	static vector<D3DXCOLOR>		TileColor;
	static vector<vector<POINT>>	ChangeInitPoint;

public:
	static void OptionRender(const bool& _Mode = true );
	static void TileClassInit(CMainMapMgr* _MapMgr);
private:
	friend class CTileMap;

	CTileMap*					m_TileMap;
	// ���� ��� ������ ���
	//SPTR<CFixRenderer>			m_TileRen;
	// ����׿� ������ ���
	SPTR<CFixRenderer>			m_TileOptionRen;

	
	POINT						m_TileIndex;
	Vec3						m_TilePivot;
	TILEOPTION					m_Option;

	UINT						m_TileColorNo;
	UINT						m_ChangeTileMapNo;
	// ���� Ÿ���� ��Ż Ÿ���� ��� Ÿ�ϸ��� �ٲ� ���� 
	// ĳ���Ͱ� ó�� ��ġ�� Ÿ�� �ε����� ����
	POINT						m_InitPortalIndex;

	// TILEOPTION�� ����� ü������ ��� 
	// �� Ÿ���� �ʱ� Ÿ�� ��ġ �ε����� ����� ���� �ɼǰ�
	UINT						m_MapChangeOption;
	
	bool						m_bCurling;
public:
	void Init(const TILEOPTION& _Option);
	void Update();
	void DebugRender(SPTR<CDebugMgr> _DegMgr);

public:
	void Curling(bool _Curling = true);
	void OptionRenOn();
	void OptionRenOff();
	void SetTileOption(const TILEOPTION& _Option);
	void SetStepTile(const UINT& _TileColorNo);
	void SetStepStairTile(const UINT& _TileColorNo);
	void SetWorldMapTile(const UINT& _ChangeType);
	void SetPortalTile(const UINT& _MapNo, const POINT& _InitTileIndex);
	const UINT GetTileColorNo();

public:
	const UINT GetChangeMapNo();
	const UINT GetChangeMapOption();
	const POINT GetInitPortalIndex();
	const POINT	GetTileIndex();
	const TILEOPTION GetTileOption();
	const D3DXCOLOR GetTileColor();

	// World ���� ���� Ÿ�ϸ� �̵��ÿ� Ʈ���Ű� �� Ÿ�Ͽ��� 
	// �ʱ�ȭ ��ġ POINT�� ��û�Ѵ�
	const POINT GetMapChangeInitPoint(const MAINMAPNAME& _MapNo);

public:
	void SetTileData(const TILEDATA& _Data);

public:
	void operator=(const CTile& _Other);
	bool operator==(const CTile& _Other);
	bool operator!=(const CTile& _Other);

public:
	CTile();
	CTile(CTileMap* _TileMap, const POINT& _Index, const Vec2& _Pivot, const UINT& _Option);
	CTile(CTileMap* _TileMap, const POINT& _Index, const Vec2& _Pivot, const TILEOPTION& _Option);
	~CTile();
};

