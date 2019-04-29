#include"stdafx.h"
#include "Collision.h"
#include"SceneCollisionMgr.h"
#include"ActObject.h"


CCollision::CCollision()
	:m_EnterFunc(nullptr), m_StayFunc(nullptr), m_ExitFunc(nullptr)
	, m_eColType(CT_RECT), m_Fi(COLFIGURE()), m_GroupName(_T(""))
	, m_eColMode(CM_ACTOR), m_PivotPos({ 0.0f,0.0f,0.0f })
	, m_ColCustomSize({ 1.0f,1.0f,1.0f })
	, m_bDebugRen(true)
{
	m_FindEndIter = m_ColSet.end();
}
CCollision::CCollision(CActObject* _ActObject)
	:CGameCom(_ActObject) , m_EnterFunc(nullptr), m_StayFunc(nullptr), m_ExitFunc(nullptr)
	, m_eColType(CT_RECT), m_Fi(COLFIGURE()), m_GroupName(_T(""))
	, m_eColMode(CM_ACTOR), m_PivotPos({ 0.0f,0.0f,0.0f })
	, m_ColCustomSize({ 1.0f,1.0f,1.0f })
{
	m_FindEndIter = m_ColSet.end();
}

CCollision::~CCollision()
{
}
void CCollision::CallEnter(SPTR<CCollision> _This, SPTR<CCollision> _Other) 
{
	if (nullptr == m_EnterFunc) 
		return;
	
	m_EnterFunc(_This, _Other);
}
void CCollision::CallStay(SPTR<CCollision> _This, SPTR<CCollision> _Other) 
{
	if (nullptr == m_StayFunc)
		return;
	
	m_StayFunc(_This, _Other);
}
void CCollision::CallExit(SPTR<CCollision> _This, SPTR<CCollision> _Other) 
{
	if (nullptr == m_ExitFunc)
		return;
	
	m_ExitFunc(_This, _Other);
}
bool CCollision::FICheck(SPTR<CCollision> _Other)
{
	return CCollisionFunc::ColFunc[m_eColType][_Other->GetColType()](m_Fi, _Other->m_Fi);
}
bool CCollision::FICheck(SPTR<CCollision> _Other, const Vec3& _Pivot)
{
	COLFIGURE NewFi = _Other->m_Fi;
	NewFi.Pos +=_Pivot;
	return CCollisionFunc::ColFunc[m_eColType][_Other->GetColType()](m_Fi, NewFi);
}
void CCollision::ColCheck(SPTR<CCollision> _Other)
{
	m_FindIter = m_ColSet.find(_Other);

	if (TRUE == FICheck(_Other)) 
	{
		if (m_FindIter == m_FindEndIter) 
		{
			CallEnter(this, _Other);
			_Other->CallEnter(_Other,this);
			m_ColSet.insert(_Other);
			m_FindEndIter = m_ColSet.end();
		}
		else 
		{
			CallStay(this, _Other);
			_Other->CallStay(_Other, this);
		}
	}
	else 
	{
		if (m_FindIter != m_FindEndIter || true ==_Other->IsDeath())
		{
			CallExit(this, _Other);
			_Other->CallExit(_Other, this);
			m_ColSet.erase(m_FindIter);
			m_FindEndIter = m_ColSet.end();
		}
	}
}
void CCollision::Init() 
{
	m_SceneColMgr = ParentActObject()->GetScene()->GetCollisionMgr();
}

void CCollision::InitData(const tstring& _GroupName)
{
	assert(m_SceneColMgr);
	m_PivotPos = GAMEVEC::ZERO;
	m_Fi.Size = TransForm()->GetSize();
	m_GroupName = _GroupName;
	m_eColMode = CM_ACTOR;
	m_eColType = CT_RECT;
	m_SceneColMgr->PushColList(m_GroupName, this);
}
void CCollision::InitData(COLDATA _ColData)
{
	assert(m_SceneColMgr);
	m_PivotPos = GAMEVEC::ZERO;
	m_Fi = _ColData.Figure;
	m_GroupName = _ColData.GroupName;
	m_eColMode = _ColData.ColMode;
	m_eColType = _ColData.ColType;
	m_SceneColMgr->PushColList(m_GroupName, this);

}

void CCollision::CollisionSynchro()
{
	m_Fi.Pos = TransForm()->GetPos() + m_PivotPos;

	switch (m_eColMode)
	{
	case CM_ACTOR:
	{
		if (m_Fi.Size != TransForm()->GetSize())
		{
			m_Fi.Size = TransForm()->GetSize();
		}
	}
	break;
	case CM_CUSTOM:
	{
		m_Fi.Size = m_ColCustomSize;
	}
		break;
	default:
		return;
	}

	
}
void CCollision::SetColSize()
{
	m_eColMode = CM_ACTOR;
	m_Fi.Size = TRANS->GetSize();

}
void CCollision::SetPivotPos(const Vec3& _Pivot)
{
	m_PivotPos = _Pivot;
}
void CCollision::SetPivotPos(const Vec2& _Pivot)
{
	m_PivotPos = { _Pivot.x,_Pivot.y,0.0f };
}
void CCollision::SetColSize(const Vec3& _Size )
{
	m_eColMode = CM_CUSTOM;

	m_Fi.Size = _Size;
	m_ColCustomSize = _Size;	
}
void CCollision::SetColType(const COLTYPE& _Type)
{
	if (_Type >= CT_MAX)
		return;

	m_eColType = _Type;
}
void CCollision::SetColSize(const Vec2& _Size)
{
	m_eColMode = CM_CUSTOM;

	m_Fi.Size = { _Size.x, _Size.y, 1.0f };
	m_ColCustomSize = { _Size.x, _Size.y, 1.0f };
}
void CCollision::ReleaseSet()
{
	set<SPTR<CCollision>>::iterator	StartIter = m_ColSet.begin();
	set<SPTR<CCollision>>::iterator	EndIter = m_ColSet.end();

	for (;StartIter != EndIter; ++StartIter)
	{
		if (false == ((SPTR<CCollision>)*StartIter)->IsUpdate()) 
		{
			((SPTR<CCollision>)*StartIter)->CallExit(((SPTR<CCollision>)*StartIter), this);
			((SPTR<CCollision>)*StartIter)->EraseSet(this);
		}
	}

	m_ColSet.clear();
}
void CCollision::ClearSet()
{
	m_ColSet.clear();
	m_FindEndIter =  m_ColSet.end();
}
void CCollision::EraseSet(SPTR<CCollision> _Other)
{
	if (true == m_ColSet.empty())
		return;

	m_FindIter = m_ColSet.find(_Other);

	if (m_FindIter != m_ColSet.end())
	{
		m_ColSet.erase(m_FindIter);
		m_FindEndIter = m_ColSet.end();
	}
}
const bool CCollision::IsEmptyColSet()
{
	return m_ColSet.empty();
}
const Vec3 CCollision::GetActorPos()
{
	return TRANS->GetPos();
}
void CCollision::DebugRender()
{
	
	if(true == IsActive()&& m_bDebugRen)
		GetComDebugMgr()->RectRender(D3DCOLOR_ARGB(0, 255, 255, 255), TRANS->GetPos()+ m_PivotPos , m_Fi.Size);

}
const tstring CCollision::GetActorName()
{
	return ACTOR->Name();
}
void CCollision::ColDebugRenActive(const bool& _Debug)
{
	m_bDebugRen = _Debug;
}