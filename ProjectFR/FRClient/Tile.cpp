#include "Precom.h"
#include "Tile.h"
#include "TileMap.h"
#include"MainMapMgr.h"
#include<DebugMgr.h>
#include<GameSprite.h>

CMainMapMgr*				CTile::MainMapMgr = nullptr;
bool						CTile::OptionRenderMode = true;
vector<D3DXCOLOR>			CTile::TileColor;
vector<vector<POINT>>		CTile::ChangeInitPoint;

void CTile::OptionRender(const bool& _Mode )
{
	OptionRenderMode = _Mode;

	
}
void CTile::TileClassInit(CMainMapMgr* _MapMgr)
{
	TileColor.push_back(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
	TileColor.push_back(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
	TileColor.push_back(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
	TileColor.push_back(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	TileColor.push_back(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
	TileColor.push_back(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));

	if (nullptr == _MapMgr)
	{
		assert(nullptr);
		return;
	}

	MainMapMgr = _MapMgr;

	ChangeInitPoint.clear();
	vector<POINT> Temp;
	Temp.clear();
	Temp.push_back(POINT{ 14,37 });
	Temp.push_back(POINT{ 9,37 });
	Temp.push_back(POINT{ 24,47 });
	Temp.push_back(POINT{ 24,46 });
	ChangeInitPoint.push_back(Temp);
	Temp[2] = POINT{ 41,1 };
	ChangeInitPoint.push_back(Temp);

	ChangeInitPoint.push_back(Temp);
}
CTile::CTile()
	:m_TilePivot(Vec3{ 0.0f,0.0f,0.0f }), m_TileIndex(POINT{-1,-1}), m_Option(STEPTILE)
	, m_TileOptionRen(nullptr), m_TileMap(nullptr), m_bCurling(false)
	, m_ChangeTileMapNo(0), m_TileColorNo(TILE_GREEN)
	, m_InitPortalIndex(POINT{-1,-1})
{
	//Init(m_Option);
}
CTile::CTile(CTileMap* _TileMap,const POINT& _Index, const Vec2& _Pivot, const UINT& _Option)
	: m_TilePivot(Vec3{ _Pivot.x,_Pivot.y,1.0f }), m_TileIndex(_Index)
	, m_Option((TILEOPTION)0), m_TileOptionRen(nullptr), m_TileMap(_TileMap), m_bCurling(false)
	, m_TileColorNo(TILE_GREEN)
	, m_InitPortalIndex(POINT{ -1,-1 })
	, m_MapChangeOption(0)
{
	Init((TILEOPTION)_Option);
}
CTile::CTile(CTileMap* _TileMap, const POINT& _Index, const Vec2& _Pivot, const TILEOPTION& _Option)
	: m_TilePivot(Vec3{ _Pivot.x,_Pivot.y,1.0f }), m_TileIndex(_Index)
	, m_Option(_Option), m_TileOptionRen(nullptr), m_TileMap(_TileMap), m_bCurling(false)
	, m_TileColorNo(TILE_GREEN)
	, m_MapChangeOption(0)
{
	Init(_Option);
}

CTile::~CTile()
{
}
void CTile::Init(const TILEOPTION& _Option)
{
	if (nullptr == m_TileMap) 
	{
		assert(nullptr);
		return;
	}

	CActObject* MapActor = m_TileMap->GetTileMapActor();

	if (nullptr == MapActor)
	{
		assert(nullptr);
		return;
	}

	// Tile Render¿ë ÄÚµå
	//SPTR<CGameSprite> TileMapBack = m_TileMap->GetBackgroundSprite();
	//Vec2 TileSize = m_TileMap->GetTileSize();

	//m_TileRen = MapActor->CreateCom<CFixRenderer>(RENDATA(LAYER_BACKGROUND, 1.0f));
	//m_TileRen->SetRenPivot(m_TilePivot);
	//m_TileRen->SetSprite(TileMapBack->ArticleSprite(Vec2{TileSize.x*m_TileIndex.x, TileSize.y*m_TileIndex.y }, TileSize));
	//

	m_TileOptionRen = MapActor->CreateCom<CFixRenderer>(RENDATA(LAYER_BACKEFFECT,1.0f));
	m_TileOptionRen->SetRenPivot(m_TilePivot);
	SetTileOption(_Option);
	m_TileOptionRen->MaterialOn();
	

}
void CTile::Update()
{
	if (false == IsActive())
		return;



	int a = 0;
}
void CTile::DebugRender(SPTR<CDebugMgr> _DegMgr)
{
	if (nullptr == _DegMgr)
		return;

	_DegMgr->RectRender(D3DCOLOR_ARGB(255, 0, 255, 0),m_TilePivot, CTileMap::TileSize);
}
void CTile::Curling(bool _Curling)
{
	m_bCurling = _Curling;
	if (true == m_bCurling)
		m_TileOptionRen->Off();
}

void CTile::OptionRenOn() 
{
	if (false == OptionRenderMode)
		return;

	if (true == m_bCurling)
		return;

	m_TileOptionRen->On();
}
void CTile::OptionRenOff()
{
	m_TileOptionRen->Off();
}
void CTile::SetTileOption(const TILEOPTION& _Option)
{
	if (_Option >= MAXTILETOPTION)
		return;

	m_Option = _Option;

	switch (_Option)
	{
	case STEPTILE:
	{
		m_TileOptionRen->SetSprite(_T("TileXBase"));
		m_TileOptionRen->SetMaterial(TileColor[m_TileColorNo]);
	}
	break;
	case STAIRTILE:
	{
		m_TileOptionRen->SetSprite(_T("TileBase"));
		m_TileOptionRen->SetMaterial(TileColor[TILE_GRAY]);
	}
	break;
	case STEPSTAIRTILE:
	{
		m_TileOptionRen->SetSprite(_T("TileBase"));
		m_TileOptionRen->SetMaterial(TileColor[m_TileColorNo]);
	}
	break;
	case MAPCHANGETILE:
	{
		m_TileOptionRen->SetSprite(_T("TileXBase"));
		m_TileOptionRen->SetMaterial(TileColor[TILE_BLACK]);
	}
	break;
	case WORLDMAPTILE:
	{
		m_TileOptionRen->SetSprite(_T("TileXBase"));
		m_TileOptionRen->SetMaterial(TileColor[TILE_GRAY]);
	}
	break;
	case NOENTERTILE:
	{
		m_TileOptionRen->SetSprite(_T("TileXBase"));
		m_TileOptionRen->SetMaterial(TileColor[TILE_RED]);
	}
		break;
	}
	
}
void CTile::SetStepTile(const UINT& _TileColorNo)
{
	if (_TileColorNo >= TILE_RED)
		return;

	m_TileColorNo = _TileColorNo;
	SetTileOption(STEPTILE);
}
void CTile::SetStepStairTile(const UINT& _TileColorNo)
{
	if (_TileColorNo >= TILE_RED)
		return;

	m_TileColorNo = _TileColorNo;
	SetTileOption(STEPSTAIRTILE);
}
void CTile::SetWorldMapTile(const UINT& _ChangeType)
{
	if (MAXCHANGETYPE <= _ChangeType)
		return;

	m_MapChangeOption = _ChangeType;
	SetTileOption(WORLDMAPTILE);
}
void CTile::SetPortalTile(const UINT& _MapNo, const POINT& _InitTileIndex)
{
	SPTR<CTileMap> TileMap = MainMapMgr->FindTileMap(_MapNo);

	if (nullptr == TileMap)
		return;

	if (false == TileMap->ExistTileIndex(_InitTileIndex))
		return;


	m_ChangeTileMapNo = _MapNo;
	m_InitPortalIndex = _InitTileIndex;
	SetTileOption(MAPCHANGETILE);
}
const UINT CTile::GetTileColorNo()
{
	return m_TileColorNo;
}
const UINT CTile::GetChangeMapNo()
{
	return m_ChangeTileMapNo;
}
const UINT CTile::GetChangeMapOption()
{
	return m_MapChangeOption;
}
const POINT CTile::GetInitPortalIndex()
{
	return m_InitPortalIndex;
}
const POINT CTile::GetTileIndex()
{
	return m_TileIndex;
}
const TILEOPTION CTile::GetTileOption()
{
	return m_Option;
}
void CTile::operator=(const CTile& _Other)
{
	m_TileIndex = _Other.m_TileIndex;
	m_TilePivot = _Other.m_TilePivot;
	m_Option = _Other.m_Option;
}
bool CTile::operator==(const CTile& _Other)
{
	if (m_TileMap != _Other.m_TileMap)
		return false;

	return m_TileIndex == _Other.m_TileIndex;
}
bool CTile::operator!=(const CTile& _Other) 
{
	return !(m_TileIndex == _Other.m_TileIndex);
}

const D3DXCOLOR CTile::GetTileColor()
{
	return TileColor[m_TileColorNo];
}
const POINT CTile::GetMapChangeInitPoint(const MAINMAPNAME& _MapNo)
{
	if (_MapNo >= MAXMAINMAPNAME)
		return POINT{-1,-1};

	if(m_MapChangeOption >= MAXCHANGETYPE)
		return POINT{ -1,-1 };

	return ChangeInitPoint[m_MapChangeOption][_MapNo];
}
void CTile::SetTileData(const TILEDATA& _Data)
{
	m_Option = (TILEOPTION)_Data.TileType;
	m_TileIndex.x = _Data.TileXIndex;
	m_TileIndex.y = _Data.TileYIndex;

	m_TileColorNo = _Data.TileColorNo;

	m_ChangeTileMapNo = _Data.ChangeTileMapNo;
	m_InitPortalIndex.x = _Data.SpawnTileXIndex;
	m_InitPortalIndex.y = _Data.SpawnTileYIndex;

	m_MapChangeOption = _Data.WorldMapOption;

	switch (m_Option)
	{
	case STEPTILE:
		SetStepTile(m_TileColorNo);
		break;
	case STAIRTILE:
	case NOENTERTILE:
		SetTileOption(m_Option);
		break;
	case STEPSTAIRTILE:
		SetStepStairTile(m_TileColorNo);
		break;
	case MAPCHANGETILE:
		SetPortalTile(m_ChangeTileMapNo, m_InitPortalIndex);
		break;
	case WORLDMAPTILE:
		SetWorldMapTile(m_MapChangeOption);
		break;
	}
}