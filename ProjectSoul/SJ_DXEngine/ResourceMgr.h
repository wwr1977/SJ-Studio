#pragma once
#include<string>
#include<unordered_map>
#include"DXTemplate.h"
#include"ReferenceBase.h"
#include"Engine.h"


template<typename Res , typename Key = std::wstring>
class CResourceMgr
{
	friend class CSJ_DXCore;
	class CResMgrDestroyer 
	{
		friend class CResourceMgr<Res, Key>;


		CResMgrDestroyer() {}
		~CResMgrDestroyer()
		{
			CResourceMgr<Res, Key>::Release();
		}
	};

private:
	static CResMgrDestroyer													ResMgrDest;
	static std::unordered_map<Key, SPTR<Res>>								ResourceMap;


private:
	static void Release()
	{
		if(false == ResourceMap.empty())
			ResourceMap.clear();
	} 
public:
	static SPTR<Res> Find(const Key& _Key) 
	{
		return UMAPFINDSPTR<SPTR<Res>>(ResourceMap, _Key);
	}

	static const bool Erase(const Key& _Key) 
	{
		typename std::unordered_map< Key, SPTR<Res>>::iterator Del = ResourceMap.find(_Key);
		
		if (ResourceMap.end() != Del) 
		{
			ResourceMap.erase(Del);
			return true;
		}

		return false;
	}


	static SPTR<Res> Create(const Key& _Key, const typename  Res::CREATE_DESC& _Desc = Res::CREATE_DESC(),const bool& _Overwrite = false)
	{
		if (false == _Overwrite)
		{
			SPTR<Res> Find = CResourceMgr<Res, Key>::Find(_Key);
			if (nullptr != Find)
				return Find;
		}

		SPTR<Res> NewRes = Engine::CreateResourceObject<Res>();
		
		if(&typeid(Key) == &typeid(std::wstring))
			NewRes->SetPath(_Key);


		if (false == NewRes->Create(_Desc))
			return nullptr;
		

		auto Succ = ResourceMap.insert(std::unordered_map<Key, SPTR<Res>>::value_type(_Key, NewRes));
		
		if (false == Succ.second)
			Succ.first->second = NewRes;
		
		return NewRes;
	}

	// ��ο� �ε� �����͸� �־��ָ� �ڵ�����
	// �����̸�(Ȯ���� ����)���� �ʿ� ����
	static SPTR<Res> Load(const std::wstring& _Path, const typename Res::LOAD_DESC& _Desc = Res::LOAD_DESC(), const bool& _Overwrite = false)
	{
		if (false == _Overwrite) 
		{
			SPTR<Res> Find = CResourceMgr<Res, Key>::Find(CFilePathMgr::PathToFileName(_Path));

			if (nullptr != Find)
				return Find;
		}

		SPTR<Res> NewRes = Engine::CreateResourceObject<Res>();
		NewRes->SetPath(_Path);

		if (false == NewRes->Load(_Desc))
			return nullptr;

		auto Succ = ResourceMap.insert(std::unordered_map<Key, SPTR<Res>>::value_type(NewRes->GetFileName(), NewRes));

		if (false == Succ.second)
			Succ.first->second = NewRes;

		return NewRes;
	}
	// Ű�� �����ϴ� �ε� �Լ�
	static SPTR<Res> Load(const Key& _Key, const std::wstring& _Path, const typename Res::LOAD_DESC& _Desc = Res::LOAD_DESC(), const bool& _Overwrite = false)
	{
		if (false == _Overwrite)
		{
			SPTR<Res> Find = CResourceMgr<Res, Key>::Find(_Key);
			if (nullptr != Find)
				return Find;
		}

		SPTR<Res> NewRes = Engine::CreateResourceObject<Res>();

		NewRes->SetPath(_Path);

		if (false == NewRes->Load(_Desc))
			return nullptr;

		auto Succ = ResourceMap.insert(std::unordered_map<Key, SPTR<Res>>::value_type(_Key, NewRes));

		if (false == Succ.second)
			Succ.first->second = NewRes;

		return NewRes;
	}


	// �ε� ������ �ȿ� ���� ��θ� ��������鼭
	// �ڵ����� �����̸��� Ű�� ����Ѵ�.
	//static SPTR<Res> Load(const typename Res::LOAD_DESC& _Desc = Res::LOAD_DESC(), const bool& _Overwrite = false)
	//{
	//	if (false == _Overwrite)
	//	{
	//		// Dest �ȿ� ������ ��ΰ� ���� ���(�̶� DESC ����ü ���� ��θ� �����ϴ� ������ �ݵ�� std::wstring FilePath�̿��� �Ѵ�.)
	//		SPTR<Res> Find = CResourceMgr<Res, Key>::Find(CFilePathMgr::PathToFileName(_Desc.FilePath));
	//		if (nullptr != Find)
	//			return Find;
	//	}

	//	SPTR<Res> NewRes = Engine::CreateResourceObject<Res>();

	//	if (false == NewRes->Load(_Desc))
	//		return nullptr;
	//	

	//	auto Succ = ResourceMap.insert(std::unordered_map<Key, SPTR<Res>>::value_type(NewRes->GetFileName(), NewRes));
	//	
	//	if (false == Succ.second)
	//		Succ.first->second = NewRes;
	//	
	//	return NewRes;
	//}
	

private:
	CResourceMgr() {}
	~CResourceMgr() {}
};

template<typename Res, typename Key = std::wstring>
typename CResourceMgr<Res, Key>::CResMgrDestroyer				ResMgrDest;

template<typename Res, typename Key = std::wstring>
std::unordered_map<Key, SPTR<Res>>		CResourceMgr<Res, Key>::ResourceMap;


