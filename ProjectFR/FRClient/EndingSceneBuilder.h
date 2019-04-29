#pragma once
#include <SceneBuilder.h>
class CEndingSceneBuilder : public CSceneBuilder
{
private:
	// 크레딧 앤딩이 아닌경우
	// 무조건 Game Over로 간주
	bool								m_bCreditEnding;
	SPTR<class CGameOver>				m_GameOver;
	SPTR<class CEndingCredit>			m_EndingCredit;

	
public:
	void WakeUpScene();
	bool SceneBuild();
	void SceneUpdate();
	void SceneRender();

public:
	void ChangeTitleSceneEvent();

public:
	void GameOverEvent();
	void EndingEvent();

public:
	CEndingSceneBuilder();
	~CEndingSceneBuilder();
};

