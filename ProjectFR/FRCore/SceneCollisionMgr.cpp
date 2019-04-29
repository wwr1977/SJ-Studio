#include "stdafx.h"
#include "SceneCollisionMgr.h"
#include "Collision.h"

CSceneCollisionMgr::CSceneCollisionMgr(CGameScene* _PairScene)
	:m_PairScene(_PairScene)
{
}


CSceneCollisionMgr::~CSceneCollisionMgr()
{
	m_vecColCheck.clear();
	m_mapCollsion.clear();
}
void CSceneCollisionMgr::PushColList(tstring _Name, CCollision* _Col)
{
	
	unordered_map<tstring, list<SPTR<CCollision>>>::iterator  FindIter = m_mapCollsion.find(_Name);

	if (FindIter == m_mapCollsion.end()) 
	{
		list<SPTR<CCollision>> NewList = list<SPTR<CCollision>>();
		NewList.push_back(_Col);
		m_mapCollsion.insert(unordered_map<tstring, list<SPTR<CCollision>>>::value_type(_Name, NewList));
		return;
	}
	
	FindIter->second.push_back(_Col);
}
void CSceneCollisionMgr::ThisCollision()
{
	list<SPTR<CCollision>>::iterator	SrcStartIter = m_SrcIter->second.begin();
	list<SPTR<CCollision>>::iterator	SrcEndIter = m_SrcIter->second.end();

	for (; SrcStartIter != SrcEndIter; ++SrcStartIter)
	{
		/*if ( false == (*SrcStartIter)->IsUpdate())
		{
			continue;
		}*/

		list<SPTR<CCollision >>::iterator DestStartIter = SrcStartIter;
		++DestStartIter;
		list<SPTR<CCollision>>::iterator DestEndIter = SrcEndIter;

		for (; DestStartIter != DestEndIter; ++DestStartIter)
		{
			if (false == (*DestStartIter)->IsUpdate())
			{
				continue;
			}


			(*SrcStartIter)->CollisionSynchro();
			(*DestStartIter)->CollisionSynchro();

			(*SrcStartIter)->ColCheck((*DestStartIter));
			
			if (false == (*SrcStartIter)->IsUpdate())
			{
				(*SrcStartIter)->ReleaseSet();
			}
			if (false == (*DestStartIter)->IsUpdate())
			{
				(*SrcStartIter)->EraseSet((*DestStartIter));
			}
		}
	}
}
void CSceneCollisionMgr::OtherCollision()
{
	list<SPTR<CCollision>>::iterator	SrcStartIter = m_SrcIter->second.begin();
	list<SPTR<CCollision>>::iterator	SrcEndIter = m_SrcIter->second.end();

	for (;SrcStartIter != SrcEndIter; ++SrcStartIter)
	{
		if (false == (*SrcStartIter)->IsUpdate())
		{
			continue;
		}

		list<SPTR<CCollision >>::iterator DestStartIter = m_DestIter->second.begin();
		list<SPTR<CCollision>>::iterator DestEndIter = m_DestIter->second.end();

		for (; DestStartIter != DestEndIter; ++DestStartIter)
		{
			if (false == (*DestStartIter)->IsUpdate())
			{
				continue;
			}

		
			(*SrcStartIter)->CollisionSynchro();
			(*DestStartIter)->CollisionSynchro();

			(*SrcStartIter)->ColCheck((*DestStartIter));
		
			if (false == (*SrcStartIter)->IsUpdate())
			{
				(*SrcStartIter)->ReleaseSet();
			}
			if (false == (*DestStartIter)->IsUpdate())
			{
				(*SrcStartIter)->EraseSet((*DestStartIter));
			}
		}
	}
}
void CSceneCollisionMgr::Collision()
{
	m_bCheck = false;

	for (size_t i = 0; i < m_vecColCheck.size(); i++)
	{
		m_SrcIter = m_mapCollsion.find(m_vecColCheck[i].Src);
		m_DestIter = m_mapCollsion.find(m_vecColCheck[i].Dest);

		if (m_SrcIter == m_mapCollsion.end() || m_DestIter == m_mapCollsion.end()) 
		{
			continue;
		}

		if (m_SrcIter == m_DestIter) 
		{
			ThisCollision();
		}
		else 
		{
			OtherCollision();
		}
	}

	
}
void CSceneCollisionMgr::ColGroupLoopInit()
{
	m_SrcIter = m_mapCollsion.begin();
	m_DestIter = m_mapCollsion.end();
}
size_t  CSceneCollisionMgr::ActComCount()
{
	size_t Count = 0;
	
	ColGroupLoopInit();

	for (;m_SrcIter != m_DestIter; m_SrcIter++)
	{
		Count += m_SrcIter->second.size();
	}

	return Count;
}
void CSceneCollisionMgr::ReleaseCom()
{
	ColGroupLoopInit();

	for (; m_SrcIter != m_DestIter; ++m_SrcIter)
	{
		list<SPTR<CCollision>>::iterator ComColStart = m_SrcIter->second.begin();
		list<SPTR<CCollision>>::iterator ComColEnd = m_SrcIter->second.end();

		for (; ComColStart != ComColEnd; )
		{
			if (true == (*ComColStart)->IsDeath())
			{
				ComColStart = m_SrcIter->second.erase(ComColStart);
				m_bCheck = true;
			}
			else
			{
				++ComColStart;
			}
		}
	}
}