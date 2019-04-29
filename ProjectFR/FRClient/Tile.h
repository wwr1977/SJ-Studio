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
	// 실제 배경 랜더링 담당
	//SPTR<CFixRenderer>			m_TileRen;
	// 디버그용 랜더링 담당
	SPTR<CFixRenderer>			m_TileOptionRen;

	
	POINT						m_TileIndex;
	Vec3						m_TilePivot;
	TILEOPTION					m_Option;

	UINT						m_TileColorNo;
	UINT						m_ChangeTileMapNo;
	// 현재 타일이 포탈 타일일 경우 타일맵을 바꾼 이후 
	// 캐릭터가 처음 위치할 타일 인덱스를 저장
	POINT						m_InitPortalIndex;

	// TILEOPTION이 월드맵 체인지인 경우 
	// 이 타일이 초기 타일 위치 인덱스를 출력을 위한 옵션값
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

	// World 맵을 통한 타일맵 이동시에 트리거가 된 타일에게 
	// 초기화 위치 POINT를 요청한다
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

