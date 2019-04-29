#pragma once
#include <SceneBuilder.h>

class CDataRenderer;
class CStatSceneBuilder : public CSceneBuilder
{
private:
	// ���� ���� �ϴ��� �������� ��Ʈ���⸦ ����ϴ� ����
	SPTR<CDataRenderer>					m_DataRenderer;

	SPTR<CAniRenderer>					m_StatIdleModel;

public:
	void WakeUpScene();
	bool SceneBuild();
	void SceneUpdate();

	void LoadResource();

	// ���α׷� �޸𸮿� �̹��� �����Ͱ� �ε尡 �ȵȰ��
	void LoadingIdleImage(FOLDER _ForderIndex);
	void LoadingIdleAnimation(FOLDER _FolderIndex);

	// ���α׷� �޸𸮿� �̹��� �����Ͱ� ��� �ε�Ȱ��(SkillMgr�� ���� �Ϸ�� ���)
	void SettingStatModel();

public:
	CStatSceneBuilder();
	~CStatSceneBuilder();
};

