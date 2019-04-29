#include "Collider.h"
#include"Actor.h"
#include"Scene.h"
#include"DXMath.h"
#include"Base2DCollider.h"
#include"VtxShader.h"
#include"PixShader.h"
#include"Mesh.h"
#include"ResourceMgr.h"

SPTR<CVtxShader>			CCollider::ColliderVtxShader = nullptr ;
SPTR<CPixShader>			CCollider::ColliderPixShader = nullptr;
SPTR<CMesh>					CCollider::ColliderMesh = nullptr;

void CCollider::ColliderInit()
{
	ColliderVtxShader = CResourceMgr<CVtxShader>::Find(L"Base2D");
	ColliderPixShader = CResourceMgr<CPixShader>::Find(L"Base2D");
	ColliderMesh = CResourceMgr<CMesh>::Find(L"2DMesh");
}
CCollider::CCollider()
	:m_CollisionChannel(L""), m_pFi(nullptr)
	, m_EnterCallBack(nullptr), m_StayCallBack(nullptr)
	, m_ExitCallBack(nullptr), m_bCollision(false)
{
	m_setCollider.clear();
	m_setEndIter = m_setCollider.end();
}


CCollider::~CCollider()
{
	SAFE_DELETE(m_pFi);
	m_setCollider.clear();
}

void CCollider::CallEnter(SPTR<CCollider> _This, SPTR<CCollider> _Other) 
{
	if (nullptr == m_EnterCallBack)
		return;

	m_EnterCallBack(_This, _Other);
}
void CCollider::CallStay(SPTR<CCollider> _This, SPTR<CCollider> _Other) 
{
	if (nullptr == m_StayCallBack)
		return;

	m_StayCallBack(_This, _Other);
}
void CCollider::CallExit(SPTR<CCollider> _This, SPTR<CCollider> _Other) 
{
	if (nullptr == m_ExitCallBack)
		return;

	m_ExitCallBack(_This, _Other);
}
const bool CCollider::Init(const COLDATA& _InitData)
{
	m_CollisionChannel = _InitData.CollisionChannel;


	m_pBelongScene->PushCollider(_InitData.eSceneCollision,this);
	return true;
}

void CCollider::ReleaseSet() 
{
	for (SPTR<CCollider> SetCollider : m_setCollider)
	{
		if (true == SetCollider->IsActive())
			SetCollider->CallExit(SetCollider,this);
	}

	m_setCollider.clear();
	m_setEndIter = m_setCollider.end();
}
void CCollider::EraseSet(SPTR<CCollider> _Other) 
{
	if (true == m_setCollider.empty())
		return;

	m_setFindIter = m_setCollider.find(_Other);

	if (m_setFindIter != m_setCollider.end()) 
	{
		m_setCollider.erase(m_setFindIter);
		m_setEndIter = m_setCollider.end();
	}
}
void CCollider::DumpDeathCollider()
{
	std::unordered_set<CCollider*>::iterator Start = m_setCollider.begin();
	std::unordered_set<CCollider*>::iterator End = m_setCollider.end();

	for (; Start != End; )
	{
		if (nullptr == (*Start)->ACTOR )
			Start = m_setCollider.erase(Start);
		else
			++Start;
	}
}
void CCollider::Collision(SPTR<CCollider> _Other)
{
	if (nullptr == _Other)
		return;

	m_setFindIter = m_setCollider.find(_Other);

	//this->CollisionSyncro();
	//_Other->CollisionSyncro();

	if (true == CollisionCheck(_Other)) 
	{
		m_bCollision = true;
		_Other->m_bCollision = true;
		if (m_setFindIter == m_setEndIter) 
		{
			CallEnter(this,_Other);
			_Other->CallEnter(_Other, this);
			m_setCollider.insert(_Other);
			m_setEndIter = m_setCollider.end();
		}
		else 
		{
			CallStay(this, _Other);
			_Other->CallStay(_Other,this);
		}
	}
	else 
	{
		m_bCollision = false;
		_Other->m_bCollision = false;

		if (m_setFindIter != m_setEndIter || true == _Other->IsDeath()) 
		{
			CallExit(this, _Other);
			_Other->CallExit(_Other,this);
			m_setCollider.erase(m_setFindIter);
			m_setEndIter = m_setCollider.end();
		}
	}
}

const bool CCollider::CollisionCheck(SPTR<CCollider> _Other)
{
	this->CollisionSyncro();
	_Other->CollisionSyncro();

	// 자신의 충돌체 타입을 검사
	switch (m_pFi->m_eColType)
	{
	case CT_POINT:
		return PointCollision(_Other->m_pFi);
	case CT_RECT:
		return RectCollision(_Other->m_pFi);
	case CT_CIRCLE:
		return CircleCollision(_Other->m_pFi);
	case CT_2DPOLYGON:
		return false;
	case CT_3DSPHERE:
		return false;
	case CT_3DAABB:
		return false;
	case CT_3DOBB:
		return false;
	case CT_RAY:
		return false;
	case CT_3DMESH:
		return false;
	case MAXCOLTYPE:
		return false;
	}

	return false;
}

const bool CCollider::PointCollision(BASE_FI* _OtherFi)
{
	// 상대의 충돌체 검사 수행
	switch (_OtherFi->m_eColType)
	{
	case CT_POINT:
		return CollisionFunc::PointToPoint(((BASE2D_FI*)m_pFi)->m_Fi, ((BASE2D_FI*)_OtherFi)->m_Fi);
	case CT_RECT:
		return CollisionFunc::PointToRect(((BASE2D_FI*)m_pFi)->m_Fi, ((BASE2D_FI*)_OtherFi)->m_Fi);
	case CT_CIRCLE:
		return CollisionFunc::PointToCircle(((BASE2D_FI*)m_pFi)->m_Fi, ((BASE2D_FI*)_OtherFi)->m_Fi);
	case CT_2DPOLYGON:
		return false;
	case CT_3DSPHERE:
		return false;
	case CT_3DAABB:
		return false;
	case CT_3DOBB:
		return false;
	case CT_RAY:
		return false;
	case CT_3DMESH:
		return false;
	case MAXCOLTYPE:
		return false;
	}

	return false;
}
const bool CCollider::RectCollision(BASE_FI* _OtherFi)
{
	// 상대의 충돌체 검사 수행
	switch (_OtherFi->m_eColType)
	{
	case CT_POINT:
		return CollisionFunc::PointToRect(((BASE2D_FI*)_OtherFi)->m_Fi,((BASE2D_FI*)m_pFi)->m_Fi);
	case CT_RECT:
		return CollisionFunc::RectToRect(((BASE2D_FI*)m_pFi)->m_Fi, ((BASE2D_FI*)_OtherFi)->m_Fi);
	case CT_CIRCLE:
		return CollisionFunc::RectToCircle(((BASE2D_FI*)m_pFi)->m_Fi, ((BASE2D_FI*)_OtherFi)->m_Fi);
	case CT_2DPOLYGON:
		return false;
	case CT_3DSPHERE:
		return false;
	case CT_3DAABB:
		return false;
	case CT_3DOBB:
		return false;
	case CT_RAY:
		return false;
	case CT_3DMESH:
		return false;
	case MAXCOLTYPE:
		return false;
	}

	return false;
}
const bool CCollider::CircleCollision(BASE_FI* _OtherFi)
{
	// 상대의 충돌체 검사 수행
	switch (_OtherFi->m_eColType)
	{
	case CT_POINT:
		return CollisionFunc::PointToCircle(((BASE2D_FI*)_OtherFi)->m_Fi, ((BASE2D_FI*)m_pFi)->m_Fi);
	case CT_RECT:
		return CollisionFunc::RectToCircle(((BASE2D_FI*)_OtherFi)->m_Fi,((BASE2D_FI*)m_pFi)->m_Fi);
	case CT_CIRCLE:
		return CollisionFunc::CircleToCircle(((BASE2D_FI*)m_pFi)->m_Fi, ((BASE2D_FI*)_OtherFi)->m_Fi);
	case CT_2DPOLYGON:
		return false;
	case CT_3DSPHERE:
		return false;
	case CT_3DAABB:
		return false;
	case CT_3DOBB:
		return false;
	case CT_RAY:
		return false;
	case CT_3DMESH:
		return false;
	case MAXCOLTYPE:
		return false;
	}

	return false;
}

const COLTYPE CCollider::GetColType()
{
	if (nullptr == m_pFi)
		return MAXCOLTYPE;

	return m_pFi->m_eColType;
}