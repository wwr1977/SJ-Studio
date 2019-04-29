#include "stdafx.h"
#include "MultiSpriteModel.h"


CMultiSpriteModel::CMultiSpriteModel()
	:m_MultiSpriteRenderer(nullptr)
{

}


CMultiSpriteModel::~CMultiSpriteModel()
{
}
void CMultiSpriteModel::Init()
{
	m_MultiSpriteRenderer = ParentActObject()->CreateCom<CFixRenderer>(RENDATA(5));
	CEditGlobal::MAniSpriteCom = m_MultiSpriteRenderer;

}
void CMultiSpriteModel::DebugRender()
{
	

}