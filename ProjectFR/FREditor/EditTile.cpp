#include "stdafx.h"
#include "EditTile.h"
#include "EditTileMap.h"
#include "EditMapMgr.h"
//#include<DebugMgr.h>
//#include<GameSprite.h>

CEditMapMgr*				CEditTile::MainMapMgr = nullptr;
bool						CEditTile::OptionRenderMode = true;
vector<D3DXCOLOR>			CEditTile::TileColor;
vector<vector<POINT>>		CEditTile::ChangeInitPoint;

void CEditTile::OptionRender(const bool& _Mode )
{
	OptionRenderMode = _Mode;

	
}
void CEditTile::TileClassInit(CEditMapMgr* _MapMgr)
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
CEditTile::CEditTile()
	:m_TilePivot(Vec3{ 0.0f,0.0f,0.0f }), m_TileIndex(POINT{-1,-1}), m_Option(STEPTILE)
	, m_TileOptionRen(nullptr), m_TileMap(nullptr), m_bCurling(false)
	, m_ChangeTileMapNo(0), m_TileColorNo(TILE_GREEN)
	, m_InitPortalIndex(POINT{-1,-1})
{
	//Init(m_Option);
}
CEditTile::CEditTile(CEditTileMap* _TileMap,const POINT& _Index, const Vec2& _Pivot, const UINT& _Option)
	: m_TilePivot(Vec3{ _Pivot.x,_Pivot.y,1.0f }), m_TileIndex(_Index)
	, m_Option((TILEOPTION)0), m_TileOptionRen(nullptr), m_TileMap(_TileMap), m_bCurling(false)
	, m_TileColorNo(TILE_GREEN)
	, m_InitPortalIndex(POINT{ -1,-1 })
	, m_MapChangeOption(0)
{
	Init((TILEOPTION)_Option);
}
CEditTile::CEditTile(CEditTileMap* _TileMap, const POINT& _Index, const Vec2& _Pivot, const TILEOPTION& _Option)
	: m_TilePivot(Vec3{ _Pivot.x,_Pivot.y,1.0f }), m_TileIndex(_Index)
	, m_Option(_Option), m_TileOptionRen(nullptr), m_TileMap(_TileMap), m_bCurling(false)
	, m_TileColorNo(TILE_GREEN)
	, m_MapChangeOption(0)
{
	Init(_Option);
}

CEditTile::~CEditTile()
{
}
void CEditTile::Init(const TILEOPTION& _Option)
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
	m_TileOptionRen = MapActor->CreateCom<CFixRenderer>(RENDATA(LAYER_BACKEFFECT,1.0f));
	m_TileOptionRen->SetRenPivot(m_TilePivot);
	SetTileOption(_Option);
	m_TileOptionRen->MaterialOn();
	

}
void CEditTile::Update()
{


}
void CEditTile::DebugRender(SPTR<CDebugMgr> _DegMgr)
{
	if (nullptr == _DegMgr)
		return;

	_DegMgr->RectRender(D3DCOLOR_ARGB(255, 0, 255, 0),m_TilePivot, CEditTileMap::TileSize);
}
void CEditTile::Curling(bool _Curling)
{
	m_bCurling = _Curling;
	if (true == m_bCurling)
		m_TileOptionRen->Off();
}

void CEditTile::OptionRenOn()
{
	if (false == OptionRenderMode)
		return;

	if (true == m_bCurling)
		return;

	m_TileOptionRen->On();
}
void CEditTile::OptionRenOff()
{
	m_TileOptionRen->Off();
}
void CEditTile::SetTileOption(const TILEOPTION& _Option)
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
void CEditTile::SetStepTile(const UINT& _TileColorNo)
{
	if (_TileColorNo >= TILE_RED)
		return;

	m_TileColorNo = _TileColorNo;
	SetTileOption(STEPTILE);
}
void CEditTile::SetStepStairTile(const UINT& _TileColorNo)
{
	if (_TileColorNo >= TILE_RED)
		return;

	m_TileColorNo = _TileColorNo;
	SetTileOption(STEPSTAIRTILE);
}
void CEditTile::SetWorldMapTile(const UINT& _ChangeType)
{
	if (MAXCHANGETYPE <= _ChangeType)
		return;

	m_MapChangeOption = _ChangeType;
	SetTileOption(WORLDMAPTILE);
}
void CEditTile::SetPortalTile(const UINT& _MapNo, const POINT& _InitTileIndex)
{
	SPTR<CEditTileMap> TileMap = MainMapMgr->FindTileMap(_MapNo);

	if (nullptr == TileMap)
		return;

	if (false == TileMap->ExistTileIndex(_InitTileIndex))
		return;


	m_ChangeTileMapNo = _MapNo;
	m_InitPortalIndex = _InitTileIndex;
	SetTileOption(MAPCHANGETILE);
}
const UINT CEditTile::GetTileColorNo()
{
	return m_TileColorNo;
}
const UINT CEditTile::GetChangeMapNo()
{
	return m_ChangeTileMapNo;
}
const POINT CEditTile::GetInitPortalIndex()
{
	return m_InitPortalIndex;
}
const POINT CEditTile::GetTileIndex()
{
	return m_TileIndex;
}
const TILEOPTION CEditTile::GetTileOption()
{
	return m_Option;
}
const tstring CEditTile::GetTileOptionString()
{
	switch (m_Option)
	{
	case STEPTILE:
		return _T("Tile Option : Step Tile");
	case STAIRTILE:
		return _T("Tile Option : Stair Tile");
	case STEPSTAIRTILE:
		return _T("Tile Option : Step & Stair Tile");
	case MAPCHANGETILE:
		return _T("Tile Option : Portal Tile");
	case WORLDMAPTILE:
		return _T("Tile Option : WorldMap Tile");
	case NOENTERTILE:
		return _T("Tile Option : NoEnter Tile");
	}

	return _T("");
}
const tstring CEditTile::GetTileColorString()
{
	switch (m_TileColorNo)
	{
	case CEditTile::TILE_GREEN:
		return _T("Tile Color : Green");
	case CEditTile::TILE_BLUE:
		return _T("Tile Color : Blue");
	case CEditTile::TILE_YELLOW:
		return _T("Tile Color : Yellow");
	case CEditTile::TILE_RED:
		return _T("Tile Color : Red");
	case CEditTile::TILE_BLACK:
		return _T("Tile Color : Black");;
	case CEditTile::TILE_GRAY:
		return _T("Tile Color : Gray");
	}

	return _T("");
}
const tstring CEditTile::GetSpawnIndexString()
{
	TCHAR Buf[MAXKEYLENGTH];

	_stprintf_s(Buf, _T("Spawn Index : %3d * %3d"), m_InitPortalIndex.x, m_InitPortalIndex.y);
	
	return Buf;
}
const tstring CEditTile::GetChangeTileMapName()
{
	switch (m_ChangeTileMapNo)
	{
	case TOWN:
		return _T("¿¬°á ¸Ê : ¸¶À»         ");
	case RUINS:
		return _T("¿¬°á ¸Ê : À¯Àû         ");
	case FOREST:
		return _T("¿¬°á ¸Ê : ÆøÇ³ÀÇ ½£    ");
	case DEVILCASTLE:
		return _T("¿¬°á ¸Ê : ¾Ç¸¶ ¼º      ");
	case WEANPONSTORE:
		return _T("¿¬°á ¸Ê : ¹«±â & ¹æ¾î±¸Á¡ ");
	case ITEMSTORE:
		return _T("¿¬°á ¸Ê : ¾ÆÀÌÅÛ »óÁ¡   ");
	}

	return _T("");
}
const tstring CEditTile::GetWorldMapTypeString()
{
	switch (m_MapChangeOption)
	{
	case TYPE1:
		return _T("World Map Type : Type 1");
	case TYPE2:
		return _T("World Map Type : Type 2");
	}
	return _T("");
}
void CEditTile::operator=(const CEditTile& _Other)
{
	m_TileIndex = _Other.m_TileIndex;
	m_TilePivot = _Other.m_TilePivot;
	m_Option = _Other.m_Option;
}
bool CEditTile::operator==(const CEditTile& _Other)
{
	if (m_TileMap != _Other.m_TileMap)
		return false;

	return m_TileIndex == _Other.m_TileIndex;
}
bool CEditTile::operator!=(const CEditTile& _Other)
{
	return !(m_TileIndex == _Other.m_TileIndex);
}

const D3DXCOLOR CEditTile::GetTileColor()
{
	return TileColor[m_TileColorNo];
}
const POINT CEditTile::GetMapChangeInitPoint(const MAINMAPNAME& _MapNo)
{
	if (_MapNo >= MAXMAINMAPNAME)
		return POINT{-1,-1};

	if(m_MapChangeOption >= MAXCHANGETYPE)
		return POINT{ -1,-1 };

	return ChangeInitPoint[m_MapChangeOption][_MapNo];
}
const TILEDATA CEditTile::GetTileData() 
{
	TILEDATA Temp = TILEDATA();
	
	Temp.TileType = m_Option;

	Temp.TileXIndex = m_TileIndex.x;
	Temp.TileYIndex = m_TileIndex.y;

	Temp.TileColorNo = m_TileColorNo;

	Temp.ChangeTileMapNo = m_ChangeTileMapNo;
	Temp.SpawnTileXIndex = m_InitPortalIndex.x;
	Temp.SpawnTileYIndex = m_InitPortalIndex.y;

	Temp.WorldMapOption = m_MapChangeOption;

	return Temp;
}
void CEditTile::SetTileData(const TILEDATA& _Data)
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