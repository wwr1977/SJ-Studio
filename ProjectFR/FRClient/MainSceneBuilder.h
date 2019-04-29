#pragma once
#include <SceneBuilder.h>

class CFixRenderer;
class CAniRenderer;
class CGameMaster;
class CMainSceneBuilder : public CSceneBuilder
{
private:
	// MainScene의 모든 이벤트와 행동을 대신 수행]
	// 여러개의 Mgr로 구성될 예정(ScriptMgr, QuestMgr, MapMgr 등등)
	CGameMaster*							m_pGameMaster;

	SPTR<CActObject>						m_MouseObject;
	float									CurCameraRatio;

public:
	void WakeUpScene();
	bool SceneBuild();
	void SceneUpdate();
	void SceneLastUpdate();
	void SceneRender();


public:
	void ZoomUpdate();
	void InitZoom();


public:
	CMainSceneBuilder();
	~CMainSceneBuilder();
};

