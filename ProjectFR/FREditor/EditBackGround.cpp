#include "stdafx.h"
#include "EditBackGround.h"


CEditBackGround::CEditBackGround()
{
}


CEditBackGround::~CEditBackGround()
{
}

void CEditBackGround::Init() 
{
	m_BackRen = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_BACKGROUND, 0.9f));

	
	m_BackRen->SetSprite(_T("EditBack"));
	//m_BackRen->SetCustomSize(Vec2{ 1450.0f,1100.0f });
}
void CEditBackGround::Update() {}

void CEditBackGround::SetSprite(const tstring& _BackName)
{
	m_BackRen->SetSprite(_BackName);
}


