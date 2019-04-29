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
	// ī�޶�� �ڽ� Ŭ������ �˼��ְ� �Ѵ�.
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

	//ī�޶� ����Ʈ(ī�޶� 1�� �̻� ���� �����ϴ�, ������ ��۰Ŵ�)
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
	// ����ī�޶� ���� ������Ʈ
	SPTR<CActObject> GetMainCamera();
	// ���� ī�޶� ������Ʈ
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
	// ������Ʈ�� ��ó�� �Լ���(�Ҹ��� ����)
	void Release();
	// ��� �۾��� ������ ���� �������� �̷������ ������Ʈ
	// (TransForm�� m_matAxis�� �ʱ�ȭ��)
	void LastUpdate();
	

public:
	// ����,������ ������ �ϴ� �Լ�(��ų ���� or forcusing Line)
	void FontRender();
	// ����,������ ������ �ϴ� �Լ�(ī�޶� ������ �ȹ޴� ����)
	void UIFontRender();

	// ����׸� ���� ���� ������ ������ �ϴ� �Լ�(on,off ���)
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

