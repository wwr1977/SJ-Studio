#include "stdafx.h"
#include "EditTileMap.h"
#include "EditTile.h"
#include <GameSprite.h>
#include <FixRenderer.h>
#include "EditMapObject.h"

SPTR<CCamera> CEditTileMap::MapCamera = nullptr;
const Vec3 CEditTileMap::TileSize = Vec3{64.0f,64.0f,1.0f};

void CEditTileMap::SetMainSceneCamera(SPTR<CCamera> _Camera)
{
	if (nullptr == _Camera)
	{
		assert(nullptr);
		return;
	}

	MapCamera = _Camera;
}
void CEditTileMap::ReleaseMainSceneCamera()
{
	MapCamera = nullptr;
}
CEditTileMap::CEditTileMap()
	: m_TileMapNo(-1), m_TileMapName(_T(""))
	, m_TileBackground(nullptr)
	, m_PrevCamPoint(POINT{-1,-1})
	, m_bOptionRen(true)
	, m_MapMaxIndex(POINT{-1,-1})
{
	m_mapTile.clear();
}
CEditTileMap::CEditTileMap(SPTR<CGameSprite> _Background)
	: m_TileMapNo(-1), m_TileMapName(_T(""))
	, m_TileBackground(_Background)
	, m_PrevCamPoint(POINT{ -1,-1 })
	, m_bOptionRen(true)
	, m_MapMaxIndex(POINT{ -1,-1 })
{

}
CEditTileMap::~CEditTileMap()
{
	for (size_t j = 0; j < m_mapTile.size(); ++j)
		m_mapTile[j].clear();
	
	m_mapTile.clear();

	m_listMapObject.clear();
}

void CEditTileMap::InitData(const TILEMAPDATA& _Data)
{
	m_TileMapNo = _Data.TileMapNo;
	m_TileMapName = _Data.TileMapName;
	m_TileBackground = COMRESMGR->FindGameSprite(m_TileMapName);

	if (nullptr == m_TileBackground)
		return;

	m_BackgroundRen = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_BACKGROUND, 1.0f));
	m_BackgroundRen->SetSprite(m_TileBackground);

	if (nullptr == m_TileBackground)
		return;

	Vec2 TileMapSize = m_TileBackground->GetSpriteSize();

	float Left = TileMapSize.x * -0.5f;
	float Up = TileMapSize.y * 0.5f;

	Vec2 Start = Vec2{ Left + TileSize.x * 0.5f ,Up - TileSize.y * 0.5f };

	LONG XCount = (LONG)(TileMapSize.x / TileSize.x);
	LONG YCount = (LONG)(TileMapSize.y / TileSize.y);

	m_TileMapSize = Vec2{ (float)XCount* TileSize.x ,(float)YCount* TileSize.y };
	m_MapMaxIndex = POINT{ XCount, YCount };
	m_mapTile.clear();


	vector<SPTR<CEditTile>> Temp;
	Temp.assign(XCount,nullptr);
	m_mapTile.assign(YCount, Temp);

}

void CEditTileMap::Update()
{
	if (true == KEYDOUBLE("ChangeDebugMode")) 
	{
		m_bOptionRen = !m_bOptionRen;

		CEditTile::OptionRender(m_bOptionRen);
	
		if (true == m_bOptionRen) 
			AllOptionTileRenOn();
		else
			AllOptionTileRenOff();
	}
}
void CEditTileMap::PostUpdate()
{
	/*if (true == ChangeCameraPoint())
		TileMapCurling();*/
}
void CEditTileMap::DebugRender()
{
	
}
CActObject* CEditTileMap::GetTileMapActor()
{
	return ACTOR;
}
void CEditTileMap::ChangeTileMapEvent()
{
	if (nullptr == MapCamera)
		return;

	POINT WndSize = CEditGlobal::MainWindow->WndSize();
	Vec2 TRange = Vec2{ m_TileMapSize.x*0.5f - (float)(WndSize.x / 2) ,m_TileMapSize.y*0.5f - (float)(WndSize.y / 2) };

	MapCamera->SetTrackingRange(TRange);
	
}
POINT CEditTileMap::CreateTile()
{
	if (nullptr == m_TileBackground)
		return POINT{ 0,0 };

	float Left = m_TileMapSize.x * -0.5f;
	float Up = m_TileMapSize.y * 0.5f;

	Vec2 Start = Vec2{ Left + TileSize.x * 0.5f ,Up - TileSize.y * 0.5f };

	LONG XCount = m_MapMaxIndex.x;
	LONG YCount = m_MapMaxIndex.y;

	for (LONG j = 0; j < YCount; ++j)
	{
		for (LONG i = 0; i < XCount; ++i)
		{
			Vec2 TilePivot = Start + Vec2{ TileSize.x*i,TileSize.y*-j };
			m_mapTile[j][i] = new CEditTile(this , POINT{ i,j }, TilePivot, STEPTILE);
		}
	}

	TestTileSetting();

	TileMapOff();
	return POINT{ XCount ,YCount };
}
UINT CEditTileMap::GetTileMapNo()
{
	return m_TileMapNo;
}
SPTR<CGameSprite> CEditTileMap::GetBackgroundSprite()
{
	return m_TileBackground;
}
const Vec2 CEditTileMap::GetTileSize()
{
	return Vec2{ TileSize.x,TileSize.y };
}
const POINT CEditTileMap::WolrdPosToTilePoint(const Vec3& _WorldPos)
{
	POINT TilePoint = POINT{ 0,0 };

	Vec3 TilePos = Vec3{ 0.0f,0.0f,_WorldPos.z };

	TilePos.x = _WorldPos.x + (m_TileMapSize.x * 0.5f);
	TilePos.y = -(_WorldPos.y - (m_TileMapSize.y * 0.5f));

	TilePoint.x = (LONG)((TilePos.x - TileSize.x * 0.5f) / TileSize.x);
	TilePoint.y = (LONG)((TilePos.y - TileSize.y * 0.5f) / TileSize.y);

	return TilePoint;
}
const POINT CEditTileMap::WolrdPosToTileIndex(const Vec3& _WorldPos)
{
	POINT TilePoint = POINT{ -1,-1 };

	Vec3 TilePos = Vec3{ 0.0f,0.0f,_WorldPos.z };

	TilePos.x = _WorldPos.x + (m_TileMapSize.x * 0.5f);
	TilePos.y = -(_WorldPos.y - (m_TileMapSize.y * 0.5f));

	TilePoint.x = (LONG)floorf(TilePos.x / TileSize.x);
	TilePoint.y = (LONG)floorf(TilePos.y / TileSize.y);

	return TilePoint;
}
Vec2 CEditTileMap::TileIndexToWolrdPos2(const POINT& _TileIndex)
{
	Vec2 Pos = Vec2{ TileSize.x*0.5f,TileSize.y*0.5f };

	Pos.x = (Pos.x + TileSize.x * _TileIndex.x) + (m_TileMapSize.x *-0.5f);
	Pos.y = (Pos.y + TileSize.y * _TileIndex.y) + (m_TileMapSize.y *-0.5f);

	Pos.y = -Pos.y;

	return Pos;
}
SPTR<CEditTile> CEditTileMap::GetTile(const Vec3& _WorldPos)
{
	return GetTile(WolrdPosToTileIndex(_WorldPos));
}
SPTR<CEditTile> CEditTileMap::GetTile(const POINT& _TileIndex)
{
	LONG X = _TileIndex.x, Y = _TileIndex.y;

	if (X < 0) 
		return nullptr;

	if (Y < 0)
		return nullptr;

	if (m_mapTile.size() <= Y)
		return nullptr;

	if (m_mapTile[Y].size() <= X)
		return nullptr;

	return m_mapTile[Y][X];
}
bool CEditTileMap::ChangeCameraPoint()
{
	if (nullptr == MapCamera)
	{
		assert(nullptr);
		return false;
	}

	POINT CamPoint  = WolrdPosToTilePoint(MapCamera->GetCameraPos());

	if (m_PrevCamPoint == CamPoint)
		return false;

	m_PrevCamPoint = CamPoint;
	return true;
}
void CEditTileMap::TileMapCurling()
{
	AllTileCurling();

	POINT WndSize = CEditGlobal::MainWindow->WndSize();
	int XHCount = (int)((WndSize.x * 0.5f) / TileSize.x );
	int YHCount = (int)((WndSize.y * 0.5f) / TileSize.y );

	size_t StartX, EndX, StartY, EndY;

	StartX = max(m_PrevCamPoint.x - XHCount, 0);
	StartY = max(m_PrevCamPoint.y - YHCount, 0);
	
	EndX = min(m_PrevCamPoint.x + XHCount + 1 , m_mapTile[0].size()-1);
	EndY = min(m_PrevCamPoint.y + YHCount + 1, m_mapTile.size()-1);

	for (size_t j = StartY; j <= EndY; ++j)
	{
		for (size_t i = StartX; i <= EndX; i++)
		{
			if (nullptr != m_mapTile[j][i]) 
			{
				m_mapTile[j][i]->Curling(false);
				m_mapTile[j][i]->OptionRenOn();
			}
		}
	}

	return;
}
void CEditTileMap::TileMapOn()
{
	ACTOR->On();
	AllOptionTileRenOn();


	list<SPTR<CEditMapObject>>::iterator Start = m_listMapObject.begin();
	list<SPTR<CEditMapObject>>::iterator End = m_listMapObject.end();

	for (; Start != End; ++Start)
	{
		if(nullptr != (*Start))
			(*Start)->MapObjectOn();
	}
}
void CEditTileMap::TileMapOff()
{
	ACTOR->Off();
	AllOptionTileRenOff();


	list<SPTR<CEditMapObject>>::iterator Start = m_listMapObject.begin();
	list<SPTR<CEditMapObject>>::iterator End = m_listMapObject.end();

	for (; Start != End; ++Start)
	{
		if (nullptr != (*Start))
			(*Start)->MapObjectOff();
	}

}
void CEditTileMap::AllTileCurling()
{
	for (size_t j = 0; j < m_mapTile.size(); ++j)
	{
		for (size_t i = 0; i < m_mapTile[j].size(); ++i)
		{
			if (nullptr != m_mapTile[j][i])
				m_mapTile[j][i]->Curling();
		}
	}
}

void CEditTileMap::AllOptionTileRenOn()
{
	for (size_t j = 0; j < m_mapTile.size(); ++j)
	{
		for (size_t i = 0; i < m_mapTile[j].size(); ++i)
		{
			if (nullptr != m_mapTile[j][i])
				m_mapTile[j][i]->OptionRenOn();
		}
	}
}

void CEditTileMap::AllOptionTileRenOff()
{
	for (size_t j = 0; j < m_mapTile.size(); ++j)
	{
		for (size_t i = 0; i < m_mapTile[j].size(); ++i)
		{
			if (nullptr != m_mapTile[j][i]) 
				m_mapTile[j][i]->OptionRenOff();
			
		}
	}
}
const POINT CEditTileMap::GetTileMapSize()
{
	return POINT{ (LONG)m_mapTile[0].size() ,(LONG)m_mapTile.size() };
}
const POINT CEditTileMap::GetTileMapMaxIndex()
{
	return POINT{ (LONG)m_mapTile[0].size() -1 ,(LONG)m_mapTile.size() - 1 };
}
const bool CEditTileMap::ExistTileIndex(const POINT& _TileIndex)
{
	if (_TileIndex.x >= m_mapTile[0].size())
		return false;
	if (_TileIndex.y >= m_mapTile.size())
		return false;

	return true;
}
//////////////////////////////////////// TEST ¿ë ///////////////////////////////
void CEditTileMap::TestTileSetting()
{
	//if (m_TileMapNo == TOWN)
	//{
	//	for (LONG j = 18; j <= 23; ++j)
	//	{
	//		for (LONG i = 11; i <= 18; ++i)
	//			m_mapTile[j][i]->SetTileOption(NOENTERTILE);
	//	}

	//	m_mapTile[23][12]->SetPortalTile(WEANPONSTORE, POINT{ 10,11 });
	//	m_mapTile[23][17]->SetPortalTile(WEANPONSTORE, POINT{ 31,11 });

	//	/*m_mapTile[23][22]->SetTileOption(STAIRTILE);
	//	m_mapTile[23][23]->SetStepStairTile(CTile::TILE_GREEN);*/


	//	for (LONG i = 2; i <= 9; ++i)
	//		m_mapTile[19][i]->SetStepTile(CEditTile::TILE_BLUE);
	//	for (LONG i = 2; i <= 5; ++i)
	//		m_mapTile[20][i]->SetStepTile(CEditTile::TILE_BLUE);
	//	for (LONG i = 2; i <= 4; ++i)
	//		m_mapTile[21][i]->SetStepTile(CEditTile::TILE_BLUE);
	//	for (LONG i = 2; i <= 4; ++i)
	//		m_mapTile[22][i]->SetStepTile(CEditTile::TILE_BLUE);

	//	m_mapTile[20][9]->SetStepTile(CEditTile::TILE_BLUE);

	//	for (LONG i = 6; i <= 8; ++i)
	//		m_mapTile[21][i]->SetTileOption(STAIRTILE);

	//	for (LONG i = 6; i <= 8; ++i)
	//		m_mapTile[22][i]->SetStepStairTile(CEditTile::TILE_GREEN);
	//	for (LONG i = 6; i <= 8; ++i)
	//		m_mapTile[20][i]->SetStepStairTile(CEditTile::TILE_BLUE);

	//	m_mapTile[39][15]->SetWorldMapTile(CEditTile::TYPE1);
	//	m_mapTile[39][14]->SetWorldMapTile(CEditTile::TYPE1);
	//	m_mapTile[39][13]->SetWorldMapTile(CEditTile::TYPE1);

	//	/*m_mapTile[39][15]->SetTileOption(WORLDMAPTILE);
	//	m_mapTile[39][14]->SetTileOption(WORLDMAPTILE);
	//	m_mapTile[39][13]->SetTileOption(WORLDMAPTILE);*/
	//}
	//else if (m_TileMapNo == RUINS)
	//{
	//	/*m_mapTile[39][7]->SetTileOption(WORLDMAPTILE);
	//	m_mapTile[39][8]->SetTileOption(WORLDMAPTILE);
	//	m_mapTile[39][9]->SetTileOption(WORLDMAPTILE);
	//	m_mapTile[39][10]->SetTileOption(WORLDMAPTILE);*/
	//	m_mapTile[39][7]->SetWorldMapTile(CEditTile::TYPE1);
	//	m_mapTile[39][8]->SetWorldMapTile(CEditTile::TYPE1);
	//	m_mapTile[39][9]->SetWorldMapTilesssssssaaaaaaaasssssassssssssssssssssssw(CEditTile::TYPE1);
	//	m_mapTile[39][10]->SetWorldMapTile(CEditTile::TYPE1);
	//}
	//else if (m_TileMapNo == FOREST)
	//{
	//	/*
	//	m_mapTile[49][23]->SetTileOption(WORLDMAPTILE);
	//	m_mapTile[49][24]->SetTileOption(WORLDMAPTILE);
	//	m_mapTile[49][25]->SetTileOption(WORLDMAPTILE);
	//	m_mapTile[49][26]->SetTileOption(WORLDMAPTILE);
	//	m_mapTile[49][27]->SetTileOption(WORLDMAPTILE);
	//	m_mapTile[49][28]->SetTileOption(WORLDMAPTILE);
	//	*/
	//	m_mapTile[49][23]->SetWorldMapTile(CEditTile::TYPE1);
	//	m_mapTile[49][24]->SetWorldMapTile(CEditTile::TYPE1);
	//	m_mapTile[49][25]->SetWorldMapTile(CEditTile::TYPE1);
	//	m_mapTile[49][26]->SetWorldMapTile(CEditTile::TYPE1);
	//	m_mapTile[49][27]->SetWorldMapTile(CEditTile::TYPE1);
	//	m_mapTile[49][28]->SetWorldMapTile(CEditTile::TYPE1);


	//	m_mapTile[0][36]->SetWorldMapTile(CEditTile::TYPE2);
	//	m_mapTile[0][37]->SetWorldMapTile(CEditTile::TYPE2);
	//	m_mapTile[0][38]->SetWorldMapTile(CEditTile::TYPE2);
	//	m_mapTile[0][39]->SetWorldMapTile(CEditTile::TYPE2);
	//	m_mapTile[0][40]->SetWorldMapTile(CEditTile::TYPE2);
	//	m_mapTile[0][41]->SetWorldMapTile(CEditTile::TYPE2);
	//	m_mapTile[0][42]->SetWorldMapTile(CEditTile::TYPE2);
	//	m_mapTile[0][43]->SetWorldMapTile(CEditTile::TYPE2);
	//}
	//else if (m_TileMapNo == DEVILCASTLE)
	//{
	//	m_mapTile[49][24]->SetWorldMapTile(CEditTile::TYPE2);
	//	
	//}
	//else if (m_TileMapNo == WEANPONSTORE)
	//{
	//	m_mapTile[12][10]->SetPortalTile(TOWN, POINT{ 12,24 });
	//	m_mapTile[12][31]->SetPortalTile(TOWN, POINT{ 17,24 });
	//}
}
bool CEditTileMap::IsEqual(const TILEMAPDATA& _Data)
{
	if (m_TileMapNo != _Data.TileMapNo)
		return false;

	if (_Data.TileSizeX != m_mapTile[0].size())
		return false;

	if (_Data.TileSizeY != m_mapTile.size())
		return false;

	return true;
}
const TILEMAPDATA CEditTileMap::GetTileMapData()
{
	TILEMAPDATA Temp = TILEMAPDATA();
	
	Temp.TileMapNo = m_TileMapNo;
	Temp.SetTileMapName(m_TileMapName);
	Temp.TileSizeX = (UINT)m_mapTile[0].size();
	Temp.TileSizeY = (UINT)m_mapTile.size();
	
	return Temp;
}
void CEditTileMap::SaveTileMapData(FILE* _SaveFile)
{
	if (nullptr == _SaveFile)
		return;


	TILEMAPDATA Temp = this->GetTileMapData();
	size_t Check =Temp.WriteTileMapData(_SaveFile);

	size_t BufSize = sizeof(TILEDATA)*m_mapTile[0].size();
	TILEDATA* Buf = (TILEDATA*)malloc(BufSize);

	memset(Buf, -1, BufSize);

	for (size_t j = 0; j < m_mapTile.size(); j++)
	{
		size_t WriteCount = m_mapTile[j].size();

		for (size_t i = 0; i < m_mapTile[j].size(); i++)
			Buf[i] = m_mapTile[j][i]->GetTileData();
		
		TILEDATA::WriteTileData(_SaveFile,Buf, WriteCount);

		memset(Buf, -1, BufSize);
	}

	free(Buf);
	return;
}
void CEditTileMap::LoadTileMapData(FILE* _LoadFile, const TILEMAPDATA& _Data)
{
	if (nullptr == _LoadFile)
		return;

	if ( false == this->IsEqual(_Data)) 
	{
		assert(nullptr);
		return;
	}
	
	//size_t BufSize = sizeof(TILEDATA)*m_mapTile[0].size();
	//
	//TILEDATA* Buf = (TILEDATA*)malloc(BufSize);
	//memset(Buf, -1, BufSize);

	//size_t ReadCount = 0;
	//
	//for (size_t j = 0; j < m_mapTile.size(); ++j)
	//{
	//	ReadCount = m_mapTile[j].size();
	//	TILEDATA::ReadTileMapData(_LoadFile, Buf, ReadCount);
	//	
	//	for (size_t i = 0; i < ReadCount; i++)
	//		m_mapTile[j][i]->SetTileData(Buf[i]);
	//	
	//	memset(Buf, -1, BufSize);
	//}

	//free(Buf);

	size_t ReadCount = m_mapTile[0].size()*m_mapTile.size();
	size_t BufSize = sizeof(TILEDATA)*ReadCount;

	TILEDATA* Buf = (TILEDATA*)malloc(BufSize);
	memset(Buf, -1, BufSize);

	TILEDATA::ReadTileMapData(_LoadFile, Buf, ReadCount);

	size_t Offset;
	for (size_t j = 0; j < m_mapTile.size(); ++j)
	{
		Offset = m_mapTile[j].size();
		for (size_t i = 0; i < Offset; i++)
			m_mapTile[j][i]->SetTileData(*(Buf + j*Offset + i));
	}

	free(Buf);

}
void CEditTileMap::PushMapObject(SPTR<CEditMapObject> _Object)
{
	if (nullptr == _Object)
		return;

	m_listMapObject.push_back(_Object);
}
void CEditTileMap::PopMapObject(const POINT& _LBIndex)
{
	list<SPTR<CEditMapObject>>::iterator Start = m_listMapObject.begin();
	list<SPTR<CEditMapObject>>::iterator End = m_listMapObject.end();

	for (;Start != End; ++Start)
	{
		if (nullptr != *Start && (*Start)->GetLBIndex() == _LBIndex) 
		{
			(*Start)->DeadMapObject();
			Start = m_listMapObject.erase(Start);
			break;
		}
	}

	return;
}
void CEditTileMap::SaveMapObjectData(FILE* _SaveFile)
{
	if (nullptr == _SaveFile)
		return;

	if (true == m_listMapObject.empty())
		return;

	list<SPTR<CEditMapObject>>::iterator Start = m_listMapObject.begin();
	list<SPTR<CEditMapObject>>::iterator End = m_listMapObject.end();

	
	size_t BufSize = sizeof(MAPOBJECTDATA)*m_listMapObject.size();
	
	MAPOBJECTDATA* Buf = (MAPOBJECTDATA*)malloc(BufSize);
	memset(Buf, -1, BufSize);

	size_t Index = 0;

	for (; Start!= End; ++Start,++Index)
	{
		if (nullptr == *Start)
			continue;

		Buf[Index] = (*Start)->GetObjectData();
	}

	MAPOBJECTDATA::WriteData(_SaveFile, Buf, (UINT)m_listMapObject.size());

	free(Buf);
}
SPTR<CEditMapObject> CEditTileMap::FindMapObject(const POINT& _LBIndex)
{
	list<SPTR<CEditMapObject>>::iterator Start = m_listMapObject.begin();
	list<SPTR<CEditMapObject>>::iterator End = m_listMapObject.end();

	for (;Start!= End; ++Start)
	{
		if ((*Start)->GetLBIndex() == _LBIndex)
			return *Start;
	}

	return nullptr;
}