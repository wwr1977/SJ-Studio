#include "stdafx.h"
#include "PersCamera.h"


CPersCamera::CPersCamera()
	:m_Pivot({0.0f,0.0f,-3000.0f})
{
	m_fNear = 0.3f;
	m_fFar = 5000.0f;
}


CPersCamera::~CPersCamera()
{
}

void CPersCamera::Init()
{
	SetTypeInfo(this);

	m_pGameWnd = ParentActObject()->GetScene()->GetWndPointer();
	m_pDevice = ParentActObject()->GetScene()->GetWindow()->GetLPDevice();

	if (nullptr == m_pDevice)
	{
		assert(nullptr);
		return;
	}

	D3DXMatrixIdentity(&m_matProj);
	D3DXMatrixIdentity(&m_matVeiw);

}
void CPersCamera::PostUpdate()
{
	D3DXMatrixPerspectiveFovLH(&m_matProj
		, D3DX_PI / 3.0f
		, m_pGameWnd->WndAspect()
		, m_fNear
		, m_fFar);

	m_Eye = TransForm()->GetPos() + m_Pivot;
	m_At = TransForm()->GetPos();
	m_Up = Vec3{ 0.0f,1.0f,0.0f };

	D3DXMatrixLookAtLH(&m_matVeiw, &m_Eye, &m_At, &m_Up);


	//m_pDevice->SetTransform(D3DTS_PROJECTION, &m_matProj);
	//m_pDevice->SetTransform(D3DTS_VIEW, &m_matVeiw);
}

void CPersCamera::SetPivot(const Vec3& _Pivot)
{
	m_Pivot = _Pivot;
}
Mat CPersCamera::GetSceneProj() 
{
	return m_matProj;
}
Mat CPersCamera::GetSceneView()
{
	return m_matVeiw;
}
Vec3 CPersCamera::GetEye()
{
	return ACTOR->GetPos() + m_Pivot;
}