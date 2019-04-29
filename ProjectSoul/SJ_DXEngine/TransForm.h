#pragma once
#include "BaseComponent.h"
#include"DXHeader.h"
#include"DXEnum.h"


class CTransForm final : public CBaseComponent
{
	friend class CSubTrans;

public:
	static const bool IsMultiComponentCheck(CActor* _pActor);

private:
	// �ڱⰡ �Ҽӵ� ������ �θ� ������ ���ϱ���
	CTransForm * m_pParent;
	void(CTransForm::*CalWorldFunc)();

private:
	bool			m_bTransUpdate;
	bool			m_bScale;
	bool			m_bRot;
	bool			m_bTrans;
	bool			m_bRevolve;
	bool			m_bWorld;

	float			m_fMoveSpeed;
	Vec3			m_MoveDir;

	Vec3			m_Scale;
	Vec3			m_Rot;
	Vec3			m_Trans;

	Vec3			m_RevolvePivot;
	Vec3			m_RevolveRot;

	Mat				m_matScale;
	Mat				m_matRotX;
	Mat				m_matRotY;
	Mat				m_matRotZ;
	Mat				m_matRot;
	Mat				m_matTrans;
	Mat				m_matRevolve;

	Mat				m_matWorld;

public:
	void SetScale(const Vec3& _Scale)
	{
		m_bTransUpdate = false;
		m_bScale = true;
		m_Scale = _Scale;
	}
	void SetRotate(const Vec3& _Rot)
	{
		m_bTransUpdate = false;
		m_bRot = true;
		m_Rot = _Rot;
	}

	void SetPos(const Vec3& _Pos)
	{
		m_bTransUpdate = false;
		m_bTrans = true;
		m_Trans = _Pos;
	}

	void SetPos(const Vec2& _Pos)
	{
		m_bTransUpdate = false;
		m_bTrans = true;
		m_Trans.vec2 = _Pos;
	}


	void SetRevolve(const Vec3& _Pivot, const Vec3& _Rot)
	{
		m_bTransUpdate = false;
		m_bRevolve = true;
		m_RevolvePivot = _Pivot;
		m_RevolveRot = _Rot;
	}
	void SetRevolve(const Vec3& _Rot)
	{
		m_bTransUpdate = false;
		m_bRevolve = true;
		m_RevolvePivot = Vec3();
		m_RevolveRot = _Rot;
	}

	// 1���� �Ű����� : Dir*Speed*DeltaTime
	// 2��° �Ű����� : DeltaTime
	// �� ������ ���� Dir�� Speed�� ����
	// ������ ������ ��ġ�� ��ȯ
	const Vec3 Move(const Vec3& _Dir, const float& _DeltaTime)
	{
		m_bTransUpdate = false;
		Vec3 Dir = _Dir / _DeltaTime;
		m_fMoveSpeed = Dir.Length();
		m_MoveDir = Dir.GetNormalizeVec();

		m_bTrans = true;
		m_Trans += _Dir;
		return m_Trans;
	}

	const Vec3 Move(const Vec3& _Dir, const float& _Speed, const float& _DeltaTime)
	{
		m_bTransUpdate = false;

		if (m_fMoveSpeed != _Speed)
			m_fMoveSpeed = _Speed;

		if (m_MoveDir != _Dir)
			m_MoveDir = _Dir;

		m_bTrans = true;
		m_Trans += _Dir * _Speed * _DeltaTime;
		return m_Trans;
	}

	// �ݹ߿� ���� �̵�
	const Vec3 Push(const Vec3& _Dir, const float& _Speed)
	{
		m_bTransUpdate = false;
		m_bTrans = true;
		m_Trans += _Dir * _Speed;
		return m_Trans;
	}

	void SetMoveSpeed(const float& _Speed) { m_fMoveSpeed = _Speed; }
	void SetMoveDir(const Vec3& _Dir) { m_MoveDir = _Dir; }
	const float GetMoveSpeed() { return m_fMoveSpeed; }
	const Vec3 GetMoveDir() { return m_MoveDir; }

public:
	const Vec3 GetScale() { return m_Scale; }
	const Vec3 GetLocalPos() { return m_Trans; }
	const Vec3 GetLocalRotate() { return m_Rot; }
	const Vec3 GetLocalRevolve() { return m_RevolveRot; }

	const Vec3 GetWorldPos();
	


	const Mat	GetWorldMat() { return m_matWorld; }
	const Mat&  GetCRWorldMat() {return m_matWorld;}

	const Mat	GetNonScaleWorld();
	const Vec3 GetWorldAxis(const Axis& _Axis)
	{
		if (MAXAXIS <= _Axis)
			return Vec3();

		return m_matWorld.Row[_Axis].vec3.GetNormalizeVec();
	}

public:
	const bool IsMove() 
	{
		return m_bTrans; 
	}
	const bool ChangeTransform() 
	{
		//return	m_bWorld;

		// �ڽ��� ���� ����� �ٲ�ٸ�
		if (true == m_bWorld)
			return true;

		// �θ��� ���� ����� �ٲ�ٸ�
		if (nullptr != m_pParent && true == m_pParent->ChangeTransform())
			return true;

		return false;
	}
	

public:
	// �ڱⰡ �Ҽӵ� ������ �θ� ���õ� ���
	// �θ� ������ ���ϱ����� �Ű� ������ �������.
	void SetParent(CTransForm* _pParent)
	{
		if (nullptr == _pParent)
			CalWorldFunc = &CTransForm::CalWorld;
		else 
		{
			m_pParent = _pParent;
			CalWorldFunc = &CTransForm::CalWorldParent;
			(this->*CalWorldFunc)();
		}
		
	}

public:
	void CalWorld();
	void CalWorldParent();
	void CalSubTransWorld(struct _tagSubTransData* _SubTransData);

public:
	void PostUpdate(const float& _DeltaTime) override;
	void LastUpdate(const float& _DeltaTime) override;

public:
	void TransformUpdate();


public:
	CTransForm();
	~CTransForm();
};

