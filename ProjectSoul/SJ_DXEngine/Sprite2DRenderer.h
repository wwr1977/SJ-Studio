#pragma once
#include "Renderer.h"


typedef struct _tagSPRITE2DDATA 
{
	RENDER_LAYER					RenderLayer;
	SPRITE_RENSIZE_TYPE				SizeType;
	Vec3							SizeFactor;
	//float							SpriteSizeMag;

	_tagSPRITE2DDATA();
	_tagSPRITE2DDATA(const RENDER_LAYER& _Layer,const SPRITE_RENSIZE_TYPE& _SizeType,const Vec3& _CustomSize);
	_tagSPRITE2DDATA(const RENDER_LAYER& _Layer, const SPRITE_RENSIZE_TYPE& _SizeType, const float& _SpriteMag);
	~_tagSPRITE2DDATA();

}RENDATA_SPRITE2D;

typedef class CSprite2DRenderer : public CRenderer
{
private:
	size_t								m_SpriteIndex;
	//float								m_fAccTime;
	SPTR<class CSprite>					m_Sprite;
	SPTR<class CMesh>					m_Mesh;
	SPTR<class CVtxShader>				m_VS;
	SPTR<class CPixShader>				m_PS;
	SPTR<class CDepthStencilState>		m_DepthState;
	SPTR<class CBlendState>				m_BlendState;
	bool								m_bChangeBlend;

private:
	SPRITE_RENSIZE_TYPE					m_eSizeType;
	Vec3								m_SpriteSizeMag;
	Vec4								m_SpriteClipData;


public:
	const bool Init(const RENDATA& _InitData) override;
	const bool Init(const RENDATA_SPRITE2D& _InitData) ;
	void Render(const Mat& _VP) override;

public:
	const bool SetSprite(const wchar_t* _SpriteKey);
	size_t SetSpriteIndex(const size_t& _Index);
	void SetDepthState(const  wchar_t* _StateKey);
	void SetBlendState(const wchar_t* _StateKey);
	const Vec4 GetSpriteUV();
	const size_t GetSpriteIndex();


public:
	void SetSpriteSizeMag(const float& _Mag);
	void SetSpriteSizeMag(const Vec2& _Mag);
	void SetSpriteSizeXMag(const float& _Mag);
	void SetSpriteSizeYMag(const float& _Mag);

public:
	void SetClipPositionX(const float& _XPos, const bool _Less = true);
	void SetClipPositionY(const float& _YPos, const bool _Less = true);
	void ClipPositionOff();


public:
	// 화면의 랜더링시의 월드 포스를 반환
	const Vec3 GetRenWorldPos();
	const Vec3 GetRenSize();
	const Vec3 GetRenRotate();
	const Vec3 GetRenRevolve();
	const Vec3 GetCurSpriteSize();
	SPTR<class CTexture> GetSpriteTexture();



public:
	CSprite2DRenderer();
	~CSprite2DRenderer();
} CFixRenderer, Fix2DRenderer,Sprite2DRen;

