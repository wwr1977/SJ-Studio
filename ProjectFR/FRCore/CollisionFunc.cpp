#include "stdafx.h"
#include "CollisionFunc.h"


bool(*CCollisionFunc::ColFunc[CT_MAX][CT_MAX])(const COLFIGURE& _Left, const COLFIGURE& _Right) = { nullptr, };

CCollisionFunc::CCollisionFunc()
{
}


CCollisionFunc::~CCollisionFunc()
{
}

void CCollisionFunc::ColFunctionInit()
{
	ColFunc[CT_RECT][CT_RECT] = &RectToRect;
	ColFunc[CT_RECT][CT_CIRCLE] = &RectToCircle;
	ColFunc[CT_RECT][CT_POINT] = &RectToPoint;

	ColFunc[CT_CIRCLE][CT_RECT] = &CircleToRect;
	ColFunc[CT_CIRCLE][CT_CIRCLE] = &CircleToCircle;
	ColFunc[CT_CIRCLE][CT_POINT] = &CircleToPoint;

	ColFunc[CT_POINT][CT_RECT] = &PointToRect;
	ColFunc[CT_POINT][CT_CIRCLE] = &PointToCircle;
	ColFunc[CT_POINT][CT_POINT] = &PointToPoint;
}

bool CCollisionFunc::RectToRect(const COLRECT& _Left, const COLRECT& _Right)
{
	if (_Left.Left() > _Right.Right()) 
	{
		return FALSE;
	}
	if (_Left.Right() < _Right.Left()) 
	{
		return FALSE;
	}
	if (_Left.Top() < _Right.Bottom()) 
	{
		return FALSE;
	}
	if (_Left.Bottom() > _Right.Top()) 
	{
		return FALSE;
	}

	return TRUE;
}
bool CCollisionFunc::RectToCircle(const COLRECT& _Left, const COLCIRCLE& _Right)
{
	COLRECT HRect = COLRECT(_Left.Pos, { _Left.Size.x,_Left.Size.y + _Right.Diameter() });
	COLRECT WRect = COLRECT(_Left.Pos, { _Left.Size.x + _Right.Diameter(), _Left.Size.y  });

	if (TRUE == RectToPoint(_Right, HRect) || TRUE == RectToPoint(_Right, WRect)) 
	{
		return TRUE;
	}

	COLRECT OrthogonalRect = COLRECT(Vec3{ 0.0f,0.0f,0.0f }, _Left.Size);
	Vec3 NewPos = AbsPoint(_Right.Pos - _Left.Pos);
	COLCIRCLE AbsCircle = COLCIRCLE(NewPos, _Right.Radius());

	if (FALSE == CircleToPoint(AbsCircle, OrthogonalRect.RT())) 
	{
		return FALSE;
	}

	return TRUE;
}
bool CCollisionFunc::RectToPoint(const COLRECT& _Left, const COLPOINT& _Right)
{
	if (_Left.Left() > _Right.Pos.x) 
	{
		return FALSE;
	}
	if (_Left.Right() < _Right.Pos.x)
	{
		return FALSE;
	}
	if (_Left.Top() < _Right.Pos.y) 
	{
		return FALSE;
	}
	if (_Left.Bottom() > _Right.Pos.y)
	{
		return FALSE;
	}

	return TRUE;
}
	
bool CCollisionFunc::CircleToRect(const COLCIRCLE& _Left, const COLRECT& _Right)
{
	return RectToCircle(_Right, _Left);
}
bool CCollisionFunc::CircleToCircle(const COLCIRCLE& _Left, const COLCIRCLE& _Right)
{
	Vec2 DisVec = _Left.Pos - _Right.Pos;
	float Dis = D3DXVec2Length(&DisVec);

	if (Dis > _Left.Radius() + _Right.Radius()) 
	{
		return FALSE;
	}

	return TRUE;
}
bool CCollisionFunc::CircleToPoint(const COLCIRCLE& _Left, const COLPOINT& _Right)
{
	Vec2 DisVec = _Left.Pos - _Right.Pos;
	float Dis = D3DXVec2Length(&DisVec);

	if (Dis > _Left.Radius())
	{
		return FALSE;
	}

	return TRUE;
}
	
bool CCollisionFunc::PointToRect(const COLPOINT& _Left, const COLRECT& _Right)
{
	return RectToPoint(_Right, _Left);
}
bool CCollisionFunc::PointToCircle(const COLPOINT& _Left, const COLCIRCLE& _Right)
{
	return CircleToPoint(_Right, _Left);
}
bool CCollisionFunc::PointToPoint(const COLPOINT& _Left, const COLPOINT& _Right)
{
	if (_Left.Pos == _Right.Pos) 
	{
		return TRUE;
	}
	return FALSE;
}

Vec3 const CCollisionFunc::AbsPoint(Vec3& _Point)
{
	if (_Point.x < 0.0f) 
	{
		_Point.x *= -1.0f;
	}
	if (_Point.y < 0.0f)
	{
		_Point.y *= -1.0f;
	}
	if (_Point.z < 0.0f)
	{
		_Point.z *= -1.0f;
	}

	return _Point;
}