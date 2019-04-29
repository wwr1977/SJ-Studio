#include"stdafx.h"
#include "Logic.h"



CLogic::CLogic()
{
}
CLogic::CLogic(CActObject* _ActObject)
	:CGameCom(_ActObject)
{

}

CLogic::~CLogic()
{
}
void CLogic::Init() {}
void CLogic::PreUpdate() {}
void CLogic::Update() {}
void CLogic::PostUpdate() {}

const Vec3 CLogic::GetActorPos()
{
	return TRANS->GetPos();
}
void CLogic::SetActorPos(const Vec3& _Pos)
{
	TRANS->SetPos(_Pos);
}

void CLogic::SetActorImageMag(const float& _Mag)
{
	ACTOR->SetImageScale(_Mag);
}