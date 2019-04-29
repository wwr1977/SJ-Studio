#include "stdafx.h"
#include "MiddlePoint.h"


CMiddlePoint::CMiddlePoint()
{
}


CMiddlePoint::~CMiddlePoint()
{
}
void CMiddlePoint::Init()
{

}
void CMiddlePoint::PreUpdate()
{

}
void CMiddlePoint::Update()
{

}
void CMiddlePoint::PostUpdate()
{
}
void CMiddlePoint::DebugRender()
{
	GetComDebugMgr()->RectRender(D3DCOLOR_ARGB(255,255, 255, 255), { 0.0f,0.0f,0.0f }, {1000.0f,0.0f,1.0f});
	GetComDebugMgr()->RectRender(D3DCOLOR_ARGB(255,255, 255, 255), { 0.0f,0.0f,0.0f }, { 100.0f,100.0f,100.0f });

	
}
