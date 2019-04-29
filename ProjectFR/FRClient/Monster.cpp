#include "Precom.h"
#include "Monster.h"


CMonster::CMonster()
{
}


CMonster::~CMonster()
{
}
void CMonster::Init()
{
	m_Renderer = ParentActObject()->CreateCom<CFixRenderer>(RENDATA(5, 0.8f));
	m_Col = ParentActObject()->CreateCom<CCollision>(COLDATA(_T("Mon")));

	m_Renderer->SetSprite(_T("Circle"));

	ParentActObject()->SetRenStyle(ACTSIZE);

	
	TransForm()->SetScale({ 100.0f,100.0f,1.0f });
	TransForm()->SetRotX(D3DX_PI / 4.0f);
	m_Col->SetEnter(this, &CMonster::Enter);

	
}
void CMonster::PreUpdate()
{

}
void CMonster::Update()
{

}
void CMonster::PostUpdate()
{

}
void CMonster::Enter(SPTR<CCollision> _pThis, SPTR<CCollision> _pOtherCol)
{
	if (false == _pOtherCol->IsDeath()) 
	{
		//_pOtherCol->ParentActObject()->Death();
	}
}