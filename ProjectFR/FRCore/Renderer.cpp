#include"stdafx.h"
#include "Renderer.h"


D3DMATERIAL9			CRenderer::BasicMaterial;


CRenderer::CRenderer()
	:m_bAlpha(true), m_TexBackColor({0.0f,0.0f,0.0f})
	,m_bUI(false), m_bUseMaterial(false)
	, m_CustomSize(Vec2(0.0f,0.0f)),m_bCustom(false)
	, m_RenPivot(Vec2{0.0f,0.0f}), m_iOverlapCount(1)
{
	memset(&m_RendererMaterial, 0, sizeof(D3DMATERIAL9));
}

CRenderer::CRenderer(CActObject* _ActObject)
	:CGameCom(_ActObject),m_bAlpha(true), m_TexBackColor({ 0.0f,0.0f,0.0f })
	, m_bUI(false),m_bUseMaterial(false), m_iOverlapCount(1)
{
	memset(&m_RendererMaterial, 0, sizeof(D3DMATERIAL9));
}

CRenderer::~CRenderer()
{
	
}
void CRenderer::Render() 
{
	
}
void CRenderer::RendererInit(LPDIRECT3DDEVICE9 _WndDevice)
{
	BasicMaterial.Power = 3.2f;
	BasicMaterial.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); // 환경광 -> 주변환경.
	BasicMaterial.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); // 난반사 -> 자기 자신의 색깔.
	// 나는 어떤 색을 더 강하게 반사한다
	BasicMaterial.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); // 정반사 -> 빛이 반사될때의 색깔.
	// 나는 어떤 색으로 빛난다.
	BasicMaterial.Emissive = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); // 정반사 -> 빛이 반사될때의 색깔.

	_WndDevice->SetMaterial(&BasicMaterial);
}
void CRenderer::ComRender()
{
	if (true == m_bAlpha) 
	{
		GetComDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
		
		// 배경 색상 알파가 지정되지 않은 경우
		if (Vec3({-1.0f,-1.0f,-1.0f}) ==  m_TexBackColor)
			BaseAlpha();
		else 
			BackGroundAlpha();
	}
	
	// 머테리얼이 셋팅 되어있다면 랜더러에 셋팅된 머테리얼로 Set
	if (true == m_bUseMaterial) 
		COMDEV->SetMaterial(&m_RendererMaterial);
	
	this->Render();
	
	if (true == m_bAlpha) 
		GetComDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	
	// 머테리얼이 셋팅 되어있다면 다시 원래 머테리얼로 Set
	if (true == m_bUseMaterial)
		COMDEV->SetMaterial(&BasicMaterial);
}
void CRenderer::Init()
{
	SetTypeInfo(this);
	m_RendererMaterial.Power = 3.2f;
	m_RendererMaterial.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_RendererMaterial.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_RendererMaterial.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_RendererMaterial.Emissive = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

}
void CRenderer::InitData(void* p) 
{

}

void CRenderer::InitData(RENDATA _Data)
{
	m_fAlphaFactor = _Data.AlphaFactor;
	m_bAlpha = _Data.bAlpha;
	m_bUI = _Data.bUI;
	m_ProjectionMode = _Data.Projection;
	ParentActObject()->GetScene()->PushRenderer(_Data.Index, this);
	m_TexBackColor = _Data.BackColor;
}
void CRenderer::BaseAlpha()
{
	GetComDevice()->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	GetComDevice()->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	GetComDevice()->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
	GetComDevice()->SetRenderState(D3DRS_TEXTUREFACTOR, D3DXCOLOR(0.0f, 0.0f, 0.0f, m_fAlphaFactor));

	GetComDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	GetComDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);


}
void CRenderer::BackGroundAlpha()
{
	/*GetComDevice()->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	GetComDevice()->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	GetComDevice()->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
	GetComDevice()->SetRenderState(D3DRS_TEXTUREFACTOR, D3DXCOLOR(1.0f, 0.0f, 1.0f, m_fAlphaFactor));

	GetComDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	GetComDevice()->SetRenderState(D3DRS_DESTBLEND, D3DRS_TEXTUREFACTOR);*/


	//GetComDevice()->SetRenderState(D3DRS_BLENDFACTOR, D3DCOLOR_ARGB((int)(255*(1.0f-m_fAlphaFactor)), (int)(255*m_TexBackColor.x) , (int)(255 * m_TexBackColor.y), (int)(255 * m_TexBackColor.z)));
	
	
	GetComDevice()->SetRenderState(D3DRS_BLENDFACTOR, D3DXCOLOR(1.0f,1.0f,1.0f,1.0f));
	GetComDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_BLENDFACTOR);
	GetComDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	GetComDevice()->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	
	
	/*GetComDevice()->SetRenderState(D3DRS_SRCBLENDALPHA, D3DBLEND_SRCALPHA);
	GetComDevice()->SetRenderState(D3DRS_DESTBLENDALPHA, D3DBLEND_INVSRCALPHA);

	GetComDevice()->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);

	GetComDevice()->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	GetComDevice()->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	GetComDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCCOLOR);
	GetComDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);
	*/
	
}
void CRenderer::FilterAlpha() 
{
	
}

bool CRenderer::isUI()
{
	return m_bUI;
}
PROJECTIONMODE CRenderer::GetProjMode()
{
	return m_ProjectionMode;
}
void CRenderer::SetAlpha(const float& _Alpha)
{
	m_fAlphaFactor = _Alpha;
}
void CRenderer::SetMaterial(D3DXCOLOR _MatColor)
{
	//일단은 자체발광만 사용
	m_RendererMaterial.Emissive = _MatColor;
}
void CRenderer::MaterialOff() 
{
	m_bUseMaterial = false;
}
void CRenderer::MaterialOn() 
{
	m_bUseMaterial = true;
}
void CRenderer::InvalidCustomSize()
{
	m_CustomSize = Vec2{ 0.0f,0.0f };
	m_bCustom = false;
}
void CRenderer::SetCustomSize(const Vec2& _Size)
{
	if (Vec2({ 0.0f,0.0f }) == _Size)
	{
		return;
	}
	m_CustomSize = _Size;
	m_bCustom = true;
}
void CRenderer::SetCustomSize(const float& _X, const float& _Y)
{
	if (_X == 0 && _Y == 0)
	{
		return;
	}

	m_CustomSize = { _X,_Y };
	m_bCustom = true;
}
const Vec2 CRenderer::GetCustomSize()
{
	return m_CustomSize;
}
void CRenderer::SetRenPivot(const Vec3& _Pivot)
{
	m_RenPivot = { _Pivot.x,_Pivot.y };
}
void CRenderer::SetRenPivot(const Vec2& _Pivot)
{
	m_RenPivot = _Pivot;
}
void CRenderer::SetImageMag(const float& _Mag)
{
	ACTOR->SetImageScale(_Mag);
}
const Vec2 CRenderer::GetRenPivot()
{
	return m_RenPivot;
}
void CRenderer::SetOverlapCount(const int& _Count)
{
	if (1 > _Count) 
	{
		return;
	}

	m_iOverlapCount = _Count;
}
int CRenderer::GetOverlapCount() 
{
	return m_iOverlapCount;
}
const float CRenderer::GetPosY()
{
	return TRANS->GetPos().y;
}