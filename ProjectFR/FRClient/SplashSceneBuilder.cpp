#include "Precom.h"
#include "SplashSceneBuilder.h"
#include <GameSprite.h>

CSplashSceneBuilder::CSplashSceneBuilder()
	:m_SplashImageRen(nullptr), m_bRendering(true)
{
}


CSplashSceneBuilder::~CSplashSceneBuilder()
{

}
void CSplashSceneBuilder::WakeUpScene() 
{
	
}
bool CSplashSceneBuilder::SceneBuild() 
{
	CClientGlobal::SplashBuilder = this;

	SettingRenderEnvironment();

	RESMGR->LoadGameTexture(CFilePathMgr::FilePath(_T("Texture"), _T("SplashImage.png")));
	RESMGR->CreateGameSprite(_T("SplashImage"));

	CActObject* SplashObj = CreateActObject(_T("SplashImage"));
	SplashObj->SetPos(Vec3{ 0.f,0.f,7.f });

	m_SplashImageRen = SplashObj->CreateCom<CFixRenderer>(RENDATA(LAYER_UI, 1.f));
	m_SplashImageRen->SetSprite(_T("SplashImage"));
	

	return true;
}
void CSplashSceneBuilder::SceneUpdate()
{

}
void CSplashSceneBuilder::SetSplashImageRender(const bool& _Ren)
{
	CCriSectionObject<CSplashSceneBuilder>();
	m_bRendering = _Ren;
}
void CSplashSceneBuilder::SettingRenderEnvironment()
{
	RESMGR->LoadGameTexture(CFilePathMgr::FilePath(_T("UI"), _T("ColorBase.png")));

	CreateMainLight();
	CreateMainCamera();

	Mat Proj;
	Mat View;

	GetScene()->GetMainCamCom()->PostUpdate();

	Proj = GetScene()->GetMainCamCom()->GetSceneProj();
	View = GetScene()->GetMainCamCom()->GetSceneView();

	GetScene()->GetWindow()->GetLPDevice()->SetTransform(D3DTS_PROJECTION, &Proj);
	GetScene()->GetWindow()->GetLPDevice()->SetTransform(D3DTS_VIEW, &View);

}
void CSplashSceneBuilder::SplashImageRendering()
{
	for (;;)
	{
		CCriSectionObject<CSplashSceneBuilder>();
		if (false == m_bRendering)
			return;

		GetScene()->GetWindow()->DeviceRenderStart();
		m_SplashImageRen->Render();
		GetScene()->GetWindow()->DeviceRenderEnd();
	}

	return;
}
