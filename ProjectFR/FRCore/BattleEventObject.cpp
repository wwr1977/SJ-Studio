#include "stdafx.h"
#include "BattleEventObject.h"
#include "FractalTriangle.h"


CBattleEventObject::CBattleEventObject()
{
	m_TriangleObject.clear();
}


CBattleEventObject::~CBattleEventObject()
{
	m_FractalSource.clear();
	m_TriangleObject.clear();
}
void CBattleEventObject::Init() 
{
	Vec2 WndSize = CClientGlobal::Main

	CFractalTriangle::SetTriangleScale();
	CreateFractalSource();
}
void CBattleEventObject::Update() 
{
	list<SPTR<CFractalTriangle>>::iterator Start = m_TriangleObject.begin();
	list<SPTR<CFractalTriangle>>::iterator End = m_TriangleObject.end();

	for (; Start != End; ++Start)
	{
		if (nullptr != (*Start))
			(*Start)->Update();
	}
}
void CBattleEventObject::UIFontRender()
{
	list<SPTR<CFractalTriangle>>::iterator Start = m_TriangleObject.begin();
	list<SPTR<CFractalTriangle>>::iterator End = m_TriangleObject.end();

	for (; Start != End; ++Start) 
	{
		if(nullptr != (*Start))
			(*Start)->UIFontRender();
	}
	

}
void CBattleEventObject::CreateFractalSource()
{
	VERTEX  TempVtx[3];

	VERTEX LeftUp		= VERTEX({ -0.5f, 0.5f, 1.0f }, { 0.0f, 0.0f });
	VERTEX RightUp		= VERTEX({ 0.5f, 0.5f, 1.0f }, { 1.0f,0.0f });
	VERTEX RightDown	= VERTEX({ 0.5f, -0.5f, 1.0f }, { 1.0f,1.0f });
	VERTEX LeftDown		= VERTEX({ -0.5f, -0.5f, 1.0f }, { 0.0f,1.0f });
	
	Vec3 MiddlePos = Vec3{ CSJRandom::RandomFloat(-0.5f,0.5f),CSJRandom::RandomFloat(-0.5f,0.5f),1.f };
	Vec2 MiddleTex = Vec2{ MiddlePos.x + 0.5f, -MiddlePos.y + 0.5f };
	VERTEX Middle = VERTEX(MiddlePos, MiddleTex);

	TempVtx[0] = LeftUp;
	TempVtx[1] = RightUp;
	TempVtx[2] = Middle;
	m_FractalSource.push_back(CActObject::CreateActObject(_T("FractalTriangle"), ACTOR->GetScene())->CreateCom<CFractalTriangle>(TempVtx));

	TempVtx[0] = RightUp;
	TempVtx[1] = RightDown;
	TempVtx[2] = Middle;
	m_FractalSource.push_back(CActObject::CreateActObject(_T("FractalTriangle"), ACTOR->GetScene())->CreateCom<CFractalTriangle>(TempVtx));

	TempVtx[0] = RightDown;
	TempVtx[1] = LeftDown;
	TempVtx[2] = Middle;
	m_FractalSource.push_back(CActObject::CreateActObject(_T("FractalTriangle"), ACTOR->GetScene())->CreateCom<CFractalTriangle>(TempVtx));

	TempVtx[0] = LeftDown;
	TempVtx[1] = LeftUp;
	TempVtx[2] = Middle;
	
	m_FractalSource.push_back(CActObject::CreateActObject(_T("FractalTriangle"), ACTOR->GetScene())->CreateCom<CFractalTriangle>(TempVtx));

}