#include "Precom.h"
#include "BattleEventObject.h"
#include "FractalTriangle.h"
#include <GameTexture.h>


CBattleEventObject::CBattleEventObject()
	:m_ObjectBackRen(nullptr), m_ScreenSize(Vec2{0.f,0.f})
	, m_ScreenTex(nullptr), m_fAccTime(0.f)
	, m_bEvent(false)
	, m_EventSpeaker(nullptr)
{
	m_TriangleObject.clear();
}


CBattleEventObject::~CBattleEventObject()
{
	m_FractalSource.clear();
	m_TriangleObject.clear();
	m_ScreenTex = nullptr;
}
void CBattleEventObject::Init() 
{
	m_EventSpeaker = ACTOR->CreateCom<CSoundPlayer>();

	m_ScreenSize = Vec2{ (float)CClientGlobal::MainWindow->WndSize().x,(float)CClientGlobal::MainWindow->WndSize().y };

	Mat ScaleMat;
	D3DXMatrixScaling(&ScaleMat, m_ScreenSize.x, m_ScreenSize.y, 1.f);
	CFractalTriangle::SetTriangleScale(ScaleMat);
	
	m_ObjectBackRen = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_CAMERASHIELD, 1.0f,true));
	m_ObjectBackRen->SetSprite(_T("ColorBase"));
	m_ObjectBackRen->SetMaterial(D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f));
	m_ObjectBackRen->MaterialOn();
	m_ObjectBackRen->SetCustomSize(m_ScreenSize + Vec2{100.f,100.f});
	m_ObjectBackRen->Off();

	CreateFractalSource_Octa();
}
void CBattleEventObject::Update() 
{
	if (false == m_bEvent)
		return;


	if (m_fAccTime >= 4.5f) 
	{
		m_bEvent = false;
		m_ObjectBackRen->Off();
		return;
	}

	list<SPTR<CFractalTriangle>>::iterator Start = m_TriangleObject.begin();
	list<SPTR<CFractalTriangle>>::iterator End = m_TriangleObject.end();


	if (m_fAccTime <= 0.3f) 
	{
		for (; Start != End; ++Start)
		{
			if (nullptr != (*Start))
				(*Start)->BrokenUpdate(m_fAccTime);
		}
	}
	else if (m_fAccTime >= 0.8f) 
	{
		for (; Start != End; ++Start)
		{
			if (nullptr != (*Start))
				(*Start)->MoveUpdate(m_fAccTime - 0.8f);
		}
	}

		
	float Alpha = 3.3f - m_fAccTime;
	
	if (Alpha >= 0.f)
		m_ObjectBackRen->SetAlpha(Alpha);
	else
		m_ObjectBackRen->SetAlpha(0.f);
	
	m_fAccTime += DELTATIME;
}
void CBattleEventObject::UIFontRender()
{
	if (nullptr == m_ScreenTex || false == m_bEvent)
		return;

	GetComDevice()->SetTexture(0, m_ScreenTex->GetTexture());

	list<SPTR<CFractalTriangle>>::iterator Start = m_TriangleObject.begin();
	list<SPTR<CFractalTriangle>>::iterator End = m_TriangleObject.end();

	for (; Start != End; ++Start) 
	{
		if (nullptr != (*Start)) 
			(*Start)->TriangleRender();
	}
	

}

void CBattleEventObject::LoadScreenImage()
{
	tstring Path = CFilePathMgr::GetPath(_T("Texture")) + _T("TempScreen.bmp");

	m_ScreenTex = COMRESMGR->ReLoadGameTexture(Path);
}
void CBattleEventObject::CreateFractalSource()
{
	m_FractalSource.clear();
	m_TriangleObject.clear();

	TRI_VERTEX  TempTri;
	
	VERTEX LeftUp		= VERTEX({ -0.5f, 0.5f, 1.0f }, { 0.0f, 0.0f });
	VERTEX RightUp		= VERTEX({ 0.5f, 0.5f, 1.0f }, { 1.0f,0.0f });
	VERTEX RightDown	= VERTEX({ 0.5f, -0.5f, 1.0f }, { 1.0f,1.0f });
	VERTEX LeftDown		= VERTEX({ -0.5f, -0.5f, 1.0f }, { 0.0f,1.0f });
	
	Vec3 MiddlePos = Vec3{ CSJRandom::RandomFloat(-0.15f,0.15f),CSJRandom::RandomFloat(-0.15f,0.15f),1.f };
	Vec2 MiddleTex = Vec2{ MiddlePos.x + 0.5f, -MiddlePos.y + 0.5f };
	VERTEX Middle = VERTEX(MiddlePos, MiddleTex);

	TempTri.TriVertex[0] = LeftUp;
	TempTri.TriVertex[1] = RightUp;
	TempTri.TriVertex[2] = Middle;
	m_FractalSource.push_back(CActObject::CreateActObject(_T("FractalTriangle"), ACTOR->GetScene())->CreateCom<CFractalTriangle>(TempTri));

	TempTri.TriVertex[0] = RightUp;
	TempTri.TriVertex[1] = RightDown;
	TempTri.TriVertex[2] = Middle;
	m_FractalSource.push_back(CActObject::CreateActObject(_T("FractalTriangle"), ACTOR->GetScene())->CreateCom<CFractalTriangle>(TempTri));

	TempTri.TriVertex[0] = RightDown;
	TempTri.TriVertex[1] = LeftDown;
	TempTri.TriVertex[2] = Middle;
	m_FractalSource.push_back(CActObject::CreateActObject(_T("FractalTriangle"), ACTOR->GetScene())->CreateCom<CFractalTriangle>(TempTri));

	TempTri.TriVertex[0] = LeftDown;
	TempTri.TriVertex[1] = LeftUp;
	TempTri.TriVertex[2] = Middle;
	
	m_FractalSource.push_back(CActObject::CreateActObject(_T("FractalTriangle"), ACTOR->GetScene())->CreateCom<CFractalTriangle>(TempTri));
	
	m_TriangleObject.assign(m_FractalSource.begin(), m_FractalSource.end());

}
void CBattleEventObject::CreateFractalSource_Octa()
{
	m_FractalSource.clear();
	m_TriangleObject.clear();

	TRI_VERTEX  TempTri;

	VERTEX RectEnd[4];
	VERTEX RectMid[4];

	RectEnd[0] = VERTEX({ -0.5f, 0.5f, 1.0f }, { 0.0f, 0.0f });
	RectEnd[1] = VERTEX({ 0.5f, 0.5f, 1.0f }, { 1.0f,0.0f });
	RectEnd[2] = VERTEX({ 0.5f, -0.5f, 1.0f }, { 1.0f,1.0f });
	RectEnd[3] = VERTEX({ -0.5f, -0.5f, 1.0f }, { 0.0f,1.0f });

	for (size_t i = 0; i < 4; i++) 
	{
		if(0 == i % 2)
			RectMid[i] = LerpVertax(RectEnd[i], RectEnd[(i + 1) % 4], CSJRandom::RandomFloat(0.35f, 0.65f));
		else 
			RectMid[i] = LerpVertax(RectEnd[i], RectEnd[(i + 1) % 4], CSJRandom::RandomFloat(0.45f, 0.55f));
	}


	Vec3 MiddlePos = Vec3{ CSJRandom::RandomFloat(-0.2f,0.2f),CSJRandom::RandomFloat(-0.1f,0.1f),1.f };
	Vec2 MiddleTex = Vec2{ MiddlePos.x + 0.5f, -MiddlePos.y + 0.5f };
	VERTEX Middle = VERTEX(MiddlePos, MiddleTex);

	TempTri.TriVertex[2] = Middle;

	for (size_t i = 0; i < 4; i++)
	{
		TempTri.TriVertex[0] = RectEnd[i];
		TempTri.TriVertex[1] = RectMid[i];
		m_FractalSource.push_back(CActObject::CreateActObject(_T("FractalTriangle"), ACTOR->GetScene())->CreateCom<CFractalTriangle>(TempTri));

		TempTri.TriVertex[0] = RectMid[i];
		TempTri.TriVertex[1] = RectEnd[(i + 1) % 4];
		m_FractalSource.push_back(CActObject::CreateActObject(_T("FractalTriangle"), ACTOR->GetScene())->CreateCom<CFractalTriangle>(TempTri));

	}

	m_TriangleObject.assign(m_FractalSource.begin(), m_FractalSource.end());

}
void CBattleEventObject::ChangeMiddlePoint()
{
	if (true == m_FractalSource.empty())
	{
		CreateFractalSource();
		return;
	}

	Vec3 MiddlePos = Vec3{ CSJRandom::RandomFloat(-0.15f,0.15f),CSJRandom::RandomFloat(-0.15f,0.15f),1.f };
	Vec2 MiddleTex = Vec2{ MiddlePos.x + 0.5f, -MiddlePos.y + 0.5f };
	VERTEX Middle = VERTEX(MiddlePos, MiddleTex);

	
}
void CBattleEventObject::InitFractalTriangle()
{
	m_TriangleObject.clear();

	m_TriangleObject.assign(m_FractalSource.begin(), m_FractalSource.end());
}
void CBattleEventObject::FractalTriangle(const UINT& _FractalCount)
{
	UINT Count = _FractalCount;

	if (Count <= 0)
		return;

	list<SPTR<CFractalTriangle>> TempList;

	while (Count-- > 0) 
	{
		TempList.clear();
		
		list<SPTR<CFractalTriangle>>::iterator Start = m_TriangleObject.begin();
		list<SPTR<CFractalTriangle>>::iterator End = m_TriangleObject.end();

		for (;Start!=End; ++Start)
		{
			if (nullptr != *Start)
				(*Start)->CreateFractalTriangle(&TempList);
			
		}

		swap(TempList, m_TriangleObject);
		TempList.clear();
	}
}
void CBattleEventObject::EventTriger()
{
	m_bEvent = true;
	m_fAccTime = 0.f;
	m_ObjectBackRen->On();
	m_ObjectBackRen->SetAlpha(1.0f);

	LoadScreenImage();
	InitFractalTriangle();
	ChangeMiddlePoint();
	FractalTriangle(1);


	list<SPTR<CFractalTriangle>>::iterator Start = m_TriangleObject.begin();
	list<SPTR<CFractalTriangle>>::iterator End = m_TriangleObject.end();

	for (; Start != End; ++Start)
	{
		if (nullptr != (*Start))
			(*Start)->StandBy();
	}

	if (nullptr != m_EventSpeaker)
		m_EventSpeaker->PlaySound(_T("broken"));
}