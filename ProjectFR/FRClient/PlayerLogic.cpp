#include "Precom.h"
#include "PlayerLogic.h"
#include"PlayerIdle.h"
#include"PlayerAttack.h"
#include"PlayerReAction.h"
#include"PlayerDeath.h"


CPlayerLogic::CPlayerLogic()
	:m_pAniRen(nullptr), m_bRun(false), m_bChangeAni(false)
	, m_MoveDir(Vec3{0.0f,0.0f,0.0f})
{
}


CPlayerLogic::~CPlayerLogic()
{
}
void CPlayerLogic::Init()
{
	SPTR<CCollision>  Col = ParentActObject()->CreateCom<CCollision>(COLDATA(_T("Player")));
	SPTR<CSoundPlayer>  SD = ParentActObject()->CreateCom<CSoundPlayer>();
	SD->PlaySound(_T("Reminiscence"));
	m_pAniRen = ParentActObject()->CreateCom<CAniRenderer>(RENDATA(LAYER_PLAYER, 0.9f));
	m_pAniRen->CreateAnimation(ANI(Limit(_T("DemonRain"))));
	m_pAniRen->CreateAnimation(ANI(DAtk(_T("DemonRain"))));
	ParentActObject()->SetImageScale(1.3f);

	
	
	for (int i = 0; i < 8; i++)
	{
		TCHAR AniName[256] = _T("");
		_stprintf_s(AniName, _T("MapRain_Idle%d"), i);
		m_pAniRen->CreateAnimation(AniName, _T("MapRain"), ANI_LOOP, { 0 + 7 * i,6 + 7 * i });
		_stprintf_s(AniName, _T("MapRain_Run%d"), i);
		m_pAniRen->CreateAnimation(AniName, _T("MapRain"), ANI_COUNTCYCLE_REPEAT, { 56 + i * 7 ,62 + i * 7 }, 1, { 57 + i * 7, 62 + i * 7 });
		_stprintf_s(AniName, _T("MapRain_Work%d"), i);
		m_pAniRen->CreateAnimation(AniName, _T("MapRain"), ANI_COUNTCYCLE_REPEAT, { 112 + i * 7 ,118 + i * 7 }, 1, { 113 + i * 7, 118 + i * 7 });
	}
	
	//TransForm()->SetSize(Vec3{ 100.0f,100.0f,1.0f });
	//m_pAniRen->ChangeAni(_T("MapRain_Idle0"));
	//TransForm()->SetPos(Vec3{ 0.0f,0.0f,200.0f });

	TRANS->SetSize(Vec3{ 100.0f,100.0f,1.0f });
	m_pAniRen->ChangeAni(_T("MapRain_Idle0"));
	TRANS->SetPos(Vec3{ 0.0f,0.0f,70.0f });

	Col->SetEnter(this, &CPlayerLogic::Enter);
	Col->SetStay(this, &CPlayerLogic::Stay);
	Col->SetExit(this, &CPlayerLogic::Exit);

	m_PairMgr = new CStateMgr();
	m_PairMgr->CreateState<CPlayerIdle>(this);
	m_PairMgr->CreateState<CPlayerAttack>(this);
	m_PairMgr->CreateState<CPlayerReAction>(this);
	m_PairMgr->CreateState<CPlayerDeath>(this);
	

	m_PairMgr->ChangeState<CPlayerIdle>();
}
void CPlayerLogic::PreUpdate()
{
	if (true == KEYDOWN("CurPlayerState")) 
	{
		int index = m_PairMgr->GetCurStateIndex();

		switch (index)
		{
		case 0: printf("Player State : Idle\n"); break;
		case 1: printf("Player State : Attack\n"); break;
		case 2: printf("Player State : ReAction\n"); break;
		case 3: printf("Player State : Death\n"); break;
		default:
			break;
		}
	}
}
void CPlayerLogic::Update()
{
	m_PairMgr->Update();
	
	/*if (true == KEYDOWN("PlayerMove")) 
	{
		Vec3 ChangePos = CClientGlobal::MouseObeject->GetPos();
		TRANS->SetPos({ ChangePos.x,ChangePos.y,TRANS->GetPos().z });
	}*/
}
void CPlayerLogic::PostUpdate()
{

}

void CPlayerLogic::Enter(SPTR<CCollision> _pThis, SPTR<CCollision> _pOtherCol)
{
	int a = 0;
	_pOtherCol->ParentActObject()->Death();
}
void CPlayerLogic::Stay(SPTR<CCollision> _pThis, SPTR<CCollision> _pOtherCol) 
{
	int a = 0;
}
void CPlayerLogic::Exit(SPTR<CCollision> _pThis, SPTR<CCollision> _pOtherCol) 
{
	int a = 0;
}
void CPlayerLogic::ChangeState(int _Index)
{
#ifdef _DEBUG
	bool Change = m_PairMgr->ChangeState(_Index);
#endif
	m_PairMgr->ChangeState(_Index);
}
void CPlayerLogic::ChangeAni(const tstring& _AniKey)
{
	m_pAniRen->ChangeAni(_AniKey);
}

void CPlayerLogic::DebugRender()
{
	//GetComDebugMgr()->RectRender(D3DCOLOR_XRGB(255, 0, 0), TransForm()->GetWorldMat());

}