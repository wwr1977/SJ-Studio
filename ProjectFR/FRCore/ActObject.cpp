#include "stdafx.h"
#include "ActObject.h"

SPTR<CGameScene> CActObject::m_pCurScene = nullptr;

CActObject::CActObject()
	:CObjBase(_T("ActObject"))
	, m_RenderStyle(SPRITESIZE), m_fMagnification(1.0f)
{
	SetTypeInfo(this);
	m_pCurScene->PushObject(this);
	m_pTransCom = new CTransForm(this);
	m_pTransCom->SetTypeInfo(m_pTransCom);
	m_pTransCom->SetImageScale(m_fMagnification);
	m_listCom.push_back(m_pTransCom);

	
}
CActObject::CActObject(SPTR<CGameScene> _Scene)
	:CObjBase(_T("ActObject"))
	, m_RenderStyle(SPRITESIZE), m_fMagnification(1.0f)
{
	assert(_Scene);
	SetTypeInfo(this);
	_Scene->PushObject(this);
	m_pTransCom = new CTransForm(this);
	m_pTransCom->SetTypeInfo(m_pTransCom);
	m_pTransCom->SetImageScale(m_fMagnification);
	m_listCom.push_back(m_pTransCom);

}
CActObject::CActObject(const tstring& _Name, SPTR<CGameScene> _Scene)
	:CObjBase(_Name)
	, m_RenderStyle(SPRITESIZE), m_fMagnification(1.0f)
{
	assert(_Scene);
	SetTypeInfo(this);
	_Scene->PushObject(this);
	m_pTransCom = new CTransForm(this);
	m_pTransCom->SetTypeInfo(m_pTransCom);
	m_pTransCom->SetImageScale(m_fMagnification);
	m_listCom.push_back(m_pTransCom);

}
CActObject::CActObject(const tstring& _Name )
	:CObjBase(_Name)
	,m_RenderStyle(SPRITESIZE), m_fMagnification(1.0f)
{
	SetTypeInfo(this);
	m_pCurScene->PushObject(this);
	m_pTransCom = new CTransForm(this);
	m_pTransCom->SetTypeInfo(m_pTransCom);
	m_pTransCom->SetImageScale(m_fMagnification);
	m_listCom.push_back(m_pTransCom);
}
CActObject::~CActObject()
{
	LoopIterInit();
	for (; m_ComStartIter != m_ComEndIter; ++m_ComStartIter)
	{
		//(*m_ComStartIter)->SetActObject(nullptr);
		(*m_ComStartIter)->Death();
	}
	m_listCom.clear();
}

////////////////////////////////		Static			/////////////////////////////////

//////////////////////////////////		Construct		/////////////////////////////////////



CActObject*  CActObject::CreateActObject()
{
	return new CActObject();
}
CActObject* CActObject::CreateActObject(const tstring& _Name)
{
	return new CActObject(_Name);
}
CActObject*  CActObject::CreateActObject(const SPTR<CGameScene> _BelongScene)
{
	return new CActObject(_BelongScene);
}
CActObject*  CActObject::CreateActObject(const tstring& _Name, const SPTR<CGameScene> _BelongScene)
{
	return new CActObject(_Name,_BelongScene);
}

void CActObject::PreUpdate()
{
	LoopIterInit();

	CCriSectionObject<CActObject>();
	//만일 ComList에 TransForm 이 존재하지 않다면
	for (; m_ComStartIter != m_ComEndIter; ++m_ComStartIter)
	{
		if ((*m_ComStartIter)->EqualType<CTransForm>())
		{
			break;
		}
	}

	if (m_ComEndIter == m_ComStartIter) 
	{
		m_listCom.push_back(m_pTransCom);
	}


	LoopIterInit();

	for (; m_ComStartIter != m_ComEndIter; ++m_ComStartIter)
	{
		if(nullptr != *m_ComStartIter && true == (*m_ComStartIter)->IsActive())
			(*m_ComStartIter)->PreUpdate();
	}

}
void CActObject::Update()
{
	LoopIterInit();

	CCriSectionObject<CActObject>();
	for (; m_ComStartIter != m_ComEndIter; ++m_ComStartIter)
	{
		if (nullptr != *m_ComStartIter && true == (*m_ComStartIter)->IsActive())
			(*m_ComStartIter)->Update();
	}

}
void CActObject::PostUpdate()
{
	LoopIterInit();

	CCriSectionObject<CActObject>();
	for (; m_ComStartIter != m_ComEndIter; ++m_ComStartIter)
	{
		if (nullptr != *m_ComStartIter && true == (*m_ComStartIter)->IsActive())
			(*m_ComStartIter)->PostUpdate();
	}

	
}
void CActObject::FontRender()
{
	LoopIterInit();

	CCriSectionObject<CActObject>();
	for (; m_ComStartIter != m_ComEndIter; ++m_ComStartIter)
	{
		(*m_ComStartIter)->FontRender();
	}
}
void CActObject::UIFontRender()
{
	LoopIterInit();

	CCriSectionObject<CActObject>();
	for (; m_ComStartIter != m_ComEndIter; ++m_ComStartIter)
	{
		(*m_ComStartIter)->UIFontRender();
	}
}
void CActObject::DebugRender()
{
	LoopIterInit();
	
	CCriSectionObject<CActObject>();
	for (; m_ComStartIter != m_ComEndIter; ++m_ComStartIter)
	{
		(*m_ComStartIter)->DebugRender();
	}
}
void CActObject::Collision()
{
	LoopIterInit();

	CCriSectionObject<CActObject>();
	for (; m_ComStartIter != m_ComEndIter; ++m_ComStartIter)
	{
		(*m_ComStartIter)->Collision();
	}
}
void CActObject::Release()
{
	LoopIterInit();

	CCriSectionObject<CActObject>();
	for (; m_ComStartIter != m_ComEndIter; ++m_ComStartIter)
	{
		(*m_ComStartIter)->Release();
	}

}
void CActObject::LastUpdate()
{
	LoopIterInit();

	CCriSectionObject<CActObject>();
	for (; m_ComStartIter != m_ComEndIter; ++m_ComStartIter)
	{
		(*m_ComStartIter)->LastUpdate();
	}
}
void CActObject::CheckComponent()
{
	LoopIterInit();

	CCriSectionObject<CActObject>();
	for (; m_ComStartIter != m_ComEndIter;)
	{
		if (true == (*m_ComStartIter)->CObjBase::IsDeath()) 
		{
			(*m_ComStartIter)->Release();
			m_ComStartIter = m_listCom.erase(m_ComStartIter);
		}
		else 
		{
			++m_ComStartIter;
		}
	}
}

void CActObject::PushCom(CGameCom* _Com) 
{
	SPTR<CGameCom> Trans = m_listCom.back();
	m_listCom.pop_back();
	m_listCom.push_back(_Com);
	m_listCom.push_back(Trans);
}
void CActObject::SetImageScale(const float& _ImageScale)
{
	m_fMagnification = _ImageScale;
	m_pTransCom->SetImageScale(_ImageScale);
}

void CActObject::SetPos(const Vec3& _Pos)
{
	m_pTransCom->SetPos(_Pos);
}
void CActObject::SetPos(const Vec2& _Pos)
{
	m_pTransCom->SetPos(_Pos);
}
void CActObject::SetSize(const Vec3& _Size)
{
	m_pTransCom->SetSize(_Size);
}
void CActObject::Move(const Vec3& _Dir)
{
	m_pTransCom->Move(_Dir);
}
void CActObject::Death()
{
	CObjBase::Death();

	LoopIterInit();

	for (;m_ComStartIter != m_ComEndIter  ; ++m_ComStartIter)
	{
		(*m_ComStartIter)->SetActObject(nullptr);
	}
}
void CActObject::LoopIterInit()
{
	CCriSectionObject<CActObject>();
	m_ComStartIter = m_listCom.begin();
	m_ComEndIter = m_listCom.end();
}
const Vec3 CActObject::GetPos() const
{
	return m_pTransCom->GetPos();
}
const Vec3 CActObject::GetSize() const
{
	return m_pTransCom->GetSize();
}