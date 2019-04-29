#include "stdafx.h"
#include "SJMath.h"

const float CSJMath::FPI = 3.14159265358979323846f;
const float CSJMath::DPI = 3.14159265358979323846f * 2.0f;
const float CSJMath::DtR = CSJMath::FPI / 180.0f;
const float CSJMath::RtD = 180.0f / CSJMath::FPI;


const float CSJMath::RadToDeg(const float _Radian)
{
	return _Radian*RtD;
}
const float CSJMath::DegToRad(const float _Degree)
{
	return _Degree*DtR;
}
const float CSJMath::ABS(const float _Value)
{
	if (_Value <= 0)
	{
		return _Value * -1;
	}
	return _Value;
}
const int CSJMath::ABS(const int _Value)
{
	if (_Value <= 0)
	{
		return _Value * -1;
	}
	return _Value;
}
const float CSJMath::Base(const Vec3& Start, const Vec3& End)
{
	return End.x - Start.x;
}
const float CSJMath::Base(const Vec2& Start, const Vec2& End)
{
	return End.x - Start.x;
}
const float CSJMath::Hight(const Vec3& Start, const Vec3& End)
{
	return End.y - Start.y;
}
const float CSJMath::Hight(const Vec2& Start, const Vec2& End)
{
	return End.y - Start.y;
}

const float CSJMath::Square(float _Value)
{
	return (_Value*_Value);
}
const float CSJMath::Length(const Vec2& _Start, const Vec2& _End)
{
	float x = _End.x - _Start.x;
	float y = _End.y - _Start.y;

	return sqrt((x*x) + (y*y));
}
const float CSJMath::Length(const Vec3& _Start, const Vec3& _End)
{
	float x = _End.x - _Start.x;
	float y = _End.y - _Start.y;
	float z = _End.z - _Start.z;

	return sqrt((x*x) + (y*y) + (z*z));
}

/// º¤ÅÍÀÇ ±æÀÌ
const float CSJMath::Length(const Vec2& _Point)
{
	return sqrt((_Point.x*_Point.x) + (_Point.y*_Point.y));
}
const float CSJMath::Length(const Vec3& _Point)
{
	return sqrt((_Point.x*_Point.x) + (_Point.y*_Point.y) + (_Point.z*_Point.z));
}

const float CSJMath::DirToRadian(const Vec2& _Start, const Vec2& _End)
{
	float tempBase = _End.x - _Start.x;
	float tempGradian = Length(_End, _Start);

	float tempRad = acosf(tempBase / tempGradian);

	if (0 > (_End.y - _Start.y))
	{
		tempRad = FPI + tempRad;
	}

	return tempRad;
}
const float CSJMath::DirToRadian(const Vec2& _Point)
{
	float tempGradian = Length(_Point);
	float tempRad = acosf(_Point.x / tempGradian);

	if (_Point.y < 0)
	{
		tempRad = FPI + tempRad;
	}

	return tempRad;
}
const float CSJMath::DirToDegree(const Vec2& _Start, const Vec2 _End)
{
	return RtD * DirToRadian(_Start, _End);
}
const float CSJMath::DirToDegree(const Vec2& _Point)
{
	return RtD * DirToRadian(_Point);
}
const Vec2 CSJMath::AbsPoint(const Vec2& _Point)
{
	float X = _Point.x;
	float Y = _Point.y;

	if (X <= 0)
	{
		X *= -1.0f;
	}
	if (Y <= 0)
	{
		Y *= -1.0f;
	}

	return Vec2(X, Y);
}
const Vec3 CSJMath::AbsPoint(const Vec3& _Point)
{
	float X = _Point.x;
	float Y = _Point.y;
	float Z = _Point.z;

	if (X <= 0)
	{
		X *= -1.0f;
	}
	if (Y <= 0)
	{
		Y *= -1.0f;
	}
	if (Z <= 0)
	{
		Z *= -1.0f;
	}
	return Vec3(X, Y, Z);
}
void CSJMath::ConversionQuaternion(Quat* _OutQuat, const Vec3& _Axis, const float& _Rad)
{
	float HalfRad = _Rad* 0.5f;
	Vec3 Axis = _Axis;
	Axis *= sinf(HalfRad);

	*_OutQuat = Quat(Axis.x, Axis.y, Axis.z, cosf(HalfRad));

	return;
}

const Vec2 CSJMath::Vec2Lerp(Vec2* _Start,Vec2* _End, const float& _Weight)
{
	if (0.0f >= _Weight) 
	{
		return *_Start;
	}

	if (1.0f <= _Weight) 
	{
		return *_End;
	}

	Vec2 LerpVec;

	D3DXVec2Lerp(&LerpVec, _Start, _End, _Weight);
	return LerpVec;
}
const Vec3 CSJMath::Vec3Lerp(Vec3* _Start,Vec3* _End, const float& _Weight)
{
	if (0.0f >= _Weight)
	{
		return *_Start;
	}

	if (1.0f <= _Weight)
	{
		return *_End;
	}

	Vec3 LerpVec;
	D3DXVec3Lerp(&LerpVec, _Start, _End, _Weight);
	return LerpVec;
}
const Vec4 CSJMath::Vec4Lerp(Vec4* _Start, Vec4* _End, const float& _Weight)
{
	if (0.0f >= _Weight)
	{
		return *_Start;
	}

	if (1.0f <= _Weight)
	{
		return *_End;
	}

	Vec4 LerpVec;
	D3DXVec4Lerp(&LerpVec, _Start, _End, _Weight);

	return LerpVec;
}