#include "Precom.h"
#include "TestMonster.h"


CTestMonster::CTestMonster()
{
}


CTestMonster::~CTestMonster()
{
}
void CTestMonster::Init()
{
	m_AniRen = ACTOR->CreateCom<CAniRenderer>(RENDATA(LAYER_MONSTER, 1.0f));
	m_AniRen->CreateAnimation(ANI(_T("FinalBoss_idle")));
	m_AniRen->ChangeAni(_T("FinalBoss_idle"));
	TRANS->SetPos({ -300.0f,-100.0f,100.0f });
	ACTOR->SetImageScale(2.5f);


}
void CTestMonster::Update()
{
	if (true == KEYDOWN("TestMonsterMove")) 
	{
		Vec3 ChangePos = CClientGlobal::MouseObeject->GetPos();
		TRANS->SetPos({ ChangePos.x,ChangePos.y,TRANS->GetPos().z });
	}
}