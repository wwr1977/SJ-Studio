#pragma once
#include "ObjBase.h"


class CGameScene;
class CCollision;
class CSceneCollisionMgr : public CObjBase
{
public:
	struct ColCheck 
	{
		tstring Src;
		tstring Dest;

		ColCheck(const tstring& _Src, const tstring& _Dest) :Src(_Src), Dest(_Dest) {}
	};

private:
	bool															m_bCheck;
	CGameScene*														m_PairScene;
	vector<ColCheck>												m_vecColCheck;
	unordered_map<tstring, list<SPTR<CCollision>>>::iterator		m_SrcIter;
	unordered_map<tstring, list<SPTR<CCollision>>>::iterator		m_DestIter;
	unordered_map<tstring, list<SPTR<CCollision>>>					m_mapCollsion;

public:
	bool IsCheck() { return m_bCheck; }

public:
	void CreateCheckGroup(const tstring& _Src, const tstring& _Dest) 
	{
		m_vecColCheck.push_back({ _Src,_Dest });
	}
	void PushColList(tstring _Name, CCollision* _Col);

private:
	void ThisCollision();
	void OtherCollision();

public:
	void Collision();
	size_t  ActComCount();
	void ColGroupLoopInit();
	void ReleaseCom();

public:
	CSceneCollisionMgr(CGameScene* _PairScene);
	~CSceneCollisionMgr();
};

