#pragma once
#include <SceneBuilder.h>

class CDataRenderer;
class CStatSceneBuilder : public CSceneBuilder
{
private:
	// 스탯 씬의 하단의 게이지와 폰트쓰기를 담당하는 로직
	SPTR<CDataRenderer>					m_DataRenderer;

	SPTR<CAniRenderer>					m_StatIdleModel;

public:
	void WakeUpScene();
	bool SceneBuild();
	void SceneUpdate();

	void LoadResource();

	// 프로그램 메모리에 이미지 데이터가 로드가 안된경우
	void LoadingIdleImage(FOLDER _ForderIndex);
	void LoadingIdleAnimation(FOLDER _FolderIndex);

	// 프로그램 메모리에 이미지 데이터가 모두 로드된경우(SkillMgr이 생성 완료된 경우)
	void SettingStatModel();

public:
	CStatSceneBuilder();
	~CStatSceneBuilder();
};

