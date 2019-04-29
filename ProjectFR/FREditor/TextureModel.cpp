#include "stdafx.h"
#include "TextureModel.h"


CTextureModel::CTextureModel()
	:m_TexRenderer(nullptr)
{

}


CTextureModel::~CTextureModel()
{
}
void CTextureModel::Init()
{
	m_TexRenderer = ParentActObject()->CreateCom<CFixRenderer>(RENDATA(5));
	CEditGlobal::MTexCom = m_TexRenderer;
}
void CTextureModel::DebugRender()
{

}