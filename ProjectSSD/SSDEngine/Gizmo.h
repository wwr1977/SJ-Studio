#pragma once
#include "Renderer.h"
#include "DXStruct.h"
#include "OBBCollider.h"

#define GIZMOSIZE 24.f
#define GIZMOHEIGHT 200.f


enum GIZMO_PICKTYPE 
{
	PICK_X,
	PICK_Y,
	PICK_Z,
	PICK_NONE,
	MAX_GIZMO_PICKTYPE
};

enum GIZMO_TRANSMODE
{
	GIZMO_TRANS,
	GIZMO_ROTATE,
	MAX_GIZMO_TRANSMODE
};

class CCamera;
class CCollider;
class CGizmo : public CRenderer
{
private:
	CBUFFER::GizmoBuffer											m_GizmoBuff;
	SPTR<COBBCollider>												m_GizmoCol[3];

	GIZMO_PICKTYPE													m_eCurPickGizmo;
	std::set<GIZMO_PICKTYPE>										m_setPick;
	
	GIZMO_TRANSMODE													m_eTransMode;
	AXIS_TYPE														m_HoldingAxis;
	Vec3															m_HoldGizmoDir;
	bool															m_bGizmoHolding;

private:
	// �� �Լ��� ���ε� ���� �ʾ��� ��� ������� ���ϻ���(��ġ,ȸ��)�� �����ϰԵǸ� Actor�� Pos�� ����
	// �Լ��� ���ε� �� ��� �ش� �ݹ��Լ��� ȣ��
	std::function<void(const Vec3&)>								m_TransCallBack;

public:
	template<typename T>
	void SetTransCallBack(T* _Obj, void(T::*_pFunc)(const Vec3& _Dir))
	{
		if (nullptr == _Obj)
			return;

		m_TransCallBack = std::bind(_pFunc, _Obj, std::placeholders::_1);
	}

public:
	void SetTransMode();
	void SetRotateMode();
	void ChangeGizmoMode();


public:
	const bool Init(const RENDATA& _InitData);
	void PreUpdate(const float& _DeltaTime) override;
	void Update(const float& _DeltaTime)override;

public:
	void PositionGizmoUpdate();
	void RotationGizmoUpdate();

public:
	void RenderUpdate(SPTR<CCamera> _Camera);


public:
	const GIZMO_PICKTYPE GetGizmoIdx(SPTR<CCollider> _PickCol);
	void PickGizmo(SPTR<CCollider> _This, SPTR<CCollider> _Other);
	void PickEndGizmo(SPTR<CCollider> _This, SPTR<CCollider> _Other);
	
public:
	void SetGizmoPivot(const Vec3& _Pivot);

public:
	void GizmoOn();
	void GizmoOff();

public:
	CGizmo();
	~CGizmo();
};

