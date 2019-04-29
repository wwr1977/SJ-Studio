#pragma once
#include <SceneBuilder.h>
class CSplashSceneBuilder : public CSceneBuilder
{
private:
	SPTR<CFixRenderer>					m_SplashImageRen;
	bool								m_bRendering;

public:
	void WakeUpScene();
	bool SceneBuild();
	void SceneUpdate();

public:
	void SetSplashImageRender(const bool& _Ren);
	void SettingRenderEnvironment();
	void SplashImageRendering();

public:
	//static void ThreadSceneUpdate(void* _Arg);

public:
	CSplashSceneBuilder();
	~CSplashSceneBuilder();
};

