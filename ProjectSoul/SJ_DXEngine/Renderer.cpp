#include "Renderer.h"
#include"TransForm.h"
#include"Actor.h"
#include"Scene.h"
#include"DXMath.h"



CRenderer::CRenderer()
	:m_RenColorFactor(Vec4(1.f,1.f,1.f,1.f)), m_eSortingType(RENDER_NONESORTING)
	, m_eRenderOption(RENDER_OPTION_NONE), m_fFactorTime(0.f)
	, m_StartColorFactor(Vec4()), m_EndColorFactor(Vec4())
	, m_bAutoFactor(false), m_fAccFactorTime(0.f)
{
}


CRenderer::~CRenderer()
{
}


const bool CRenderer::Init(const RENDATA& _InitData)
{
	m_RenderLayer = _InitData.RenderLayer;

	m_pBelongScene->GetRenderMgr()->PushRenderer(this);
	return true;
}

void CRenderer::PostUpdate(const float& _DeltaTime)
{
	if (false == m_bAutoFactor)
		return;


	if (m_fFactorTime <= m_fAccFactorTime)
	{
		m_bAutoFactor = false;
		m_RenColorFactor = m_EndColorFactor;
		return;
	}


	m_RenColorFactor = DXMath::Vec4Lerp(m_StartColorFactor, m_EndColorFactor, m_fAccFactorTime / m_fFactorTime);
	m_fAccFactorTime += _DeltaTime;
}
void CRenderer::SetRenPivot(const Vec3& _Pivot)
{
	m_RenPivot = _Pivot;
	SetSubPivot(_Pivot);
}
void CRenderer::SetRenSize(const Vec3& _Size) 
{
	m_RenSize = _Size;
	SetSubSize(_Size);
}
void CRenderer::SetAlpha(const float& _Alpha)
{
	m_RenColorFactor.w = _Alpha;
}
const Vec3 CRenderer::GetRenSize()
{
	if (nullptr == m_pSubData)
		return TRANS->GetScale();

	return m_pSubData->SubSize;
}
const Vec3 CRenderer::GetRenPivot()
{
	if (nullptr == m_pSubData)
		return Vec3::Zero3;

	return m_pSubData->SubPivot;
}