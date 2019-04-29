#pragma once
#include "ObjBase.h"

class CGameScene;
class CGameWindow;
class CSceneBuilder :public CObjBase
{
public:
	friend CGameScene;
	friend CGameWindow;

protected:
	CGameScene* m_pScene;
	SPTR<CResourceMgr>	m_pResMgr;


	void SetGameScene(CGameScene* _pScene) 
	{
		if (nullptr == _pScene) 
		{
			return;
		}
		m_pScene = _pScene;
		m_pResMgr = m_pScene->GetWindow()->GetResourceMgr();
	}

	//Ư�� ȿ�� ���� ����
protected:
	function<void(void)>			m_SceneEffectFunc;

	// Ư�� ȿ�� ���� ����
	bool							m_bSceneEffect;
	DWORD							m_EffectFlag;
	float							m_fSceneEffectTime;
	float							m_fAccSceneEffectTime;

	// ���� ȿ���� ����ϴ� ����(ī�޶� �����̴� ������ ũ��)
	Vec2							m_SpaceSize;
	

	// �� in,out ȿ������ ����ϴ� ����
	Vec3							m_ZoomPos;
	float							m_fZoomRatio;
	//	ZoomRatio {StartRatio,EndRatio}
	Vec2							m_ZoomRatio;
	// Fade In/Out , Zoom Effect ���� ������
	float							m_fSubTime;
	float							m_fAccSubTime;

	float							m_fPow;



protected:
	void CreateCollisionGroup(const tstring& _Src, const tstring& _Dest);

public:
	CActObject* CreateActObject(const tstring& _ActName = _T(""));

public:
	SPTR<CResourceMgr> GetResourceMgr() 
	{
		return m_pResMgr;
	}
	CGameScene* GetScene();
	void CreateMainCamera();
	void CreateMainLight();
	SPTR<CCamera>	GetMainCam();

public:
	virtual void WakeUpScene() = 0;
	virtual bool SceneBuild() = 0;
	virtual void SceneUpdate();
	virtual void SceneLastUpdate();
	virtual void SceneRender();

	// Ư�� ȿ�� �Լ�
protected:
	template<typename T>
	void SetSceneEffect(T* _Obj, void(T::*_pFunc)(void))
	{
		if (nullptr == _pFunc || nullptr == _Obj) 
		{
			assert(nullptr);
			return;
		}
	
		m_SceneEffectFunc = bind(_pFunc, _Obj);
	}
private:
	// ���� Update �Լ�
	void YEarthquake();
	void XEarthquake();
	void CircleEarthquake();
	void RectEarthquake();
	// ����,���� Update �Լ�
	void BlackOut();
	void InitBlackOut();
	void EndBlackOut();
	// Fade In & Out Update �Լ�
	void FadeEffect();
	// Zoom In & Out Update �Լ�
	void ZoomEffect();

public:
	void EndBlackOutEffect();
	void EndZoomEffect();
	void EndFadeEffect();
public:
	// SceneEffect ����
	bool SceneEffect(DWORD _Flag, const float& _Time);
	// ���� Data ����
	const Vec2 GetSpaceSize(DWORD _Power);
	void SetEarthquakeData(const Vec2& _SpaceSize,const float& _PulseTime = 0.02f);
	// Fade Effect Data ����
	void SetFadeData(const float& _FadeTime);
	void SetFadeData(const float& _FadeTime,const float& _Pow);
	// Zoom Effect Data ����
	void SetZoomData(const Vec3& _ZoomPos, const float& _Near,const float& _ZoomTime);
	void SetZoomData(const Vec3& _ZoomPos, const Vec2& _ZoomRatio, const float& _ZoomTime);
	// ���� ����Ʈ �������� ǥ��
	bool IsSceneEffectOn();


protected:
	// ���� ���� ������ ��Ʈ�� ����ϴ� �Լ�
	void ScenenCurStateMsg();

public:
	CSceneBuilder();
	~CSceneBuilder();
};

