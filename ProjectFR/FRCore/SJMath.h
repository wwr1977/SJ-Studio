#pragma once

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
class CSJMath
{
public:
	static const float FPI;
	static const float DPI;
	static const float RtD;
	static const float DtR;
	static const float Root2;


public:
	static const float RadToDeg(const float _Radian);
	static const float DegToRad(const float _Degree);
	static const float ABS(const float _Value);
	static const int ABS(const int _Value);

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

	static void ConversionQuaternion(Quat* _OutQuat,const Vec3& _Axis, const float& _Rad);

	static const Vec2 Vec2Lerp(Vec2* _Start, Vec2* _End, const float& _Weight);
	static const Vec3 Vec3Lerp(Vec3* _Start, Vec3* _End, const float& _Weight);
	static const Vec4 Vec4Lerp(Vec4* _Start, Vec4* _End, const float& _Weight);

private:
	CSJMath() {}
	~CSJMath() {}
};

