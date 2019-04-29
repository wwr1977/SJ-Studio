#include "RectCollider.h"
#include"DXStruct.h"
#include"VtxShader.h"
#include"PixShader.h"
#include"Mesh.h"
#include"Camera.h"
#include"Scene.h"



CRectCollider::CRectCollider()
{
	m_pFi = new BASE2D_FI(CT_RECT);
}


CRectCollider::~CRectCollider()
{

}

void CRectCollider::DebugRender()
{
	SPTR<CCamera> SceneCam = SCENE->GetSceneCamera(MAINCAMERA);

	if (nullptr == SceneCam)
		return;

	CollisionSyncro();
	SubTransUpdate();
	
	CBUFFER::Base2DBuffer BaseBuffer;


	if(m_bCollision)
		BaseBuffer = CBUFFER::Base2DBuffer(Vec4::Red.vec3, BASE2D_RECT,Vec4(1.f/m_ColSize.x, 1.f / m_ColSize.y, m_fBorderSize));
	else 
		BaseBuffer = CBUFFER::Base2DBuffer(m_DebugColor, BASE2D_RECT, Vec4(1.f / m_ColSize.x, 1.f / m_ColSize.y, m_fBorderSize));

	Mat WVP = GetSubTransCRWorld() * SceneCam->GetVPMatrix();

	CCollider::ColliderMesh->Update();
	CCollider::ColliderVtxShader->Update();
	CCollider::ColliderPixShader->Update();
	CCollider::ColliderVtxShader->BufferUpdate<Mat>(0, &WVP.GetTransposeMat());
	CCollider::ColliderPixShader->BufferUpdate<CBUFFER::Base2DBuffer>(0, &BaseBuffer);

	CCollider::ColliderMesh->Render();

}	
