#include "Precom.h"
#include "FractalTriangle.h"
#include <GameMesh.h>

Mat CFractalTriangle::TriangleScaleMat;

void CFractalTriangle::SetTriangleScale(Mat& _Mat)
{
	TriangleScaleMat = _Mat;

}
CFractalTriangle::CFractalTriangle()
	:m_TriMesh(nullptr)
	,m_TriCenter(VERTEX())
	, m_InitVec(Vec2{0.f,0.f})
	, m_fAccTime(0.f)
	, m_fInitSpeed(0.f)
	, m_fGravitySpeed(0.f)
	, m_fBrokenTime(0.3f)
	, m_TriRotRad(0.f)
	, m_TriTransVec(Vec2{0.f,0.f})
{
	D3DXMatrixIdentity(&m_TriRot);
	D3DXMatrixIdentity(&m_TriTrans);
}


CFractalTriangle::~CFractalTriangle()
{
	m_TriMesh = nullptr;
}
void CFractalTriangle::Init()
{
	ChangeTriangleMatData();
}
void CFractalTriangle::InitData(TRI_VERTEX& _VtxBuf)
{
	memcpy_s(&m_TriangleVertax, sizeof(m_TriangleVertax),_VtxBuf.TriVertex, sizeof(_VtxBuf.TriVertex));

	m_TriMesh = COMRESMGR->CreateTriMesh(m_TriangleVertax);
	
	for (size_t i = 0; i < TRICOUNT; i++)
		m_TriCenter += ONETHIRD*m_TriangleVertax[i];

}
void CFractalTriangle::Update()
{
	
}
void CFractalTriangle::TriangleRender()
{
	if (nullptr == m_TriMesh)
		return;

	GetComDevice()->SetTransform(D3DTS_WORLD, &m_TriWorld);
	
	if(nullptr != m_TriMesh)
		m_TriMesh->Render(GetComDevice(), D3DPRIMITIVETYPE::D3DPT_TRIANGLELIST);
}
void CFractalTriangle::BrokenUpdate(const float& _Time)
{
	if (_Time <= 0.f)
		return;

	float Weight = powf(_Time / m_fBrokenTime, 0.15f);

	Vec2 Zero = GAMEVEC::ZERO2;
	Vec2 Cur = CSJMath::Vec2Lerp(&Zero, &m_TriTransVec, Weight);
	
	

	D3DXMatrixTranslation(&m_TriTrans, Cur.x, Cur.y, 0.f);
	D3DXMatrixRotationZ(&m_TriRot, m_TriRotRad * Weight);

	m_TriWorld = TriangleScaleMat*m_TriRot*m_TriTrans;
}
void CFractalTriangle::MoveUpdate(const float& _Time)
{
	if (_Time <= 0.f)
		return;

	Vec2 GravityVec = Vec2{ 0.f,-1.f };
	
	Vec2 CurTrans = m_InitVec*m_fInitSpeed*_Time;

	CurTrans += GravityVec*m_fGravitySpeed*_Time*_Time;
	
	Mat MoveTrans;
	D3DXMatrixTranslation(&MoveTrans, CurTrans.x, CurTrans.y, 0.f);

	m_TriWorld = TriangleScaleMat*m_TriRot*m_TriTrans*MoveTrans;
}
void CFractalTriangle::CreateFractalTriangle(list<SPTR<CFractalTriangle>>* _List)
{
	if (nullptr == _List)
		return;

	VERTEX Mid = RandomMiddleVertex(0.1f);
	VERTEX LineMid[TRICOUNT];

	for (size_t i = 0; i < TRICOUNT; i++)
		LineMid[i] = LerpVertax(m_TriangleVertax[i], m_TriangleVertax[(i + 1) % TRICOUNT], CSJRandom::RandomFloat(0.2f, 0.8f));
	
	TRI_VERTEX Triangle;
	Triangle.TriVertex[2] = Mid;
	
	for (size_t i = 0; i < TRICOUNT; i++)
	{
		Triangle.TriVertex[0] = m_TriangleVertax[i % TRICOUNT];
		Triangle.TriVertex[1] = LineMid[ i  % TRICOUNT];

		_List->push_back(CActObject::CreateActObject(_T("FractalTriangle"), ACTOR->GetScene())->CreateCom<CFractalTriangle>(Triangle));

		Triangle.TriVertex[0] = LineMid[i % TRICOUNT];
		Triangle.TriVertex[1] = m_TriangleVertax[ (i + 1) % TRICOUNT];

		_List->push_back(CActObject::CreateActObject(_T("FractalTriangle"), ACTOR->GetScene())->CreateCom<CFractalTriangle>(Triangle));
	}



	/*for (size_t i = 0; i < TRICOUNT; i++)
	{
		Triangle.TriVertex[0] = m_TriangleVertax[i % TRICOUNT];
		Triangle.TriVertex[1] = m_TriangleVertax[(i + 1) % TRICOUNT];
		
		_List->push_back(CActObject::CreateActObject(_T("FractalTriangle"),ACTOR->GetScene())->CreateCom<CFractalTriangle>(Triangle));
	}*/

}
const VERTEX CFractalTriangle::RandomMiddleVertex(const float& _w)
{
	VERTEX MidTri[TRICOUNT];

	for (size_t i = 0; i < TRICOUNT; i++)
		MidTri[i] = _w * m_TriangleVertax[i] + (1.f - _w)*m_TriCenter;

	float A, B, C;

	A = CSJRandom::RandomFloat(0.f, 1.f);
	B = CSJRandom::RandomFloat(0.f, 1.f - A);
	C = 1.f - A - B;

	return MidTri[0] * A + MidTri[1] * B + MidTri[2] * C;
	
}
void CFractalTriangle::ChangeMiddlePoint(const VERTEX& _Mid)
{
	m_TriangleVertax[2] = _Mid;

	if (nullptr == m_TriMesh)
		return;

	m_TriMesh->FILLVTX<VERTEX>(m_TriangleVertax);
	ChangeTriangleMatData();
}
void CFractalTriangle::ChangeTriangleMatData()
{
	m_TriTransVec = Vec2{ CSJRandom::RandomFloat(-50.f,50.f),CSJRandom::RandomFloat(-20.f,20.f) };
	m_TriRotRad = CSJRandom::RandomFloat(-D3DX_PI*0.05f, D3DX_PI*0.05f);

	D3DXMatrixIdentity(&m_TriTrans);
	D3DXMatrixIdentity(&m_TriRot);

	m_TriWorld = TriangleScaleMat*m_TriRot*m_TriTrans;
}
void CFractalTriangle::StandBy()
{
	m_fAccTime = 0.f; 
	m_fGravitySpeed = CSJRandom::RandomFloat(400.f, 600.f);
	m_TriWorld = TriangleScaleMat*m_TriRot*m_TriTrans;
}

////////////////////////////////// TEST		/////////////////////////////
void CFractalTriangle::CreateTestMesh()
{
	float w = CSJRandom::RandomFloat(0.2f,0.8f);

	for (size_t i = 0; i < TRICOUNT; i++)
		m_TestVertax[i] =  w * m_TriangleVertax[i] + (1.f - w)*m_TriCenter;

	//m_TestMesh = COMRESMGR->CreateTriMesh(m_TestVertax);
}
void CFractalTriangle::CheckTriRatio()
{
	Vec3 T1, T2, T3,t1,t2,t3;
	float A, B, C, a, b, c;

	T1 = m_TriangleVertax[0].vPos - m_TriangleVertax[1].vPos;
	T2 = m_TriangleVertax[1].vPos - m_TriangleVertax[2].vPos;
	T3 = m_TriangleVertax[2].vPos - m_TriangleVertax[0].vPos;

	t1 = m_TestVertax[0].vPos - m_TestVertax[1].vPos;
	t2 = m_TestVertax[1].vPos - m_TestVertax[2].vPos;
	t3 = m_TestVertax[2].vPos - m_TestVertax[0].vPos;
	
	A = D3DXVec3Length(&T1);
	B = D3DXVec3Length(&T2);
	C = D3DXVec3Length(&T3);

	a = D3DXVec3Length(&t1);
	b = D3DXVec3Length(&t2);
	c = D3DXVec3Length(&t3);

	float Check1, Check2, Check3;

	Check1 = A / a;
	Check2 = B / b;
	Check3 = C / c;


	int aaa = 0;
}

