#include"Sprite2DRenderer.h"
#include"Mesh.h"
#include"VtxShader.h"
#include"PixShader.h"
#include"Device.h"
#include"BlendState.h"
#include"ResourceMgr.h"
#include"TransForm.h"
#include"ConstantBuffer.h"
#include"Sprite.h"
#include"TimerMgr.h"
#include"DXStruct.h"
#include"DepthStencilState.h"



_tagSPRITE2DDATA::_tagSPRITE2DDATA() 
	:RenderLayer(MAX_RENDER_LAYER), SizeType(RENSIZE_SPRITESIZE), SizeFactor(Vec3(0.f,0.f,1.f))
{

}
_tagSPRITE2DDATA::_tagSPRITE2DDATA(const RENDER_LAYER& _Layer, const SPRITE_RENSIZE_TYPE& _SizeType, const Vec3& _CustomSize) 
	: RenderLayer(_Layer), SizeType(_SizeType), SizeFactor(_CustomSize)
{

}
_tagSPRITE2DDATA::_tagSPRITE2DDATA(const RENDER_LAYER& _Layer, const SPRITE_RENSIZE_TYPE& _SizeType, const float& _SpriteMag) 
	: RenderLayer(_Layer), SizeType(_SizeType), SizeFactor(Vec3(_SpriteMag, _SpriteMag, 1.f))
{

}
_tagSPRITE2DDATA::~_tagSPRITE2DDATA() 
{

}


CSprite2DRenderer::CSprite2DRenderer()
	:m_SpriteIndex(0), m_SpriteSizeMag(Vec3()), m_SpriteClipData(Vec4())
	, m_DepthState(nullptr),m_BlendState(nullptr), m_bChangeBlend(false)
	, m_Sprite(nullptr), m_Mesh(nullptr), m_VS(nullptr), m_PS(nullptr)
{
	// 클립 옵션을 사용안할경우 z,w값은 0으로 셋팅
	// 옵션값은 인트형으로 전송
	m_SpriteClipData.iz = 0;
	m_SpriteClipData.iw = 0;
}


CSprite2DRenderer::~CSprite2DRenderer()
{
	m_DepthState = nullptr;
	m_BlendState = nullptr;
}

const bool CSprite2DRenderer::Init(const RENDATA& _InitData)
{
	m_Mesh = CResourceMgr<CMesh>::Find(L"2DMesh");
	m_VS = CResourceMgr<CVtxShader>::Find(L"Basic");
	m_PS = CResourceMgr<CPixShader>::Find(L"Basic");
	m_BlendState = CResourceMgr<CBlendState>::Find(L"BasicBlend");

	return CRenderer::Init(_InitData);
}
const bool CSprite2DRenderer::Init(const RENDATA_SPRITE2D& _InitData)
{
	m_Mesh = CResourceMgr<CMesh>::Find(L"2DMesh");
	m_VS = CResourceMgr<CVtxShader>::Find(L"Basic");
	m_PS = CResourceMgr<CPixShader>::Find(L"Basic");
	m_BlendState = CResourceMgr<CBlendState>::Find(L"BasicBlend");

	m_eSizeType = _InitData.SizeType;

	switch (_InitData.SizeType)
	{
	case RENSIZE_SPRITESIZE:
		m_SpriteSizeMag = _InitData.SizeFactor;
		
	break;
	case RENSIZE_CUSTOMSIZE:
		SetRenSize(_InitData.SizeFactor);
	break;
	default:
		break;
	}

	return CRenderer::Init(RENDATA{ _InitData.RenderLayer });
}
void CSprite2DRenderer::Render(const Mat& _VP)
{
	if (true == CGameObject::IsDebug())
		int a = 0;

	CDevice::GetContext()->HSSetShader(nullptr, 0, 0);
	CDevice::GetContext()->DSSetShader(nullptr, 0, 0);
	CDevice::GetContext()->GSSetShader(nullptr, 0, 0);

	if (nullptr == m_Sprite)
		return;


	Vec4 SpriteUV = m_Sprite->GetSpriteUV(m_SpriteIndex);
	SubTransUpdate();

	Vec3 RenSize = GetRenSize();

	Mat World = GetSubTransCRWorld();

	if (RenSize.x < 0.f) 
	{
		SpriteUV.x = SpriteUV.x + SpriteUV.z;
		SpriteUV.z = -SpriteUV.z;
		World.Row[0] = -World.Row[0];
	}

	if (RenSize.y < 0.f) 
	{
		SpriteUV.y = SpriteUV.y + SpriteUV.w;
		SpriteUV.w = -SpriteUV.w;
		World.Row[1] = -World.Row[1];
	}

	if (m_eSortingType == RENDER_YSORTING) 
	{
		float Z = TRANS->GetWorldPos().y;

		if (Z >= 0.f) 
		{
			Z += 1.f;
			Z = std::log2f(Z);
		}
		else 
		{
			Z -= 1.f;
			Z = -std::log2f(-Z);
		}


		World._43 = Z * 0.01f;
	}

	//WVP = WVP * _VP;
	CBUFFER::TransMatrix TransMat;
	TransMat.World = World.GetTransposeMat();
	TransMat.WVP = (World * _VP).GetTransposeMat();
	
	m_Mesh->Update();
	

	m_VS->BufferUpdate<CBUFFER::TransMatrix>(0, &TransMat);
	m_VS->BufferUpdate<Vec4>(1, &SpriteUV);
	//m_VS->BufferUpdate<Vec4>(1, m_Sprite->GetSpriteUV_Pointer(m_SpriteIndex));

	CBUFFER::Sprite2DPixBuffer PixBuffer;
	PixBuffer.PixelFactor = m_RenColorFactor;
	PixBuffer.ClipPosition = m_SpriteClipData;
	PixBuffer.RenOption = IVec4(m_eRenderOption, 0, 0, 0);

	m_PS->BufferUpdate<CBUFFER::Sprite2DPixBuffer>(0, &PixBuffer);

	m_VS->Update();
	m_PS->Update();

	m_Sprite->Update();

	if(nullptr != m_BlendState)
		m_BlendState->Update();

	if (nullptr != m_DepthState)
		m_DepthState->Update(0);

	m_Mesh->Render();

	DXCONTEXT->OMSetDepthStencilState(nullptr, 0);

	if (m_bChangeBlend)
		CResourceMgr<CBlendState>::Find(L"BasicBlend")->Update();
}

const bool CSprite2DRenderer::SetSprite(const wchar_t* _SpriteKey)
{
	SPTR<CSprite> ChangeSprite = CResourceMgr<CSprite>::Find(_SpriteKey);

	if (nullptr == ChangeSprite)
		return false;

	m_Sprite = ChangeSprite;
	m_SpriteIndex = 0;

	if (RENSIZE_SPRITESIZE == m_eSizeType) 
		SetRenSize(m_Sprite->GetSpriteSize() * m_SpriteSizeMag);
	
	return true;
}
size_t CSprite2DRenderer::SetSpriteIndex(const size_t& _Index)
{
	if (m_Sprite->GetMaxSpriteIndex() < _Index)
		return m_SpriteIndex;

	return m_SpriteIndex = _Index;
}
void CSprite2DRenderer::SetDepthState(const  wchar_t* _StateKey)
{
	m_DepthState = CResourceMgr<CDepthStencilState>::Find(_StateKey);
}
void CSprite2DRenderer::SetBlendState(const wchar_t* _StateKey)
{
	if (nullptr == CResourceMgr<CBlendState>::Find(_StateKey))
		return;


	m_BlendState = CResourceMgr<CBlendState>::Find(_StateKey);

	if (m_BlendState == CResourceMgr<CBlendState>::Find(L"BasicBlend"))
		m_bChangeBlend = false;
	else
		m_bChangeBlend = true;
	
}
const Vec4 CSprite2DRenderer::GetSpriteUV()
{
	return m_Sprite->GetSpriteUV(m_SpriteIndex);
}
const size_t CSprite2DRenderer::GetSpriteIndex()
{
	return m_SpriteIndex;
}
void CSprite2DRenderer::SetSpriteSizeMag(const Vec2& _Mag)
{
	if (RENSIZE_SPRITESIZE != m_eSizeType)
		return;

	m_SpriteSizeMag.vec2 = _Mag;
	SetRenSize(m_Sprite->GetSpriteSize() * m_SpriteSizeMag);
}
void CSprite2DRenderer::SetSpriteSizeMag(const float& _Mag)
{
	if (RENSIZE_SPRITESIZE != m_eSizeType)
		return;

	m_SpriteSizeMag.vec2 = Vec2(_Mag,_Mag);
	SetRenSize(m_Sprite->GetSpriteSize() * m_SpriteSizeMag);
}

void CSprite2DRenderer::SetSpriteSizeXMag(const float& _Mag) 
{
	if (RENSIZE_SPRITESIZE != m_eSizeType)
		return;

	m_SpriteSizeMag.x = _Mag;
	SetRenSize(m_Sprite->GetSpriteSize() * m_SpriteSizeMag);
}
void CSprite2DRenderer::SetSpriteSizeYMag(const float& _Mag) 
{
	if (RENSIZE_SPRITESIZE != m_eSizeType)
		return;

	m_SpriteSizeMag.y = _Mag;
	SetRenSize(m_Sprite->GetSpriteSize() * m_SpriteSizeMag);
}
void CSprite2DRenderer::SetClipPositionX(const float& _XPos , const bool _Less /*= true*/)
{
	m_SpriteClipData.x = _XPos;

	if(_Less )
		m_SpriteClipData.iz = -1;
	else 
		m_SpriteClipData.iz = 1;
}
void CSprite2DRenderer::SetClipPositionY(const float& _YPos, const bool _Less /*= true*/)
{
	m_SpriteClipData.y = _YPos;
	
	if (_Less)
		m_SpriteClipData.iw = -1;
	else 
		m_SpriteClipData.iw = 1;
}
void CSprite2DRenderer::ClipPositionOff()
{
	m_SpriteClipData.z = 0;
	m_SpriteClipData.w = 0;
}
const Vec3 CSprite2DRenderer::GetRenWorldPos()
{
	return GetSubTransCRWorld().Row[3].vec3;
}
const Vec3 CSprite2DRenderer::GetRenSize()
{
	if (nullptr != m_pSubData)
		return m_pSubData->SubSize;

	return TRANS->GetScale();
}

const Vec3 CSprite2DRenderer::GetRenRotate()
{
	return TRANS->GetLocalRotate();
}
const Vec3 CSprite2DRenderer::GetRenRevolve()
{
	return TRANS->GetLocalRevolve();
}
const Vec3 CSprite2DRenderer::GetCurSpriteSize()
{
	return m_Sprite->GetSpriteSize();
}
SPTR<class CTexture> CSprite2DRenderer::GetSpriteTexture()
{
	if (nullptr == m_Sprite)
		return nullptr;

	return m_Sprite->GetTexture();
}