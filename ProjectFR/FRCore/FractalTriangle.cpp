#include "stdafx.h"
#include "FractalTriangle.h"

Mat CFractalTriangle::TriangleScaleMat;

void CFractalTriangle::SetTriangleScale(Mat& _Mat)
{
	TriangleScaleMat = _Mat;
}
CFractalTriangle::CFractalTriangle()
	:m_TriMesh(nullptr)
{

}


CFractalTriangle::~CFractalTriangle()
{
	m_TriMesh = nullptr;
}
void CFractalTriangle::Init()
{

}
void CFractalTriangle::InitData(void* _VtxBuf)
{
	if (nullptr == _VtxBuf)
		return;

	if (sizeof(VERTEX)*TRICOUNT != sizeof(&_VtxBuf))
		return;

	memcpy_s(_VtxBuf, sizeof(&_VtxBuf), m_TriangleVertax, sizeof(VERTEX)*TRICOUNT);

	m_TriMesh = COMRESMGR->CreateTriMesh((VERTEX*)_VtxBuf);

}
void CFractalTriangle::Update()
{

}
void CFractalTriangle::UIFontRender() 
{

}
void CFractalTriangle::CreateFractalTriangle(list<SPTR<CFractalTriangle>>* _List)
{
	if (nullptr == _List)
		return;


}