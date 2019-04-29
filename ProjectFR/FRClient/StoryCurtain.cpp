#include "Precom.h"
#include "StoryCurtain.h"


CStoryCurtain::CStoryCurtain()
	: m_UpRen(nullptr), m_DownRen(nullptr)
	, m_fAccTime(0.f)
	, m_UpStartPos(Vec2{0.f,0.f})
	, m_UpEndPos(Vec2{ 0.f,0.f })
	, m_DownStartPos(Vec2{ 0.f,0.f })
	, m_DownEndPos(Vec2{ 0.f,0.f })
	, m_bCurtainUpdate(false)
	, m_fUpdateTime(2.f)
{
}


CStoryCurtain::~CStoryCurtain()
{
}
void CStoryCurtain::Init() 
{
	Vec2 WndSize = Vec2{ (float)CClientGlobal::MainWindow->WndSize().x,(float)CClientGlobal::MainWindow->WndSize().y };

	m_UpStartPos = Vec2{ 0.f,WndSize.y*0.5f + 60.f };
	m_UpEndPos = Vec2{ 0.f,WndSize.y*0.5f  };
	m_DownStartPos = Vec2{ 0.f,WndSize.y*-0.5f - 60.f };
	m_DownEndPos = Vec2{ 0.f,WndSize.y*-0.5f };


	m_UpRen = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_FRONTEFFECT,0.98f,true));
	m_UpRen->SetSprite(_T("ColorBase"));
	m_UpRen->SetMaterial(D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.f));
	m_UpRen->MaterialOn();
	m_UpRen->SetCustomSize(Vec2{ 1500.f,100.f });
	m_UpRen->SetRenPivot(m_UpStartPos);
	
	m_DownRen = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_FRONTEFFECT, 0.98f, true));
	m_DownRen->SetSprite(_T("ColorBase"));
	m_DownRen->SetMaterial(D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.f));
	m_DownRen->MaterialOn();
	m_DownRen->SetCustomSize(Vec2{ 1500.f,100.f });
	m_DownRen->SetRenPivot(m_DownStartPos);
}
void CStoryCurtain::Update() 
{
	if (!m_bCurtainUpdate)
		return;

	switch (m_CurUpdate)
	{
	case CStoryCurtain::STORY_START:
		StortStartUpdate();
		break;
	case CStoryCurtain::STORY_END:
		StortEndUpdate();
		break;
	}

	m_fAccTime += DELTATIME;
}
void CStoryCurtain::StortStartUpdate() 
{
	if (m_fAccTime >= m_fUpdateTime) 
	{
		m_bCurtainUpdate = false;
		return;
	}

	float t = m_fAccTime / 1.f;

	Vec2 UpPivot, DownPivot;

	UpPivot = CSJMath::Vec2Lerp(&m_UpStartPos, &m_UpEndPos, t);
	DownPivot = CSJMath::Vec2Lerp(&m_DownStartPos, &m_DownEndPos, t);

	m_UpRen->SetRenPivot(UpPivot);
	m_DownRen->SetRenPivot(DownPivot);
}
void CStoryCurtain::StortEndUpdate() 
{
	if (m_fAccTime >= 1.1f)
	{
		m_bCurtainUpdate = false;
		return;
	}

	float t = m_fAccTime / 1.f;

	Vec2 UpPivot, DownPivot;

	UpPivot = CSJMath::Vec2Lerp(&m_UpEndPos, &m_UpStartPos, t);
	DownPivot = CSJMath::Vec2Lerp(&m_DownEndPos, &m_DownStartPos, t);

	m_UpRen->SetRenPivot(UpPivot);
	m_DownRen->SetRenPivot(DownPivot);
}
void CStoryCurtain::StoryEvent(const bool& _Start) 
{
	m_bCurtainUpdate = true;
	m_fAccTime = 0.f;

	if (_Start)
		m_CurUpdate = STORY_START;
	else
		m_CurUpdate = STORY_END;
}
void CStoryCurtain::SetCurtain(const bool& _EndPos)
{
	if (_EndPos) 
	{
		m_UpRen->SetRenPivot(m_UpEndPos);
		m_DownRen->SetRenPivot(m_DownEndPos);
	}
	else 
	{
		m_UpRen->SetRenPivot(m_UpStartPos);
		m_DownRen->SetRenPivot(m_DownStartPos);
	}
}
void CStoryCurtain::InvalidateUpdate()
{
	m_bCurtainUpdate = false;
	m_fAccTime = 0.f;
}
const bool CStoryCurtain::IsCurtainUpdate() 
{
	return m_bCurtainUpdate;
}
const CStoryCurtain::STORYEVENT CStoryCurtain::GetCurEvent()
{
	return m_CurUpdate;
}
void CStoryCurtain::RenderEnable(const bool& _Enable)
{
	InvalidateUpdate();
	m_UpRen->Active(_Enable);
	m_DownRen->Active(_Enable);
}