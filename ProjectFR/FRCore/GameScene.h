#pragma once
#include "ObjBase.h"


class CActObject;
class CSceneBuilder;
class CSceneRenderMgr;
class CSceneCollisionMgr;
class CRenderer;
class CAniRenderer;
class CDebugMgr;
class CCamera;
class CPersCamera;
class CLight;
class CGameScene : public CObjBase
{
public:
	friend class CGameWindow;
	friend class CSceneBuilder;

protected:
	// 카메라는 자식 클래스가 알수있게 한다.
	SPTR<CActObject>							m_MainCamera;
	SPTR<CCamera>								m_MainCamCom;
	SPTR<CPersCamera>							m_MainPersCamCom;
	SPTR<CLight>								m_MainLight;

private:
	bool										m_bLoading;
	CGameWindow*								m_ParentWindow;
	SPTR<CDebugMgr>								m_ParentDebugMgr;
	SPTR<CSceneBuilder>							m_Builder;
	SPTR<CSceneRenderMgr>						m_PairRenderMgr;
	CSceneCollisionMgr*							m_PairCollisionMgr;

	list<SPTR<CActObject>>						m_listActObject;
	list<SPTR<CActObject>>::iterator			m_StartActIter;
	list<SPTR<CActObject>>::iterator			m_EndActIter;

	//카메라 리스트(카메라가 1개 이상 존재 가능하다, 하지만 잠글거다)
	list<SPTR<CActObject>>						m_listCameraObject;
	list<SPTR<CActObject>>::iterator			m_StartCamIter;
	list<SPTR<CActObject>>::iterator			m_EndCamIter;


public:
	bool isLoading() { return m_bLoading; }
	bool isSetting() 
	{
		if (nullptr != m_Builder && nullptr != m_ParentWindow) 
		{
			return TRUE;
		}
		return FALSE;
	
	}
	SPTR<CSceneBuilder> Builder();
	SPTR<CGameWindow>	GetWindow() 
	{
		return m_ParentWindow;
	}
	CGameWindow*	GetWndPointer() 
	{
		return m_ParentWindow;
	}
	SPTR<CSceneBuilder>	GetBuilder();
	CSceneCollisionMgr* GetCollisionMgr();
	SPTR<CSceneRenderMgr> GetRenderMgr();
	SPTR<CDebugMgr>	GetDebugMgr();
	// 메인카메라가 붙은 오브젝트
	SPTR<CActObject> GetMainCamera();
	// 메인 카메라 컴포넌트
	SPTR<CCamera>	GetMainCamCom();
	SPTR<CPersCamera> GetMainPersCamCom();
	SPTR<CLight>	GetMainLight();

	void PushRenderer(int _Index, CRenderer* _pRenderer);
	

public:
	bool PushObject(SPTR<CActObject> _pObject);
	CActObject* CreateSubCamera(const tstring& _CamName, const Vec3& _CamPos = Vec3{ 0.0f ,0.0f ,-100.0f});
	
private:
	bool Init(SPTR<CSceneBuilder> _Builder ,CGameWindow* _Window);
	bool Loading();
	bool WakeUp();
	CActObject* CreateMainCamera();
	CActObject* CreateMainLight();


public:
	void LoopIterInit();
	void LoopCamIterInit();


public:
	bool CheckSubMgr();
	size_t RenderComCount();
	size_t CollisionComCount();
	size_t ActObjectCount();

private:
	void SceneUpdate();
	void SceneLastUpdate();
	void Progress();
	void PreUpdate();
	void Update();
	void PostUpdate();
	void Render();
	void Collision();
	// 컴포넌트의 후처리 함수용(소멸자 역할)
	void Release();
	// 모든 작업을 종료후 가장 마지막에 이루워지는 업데이트
	// (TransForm의 m_matAxis를 초기화용)
	void LastUpdate();
	

public:
	// 글자,라인을 랜더링 하는 함수(스킬 설명 or forcusing Line)
	void FontRender();
	// 글자,라인을 랜더링 하는 함수(카메라 영향을 안받는 버젼)
	void UIFontRender();

	// 디버그를 위한 글자 라인을 랜더링 하는 함수(on,off 기능)
	void DebugRender();
	const int GetRenderOnComponent();

public:
	void SetSceneEffect(const SCENEEFFECTDATA& _Data, const Vec3 _UnitPos);
	void SetSceneEffect(const SCENEEFFECTDATA& _Data, const Vec2 _UnitPos);


public:
	CGameScene();
	CGameScene(tstring _Name, SPTR<CSceneBuilder> _Builder  );
	CGameScene(tstring _Name, SPTR<CSceneBuilder> _Builder,CGameWindow* _Window);
	~CGameScene();
};

