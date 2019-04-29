#include "Precom.h"
#include "MapObject.h"
#include "MainMapMgr.h"
#include "TileMap.h"
#include <GameSprite.h>


CMainMapMgr*			CMapObject::MapMgr = nullptr;
SPTR<CGameSprite>		CMapObject::ObjectDebugSprite = nullptr;

void CMapObject::SetMapMgr(CMainMapMgr* _Mgr)
{
	if (nullptr == _Mgr)
		return;

	MapMgr = _Mgr;

	ObjectDebugSprite = CClientGlobal::MainWindow->GetResourceMgr()->FindGameSprite(_T("ObjectDebug"));
}
CMapObject::CMapObject()
	:m_ObjectNo(-1), m_LBIndex(POINT{ -1,-1 }), m_ObjectRen(nullptr)
	, m_ObjectKName(_T(""))
	, m_DebugRen(nullptr)
{
}


CMapObject::~CMapObject()
{
	m_BelongTileMap = nullptr;
}
void CMapObject::Init()
{

}
void CMapObject::InitData(const UINT& _ObjectNo)
{
	if (nullptr == MapMgr)
		return;

	if (_ObjectNo >= MapMgr->GetObjectCount())
		return;

	m_ObjectNo = _ObjectNo;
	
	switch (_ObjectNo)
	{
	case 12:
	case 15:
	case 16:
	case 17:
		m_ObjectRen = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_BACKGROUND, 1.f));
		break;
	default:
		m_ObjectRen = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_MAPOBJECT, 1.f));
		break;
	}


	m_ObjectRen->SetSprite(MapMgr->GetObjectName(_ObjectNo));
	Vec2 ObjectSize = m_ObjectRen->GetCurSpriteSize();

	m_ObjectRen->SetRenPivot(Vec2{ (ObjectSize.x - CTileMap::TileSize.x)*0.5f, (ObjectSize.y - CTileMap::TileSize.y)*0.5f });
	ACTOR->Off();
}

void CMapObject::DebugRender()
{
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
void CMapObject::SetTileLBIndex(const POINT& _Index)
{
	m_LBIndex = _Index;

	Vec2 WorldPos = m_BelongTileMap->TileIndexToWolrdPos2(_Index);

	TRANS->SetPos(Vec3{ WorldPos.x,WorldPos.y, TRANS->GetPos().z });

	return;
}
void CMapObject::SetTileMap(CTileMap* _TileMap)
{
	if (nullptr == _TileMap)
		return;

	m_BelongTileMap = _TileMap;
}

const POINT CMapObject::GetLBIndex()
{
	return m_LBIndex;
}
UINT CMapObject::GetObjectNo()
{
	return m_ObjectNo;
}
void CMapObject::MapObjectOn()
{
	ACTOR->On();
}
void CMapObject::MapObjectOff()
{
	ACTOR->Off();
}
void CMapObject::EnableMapObject(const BOOL& _Enable)
{
	ACTOR->Active((bool)_Enable);
}