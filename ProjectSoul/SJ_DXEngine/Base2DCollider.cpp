#include "Base2DCollider.h"
#include"Actor.h"



CBase2DCollider::CBase2DCollider()
	:m_DebugColor(Vec3(0.f,1.f,1.f)), m_ColSize(Vec2()), m_ColPivot(Vec3())
	, m_fBorderSize(2.f)
{
}


CBase2DCollider::~CBase2DCollider()
{
	
}


void CBase2DCollider::CollisionSyncro()
{
	Vec3 ActorPos = ACTOR->GetWorldPos();

	((BASE2D_FI*)m_pFi)->m_Fi.Pos = Vec2(ActorPos.x + m_ColPivot.x, ActorPos.y + m_ColPivot.y);
	((BASE2D_FI*)m_pFi)->m_Fi.Size = m_ColSize;
}
const Vec3 CBase2DCollider::GetColliderPos()
{
	return ((BASE2D_FI*)m_pFi)->m_Fi.Pos;

}
const Vec3 CBase2DCollider::GetColliderSize()
{
	return ((BASE2D_FI*)m_pFi)->m_Fi.Size;
}