#pragma once
#include "GameCom.h"

class CSceneCollisionMgr;
class CCollision : public CGameCom
{
private:
	CSceneCollisionMgr*					m_SceneColMgr;

	// 충돌체 크기가 명시적 지정 or ActObjectSize로 자동화 
	COLMODE								m_eColMode;
	// 충돌체 모양이 Rect,Circle,Point 파악
	COLTYPE								m_eColType;
	// 충돌체의 위치와 크기 정보를 갖는다.
	COLFIGURE							m_Fi;
	tstring								m_GroupName;
	Vec3								m_PivotPos;
	Vec3								m_ColCustomSize;

	bool								m_bDebugRen;
	
	function<void(SPTR<CCollision>, SPTR<CCollision>)>		m_EnterFunc;
	function<void(SPTR<CCollision>, SPTR<CCollision>)>		m_StayFunc;
	function<void(SPTR<CCollision>, SPTR<CCollision>)>		m_ExitFunc;

	set<SPTR<CCollision>>::iterator					m_FindIter;
	set<SPTR<CCollision>>::iterator					m_FindEndIter;
	set<SPTR<CCollision>>							m_ColSet;

public:
	void CollisionSynchro();
	void SetColSize(const Vec3& _Size );
	void SetColSize(const Vec2& _Size );
	void SetColType(const COLTYPE& _Type);
	void SetColSize();
	void SetPivotPos(const Vec3& _Pivot);
	void SetPivotPos(const Vec2& _Pivot);
	
	COLTYPE GetColType() 
	{
		return m_eColType;
	}

public:
	template<typename T>
	void SetEnter(T* _Obj, void(T::*pFunc)(SPTR<CCollision>, SPTR<CCollision>)) 
	{
		m_EnterFunc = bind(pFunc, _Obj, placeholders::_1, placeholders::_2);
	}

	template<typename T>
	void SetStay(T* _Obj, void(T::*pFunc)(SPTR<CCollision>, SPTR<CCollision>))
	{
		m_StayFunc = bind(pFunc, _Obj, placeholders::_1, placeholders::_2);
	}

	template<typename T>
	void SetExit(T* _Obj, void(T::*pFunc)(SPTR<CCollision>, SPTR<CCollision>))
	{
		m_ExitFunc = bind(pFunc, _Obj, placeholders::_1, placeholders::_2);
	}
	
public:
	void CallEnter(SPTR<CCollision> _This, SPTR<CCollision> _Other);
	void CallStay(SPTR<CCollision> _This, SPTR<CCollision> _Other);
	void CallExit(SPTR<CCollision> _This, SPTR<CCollision> _Other);

public:
	bool FICheck(SPTR<CCollision> _Other);
	bool FICheck(SPTR<CCollision> _Other, const Vec3& _Pivot);
	void ColCheck(SPTR<CCollision> _Other);
	void ReleaseSet();
	void ClearSet();
	void EraseSet(SPTR<CCollision> _Other);
	const bool IsEmptyColSet();
	const Vec3 GetActorPos();
public:
	void Init();
	void DebugRender();
	
	virtual void InitData(const tstring& _GroupName);
	virtual void InitData(COLDATA _Data);

public:
	const tstring GetActorName();
	void ColDebugRenActive(const bool& _Debug);

public:
	CCollision();
	CCollision(CActObject* _ActObject);
	~CCollision();
};

