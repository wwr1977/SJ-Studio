#pragma once
#include"DXHeader.h"

enum BASE2DTYPE
{
	BASE2D_POINT,
	BASE2D_RECT,
	BASE2D_CIRCLE,
};

class CBUFFER
{
public:
	typedef struct Sprite2DTransMatBuffer
	{
		Mat	WVP;
		Mat World;

		Sprite2DTransMatBuffer() :WVP(Mat()), World(Mat()) {}
		Sprite2DTransMatBuffer(const Mat& _WVP, const Mat& _World)
			:WVP(_WVP), World(_World)
		{}
		~Sprite2DTransMatBuffer() {}
	}TransMatrix;


	struct TilePixelBuffer 
	{
		Vec4		ColorFactor;
		IVec4		Options;


		TilePixelBuffer() : ColorFactor(Vec4()),Options(IVec4()){}
		~TilePixelBuffer() {}
	};
	struct Sprite2DBuffer
	{
		Vec4	SpriteUV;
		IVec4   ReverseUV;

		Sprite2DBuffer() :SpriteUV(Vec4()), ReverseUV(IVec4()){}
		Sprite2DBuffer(const Vec4& _SpriteUV, const Vec4& _ReverseUV )
			:SpriteUV(_SpriteUV), ReverseUV(_ReverseUV)
		{}
		~Sprite2DBuffer() {}
	};

	struct Sprite2DPixBuffer
	{
		Vec4	PixelFactor;
		Vec4	ClipPosition;
		IVec4   RenOption;
	
		Sprite2DPixBuffer() :PixelFactor(Vec4()), ClipPosition(Vec4()), RenOption(IVec4()){}
		Sprite2DPixBuffer(const Vec4& _PixelFactor, const Vec4& _ClipPosition)
			:PixelFactor(_PixelFactor), ClipPosition(_ClipPosition), RenOption(IVec4())
		{}
		~Sprite2DPixBuffer() {}
	};

	struct Base2DBuffer 
	{
		Vec3	Color;
		int		Type;
		Vec4	BorderData;

		Base2DBuffer() :Color(1.f,1.f,1.f),Type(BASE2D_POINT), BorderData(Vec4()){}
		Base2DBuffer(const Vec3& _Color,const BASE2DTYPE& _Type,const Vec4& _BorderData)
			:Color(_Color), Type(_Type), BorderData(_BorderData)
		{}
		~Base2DBuffer() {}
	};

	struct ShadowTraceBuffer
	{
		Vec4	PlayerUV;
		Vec4	TileUV;
		Vec4	PlayerUVRatio;
		Vec4	RotateAngle;
		IVec4   Options;

		ShadowTraceBuffer() :TileUV(Vec4()), PlayerUVRatio(Vec4()) , PlayerUV(Vec4()), RotateAngle(Vec4()), Options(IVec4()){}
		ShadowTraceBuffer(const Vec4& _PlayerUV, const Vec4& _TileUV, const Vec4& _PlayerUVRatio,const Vec4& _RotAngle)
			: PlayerUV(_PlayerUV),TileUV(_TileUV), PlayerUVRatio(_PlayerUVRatio), RotateAngle(_RotAngle)
			, Options(IVec4())
		{}
		~ShadowTraceBuffer() {}
	};

	struct GlobalInstanceDuffer 
	{
		Mat				VPMat;
		Vec4			TileData;

		GlobalInstanceDuffer() :VPMat(Mat()),TileData(Vec4()){}
		GlobalInstanceDuffer(const Mat& _VP,const Vec4& _Data) :VPMat(_VP), TileData(_Data) {}
		~GlobalInstanceDuffer() {}
	};

	struct CParticleBuffer 
	{
		Mat				VPMat;
		Vec4			ParticleGlobalValue;

		CParticleBuffer() : VPMat(Mat()),ParticleGlobalValue(Vec4()) { }
		CParticleBuffer(const Mat& _VP, const Vec4& _GlobalValue)  : VPMat(_VP),ParticleGlobalValue(_GlobalValue) {}
		~CParticleBuffer() {}
	};

	struct TrailBuffer 
	{
		Mat				VPMat;
		Vec4			SpriteUV;
		Vec4			TrailData;
		Vec4			PerFrameData;

		TrailBuffer() 
			: VPMat(Mat()), SpriteUV(Vec4()), TrailData(Vec4()), PerFrameData(Vec4())
		{
		}
		TrailBuffer(const Mat& _VP, const Vec4& _SpriteUV) 
			: VPMat(_VP), SpriteUV(_SpriteUV), TrailData(Vec4()), PerFrameData(Vec4())
		{}
		~TrailBuffer() {}
	};

	struct AfterImageBuffer 
	{
		Mat				VPMat;
		Vec4			SpriteUV;
		Vec4			AfterImageData;

		AfterImageBuffer()
			: VPMat(Mat()), SpriteUV(Vec4()), AfterImageData(Vec4())
		{
		}
		AfterImageBuffer(const Mat& _VP, const Vec4& _SpriteUV)
			: VPMat(_VP), SpriteUV(_SpriteUV), AfterImageData(Vec4())
		{}
		~AfterImageBuffer() {}
	};


	struct LineBuffer 
	{
		Mat				ViewProj;
		Vec4			SpriteUV;

		LineBuffer() :ViewProj(Mat()), SpriteUV(Vec4()) {}
		~LineBuffer() {}
	};


private:
	CBUFFER() {}
	virtual ~CBUFFER() = 0{}
};

