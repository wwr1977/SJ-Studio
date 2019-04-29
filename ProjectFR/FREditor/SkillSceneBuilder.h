#pragma once
#include<SceneBuilder.h>


class CActObject;
class CFixRenderer;
class CAniRenderer;
class CEditEffectMgr;
class CSkillMgr;
class CSkillSceneBuilder : public CSceneBuilder
{
	SPTR<CSkillMgr>			m_SkillMgr;
	SPTR<CActObject>		m_MainCameraMen;

	// ��Ƽ ��������Ʈ DLG���� ����� ������Ʈ �� ������
	SPTR<CActObject>		m_TexModel;
	float					m_fTexScale;

	SPTR<CActObject>		m_AniSpriteModel;
	float					m_fSpriteScale;

	SPTR<CActObject>		m_BaseAniModel;
	float					m_fAniScale;

	// �ִϸ��̼� DLG���� ����ϴ� ������Ʈ�� ������
	SPTR<CActObject>		m_AnimationModel;
	SPTR<CAniRenderer>		m_AnimationCom;

	// ����Ʈ DLG���� ����ϴ� ������Ʈ�� ������
	SPTR<CActObject>		m_EffectAniModel;
	SPTR<CAniRenderer>		m_EffectAniCom;
	// ����Ʈ �����Ϳ��� ���� ������ ���带 �÷��� �ϱ����� ������Ʈ
	SPTR<CSoundPlayer>		m_EffectSoundPlayer;
	
	

	void KeySetting();
	//void CreateMainCamera();
	void SettingMulSpriteModel();
	void SyncSpriteSize(SPTR<CActObject> _pObject,SPTR<CFixRenderer> _Renderer);
	


public:
	void WakeUpScene();
	bool SceneBuild();
	void SceneUpdate();
	void CheckCamMove();
	void SceneRender();

public:
	CSkillSceneBuilder();
	~CSkillSceneBuilder();
};

