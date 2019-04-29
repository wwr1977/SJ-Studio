#pragma once
#include<math.h>
#include<cmath>
#include"DXHeader.h"


class  Sigmoid
{
	float		Max;
	float		Min;
	float		Power;

public:
	const float operator()(const float& _Value)
	{
		float A = expf(-Power*_Value);
		A = A* ((Max - Min) / Min);
		A += 1;
		float Result = Max / A;
		return Result;
	}
	// Sigmide Function =  1/ 1 + exp(-ax) 

	void operator=(const Sigmoid& _Other) 
	{
		Max = _Other.Max;
		Min = _Other.Min;
		Power = _Other.Power;
	}


	Sigmoid()
		: Max(1.0f), Min(0.000001f), Power(1.0f)
	{}
	Sigmoid(const float& _Max, const float& _Min, const float& _Pow)
		: Max(_Max), Min(_Min), Power(_Pow)
	{}
	Sigmoid(const Sigmoid& _Other)
		: Max(_Other.Max), Min(_Other.Min), Power(_Other.Power)
	{}

	~Sigmoid() {}
};
class DXMath
{
public:
	static const float FPI;
	static const float DPI;
	static const float RtD;
	static const float DtR;
	static const float Root2;
	static const float Epsilon;

public:
	static const float ExceptSmallNumber(const float& _Value);

public:
	template<typename T>
	static void Swap(T& _Left, T& _Right) 
	{
		T Temp = _Left;
		_Left = _Right;
		_Right = Temp;
	}

	static const float RadToDeg(const float _Radian);
	static const float DegToRad(const float _Degree);

	static const Vec2 GetDir(const float& _Radian);
	//static const float ABS(const float _Value);
	//static const int ABS(const int _Value);

	///두 점사이의 밑변의 크기와 방향
	static const float Base(const Vec3& Start, const Vec3& End);
	static const float Base(const Vec2& Start, const Vec2& End);
	///두 점사이의 높이의 크기와 방향
	static const float Hight(const Vec3& Start, const Vec3& End);
	static const float Hight(const Vec2& Start, const Vec2& End);
	
	/// 제곱수를 출력
	static const float Square(float _Value);

	///두 점사이의 거리
	static const float Length(const Vec2& _Start, const Vec2& _End);
	static const float Length(const Vec3& _Start, const Vec3& _End);
	/// 벡터의 길이
	static const float Length(const Vec2& _Point);
	static const float Length(const Vec3& _Point);
	
	
	///두 점이 이루는 방향 벡터의 라디안값을 출력
	static const float DirToRadian(const Vec2& _Start, const Vec2& _End);
	static const float DirToRadian(const Vec2& _Point);


	static const float DirToDegree(const Vec2& _Start, const Vec2 _End);
	static const float DirToDegree(const Vec2& _Point);
	
	static const Vec2 AbsPoint(const Vec2& _Point);
	static const Vec3 AbsPoint(const Vec3& _Point);

	static const Vec2 Vec2Lerp(Vec2* _Start, Vec2* _End, const float& _Weight);
	static const Vec3 Vec3Lerp(Vec3* _Start, Vec3* _End, const float& _Weight);
	static const Vec4 Vec4Lerp(Vec4* _Start, Vec4* _End, const float& _Weight);

	static const float FloatLerp(const float& _Start, const float& _End, const float& _Weight);
	static const Vec2 Vec2Lerp(const Vec2& _Start,const Vec2& _End, const float& _Weight);
	static const Vec3 Vec3Lerp(const Vec3& _Start,const Vec3& _End, const float& _Weight);
	static const Vec4 Vec4Lerp(const Vec4& _Start,const Vec4& _End, const float& _Weight);

	static const Vec2 Vec2Slerp(const Vec2& _Start, const Vec2& _End, const float& _Weight);
	
	static const float Vector2Dot(const Vec2& _Left, const Vec2& _Right);
	static const float Vector3Dot(const Vec3& _Left, const Vec3& _Right);
	static const float Vector4Dot(const Vec4& _Left, const Vec4& _Right);

	static const float Vector2Cross(const Vec2& _Left, const Vec2& _Right);
	static const Vec3 Vector3Cross(const Vec3& _Left, const Vec3& _Right);
	static const Vec4 Vector4Cross(const Vec4& _Left, const Vec4& _Right);
		   
	static const float Vector2Angle(const Vec2& _Left, const Vec2& _Right);
	static const float Vector3Angle(const Vec3& _Left, const Vec3& _Right);

	static const float Vector2Angle(const Vec2& _Vec);

	static const Vec2 EpsilonCheck(const Vec2& _Vec2);
	static const Vec3 EpsilonCheck(const Vec3& _Vec3);
	static const Vec4 EpsilonCheck(const Vec4& _Vec4);

	//static const float Clamp(const float& _Input, const float& _Min, const float& _Max);
	
	template<typename T>
	static const T Clamp(const T& _Value, const T&   _Min, const T& _Max)
	{

		T Value = _Value;
		T Min = _Min, Max = _Max;
		
		if (Min > Max)
			Swap(Min, Max);
		
		Value = (Value > Min) ? Value : Min;
		return (Value < Max) ? Value : Max;
	}


	template<typename T>
	static const T LinearBezierCurve(const T& _Start, T& _End, const float& _Weight) 
	{
		if (0.0f >= _Weight)
			return _Start;

		if (1.0f <= _Weight)
			return _End;

		return _Start*(1.f-_Weight) + _End * _Weight;
	}
	
	template<typename T>
	static const T QuadraticBezierCurve(const T& P1, const T& P2, const T& _P3, const float& _Weight) 
	{
		if (0.0f >= _Weight)
			return P1;

		if (1.0f <= _Weight)
			return _P3;

		float t = _Weight;
		float Invt = 1.f - _Weight;

		return (P1 * powf(Invt, 2.f))  + ( P2* 2.f*t*Invt)+ (_P3 * powf(t,2.f));
	}
	
	template<typename T>
	static const Vec3 CubicBezierCurve(const T& P1, const T& P2, const T& _P3, const T& _P4, const float& _Weight) 
	{

		if (0.0f >= _Weight)
			return P1;

		if (1.0f <= _Weight)
			return _P3;

		float t = _Weight;
		float Invt = 1.f - _Weight;

		return (P1 * powf((1.f - t), 3.f)) + (P2* 3.f*t*powf(Invt,2.f)) + (_P3 * 3.f * powf(t, 2.f)* Invt) + (_P4 *powf(t,3.f));
	}

	template<typename T>
	static const T ABS(const T& _Value) 
	{
		if (_Value < (T)0)
			return -_Value;
		
		return _Value;
	}
	//static void ConversionQuaternion(Quat* _OutQuat,const Vec3& _Axis, const float& _Rad);



private:
	DXMath() {}
	virtual ~DXMath() = 0 {}
};


class CollisionFunc 
{
public:
	static const bool PointToPoint(const VEC4& _Left, const VEC4& _Right);
	static const bool PointToRect(const VEC4& _Left, const VEC4& _Right);
	static const bool PointToCircle(const VEC4& _Left, const VEC4& _Right);
	
	static const bool RectToRect(const VEC4& _Left, const VEC4& _Right);
	static const bool RectToCircle(const VEC4& _Left, const VEC4& _Right);
		  
	static const bool CircleToCircle(const VEC4& _Left, const VEC4& _Right);

public:
	static const bool Ray2DToLinesegment2D(const Vec2& _RayPos, const Vec2& _RayDir, const Vec2& _SegStart, const Vec2& _SegEnd);
	static const bool Ray2DToLinesegment2D(const Vec2& _RayPos, const Vec2& _RayDir, const Vec4& _LineSeg);
	static const bool Linesegment2DToLinesegment2D(const Vec2& _LeftStart, const Vec2& _LeftEnd, const Vec2& _RightStart, const Vec2& _RightEnd);
	static const bool Linesegment2DToLinesegment2D(const Vec4& _LeftSeg, const Vec4& _RightSeg);
private:
	CollisionFunc() {}
	virtual ~CollisionFunc() = 0 {}
};