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

	// 멀티 스프라이트 DLG에서 사용할 오브젝트 와 랜더러
	SPTR<CActObject>		m_TexModel;
	float					m_fTexScale;

	SPTR<CActObject>		m_AniSpriteModel;
	float					m_fSpriteScale;

	SPTR<CActObject>		m_BaseAniModel;
	float					m_fAniScale;

	// 애니메이션 DLG에서 사용하는 오브젝트와 랜더러
	SPTR<CActObject>		m_AnimationModel;
	SPTR<CAniRenderer>		m_AnimationCom;

	// 이팩트 DLG에서 사용하는 오브젝트와 랜더러
	SPTR<CActObject>		m_EffectAniModel;
	SPTR<CAniRenderer>		m_EffectAniCom;
	// 이팩트 에디터에서 현재 지정된 사운드를 플레이 하기위한 컴포넌트
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

