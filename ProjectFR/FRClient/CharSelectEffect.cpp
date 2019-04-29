#include "Precom.h"
#include "CharSelectEffect.h"
#include "BattleUnit.h"

CCharSelectEffect::CCharSelectEffect()
	:m_EffRen(nullptr), m_fEffectScale(2.f)
	, m_fAccTime(0.0f), m_fRotSpeed(D3DX_PI/10.0f)
	, m_pPairUnit(nullptr), m_EffectPivot(Vec3(0.0f,0.0f,0.0f))
	, m_bReduceSize(true), m_fAccSizeTime(0.0f)
	, m_NewEffectRen(nullptr)
{

}


CCharSelectEffect::~CCharSelectEffect()
{
}
void CCharSelectEffect::InitData(CBattleUnit* _PairUnit)
{
	if (nullptr == _PairUnit) 
	{
		assert(nullptr);
		return;
	}

	m_pPairUnit = _PairUnit;
}
void CCharSelectEffect::Init() 
{
	/*m_EffRen = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_BACKEFFECT,0.95f,PERSPECTIVE));
	m_EffRen->SetSprite(_T("SelectCircle"));
	SetImageScale(1.1f);*/

	m_NewEffectRen = ACTOR->CreateCom<CAniRenderer>(RENDATA(LAYER_BACKEFFECT, 0.9f, { 0.0f,0.0f,0.0f }));
	m_NewEffectRen->CreateAnimation(_T("SelectCircle2"), _T("SelectCircle2"), ANI_LOOP);
	m_NewEffectRen->ChangeAni(_T("SelectCircle2"));
	m_NewEffectRen->SetRenPivot(Vec2{ 10.f, 50.f });

}
void CCharSelectEffect::PreUpdate()
{
	//// 1단계 : 자신의 노말을 카메라를 향하게 돌린다
	//TraceCamDir();
	//
	//// 2단계 : 돌아간 Axis의 U(Tangent)를 기준으로 회전
	//TRANS->AxisRot(TANGENT, D3DX_PI / 3.0f);

}
void CCharSelectEffect::Update()
{

	//m_fAccTime += DELTATIME;
	//// 3 단계 : PreUpdate 맞춘 Normal로 회전
	//TRANS->NormalRot(m_fRotSpeed * m_fAccTime );
	//if (m_fAccTime >= 20.0f) 
	//{
	//	m_fAccTime -= 20.0f;
	//}

	//ChangeSize();

}
void CCharSelectEffect::On() 
{
	ACTOR->SetImageScale(m_fEffectScale);
	ACTOR->On();
}
void CCharSelectEffect::Off() 
{
	m_fAccTime = 0.0f;
	m_fAccSizeTime = 0.0f;
	m_bReduceSize = true;
	ACTOR->Off();
}
void CCharSelectEffect::SetPos(const Vec3& _Pos)
{
	TRANS->SetPos(_Pos);
}
void CCharSelectEffect::SetEffectPivot(const Vec3& _Pos)
{
	m_EffectPivot = _Pos;
	m_NewEffectRen->SetRenPivot(Vec2{ m_EffectPivot.x,m_EffectPivot.y });
}
void CCharSelectEffect::SetCustomSize(const Vec3& _CustomSize)
{
	ACTOR->SetSize(_CustomSize);
}
void CCharSelectEffect::SetImageScale(const float& _Scale)
{
	m_fEffectScale = _Scale;
	//ACTOR->SetImageScale(_Scale);

	m_EffRen->SetCustomSize(_Scale * m_EffRen->GetCurSpriteSize());
}
// 카메라의 눈을 바라보도록 Object Space를 돌린다
void CCharSelectEffect::TraceCamDir()
{
	Vec3 CamEye = ACTOR->GetScene()->GetMainPersCamCom()->GetEye();
	Vec3 Nor = TRANS->AxisN();
	Vec3 Pos = ACTOR->GetPos();

	CamEye -= (ACTOR->GetPos() + m_EffectPivot);
	D3DXVec3Normalize(&CamEye, &CamEye);
	
	float _Rad = 0.0f;
	Vec3 Axis;
	
	_Rad = acosf( D3DXVec3Dot(&Nor, &CamEye) );
	D3DXVec3Cross(&Axis, &Nor, &CamEye);
	
	D3DXVec3Normalize(&Axis, &Axis);
	Quat RotQuat;
	CSJMath::ConversionQuaternion(&RotQuat, Axis, _Rad);

	TRANS->AxisRot(RotQuat);
}
void CCharSelectEffect::ChangeSize()
{
	if (true == m_bReduceSize)
	{
		Vec2 StartSize = m_fEffectScale * m_EffRen->GetCurSpriteSize();
		Vec2 EndSize = 0.85f * m_fEffectScale * m_EffRen->GetCurSpriteSize();

		Vec2 CurSize = CSJMath::Vec2Lerp(&StartSize, &EndSize, 0.4f * m_fAccSizeTime);
		
		m_EffRen->SetCustomSize(CurSize);

		if (CurSize == EndSize) 
		{
			m_bReduceSize = false;
			m_fAccSizeTime = 0.0f;
			return;
		}
	}
	else 
	{
		Vec2 StartSize = 0.85f * m_fEffectScale * m_EffRen->GetCurSpriteSize();
		Vec2 EndSize = m_fEffectScale* m_EffRen->GetCurSpriteSize();
		
		Vec2 CurSize = CSJMath::Vec2Lerp(&StartSize,&EndSize, 1.0f * m_fAccSizeTime);

		m_EffRen->SetCustomSize(CurSize);

		if (CurSize == EndSize)
		{
			m_bReduceSize = true;
			m_fAccSizeTime = 0.0f;
			return;
		}
	}

	m_fAccSizeTime += DELTATIME;

}