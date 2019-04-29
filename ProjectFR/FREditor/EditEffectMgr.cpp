#include "stdafx.h"
#include "EditEffectMgr.h"
#include "EditEffect.h"

CEditEffectMgr::CEditEffectMgr()
	:m_PairScene(nullptr), m_NextEffectNumber(-1)
{
}


CEditEffectMgr::~CEditEffectMgr()
{
	m_vecEffect.clear();

	for (size_t i = 0; i < m_vecSpareEffect.size(); ++i)
	{
		m_vecSpareEffect[i].clear();
	}
}
void CEditEffectMgr::SettingScene(CGameScene* _Scene)
{
	if (nullptr == _Scene)
	{
		assert(nullptr);
		return;
	}

	m_PairScene = _Scene;
}
SPTR<CEditEffect> CEditEffectMgr::operator[](const size_t& _Index)
{
	if (m_vecEffect.size() <= _Index)
	{
		return nullptr;
	}

	return m_vecEffect[_Index];
}
SPTR<CEditEffect> CEditEffectMgr::operator[](const int& _Index)
{
	return operator[]((size_t)_Index);
}
SPTR<CEditEffect> CEditEffectMgr::operator[](const tstring& _EffectName)
{
	if (false == ExistEffect(_EffectName))
	{
		return nullptr;
	}

	return FindEffect(_EffectName);
}
int	 CEditEffectMgr::RandomEffectOn()
{
	size_t EffectLastIndex = m_vecEffect.size() - 1;

	int Random;

	do
	{
		Random = CSJRandom::RandomInt(0, (int)EffectLastIndex);
	} while (nullptr == EffectOn(Random));

	return Random;
}
SPTR<CEditEffect> CEditEffectMgr::PopEffect(const size_t& _Index)
{
	if (_Index >= m_vecEffect.size() || nullptr == m_vecEffect[_Index])
	{
		return nullptr;
	}

	if (false == m_vecEffect[_Index]->IsPlay())
	{
		return m_vecEffect[_Index];
	}

	return PopSpareEffect(_Index);
}
SPTR<CEditEffect> CEditEffectMgr::EffectOn(const size_t& _Index)
{
	if (_Index >= m_vecEffect.size())
	{
		return nullptr;
	}

	// 원본이 없다면 출력 실패
	if (nullptr == m_vecEffect[_Index])
		return nullptr;

	// 원본 or 복사본 List의 Effect 중 하나를 출력해준다.
	SPTR<CEditEffect> OnEffect = PopEffect(_Index);
	OnEffect->EffectOn();

	return OnEffect;

}
SPTR<CEditEffect> CEditEffectMgr::EffectOn(const int& _Index)
{
	return EffectOn((size_t)_Index);
}
SPTR<CEditEffect> CEditEffectMgr::EffectOn(const unsigned int& _Index)
{
	return EffectOn((size_t)_Index);
}
CEditEffect* CEditEffectMgr::CreateSpareEffect(const unsigned int& _Index)
{
	return CreateSpareEffect((size_t)_Index);
}
SPTR<CEditEffect> CEditEffectMgr::EffectOn(const tstring& _EffectName)
{
	size_t EffectIndex = CEditGlobal::GetEffectIndex(_EffectName);

	if (EffectIndex == -1)
	{
		return nullptr;
	}

	return EffectOn(EffectIndex);

}
BOOL CEditEffectMgr::ExistEffect(const tstring& _EffectName)
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
SPTR<CEditEffect> CEditEffectMgr::FindEffect(const size_t& _EffectIndex)
{
	if (m_vecEffect.size() <= _EffectIndex)
	{
		return nullptr;
	}

	return m_vecEffect[_EffectIndex];
}
SPTR<CEditEffect> CEditEffectMgr::FindEffect(const tstring& _EffectName)
{
	return FindEffect(CEditGlobal::GetEffectIndex(_EffectName));
}

CEditEffect* CEditEffectMgr::CreateEffect(const EFFECTDATA& _EffectData)
{
	if (nullptr == m_PairScene)
	{
		assert(nullptr);
		return nullptr;
	}

	if (nullptr == CEditGlobal::GetAniData(_EffectData.EffectAniName))
	{
		assert(nullptr);
		return nullptr;
	}

	CActObject* EffectObject = CActObject::CreateActObject(_EffectData.EffectName, m_PairScene);
	CEditEffect* NewEffect = EffectObject->CreateCom<CEditEffect>(_EffectData);


	if (nullptr == NewEffect)
	{
		assert(nullptr);
		return nullptr;
	}

	NewEffect->SetOriginal();

	// Edit에서는 EffectData를 생성하므로 
	// 현재 vecEffect보다 큰 EffectIndex가 들어온 경우
	// Vector를 확장한다.
	if (m_vecEffect.size() <= NewEffect->GetEffectIndex())
	{
		// 1. 정밀 확장
		//int ExpCount = NewEffect->GetEffectIndex() + 1 - m_vecEffect.size();

		//if (ExpCount > 0)
			//m_vecEffect.insert(m_vecEffect.end(), ExpCount, nullptr);
		int AccExpCount = 1;

		// 2. 2배 확장
		do {
			AccExpCount *= 2;
			m_vecEffect.insert(m_vecEffect.end(),m_vecEffect.size(), nullptr);

		} while (m_vecEffect.size() <= NewEffect->GetEffectIndex());
	
	}

	map<tstring, size_t>::iterator Find = CEditGlobal::EffectIndexMap.find(NewEffect->GetEffectName());

	// 같은 이름으로 이펙트가 들어오려하면 생성을 막는다.
	if (Find == CEditGlobal::EffectIndexMap.end()) 
	{
		CEditGlobal::EffectIndexMap.insert(map<tstring, size_t>::value_type(NewEffect->GetEffectName(), NewEffect->GetEffectIndex()));
		m_vecEffect[NewEffect->GetEffectIndex()] = NewEffect;
		NewEffect->EffectOff();
		return NewEffect;
	}

	return nullptr;
}
CEditEffect* CEditEffectMgr::ChangeEffect(const EFFECTDATA& _EffectData)
{
	if (nullptr == m_PairScene)
	{
		assert(nullptr);
		return nullptr;
	}

	if (nullptr == CEditGlobal::GetAniData(_EffectData.EffectAniName))
	{
		assert(nullptr);
		return nullptr;
	}

	CActObject* EffectObject = CActObject::CreateActObject(_EffectData.EffectName, m_PairScene);
	CEditEffect* NewEffect = EffectObject->CreateCom<CEditEffect>(_EffectData);

	NewEffect->SetOriginal();

	if (NewEffect->GetEffectIndex() < m_vecEffect.size())
		m_vecEffect[NewEffect->GetEffectIndex()] = NewEffect;

	return NewEffect;
}
CEditEffect* CEditEffectMgr::CreateSpareEffect(const size_t& _Index)
{
	if (nullptr == m_PairScene)
	{
		assert(nullptr);
		return nullptr;
	}

	EFFECTDATA CopyData(CEditGlobal::EffectData[_Index]);


	if (nullptr == CEditGlobal::GetAniData(CopyData.EffectAniName))
	{
		assert(nullptr);
		return nullptr;
	}

	CActObject* EffectObject = CActObject::CreateActObject(CopyData.EffectName, m_PairScene);
	CEditEffect* NewSpareEffect = EffectObject->CreateCom<CEditEffect>(CopyData);

	if (nullptr == NewSpareEffect)
	{
		assert(nullptr);
		return nullptr;
	}

	m_vecSpareEffect[CopyData.EffectIndex].push_back(NewSpareEffect);
	NewSpareEffect->EffectOn();
	return NewSpareEffect;
}
void CEditEffectMgr::EffectAllOff()
{
	for (size_t i = 0; i < m_vecEffect.size(); i++)
	{
		if (nullptr == m_vecEffect[i])
			continue;

		m_vecEffect[i]->EffectOff();
	}

	return;
}
BOOL CEditEffectMgr::LoadEffectData()
{
	int Count = 0;

	m_vecEffect.reserve(CEditGlobal::EffectData.size());
	m_vecEffect.assign(CEditGlobal::EffectData.size(), nullptr);

	for (size_t i = 0; i < CEditGlobal::EffectData.size(); i++)
	{
		list<SPTR<CEditEffect>> SpareList = list<SPTR<CEditEffect>>();
		m_vecSpareEffect.push_back(SpareList);
	}

	m_vecSpareEffect.reserve(CEditGlobal::EffectData.size());


	for (size_t i = 0; i < CEditGlobal::EffectData.size(); i++)
	{
		CEditEffect* LoadEffect = CreateEffect(CEditGlobal::EffectData[i]);

		if (nullptr != LoadEffect)
		{
			CEditGlobal::EffectIndexMap.insert(map<tstring, size_t>::value_type(LoadEffect->GetEffectName(), LoadEffect->GetEffectIndex()));
			++Count;
		}
	}

	if (Count != CEditGlobal::EffectData.size() || 0 == Count)
	{
		return FALSE;
	}

	return TRUE;
}
bool CEditEffectMgr::isEffectOn(const size_t& _Index)
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

SPTR<CEditEffect> CEditEffectMgr::PopSpareEffect(const size_t _Index)
{
	// 처음 복사본을 생성할 경우
	if (true == m_vecSpareEffect[_Index].empty())
	{
		return CreateSpareEffect(_Index);
	}

	list<SPTR<CEditEffect>>::iterator Start = m_vecSpareEffect[_Index].begin();
	list<SPTR<CEditEffect>>::iterator End = m_vecSpareEffect[_Index].end();
	// 1. 리스트 안의 재생중이 아닌 Effect중 하나를 Pop 
	for (; Start != End; ++Start)
	{
		if (nullptr == (*Start))
			continue;

		if (false == (*Start)->IsPlay())
		{
			return (*Start);
		}
	}

	// 2. 리스트안의 모든 Effect가 재생중일 땐 사본을 만들어서 
	//	  리스트에 추가한후 리턴 한다.
	return CreateSpareEffect(_Index);

}

BOOL CEditEffectMgr::EmptyEffectMap()
{
	if (0 >= m_vecEffect.size())
	{
		return TRUE;
	}
	return FALSE;
}

vector<SPTR<CEditEffect>>*	CEditEffectMgr::GetEffectVector()
{
	return &m_vecEffect;
}

size_t CEditEffectMgr::SearchNextEffectIndex()
{
	for (size_t i = 0; i < m_vecEffect.size(); i++)
	{
		if (nullptr == m_vecEffect[i])
		{
			m_NextEffectNumber = i;
			return m_NextEffectNumber;
		}
	}
	return m_vecEffect.size();
}

BOOL CEditEffectMgr::DeleteEffect(const tstring& _EffectName)
{
	for (size_t i = 0; i < m_vecEffect.size(); i++)
	{
		if (nullptr != m_vecEffect[i] && m_vecEffect[i]->GetEffectName() == _EffectName)
		{
			map<tstring, size_t>::iterator Find = CEditGlobal::EffectIndexMap.find(_EffectName);
			if (Find != CEditGlobal::EffectIndexMap.end())
				CEditGlobal::EffectIndexMap.erase(Find);
			m_vecEffect[i] = nullptr;
			SearchNextEffectIndex();

			return TRUE;
		}
	}

	return FALSE;
}


BOOL CEditEffectMgr::SaveEffectData(FILE* _WriteFile)
{
	// 확인용 변수
	size_t Count = 0;

	CEditGlobal::EffectData.clear();
	CEditGlobal::EffectData.assign(m_vecEffect.size(), EFFECTDATA());
	CEditGlobal::EffectIndexMap.clear();


	for (size_t i = 0; i < m_vecEffect.size(); i++)
	{
		if (nullptr == m_vecEffect[i])
		{
			++Count;
		}
		else
		{
			EFFECTDATA SaveData = m_vecEffect[i]->GetEffectData();
			SaveData.SaveDataToFile(_WriteFile);
			CEditGlobal::EffectData[SaveData.EffectIndex] = SaveData;
			CEditGlobal::EffectIndexMap.insert(map<tstring, size_t>::value_type(SaveData.EffectName, SaveData.EffectIndex));
			++Count;
		}
	}

	if (Count == m_vecEffect.size())
	{
		return TRUE;
	}

	return FALSE;
}
void CEditEffectMgr::SettingCurEffectLayer(const bool& _bChar)
{
	CEditEffect::SettingCurEffectLayer(_bChar);
}
BOOL CEditEffectMgr::LoadEffectData(FILE* _ReadFile, vector<EFFECTDATA>* _TempVec)
{
	int Count = 0;

	if (nullptr == _ReadFile || nullptr == _TempVec)
	{
		return FALSE;
	}

	m_vecEffect.clear();
	CEditGlobal::EffectData.clear();
	CEditGlobal::EffectIndexMap.clear();
	
	UINT MaxIndex = 0;

	for (size_t i = 0; i < _TempVec->size(); i++)
	{
		MaxIndex = max(MaxIndex, (*_TempVec)[i].EffectIndex);
	}
	

	m_vecEffect.assign(MaxIndex + 1, nullptr);

	for (size_t i = 0; i < _TempVec->size(); i++)
	{
		CEditEffect* NewEffect = CreateEffect((*_TempVec)[i]);

		if (nullptr != NewEffect)
		{
			map<tstring, size_t>::iterator Find = CEditGlobal::EffectIndexMap.find(NewEffect->GetEffectName());
			++Count;
			continue;
		}
		
		int a = 0;
	}
	
	CEditGlobal::EffectData.clear();
	CEditGlobal::EffectData.insert(CEditGlobal::EffectData.end(),_TempVec->begin(), _TempVec->end());

	for (size_t i = 0; i < CEditGlobal::EffectData.size(); i++)
	{
		list<SPTR<CEditEffect>> SpareList = list<SPTR<CEditEffect>>();
		m_vecSpareEffect.push_back(SpareList);
	}

	if (Count != _TempVec->size() || 0 == Count)
	{
		return FALSE;
	}

	return TRUE;
}
//
//CEditEffectMgr::CEditEffectMgr()
//	:m_PairScene(nullptr), m_NextEffectNumber(-1)
//{
//}
//
//
//CEditEffectMgr::~CEditEffectMgr()
//{
//}
//void CEditEffectMgr::SettingScene(CGameScene* _Scene)
//{
//	if (nullptr == _Scene) 
//	{
//		assert(nullptr);
//		return;
//	}
//
//	m_PairScene = _Scene;
//}
//BOOL CEditEffectMgr::EmptyEffectMap()
//{
//	/*if (0 >= m_mapEffect.size()) 
//	{
//		return TRUE;
//	}*/
//	if (0 >= m_vecEffect.size())
//	{
//		return TRUE;
//	}
//	return FALSE;
//}
//BOOL CEditEffectMgr::ExistEffect(const tstring& _EffectName)
//{
//	for (size_t i = 0; i < m_vecEffect.size(); i++)
//	{
//		if (m_vecEffect[i] != nullptr && _EffectName == m_vecEffect[i]->GetEffectName()) 
//		{
//			return TRUE;
//		}
//	}
//	return FALSE;
//}
//SPTR<CEditEffect> CEditEffectMgr::FindEffect(const tstring& _EffectName)
//{
//	for (size_t i = 0; i < m_vecEffect.size(); i++)
//	{
//		if (m_vecEffect[i] != nullptr && _EffectName == m_vecEffect[i]->GetEffectName())
//		{
//			return m_vecEffect[i];
//		}
//	}
//	return nullptr;
//}
//
//CEditEffect* CEditEffectMgr::CreateEffect(const EFFECTDATA& _EffectData)
//{
//	if (nullptr == m_PairScene) 
//	{
//		assert(nullptr);
//		return nullptr;
//	}
//
//	if (nullptr == CEditGlobal::GetAniData(_EffectData.EffectAniName)) 
//	{
//		return nullptr;
//	}
//
//	CActObject* EffectObject =CActObject::CreateActObject(_EffectData.EffectName, m_PairScene);
//	CEditEffect* NewEffect = EffectObject->CreateCom<CEditEffect>(_EffectData);
//
//	if (nullptr == NewEffect)
//	{
//		assert(nullptr);
//		return nullptr;
//	}
//
//	NewEffect->SetOriginal();
//
//	//size_t PushIndex = SearchNextEffectIndex();
//	size_t PushIndex = _EffectData.EffectIndex;
//
//	if (PushIndex == m_vecEffect.size() && NewEffect->GetEffectIndex() == PushIndex) 
//	{
//		m_vecEffect.push_back(NewEffect);
//	}
//	else if (PushIndex < m_vecEffect.size()) 
//	{
//		m_vecEffect[PushIndex] = NewEffect;
//		if (PushIndex != NewEffect->GetEffectIndex()) 
//		{
//			NewEffect->SetEffectIndex(PushIndex);
//		}
//	}
//	
//	return NewEffect;
//}
//
//vector<SPTR<CEditEffect>>*	CEditEffectMgr::GetEffectVector()
//{
//	return &m_vecEffect;
//}
//void CEditEffectMgr::EffectAllOff()
//{
//	for (size_t i = 0; i < m_vecEffect.size(); i++)
//	{
//		if (nullptr == m_vecEffect[i])
//			continue;
//
//		m_vecEffect[i]->EffectOff();
//	}
//	return;
//}

//size_t CEditEffectMgr::SearchNextEffectIndex()
//{
//	for (size_t i = 0; i < m_vecEffect.size(); i++)
//	{
//		if (nullptr == m_vecEffect[i])
//		{
//			m_NextEffectNumber = i;
//			return m_NextEffectNumber;
//		}
//	}
//	return m_vecEffect.size();
//}