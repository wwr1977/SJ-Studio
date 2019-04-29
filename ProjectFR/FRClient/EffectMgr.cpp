#include "Precom.h"
#include "EffectMgr.h"
#include "Effect.h"

CEffectMgr::CEffectMgr()
{
}


CEffectMgr::~CEffectMgr()
{
	m_vecEffect.clear();

	for (size_t i = 0; i < m_vecSpareEffect.size(); ++i)
	{
		m_vecSpareEffect[i].clear();
	}
}
void CEffectMgr::SettingScene(CGameScene* _Scene)
{
	if (nullptr == _Scene)
	{
		assert(nullptr);
		return;
	}

	m_PairScene = _Scene;
}
void CEffectMgr::SettingCurEffectLayer(const bool& _bChar)
{
	CEffect::SettingCurEffectLayer(_bChar);
}
SPTR<CEffect> CEffectMgr::operator[](const size_t& _Index)
{
	if(m_vecEffect.size() <= _Index)
	{
		return nullptr;
	}

	return m_vecEffect[_Index];
}
SPTR<CEffect> CEffectMgr::operator[](const int& _Index)
{
	return operator[]((size_t)_Index);
}
SPTR<CEffect> CEffectMgr::operator[](const tstring& _EffectName)
{
	if (false == ExistEffect(_EffectName)) 
	{
		return nullptr;
	}

	return FindEffect(_EffectName);
}
int	 CEffectMgr::RandomEffectOn()
{
	size_t EffectLastIndex = m_vecEffect.size() - 1;

	int Random;
	
	do 
	{
		Random	= CSJRandom::RandomInt(0, (int)EffectLastIndex);
	} while ( nullptr == EffectOn(Random) );

	return Random;
}
SPTR<CEffect> CEffectMgr::PopEffect(const size_t& _Index)
{
	if (_Index >= m_vecEffect.size() ||nullptr == m_vecEffect[_Index])
	{
		return nullptr;
	}

	if (false == m_vecEffect[_Index]->IsPlay())
	{
		return m_vecEffect[_Index];
	}

	return PopSpareEffect(_Index);
}
SPTR<CEffect> CEffectMgr::EffectOn(const size_t& _Index)
{
	if (_Index >= m_vecEffect.size()) 
	{
		return nullptr;
	}

	// 원본이 없다면 출력 실패
	if (nullptr == m_vecEffect[_Index])
		return nullptr;
	
	// 원본 or 복사본 List의 Effect 중 하나를 출력해준다.
	SPTR<CEffect> OnEffect =  PopEffect(_Index);
	OnEffect->EffectOn();

	return OnEffect;

	//m_vecEffect[_Index]->EffectOn();
	//return true;
}
SPTR<CEffect> CEffectMgr::EffectOn(const int& _Index)
{
	return EffectOn((size_t)_Index);
}
SPTR<CEffect> CEffectMgr::EffectOn(const unsigned int& _Index)
{
	return EffectOn((size_t)_Index);
}
CEffect* CEffectMgr::CreateSpareEffect(const unsigned int& _Index)
{
	return CreateSpareEffect((size_t)_Index);
}
SPTR<CEffect> CEffectMgr::EffectOn(const tstring& _EffectName)
{
	size_t EffectIndex = CClientGlobal::GetEffectIndex(_EffectName);

	if (EffectIndex == -1) 
	{
		return nullptr;
	}

	return EffectOn(EffectIndex);
	
	//SPTR<CEffect> Effect = FindEffect(_EffectName);

	//if (nullptr == Effect) 
	//{
	//	return false;
	//}

	//Effect->EffectOn();
	//return true;
}
BOOL CEffectMgr::ExistEffect(const tstring& _EffectName)
{
	for (size_t i = 0; i < m_vecEffect.size(); i++)
	{
		if (_EffectName == m_vecEffect[i]->GetEffectName()) 
		{
			return TRUE;
		}
	}
	return FALSE;
}
SPTR<CEffect> CEffectMgr::FindEffect(const size_t& _EffectIndex)
{
	if (m_vecEffect.size() <= _EffectIndex) 
	{
		return nullptr;
	}

	return m_vecEffect[_EffectIndex];
}
SPTR<CEffect> CEffectMgr::FindEffect(const tstring& _EffectName)
{
	return FindEffect(CClientGlobal::GetEffectIndex(_EffectName));
}

CEffect* CEffectMgr::CreateEffect(const EFFECTDATA& _EffectData)
{
	if (nullptr == m_PairScene)
	{
		assert(nullptr);
		return nullptr;
	}

	if (nullptr == CClientGlobal::GetAniData(_EffectData.EffectAniName))
	{
		assert(nullptr);
		return nullptr;
	}

	CActObject* EffectObject = CActObject::CreateActObject(_EffectData.EffectName, m_PairScene);
	CEffect* NewEffect = EffectObject->CreateCom<CEffect>(_EffectData);

	if (nullptr == NewEffect)
	{
		assert(nullptr);
		return nullptr;
	}

	NewEffect->SetOriginal();

	if (m_vecEffect.size() <= NewEffect->GetEffectIndex()) 
	{
		assert(nullptr);
		return nullptr;
	}

	m_vecEffect[NewEffect->GetEffectIndex()] = NewEffect;

	return NewEffect;
}
CEffect* CEffectMgr::CreateSpareEffect(const size_t& _Index)
{
	if (nullptr == m_PairScene)
	{
		assert(nullptr);
		return nullptr;
	}

	EFFECTDATA CopyData(CClientGlobal::EffectData[_Index]);


	if (nullptr == CClientGlobal::GetAniData(CopyData.EffectAniName))
	{
		assert(nullptr);
		return nullptr;
	}

	CActObject* EffectObject = CActObject::CreateActObject(CopyData.EffectName, m_PairScene);
	CEffect* NewSpareEffect = EffectObject->CreateCom<CEffect>(CopyData);

	if (nullptr == NewSpareEffect)
	{
		assert(nullptr);
		return nullptr;
	}

	m_vecSpareEffect[CopyData.EffectIndex].push_back(NewSpareEffect);
	NewSpareEffect->EffectOn();
	return NewSpareEffect;
}
void CEffectMgr::EffectAllOff()
{
	for (size_t i = 0; i < m_vecEffect.size(); i++)
	{
		if (nullptr == m_vecEffect[i]) 
			continue;
		
		m_vecEffect[i]->EffectOff();
	}

	return;
}
BOOL CEffectMgr::LoadEffectData()
{
	int Count = 0;

	m_vecEffect.reserve(CClientGlobal::EffectData.size());
	m_vecEffect.assign(CClientGlobal::EffectData.size(), nullptr);

	for (size_t i = 0; i < CClientGlobal::EffectData.size(); i++)
	{
		list<SPTR<CEffect>> SpareList = list<SPTR<CEffect>>();
		m_vecSpareEffect.push_back(SpareList);
	}

	m_vecSpareEffect.reserve(CClientGlobal::EffectData.size());
	

	for (size_t i = 0; i < CClientGlobal::EffectData.size(); i++)
	{
		CEffect* LoadEffect = CreateEffect(CClientGlobal::EffectData[i]);

		if (nullptr != LoadEffect)
		{
			CClientGlobal::EffectIndexMap.insert(map<tstring, size_t>::value_type(LoadEffect->GetEffectName(), LoadEffect->GetEffectIndex()));
			++Count;
		}
	}

	if (Count != CClientGlobal::EffectData.size() || 0 == Count)
	{
		return FALSE;
	}

	return TRUE;
}
bool CEffectMgr::isEffectOn(const size_t& _Index)
{
	if (_Index >= m_vecEffect.size()) 
	{
		return false;
	}
	
	if (nullptr == m_vecEffect[_Index]) 
	{
		return false;
	}

	return m_vecEffect[_Index]->IsPlay();
}

SPTR<CEffect> CEffectMgr::PopSpareEffect(const size_t _Index)
{
	// 처음 복사본을 생성할 경우
	if (true == m_vecSpareEffect[_Index].empty()) 
	{
		return CreateSpareEffect(_Index);
	}

	list<SPTR<CEffect>>::iterator Start = m_vecSpareEffect[_Index].begin();
	list<SPTR<CEffect>>::iterator End = m_vecSpareEffect[_Index].end();
	// 1. 리스트 안의 재생중이 아닌 Effect중 하나를 Pop 
	for (;Start!= End ; ++Start)
	{
		if (nullptr == (*Start))
			continue;

		if (false == (*Start)->IsPlay()) 
		{
			return (*Start);
		}
	}

	// 2. 리스트안의 모든 Effect가 재생줄일 땐 사본을 만들어서 
	//	  리스트에 추가한후 리턴 한다.
	return CreateSpareEffect(_Index);

}

void CEffectMgr::DebugButtonEvent()
{
	for (size_t i = 0; i < m_vecEffect.size(); i++)
	{
		if (nullptr != m_vecEffect[i])
			m_vecEffect[i]->ReverseLineRendering();

		
		list<SPTR<CEffect>>::iterator Start = m_vecSpareEffect[i].begin();
		list<SPTR<CEffect>>::iterator End = m_vecSpareEffect[i].end();
	
		for (; Start!= End; ++Start)
			(*Start)->ReverseLineRendering();
	}

	
}