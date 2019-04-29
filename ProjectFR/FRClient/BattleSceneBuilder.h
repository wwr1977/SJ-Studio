#pragma once
#include <SceneBuilder.h>

class CDemageFontGroup;
class CBattleUnit;
class CUnitStatusBar;
class CTurnOrderUI;
class CTurnMgr;
class CTestEffect;
class CBattleSceneBuilder : public CSceneBuilder
{
private:
	SPTR<CActObject>								m_MouseObject;
	SPTR<CTurnMgr>									m_TurnMgr;
									
private:
	void WakeUpScene();
	bool SceneBuild();
	void SceneUpdate();	
	void SceneRender();
	
public:
	void ZoomCheck();
	void CameraMoveCheck();
	SPTR<CCamera> GetMainCamCom();


public:
	CBattleSceneBuilder();
	~CBattleSceneBuilder();
};

