#pragma once
#include <SceneBuilder.h>

class CFixRenderer;
class CAniRenderer;
class CLoadingSceneBuilder : public CSceneBuilder
{
	enum
	{
		MAXHANDLE = 16,
	};

private:
	SPTR<CActObject>						m_MouseObject;
	HANDLE									m_pHandle[MAXHANDLE];
	int										m_iCurHandleIndex;

public:
	void WakeUpScene();
	bool SceneBuild();
	void SceneUpdate();


	void LoadingTexture();
	void CreateSprite();
	void CreateMultiSprite();
	void CreateSkillIcon();
	void CreateGameSound();
	void LoadAnimationData();
	void LoadMultiSpriteData();
	void LoadStatData();
	void LoadMonsterData();
	void LoadEffectData();
	void LoadSkillData();
	void LoadItemData();
	void ThreadWaiting();

	void SceneSerialLoading();
	void SceneThreadLoading();

public:
	CLoadingSceneBuilder();
	~CLoadingSceneBuilder();
};

