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

	//특수 효과 관련 변수
protected:
	function<void(void)>			m_SceneEffectFunc;

	// 특수 효과 공용 변수
	bool							m_bSceneEffect;
	DWORD							m_EffectFlag;
	float							m_fSceneEffectTime;
	float							m_fAccSceneEffectTime;

	// 지진 효과에 사용하는 변수(카메라가 움직이는 영역의 크기)
	Vec2							m_SpaceSize;
	

	// 줌 in,out 효과에서 사용하는 변수
	Vec3							m_ZoomPos;
	float							m_fZoomRatio;
	//	ZoomRatio {StartRatio,EndRatio}
	Vec2							m_ZoomRatio;
	// Fade In/Out , Zoom Effect 에서 공용사용
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

	// 특수 효과 함수
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
	// 지진 Update 함수
	void YEarthquake();
	void XEarthquake();
	void CircleEarthquake();
	void RectEarthquake();
	// 암전,섬광 Update 함수
	void BlackOut();
	void InitBlackOut();
	void EndBlackOut();
	// Fade In & Out Update 함수
	void FadeEffect();
	// Zoom In & Out Update 함수
	void ZoomEffect();

public:
	void EndBlackOutEffect();
	void EndZoomEffect();
	void EndFadeEffect();
public:
	// SceneEffect 셋팅
	bool SceneEffect(DWORD _Flag, const float& _Time);
	// 지진 Data 셋팅
	const Vec2 GetSpaceSize(DWORD _Power);
	void SetEarthquakeData(const Vec2& _SpaceSize,const float& _PulseTime = 0.02f);
	// Fade Effect Data 셋팅
	void SetFadeData(const float& _FadeTime);
	void SetFadeData(const float& _FadeTime,const float& _Pow);
	// Zoom Effect Data 셋팅
	void SetZoomData(const Vec3& _ZoomPos, const float& _Near,const float& _ZoomTime);
	void SetZoomData(const Vec3& _ZoomPos, const Vec2& _ZoomRatio, const float& _ZoomTime);
	// 현재 이펙트 진행중을 표기
	bool IsSceneEffectOn();


protected:
	// 현재 씬의 정보를 폰트로 출력하는 함수
	void ScenenCurStateMsg();

public:
	CSceneBuilder();
	~CSceneBuilder();
};

