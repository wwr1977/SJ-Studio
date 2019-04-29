#include "stdafx.h"
#include "BaseAniModel.h"


CBaseAniModel::CBaseAniModel()
	:m_BaseAniRenderer(nullptr)
{
}


CBaseAniModel::~CBaseAniModel()
{
}
void CBaseAniModel::Init()
{
	m_BaseAniRenderer = ParentActObject()->CreateCom<CAniRenderer>(RENDATA(5));
	CEditGlobal::MBaseAniCom = m_BaseAniRenderer;

}
void CBaseAniModel::DebugRender()
{

}