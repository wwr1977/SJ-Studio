#pragma once
#include "ObjBase.h"

class CGameScene;
class CGameCom;
class CTransForm;
class CActObject : public CObjBase
{
private:
	friend CGameScene;
	friend bool CGameWindow::ChangeScene(const tstring& _Name);

public:
	static SPTR<CGameScene>					m_pCurScene;
private:
	CGameScene*								m_pParentScene;
	CTransForm*								m_pTransCom;
	
	RENDERSTYLE								m_RenderStyle;
	// 이미지의 몇배율로 크기 조정할것인가
	float									m_fMagnification;

private:
	list<SPTR<CGameCom>>							m_listCom;
	list<SPTR<CGameCom>>::iterator					m_ComStartIter;
	list<SPTR<CGameCom>>::iterator					m_ComEndIter;

public:
	static CActObject*  CreateActObject();
	static CActObject*  CreateActObject(const tstring& _Name);
	static CActObject*  CreateActObject(SPTR<CGameScene> _BelongScene);
	static CActObject*  CreateActObject(const tstring& _Name,SPTR<CGameScene> _BelongScene);

public:
	void SetScene(CGameScene* _pScene) 
	{
		m_pParentScene = _pScene;
	}
	CGameScene* GetScene() 
	{
		return m_pParentScene;
	}

public:
	void SetRenStyle(RENDERSTYLE _Style) 
	{
		m_RenderStyle = _Style;
	}
	


public:
	
	SPTR<CTransForm> GetTransCom() 
	{
		return m_pTransCom;
	}
	RENDERSTYLE GetRenderStyle() 
	{
		return m_RenderStyle;
	}
	const float GetMagnification() 
	{
		return m_fMagnification;
	}
	//이게 이미지 출력배율을 셋팅
	void SetImageScale(const float& _ImageScale);

	void SetPos(const Vec3& _Pos);
	void SetPos(const Vec2& _Pos);
	void SetSize(const Vec3& _Size);
	void Move(const Vec3& _Dir);

	const Vec3 GetPos() const;
	const Vec3 GetSize() const;


	

public:
	template<typename T>
	T* CreateCom(void* _pData = nullptr)
	{
		assert(&typeid(T) != CDX9Core::m_TransComInfo);
		if (&typeid(T) == CDX9Core::m_TransComInfo) 
		{
			return nullptr;
		}
		
		T* NewCom = new T();
		NewCom->TypeInit();
		NewCom->SetActObject(this);
		NewCom->SetTransForm(m_pTransCom);
		NewCom->BaseComInit();
		NewCom->Init();
		NewCom->InitData(_pData);
		
		SPTR<CGameCom> Trans = m_listCom.back();
		m_listCom.pop_back();
		m_listCom.push_back(NewCom);
		m_listCom.push_back(Trans);
		
		return NewCom;
	}
	
	template<typename T, typename STARTVALUE>
	T* CreateCom(STARTVALUE _pData)
	{
		assert(&typeid(T) != CDX9Core::m_TransComInfo);
		if (&typeid(T) == CDX9Core::m_TransComInfo)
		{
			return nullptr;
		}

		T* NewCom = new T();
		NewCom->TypeInit();
		NewCom->SetActObject(this);
		NewCom->SetTransForm(m_pTransCom);
		NewCom->BaseComInit();
		NewCom -> Init();
		NewCom -> InitData(_pData);

		SPTR<CGameCom> Trans = m_listCom.back();
		m_listCom.pop_back();
		m_listCom.push_back(NewCom);
		m_listCom.push_back(Trans);

		return NewCom;
	}

	template<typename T>
	BOOL EraseCom(const tstring _Name = _T("")) 
	{
		LoopIterInit();

		for (; m_ComStartIter != m_ComEndIter; ++m_ComStartIter)
		{
			if (true == (*m_ComStartIter)->EqualType<T>() 
				&& (*m_ComStartIter)->Name() == _Name) 
			{
				(*m_ComStartIter)->Death();
			}
		}

	}

private:
	void LoopIterInit();

private:
	void PreUpdate();
	void Update();
	void PostUpdate();
	void FontRender();
	void UIFontRender();
	void DebugRender();
	void Collision();
	void Release();
	void LastUpdate();
public:
	void Death();
	void CheckComponent();

private:
	void PushCom(CGameCom* _Com);
	

private:
	CActObject();
	CActObject(const tstring& _Name);
	CActObject(SPTR<CGameScene> _Scene);
	CActObject(const tstring& _Name,SPTR<CGameScene> _Scene);
	virtual ~CActObject();
};

