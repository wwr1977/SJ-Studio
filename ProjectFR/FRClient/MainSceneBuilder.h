#pragma once
#include <SceneBuilder.h>

class CFixRenderer;
class CAniRenderer;
class CGameMaster;
class CMainSceneBuilder : public CSceneBuilder
{
private:
	// MainScene�� ��� �̺�Ʈ�� �ൿ�� ��� ����]
	// �������� Mgr�� ������ ����(ScriptMgr, QuestMgr, MapMgr ���)
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

