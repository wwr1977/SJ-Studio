#pragma once
#include <SceneBuilder.h>
class CTitleSceneBuilder : public CSceneBuilder
{
	bool							m_bIntro;
	bool							m_bKeyCheck;
	bool							m_bGameStart;

	SPTR<class CTitleIntroObject>	m_IntroObject;
	SPTR<class CTitleBackGround>	m_MainTitle;


public:
	void WakeUpScene();
	bool SceneBuild();
	void SceneUpdate();
	void SceneLastUpdate();
	void SceneRender();

public:
	void SetKeyCheck(const bool& _Check);
	void KeyCheck();
	void GameStartUpdate();

public:
	void IntroEndEvent();

public:
	CTitleSceneBuilder();
	~CTitleSceneBuilder();
};

