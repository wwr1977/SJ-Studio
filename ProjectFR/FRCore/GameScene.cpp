#include "stdafx.h"
#include "GameScene.h"
#include"SceneBuilder.h"
#include"SceneRenderMgr.h"
#include"SceneCollisionMgr.h"
#include"Camera.h"
#include"PersCamera.h"



CGameScene::CGameScene()
{

}

CGameScene::CGameScene(tstring _Name, SPTR<CSceneBuilder> _Builder  )
	:CObjBase(_Name),m_Builder(_Builder),m_MainCamera(nullptr),m_PairRenderMgr(nullptr)
	, m_PairCollisionMgr(nullptr),m_ParentDebugMgr(nullptr), m_MainPersCamCom(nullptr)
	, m_MainLight(nullptr)
{
	if (nullptr == _Builder) 
	{
		assert(FALSE);
		return;
	}

	m_PairRenderMgr = new CSceneRenderMgr(this);
	m_PairCollisionMgr = new CSceneCollisionMgr(this);

}
CGameScene::CGameScene(tstring _Name, SPTR<CSceneBuilder> _Builder, CGameWindow* _Window)
	:CObjBase(_Name), m_Builder(_Builder), m_ParentWindow(_Window), m_MainCamera(nullptr)
	, m_PairRenderMgr(nullptr), m_PairCollisionMgr(nullptr), m_ParentDebugMgr(nullptr)
	, m_MainLight(nullptr)
{
	if (nullptr == _Builder || nullptr == _Window)
	{
		assert(FALSE);
		return;
	}

	m_PairRenderMgr = new CSceneRenderMgr(this);
	m_PairCollisionMgr = new CSceneCollisionMgr(this);
	m_ParentDebugMgr = m_ParentWindow->GetDebugMgr();
}
CGameScene::~CGameScene()
{
	//SAFE_DELETE(m_PairRenderMgr.p)
	SAFE_DELETE(m_PairCollisionMgr)
	m_listActObject.clear();
	m_listCameraObject.clear();

	m_MainCamera = nullptr;
	m_MainCamCom = nullptr;
	m_MainPersCamCom = nullptr;
	m_MainLight = nullptr;
	m_ParentDebugMgr = nullptr;
	m_Builder = nullptr;

}
bool CGameScene::Init(SPTR<CSceneBuilder> _Builder,CGameWindow* _Window)
{
	assert(_Builder);
	assert(_Window);


	if (nullptr == _Builder || nullptr == _Window) 
	{
		return FALSE;
	}
	
	m_Builder = _Builder;
	m_ParentWindow = _Window;

	if (nullptr == m_Builder || nullptr == m_ParentWindow)
	{
		return FALSE;
	}
	



	return TRUE;
}
bool CGameScene::Loading()
{
	if (TRUE == m_bLoading) 
	{
		return TRUE;
	}

	m_bLoading = m_Builder->SceneBuild();

	return  m_bLoading;

}

bool CGameScene::WakeUp()
{
	m_Builder->WakeUpScene();
	return TRUE;
}
SPTR<CSceneBuilder> CGameScene::Builder() 
{
	return m_Builder;
}
void CGameScene::Progress()
{
	SceneUpdate();
	//CCriSectionObject<CGameScene>();
	PreUpdate();
	//CCriSectionObject<CGameScene>();
	Update();
	//CCriSectionObject<CGameScene>();
	PostUpdate();
	//CCriSectionObject<CGameScene>();
	Collision();
	//CCriSectionObject<CGameScene>();
	Render();
	//CCriSectionObject<CGameScene>();
	Release();
	//CCriSectionObject<CGameScene>();
	LastUpdate();
	SceneLastUpdate();
}
void CGameScene::SceneUpdate()
{
	m_Builder->SceneUpdate();
}
void CGameScene::SceneLastUpdate()
{
	m_Builder->SceneLastUpdate();
}
void CGameScene::PreUpdate()
{
	if (nullptr == m_MainCamera ) 
	{
		CActObject* MainCam = CActObject::CreateActObject(_T("MainCam"));
		m_MainCamera = MainCam;
		m_listCameraObject.clear();
		m_listCameraObject.push_back(MainCam);
	}

	LoopIterInit();
	LoopCamIterInit();

	
	for (; m_StartActIter != m_EndActIter ; ++m_StartActIter)
	{
		if (true == (*m_StartActIter)->IsActive() && false == (*m_StartActIter)->IsDeath())
		{
			(*m_StartActIter)->PreUpdate();
		}
	}

	for (; m_StartCamIter!= m_EndCamIter; ++m_StartCamIter )
	{
		if (true == (*m_StartCamIter)->IsActive() && false == (*m_StartCamIter)->IsDeath())
		{
			(*m_StartCamIter)->PreUpdate();
		}
	}
}
void CGameScene::Update()
{
	LoopIterInit();
	LoopCamIterInit();

	for (; m_StartActIter != m_EndActIter; ++m_StartActIter)
	{
		if (true == (*m_StartActIter)->IsActive() && false == (*m_StartActIter)->IsDeath())
		{
			(*m_StartActIter)->Update();
		}
	}

	for (; m_StartCamIter != m_EndCamIter; ++m_StartCamIter)
	{
		if (true == (*m_StartCamIter)->IsActive() && false == (*m_StartCamIter)->IsDeath())
		{
			(*m_StartCamIter)->Update();
		}
	}
}
void CGameScene::PostUpdate()
{
	LoopIterInit();
	LoopCamIterInit();

	for (; m_StartActIter != m_EndActIter; ++m_StartActIter)
	{
		if (true == (*m_StartActIter)->IsActive() && false == (*m_StartActIter)->IsDeath())
		{
			(*m_StartActIter)->PostUpdate();
		}
	}

	for (; m_StartCamIter != m_EndCamIter; ++m_StartCamIter)
	{
		if (true == (*m_StartCamIter)->IsActive() && false == (*m_StartCamIter)->IsDeath())
		{
			(*m_StartCamIter)->PostUpdate();
		}
	}
}
void CGameScene::Render()
{	
	m_PairRenderMgr->Render();

}
void CGameScene::FontRender()
{
	LoopIterInit();
	LoopCamIterInit();

	for (; m_StartActIter != m_EndActIter; ++m_StartActIter)
	{
		if (true == (*m_StartActIter)->IsActive() && false == (*m_StartActIter)->IsDeath())
		{
			(*m_StartActIter)->FontRender();
		}
	}

	for (; m_StartCamIter != m_EndCamIter; ++m_StartCamIter)
	{
		if (true == (*m_StartCamIter)->IsActive() && false == (*m_StartCamIter)->IsDeath())
		{
			(*m_StartCamIter)->FontRender();
		}
	}

}
void CGameScene::UIFontRender()
{
	LoopIterInit();
	LoopCamIterInit();

	for (; m_StartActIter != m_EndActIter; ++m_StartActIter)
	{
		if (true == (*m_StartActIter)->IsActive() && false == (*m_StartActIter)->IsDeath())
		{
			(*m_StartActIter)->UIFontRender();
		}
	}

	for (; m_StartCamIter != m_EndCamIter; ++m_StartCamIter)
	{
		if (true == (*m_StartCamIter)->IsActive() && false == (*m_StartCamIter)->IsDeath())
		{
			(*m_StartCamIter)->UIFontRender();
		}
	}

}
void CGameScene::DebugRender()
{
	LoopIterInit();
	LoopCamIterInit();

	for (; m_StartActIter != m_EndActIter; ++m_StartActIter)
	{
		if (true == (*m_StartActIter)->IsActive() && false == (*m_StartActIter)->IsDeath())
		{
			(*m_StartActIter)->DebugRender();
		}
	}

	for (; m_StartCamIter != m_EndCamIter; ++m_StartCamIter)
	{
		if (true == (*m_StartCamIter)->IsActive() && false == (*m_StartCamIter)->IsDeath())
		{
			(*m_StartCamIter)->DebugRender();
		}
	}
}
void CGameScene::Collision()
{
	m_PairCollisionMgr->Collision();
}
void CGameScene::Release()
{
	LoopIterInit();
	LoopCamIterInit();

	m_PairRenderMgr->ReleaseCom();
	m_PairCollisionMgr->ReleaseCom();

	for (; m_StartActIter != m_EndActIter; )
	{
		if (true == (*m_StartActIter)->IsDeath()) 
		{
			(*m_StartActIter)->Release();
			m_StartActIter = m_listActObject.erase(m_StartActIter);
		}
		else 
		{
			(*m_StartActIter)->CheckComponent();
			++m_StartActIter;
		}
	}

	// 카메라 오브젝트 
	for (; m_StartCamIter != m_EndCamIter; )
	{
		if (true == (*m_StartCamIter)->IsDeath())
		{
			(*m_StartCamIter)->Release();
			m_StartCamIter = m_listActObject.erase(m_StartCamIter);
		}
		else
		{
			++m_StartCamIter;
		}
	}

	
}
void CGameScene::LastUpdate()
{
	LoopIterInit();
	LoopCamIterInit();

	for (; m_StartActIter != m_EndActIter; ++m_StartActIter)
	{
		if (true == (*m_StartActIter)->IsActive() && false == (*m_StartActIter)->IsDeath())
		{
			(*m_StartActIter)->LastUpdate();
		}
		int a = 0;

	}

	for (; m_StartCamIter != m_EndCamIter; ++m_StartCamIter)
	{
		if (true == (*m_StartCamIter)->IsActive() && false == (*m_StartCamIter)->IsDeath())
		{
			(*m_StartCamIter)->LastUpdate();
		}
		int a = 0;
	}

	
}
bool CGameScene::PushObject(SPTR<CActObject> _pObject) 
{
	assert(_pObject);
	if (nullptr == _pObject) 
	{
		return FALSE;
	}

	_pObject->SetScene(this);
	m_listActObject.push_back(_pObject);
	return TRUE;

}

CActObject* CGameScene::CreateSubCamera(const tstring& _CamName,const Vec3& _CamPos /* = Vec3{ 0.0f ,0.0f ,-100.0f} */)
{
	//카메라 여러대 쓸일 생기면 그때 풀것
	return nullptr;
	
	
	CActObject* NewCamera = CActObject::CreateActObject(_CamName);

	if (_T("MainCamera") == _CamName) 
	{
		return nullptr;
	}

	//카메라 오브젝트는 기본적으로 카메라 컴포넌트를 갖는다
	NewCamera->CreateCom<CCamera>();
	NewCamera->GetTransCom()->SetPos(_CamPos);

	m_listCameraObject.push_back(NewCamera);
	return NewCamera;
}
CActObject* CGameScene::CreateMainCamera()
{
	CActObject* NewCamera = CActObject::CreateActObject(_T("MainCamera"),this);
	m_MainCamCom = NewCamera->CreateCom<CCamera>();
	m_MainPersCamCom = NewCamera->CreateCom<CPersCamera>();

	NewCamera->GetTransCom()->SetPos(Vec3{ 0.0f, 0.0f, 10.0f });

	m_MainCamera = NewCamera;
	m_listCameraObject.push_back(NewCamera);

	if ( nullptr == m_MainCamera || nullptr == m_MainCamCom || nullptr == m_MainPersCamCom)
		exit(0);

	return NewCamera;
}
CActObject* CGameScene::CreateMainLight()
{
	CActObject* NewLight = CActObject::CreateActObject(_T("MainLight"),this);
	NewLight->SetPos(Vec3(0.0f, 0.0f, -100.0f));
	m_MainLight = NewLight->CreateCom<CLight>();
	m_MainLight->SetDirectionLight();

	

	return NewLight;
}
void CGameScene::LoopIterInit()
{
	CCriSectionObject<CGameScene>();
	m_StartActIter = m_listActObject.begin();
	m_EndActIter = m_listActObject.end();
}
void CGameScene::LoopCamIterInit()
{
	CCriSectionObject<CGameScene>();
	m_StartCamIter = m_listCameraObject.begin();
	m_EndCamIter = m_listCameraObject.end();
}
void CGameScene::PushRenderer(int _Index, CRenderer* _pRenderer)
{
	if (nullptr == _pRenderer)
	{
		return;
	}
	m_PairRenderMgr->PushRenderer(_Index, _pRenderer);

}
CSceneCollisionMgr* CGameScene::GetCollisionMgr()
{
	return m_PairCollisionMgr;
}
SPTR<CSceneRenderMgr> CGameScene::GetRenderMgr()
{
	return m_PairRenderMgr;
}

bool CGameScene::CheckSubMgr()
{
	return m_PairRenderMgr->IsCheck() || m_PairCollisionMgr->IsCheck();
}
size_t CGameScene::RenderComCount()
{
	return m_PairRenderMgr->ActComCount();
}
size_t CGameScene::CollisionComCount()
{
	return m_PairCollisionMgr->ActComCount();
}
size_t CGameScene::ActObjectCount()
{
	return m_listActObject.size();
}
SPTR<CDebugMgr>	CGameScene::GetDebugMgr()
{
	return  m_ParentDebugMgr;
}
// 메인카메라가 붙은 오브젝트
SPTR<CActObject> CGameScene::GetMainCamera() 
{
	return m_MainCamera;
}
// 메인 카메라 컴포넌트
SPTR<CCamera> CGameScene::GetMainCamCom()
{
	return m_MainCamCom;
}
SPTR<CPersCamera> CGameScene::GetMainPersCamCom()
{
	return m_MainPersCamCom;
}
SPTR<CLight>	CGameScene::GetMainLight()
{
	return m_MainLight;
}
SPTR<CSceneBuilder>	CGameScene::GetBuilder()
{
	return m_Builder;
}
const int CGameScene::GetRenderOnComponent()
{
	return m_PairRenderMgr->GetRenderOnComponent();
}
void CGameScene::SetSceneEffect(const SCENEEFFECTDATA& _Data, const Vec2 _UnitPos)
{
	return SetSceneEffect(_Data, Vec3{ _UnitPos.x,_UnitPos.y,-10.0f });
}
void CGameScene::SetSceneEffect(const SCENEEFFECTDATA& _Data,const Vec3 _UnitPos)
{
	if (nullptr == m_Builder) 
	{
		return;
	}

	DWORD Flag = _Data.SceneEffectFlag & 0xFF000000L;

	m_Builder->SceneEffect(_Data.SceneEffectFlag, _Data.SceneEffectTime);

	switch (Flag)
	{
	case SCENE_EFFECT_EARTHQUAKE:
		m_Builder->SetEarthquakeData(_Data.Earthquake_SpaceSize);
		break;
	case SCENE_EFFECT_BLACKOUT:
	case SCENE_EFFECT_FLASH:
		m_Builder->InitBlackOut();
	case SCENE_EFFECT_FADE_IN:
	case SCENE_EFFECT_FADE_OUT:
		m_Builder->SetFadeData(_Data.SubTime);
	case SCENE_EFFECT_ZOOM_IN:
	case SCENE_EFFECT_ZOOM_OUT:
		m_Builder->SetZoomData(_UnitPos + _Data.ZoomPos, _Data.ZoomRatio, _Data.SubTime);
		break;
	}
}