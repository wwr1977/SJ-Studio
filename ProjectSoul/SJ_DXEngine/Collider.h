#pragma once
#include"DXEnum.h"
#include"DXHeader.h"
#include"SubTrans.h"
#include<unordered_set>
#include<functional>

struct COLDATA
{
	std::wstring			CollisionChannel;
	SCENECOLLISION			eSceneCollision;
};

struct BASE_FI 
{
public:
	COLTYPE						m_eColType;
public:
	BASE_FI() :m_eColType(MAXCOLTYPE){}
	BASE_FI(const COLTYPE& _Type) :m_eColType(_Type) {}
	virtual ~BASE_FI() = 0 {}
};

class CCollider : public CSubTrans
{
	friend class Engine;
	friend class CActor;
	friend class CSceneCollisionMgr;
	friend class CCollisionChannel;
	friend class CSJ_DXCore;

protected:
	static SPTR<class CVtxShader>			ColliderVtxShader;
	static SPTR<class CPixShader>			ColliderPixShader;
	static SPTR<class CMesh>				ColliderMesh;

public:
	static void ColliderInit();

private:
	std::unordered_set<CCollider*>						m_setCollider;
	std::unordered_set<CCollider*>::iterator			m_setFindIter;
	std::unordered_set<CCollider*>::iterator			m_setEndIter;

private:
	std::function<void(SPTR<CCollider>, SPTR<CCollider>)>			m_EnterCallBack;
	std::function<void(SPTR<CCollider>, SPTR<CCollider>)>			m_StayCallBack;
	std::function<void(SPTR<CCollider>, SPTR<CCollider>)>			m_ExitCallBack;

public:
	template<typename T>
	void SetEnterCallBack(T* _Obj,void(T::*_pFunc)(SPTR<CCollider>, SPTR<CCollider>))
	{
		if (nullptr == _Obj)
			return;

		m_EnterCallBack = std::bind(_pFunc, _Obj, std::placeholders::_1, std::placeholders::_2);
	}

	template<typename T>
	void SetStayCallBack(T* _Obj, void(T::*_pFunc)(SPTR<CCollider>, SPTR<CCollider>))
	{
		if (nullptr == _Obj)
			return;

		m_StayCallBack = std::bind(_pFunc, _Obj, std::placeholders::_1, std::placeholders::_2);
	}

	template<typename T>
	void SetExitCallBack(T* _Obj, void(T::*_pFunc)(SPTR<CCollider>, SPTR<CCollider>))
	{
		if (nullptr == _Obj)
			return;

		m_ExitCallBack = std::bind(_pFunc, _Obj, std::placeholders::_1, std::placeholders::_2);
	}

private:
	void CallEnter(SPTR<CCollider> _This, SPTR<CCollider> _Other);
	void CallStay(SPTR<CCollider> _This, SPTR<CCollider> _Other);
	void CallExit(SPTR<CCollider> _This, SPTR<CCollider> _Other);

protected:
	std::wstring								m_CollisionChannel;
	BASE_FI*									m_pFi;
	bool										m_bCollision;

public:
	std::wstring GetChannel() { return m_CollisionChannel; }

private:
	const bool Init(const COLDATA& _InitData);
	virtual void DebugRender() {}

private:
	void ReleaseSet();
	void EraseSet(SPTR<CCollider> _Other);
	void DumpDeathCollider();


private:
	void Collision(SPTR<CCollider> _Other);
	

public:
	const bool CollisionCheck(SPTR<CCollider> _Other);

private:
	const bool PointCollision(BASE_FI* _OtherFi);
	const bool RectCollision(BASE_FI* _OtherFi);
	const bool CircleCollision(BASE_FI* _OtherFi);

private :
	virtual void CollisionSyncro() {}
	
public:
	virtual const Vec3 GetColliderPos() { return Vec3(); }
	virtual const Vec3 GetColliderSize() { return Vec3(); }
public:
	const COLTYPE GetColType();



public:
	CCollider();
	~CCollider();
};

