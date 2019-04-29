#include "stdafx.h"
#include "TransForm.h"
#include "ActObject.h"



CTransForm::CTransForm()
	:m_bScale(false)
	, m_bTrans(false)
	, m_bRot(false)
	, m_bWorld(false)
	, m_Pos(Vec3{0.0f,0.0f,10.0f})
	, m_Scale(Vec3{1.0f,1.0f,1.0f})
	, m_Rot(Vec3{0.0f,0.0f,0.0f})
{
	m_bMulti = false;

	D3DXMatrixIdentity(&matScale);
	D3DXMatrixIdentity(&matRotationU);
	D3DXMatrixIdentity(&matRotationV);
	D3DXMatrixIdentity(&matRotationN);
	D3DXMatrixIdentity(&matRotation);
	D3DXMatrixIdentity(&matTrans);
	D3DXMatrixIdentity(&matRevole);
	D3DXMatrixIdentity(&matWorld);

	D3DXMatrixIdentity(&m_AxisMat);
	m_AxisMat._33 = -1.0f;


	SetParent(nullptr);
}

CTransForm::CTransForm(CActObject* _ActObject)
	:CGameCom(_ActObject)
	, m_bScale(false)
	, m_bTrans(false)
	, m_bRot(false)
	, m_bWorld(false)
	, m_Pos(Vec3{ 0.0f,0.0f,10.0f })
	, m_Scale(Vec3{ 1.0f,1.0f,1.0f })
	, m_Rot(Vec3{ 0.0f,0.0f,0.0f })
{
	m_bMulti = false;

	D3DXMatrixIdentity(&matScale);
	D3DXMatrixIdentity(&matRotationU);
	D3DXMatrixIdentity(&matRotationV);
	D3DXMatrixIdentity(&matRotationN);
	D3DXMatrixIdentity(&matRotation);
	D3DXMatrixIdentity(&matTrans);
	D3DXMatrixIdentity(&matRevole);
	D3DXMatrixIdentity(&matWorld);
	
	
	D3DXMatrixIdentity(&m_AxisMat);
	m_AxisMat._33 = -1.0f;

	//본 에니메이션에서의 자신의 부모 본(ActObject 아님!)
	SetParent(nullptr);

}
CTransForm::~CTransForm()
{
}
bool CTransForm::IsActorActive()
{
	return ACTOR->IsActive();
}
bool CTransForm::ChangeTranspose()
{
	return m_bTrans;
}
bool CTransForm::ChangeWorld()
{
	return m_bWorld;
}
void CTransForm::PreUpdate()
{

}
void CTransForm::Update()
{

	if (true == m_bScale)
	{
		D3DXMatrixScaling(&matScale, m_Scale.x, m_Scale.y, m_Scale.z);
		//m_bScale = false;
		m_bWorld = true;
	}

	if (true == m_bRot)
	{
		Vec3 U = AxisU();
		Vec3 V = AxisV();
		Vec3 N = AxisN();

		D3DXMatrixRotationAxis(&matRotationU, &U, m_Rot.x);
		D3DXMatrixRotationAxis(&matRotationV, &V, m_Rot.y);
		D3DXMatrixRotationAxis(&matRotationN, &N, m_Rot.z);
		matRotation = matRotationU*matRotationV*matRotationN;
		
		//m_bRot = false;
		m_bWorld = true;
	}

	if (true == m_bTrans)
	{
		D3DXMatrixTranslation(&matTrans, m_Pos.x, m_Pos.y, m_Pos.z);
		//m_bTrans = false;
		m_bWorld = true;
	}


	if (true == m_bWorld)
	{
		if (nullptr != CalWorldFunc)
		{
			(this->*CalWorldFunc)();
		}
		//m_bWorld = false;
	}
}
void CTransForm::PostUpdate()
{

}
void CTransForm::LastUpdate() 
{
	D3DXMatrixIdentity(&m_AxisMat);
	m_AxisMat._33 = -1.0f;

	m_bScale = false;
	m_bRot = false;
	m_bTrans = false;
	m_bWorld = false;
}
void CTransForm::CalWorld()
{
	matWorld = matScale * m_AxisMat * matRotation * matTrans * matRevole;
}
void CTransForm::CalWorldParent()
{
	if (nullptr == m_pParent)
	{
		matWorld = matScale * m_AxisMat * matRotation * matTrans * matRevole;
		return;
	}
                               
	matWorld = matScale * m_AxisMat * matRotation * matTrans * matRevole * m_pParent->GetWorldMat();
}

void CTransForm::GetRenWorld(Mat* _OutMat,const Vec2& _SpritePivot,const Vec2& _SpriteSize)
{
	assert(_OutMat);
	if (nullptr == _OutMat) 
	{
		return;
	}
	// 피벗은 조각난 스프라이트의 중심에서의 상대적 거리이므로 
	// 그릴때 오브젝트의 중심은 그 상대 거리의 역방향으로 이동해야한다.
	Vec3 RenPos = { m_Pos.x - _SpritePivot.x,m_Pos.y - _SpritePivot.y ,m_Pos.z };
	Vec2 RenSize =  _SpriteSize * ParentActObject()->GetMagnification();
	
	if (RenSize.y <= 0)
		RenSize.y *= -1.f;

	Mat MTrans, MSize, OutMat;
	
	D3DXMatrixTranslation(&MTrans, RenPos.x, RenPos.y, RenPos.z);
	D3DXMatrixScaling(&MSize, RenSize.x, RenSize.y, 1.0f);

	if (nullptr == m_pParent) 
	{
		OutMat = MSize *  m_AxisMat * matRotation * MTrans * matRevole;
	}
	else 
	{
		OutMat = MSize * m_AxisMat * matRotation * MTrans * matRevole * m_pParent->GetWorldMat();
	}
	memcpy(_OutMat, &OutMat, sizeof(Mat));
	return;
}
void CTransForm::GetRenWorld(Mat* _OutMat,const Vec2& _SpritePivot,const Vec2& RenPivot,const Vec2& _SpriteSize)
{
	return GetRenWorld(_OutMat, _SpritePivot - RenPivot, _SpriteSize);
}
void CTransForm::GetObjWorld(Mat* _OutMat)
{
	assert(_OutMat);

	if (nullptr == _OutMat)
	{
		return;
	}
	memcpy(_OutMat, matWorld, sizeof(Mat));
	return;
}
void CTransForm::GetObjWorld(Mat* _OutMat,const Vec2& _RenPivot)
{
	assert(_OutMat);
	
	if (nullptr == _OutMat)
	{
		return;
	}

	Vec3 RenPos = { m_Pos.x + _RenPivot.x,m_Pos.y + _RenPivot.y ,m_Pos.z };
	Mat MTrans,OutMat;

	D3DXMatrixTranslation(&MTrans, RenPos.x, RenPos.y, RenPos.z);

	if (nullptr == m_pParent)
	{
		OutMat = matScale * m_AxisMat * matRotation * MTrans * matRevole;
	}
	else
	{
		OutMat = matScale * m_AxisMat * matRotation * MTrans * matRevole * m_pParent->GetWorldMat();
	}

	memcpy(_OutMat, &OutMat, sizeof(Mat));

	return;
}
const Vec3	CTransForm::GetPos()
{
	if (nullptr == m_pParent)
		return m_Pos;

	Mat ParentMat = m_pParent->GetWorldMat();
	Vec4 Pos;
	
	D3DXVec3Transform(&Pos, &m_Pos, &ParentMat);
	return Vec3(Pos.x, Pos.y, Pos.z);
}