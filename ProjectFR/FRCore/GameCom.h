#pragma once
#include "ObjBase.h"

class CActObject;
class CTransForm;
class CGameCom : public CObjBase
{
protected:
	bool							m_bMulti;
	SPTR<CCamera>					m_SceneMainCamera;

private:
	CActObject*						m_pParentObject;
	CTransForm*						m_pTrans;
	LPDIRECT3DDEVICE9				m_pComDevice;
	SPTR<CResourceMgr>				m_ComResourceMgr;
	SPTR<CDebugMgr>					m_ComDebugMgr;

private:
	friend CActObject;
	void SetActObject(CActObject* _Object);
	void SetTransForm(CTransForm* _pTrans);

	

public:
	virtual void InitData(void* pData);
	
public:
	const bool isMulti() { return m_bMulti; }
	bool IsDeath();
	bool IsUpdate();
	const bool IsActorOn();
	void BaseComInit();

	virtual void Init();
	virtual void PreUpdate();
	virtual void Update();
	virtual void PostUpdate();
	virtual void Render();
	virtual void FontRender();
	virtual void UIFontRender();
	virtual void DebugRender();
	virtual void Collision();
	virtual void Release();
	// ���� ������ Update�� ���������ӿ��� ����� 
	// ������ �ʱ�ȭ �ϴ� ����(TransForm�� m_AxisMat �ʱ�ȭ��)
	virtual void LastUpdate();

public:
	CActObject* ParentActObject();
	const tstring GetActorName();
	CTransForm*	TransForm();

public:
	LPDIRECT3DDEVICE9 GetComDevice();
	SPTR<CResourceMgr> GetComResourceMgr();
	SPTR<CDebugMgr> GetComDebugMgr();
	

public:
	void ActorOn();
	void ActorOff();
	void ActorActive(const bool& _Enable);

public:
	CGameCom();
	CGameCom(CActObject* _ActObject);
	virtual ~CGameCom();
};

