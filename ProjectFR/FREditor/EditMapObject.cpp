#include "stdafx.h"
#include "EditMapObject.h"
#include "EditMapMgr.h"
#include "EditTileMap.h"

CEditMapMgr*			CEditMapObject::MapMgr = nullptr;
SPTR<CGameSprite>		CEditMapObject::ObjectDebugSprite = nullptr;

void CEditMapObject::SetMapMgr(CEditMapMgr* _Mgr)
{
	if (nullptr == _Mgr)
		return;

	MapMgr = _Mgr;

	ObjectDebugSprite = CEditGlobal::MainWindow->GetResourceMgr()->FindGameSprite(_T("ObjectDebug"));
}
CEditMapObject::CEditMapObject()
	:m_ObjectNo(-1),m_LBIndex(POINT{-1,-1}), m_ObjectRen(nullptr)
	, m_ObjectKName(_T("")),m_bTest(false)
	, m_DebugRen(nullptr)
{
}


CEditMapObject::~CEditMapObject()
{
	m_BelongTileMap = nullptr;
}
void CEditMapObject::Init()
{
	m_ObjectRen = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_MAPOBJECT, 1.0f));
	
}
void CEditMapObject::InitData(const UINT& _ObjectNo) 
{
	if (nullptr == MapMgr)
		return;

	if (_ObjectNo >= MapMgr->GetObjectCount())
		return;

	if (nullptr == m_ObjectRen)
		return;

	m_ObjectNo = _ObjectNo;
	m_ObjectRen->SetSprite(MapMgr->GetObjectName(_ObjectNo));
	Vec2 ObjectSize = m_ObjectRen->GetCurSpriteSize();

	m_ObjectRen->SetRenPivot(Vec2{ (ObjectSize.x - CEditTileMap::TileSize.x)*0.5f, (ObjectSize.y - CEditTileMap::TileSize.y)*0.5f });

}

void CEditMapObject::DebugRender()
{
	if (true == m_bTest)
		return;

	if (nullptr == ObjectDebugSprite)
		return;

	GetComDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	GetComDevice()->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	GetComDevice()->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	GetComDevice()->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
	GetComDevice()->SetRenderState(D3DRS_TEXTUREFACTOR, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));

	GetComDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	GetComDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	Mat World;
	Vec2 Pivot = m_ObjectRen->GetRenPivot();
	Vec2 Size = m_ObjectRen->GetCurSpriteSize();

	TRANS->GetRenWorld(&World, -Pivot, Size);

	GetComDevice()->SetTransform(D3DTS_WORLD, &World);
	ObjectDebugSprite->Render();

}
void CEditMapObject::SetTileLBIndex(const POINT& _Index) 
{
	m_LBIndex = _Index;

	Vec2 WorldPos = m_BelongTileMap->TileIndexToWolrdPos2(_Index);

	TRANS->SetPos(Vec3{ WorldPos.x,WorldPos.y, TRANS->GetPos().z });

	return;
}
void CEditMapObject::SetTileMap(CEditTileMap* _TileMap)
{
	if (nullptr == _TileMap)
		return;

	m_BelongTileMap = _TileMap;
}

const POINT CEditMapObject::GetLBIndex()
{
	return m_LBIndex;
}
void CEditMapObject::MapObjectOn()
{
	ACTOR->On();
}
void CEditMapObject::MapObjectOff() 
{
	ACTOR->Off();
}
void CEditMapObject::EnableMapObject(const BOOL& _Enable)
{
	ACTOR->Active((bool)_Enable);
}
void CEditMapObject::SetTestMapObject()
{
	m_bTest = true;

	m_DebugRen = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_MONBACK, 1.0f));
	m_DebugRen->SetSprite(_T("ObjectDebug"));
	m_DebugRen->SetMaterial(D3DXCOLOR(0.8f, 0.1f, 0.1f, 1.0f));
	m_DebugRen->MaterialOn();
}
void CEditMapObject::ChangeMapObject(const UINT& _ObjectNo)
{
	if (nullptr == MapMgr)
		return;

	if (_ObjectNo >= MapMgr->GetObjectCount())
		return;

	m_ObjectNo = _ObjectNo;
	m_ObjectKName = MapMgr->GetObjectKName(_ObjectNo);

	m_ObjectRen->SetSprite(MapMgr->GetObjectName(_ObjectNo));
	Vec2 ObjectSize = m_ObjectRen->GetCurSpriteSize();

	m_ObjectRen->SetRenPivot(Vec2{ (ObjectSize.x - CEditTileMap::TileSize.x)*0.5f, (ObjectSize.y - CEditTileMap::TileSize.y)*0.5f });

}
void CEditMapObject::DeadMapObject()
{
	ACTOR->Death();
}
MAPOBJECTDATA CEditMapObject::GetObjectData()
{
	MAPOBJECTDATA Temp;

	Temp.TileMapNo = m_BelongTileMap->GetTileMapNo();
	Temp.MapObjectNo = m_ObjectNo;
	Temp.LeftIndex = m_LBIndex.x;
	Temp.BottomIndex = m_LBIndex.y;

	return Temp;
}
UINT CEditMapObject::GetObjectNo()
{
	return m_ObjectNo;
}