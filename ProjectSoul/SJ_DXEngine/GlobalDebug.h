#pragma once
#include "ReferenceBase.h"
#include "DXHeader.h"

class GlobalDebug
{
private:
	friend class CSJ_DXCore;

public:
	static bool												bDebugMode;
private:
	static SPTR<class CMesh>								DebugMash;
	static SPTR<class CVtxShader>							DebugVtxShader;
	static SPTR<class CPixShader>							DebugPixShader;


private:
	static void InitGlobalDebug();
	static void ReleaseGlobalDebug();

public:
	static void DrowDebugRect(const Mat& _VP,const Vec3& _Pos, const Vec3& _Size,const Vec3& _Color = Vec3::One3, const float& _BorderSize = 2.f);
	static void DrowDebugCircle(const Mat& _VP, const Vec3& _Pos, const Vec3& _Size, const Vec3& _Color = Vec3::One3, const float& _BorderSize = 2.f);

//public:
//	static void DrawString(const wchar_t* _FontKey, const wchar_t* _Text, const float& _FontSize, const Vec2& _Pos, const Vec4& Color, const enum FW1_TEXT_FLAG& _Align = FW1_TEXT_FLAG::FW1_TOP);
//
private:
	GlobalDebug() {}
	virtual ~GlobalDebug() = 0 {}
};

