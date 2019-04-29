#pragma once
#include <SceneBuilder.h>

class CEditMapMgr;
class CMapSceneBuilder : public CSceneBuilder
{
	CEditMapMgr*					m_pMapMgr;
	float							m_fCurCameraRatio;
	Vec2							m_CurMousePos;
	bool							m_bInWindow;


public:
	void WakeUpScene();
	bool SceneBuild();
	void SceneUpdate();
	void SceneRender();


public:
	void CreateMapSprite();
	void CreateObjectSprite();
	void MousePosUpdate();
	void CheckCamMove();
	void ZoomUpdate();
	const bool IsMousePosInWindow();

public:
	CMapSceneBuilder();
	~CMapSceneBuilder();
};

