#pragma once
#include"UserHeader.h"

class CCollisionFunc
{

public:
	static bool(*CCollisionFunc::ColFunc[CT_MAX][CT_MAX])(const COLFIGURE& _Left, const COLFIGURE& _Right);

public:
	static void ColFunctionInit();

	static bool RectToRect(const COLRECT& _Left, const COLRECT& _Right);
	static bool RectToCircle(const COLRECT& _Left, const COLCIRCLE& _Right);
	static bool RectToPoint(const COLRECT& _Left, const COLPOINT& _Right);

	static bool CircleToRect(const COLCIRCLE& _Left, const COLRECT& _Right);
	static bool CircleToCircle(const COLCIRCLE& _Left, const COLCIRCLE& _Right);
	static bool CircleToPoint(const COLCIRCLE& _Left, const COLPOINT& _Right);

	static bool PointToRect(const COLPOINT& _Left, const COLRECT& _Right);
	static bool PointToCircle(const COLPOINT& _Left, const COLCIRCLE& _Right);
	static bool PointToPoint(const COLPOINT& _Left, const COLPOINT& _Right);


	static const Vec3 AbsPoint(Vec3& _Point);
private:
	CCollisionFunc();
	~CCollisionFunc();
};

