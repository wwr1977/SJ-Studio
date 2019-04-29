#pragma once
#include <SceneBuilder.h>
class CEndingSceneBuilder : public CSceneBuilder
{
private:
	// ũ���� �ص��� �ƴѰ��
	// ������ Game Over�� ����
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

