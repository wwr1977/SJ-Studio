#include "Precom.h"
#include "UnitPortrait.h"


CUnitPortrait::CUnitPortrait()
{
}


CUnitPortrait::~CUnitPortrait()
{
}

void CUnitPortrait::Init()
{
	m_UnitPortrait = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_UI, 0.9f));
	ACTOR->SetRenStyle(ACTSIZE);
	TRANS->SetSize({ 38.0f, 48.0f });
}
void CUnitPortrait::Update()
{

}
void CUnitPortrait::FontRender()
{

}
void CUnitPortrait::SetUnitPortrait(const tstring& _Portrait)
{
	if (nullptr == m_UnitPortrait) 
	{
		assert(nullptr);
		return;
	}

	m_UnitPortrait->SetSprite(_Portrait);
}