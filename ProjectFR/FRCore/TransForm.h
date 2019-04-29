#pragma once
#include "GameCom.h"


class CTransForm : public CGameCom
{
private:
	CTransForm*			m_pParent;
	void(CTransForm::*CalWorldFunc)();
	float				m_fImageScale;


private:
	bool		m_bScale;
	bool		m_bRot;
	bool		m_bTrans;
	bool		m_bWorld;

	Vec3		m_Scale;
	Vec3		m_Rot;
	Vec3		m_Pos;

	Mat			m_AxisMat;

	Mat			matScale;
	Mat			matRotation;
	Mat			matRotationU;
	Mat			matRotationV;
	Mat			matRotationN;
	Mat			matTrans;
	Mat			matRevole;

	Mat			matWorld;
	//	월드 행렬 생성과정에서 Scale이 제외된 행렬



public:
	bool IsActorActive();
	bool ChangeTranspose();
	bool ChangeWorld();
	void SetParent(CTransForm* _pParent)
	{
		if (nullptr == _pParent)
		{
			CalWorldFunc = &CTransForm::CalWorld;
		}
		else
		{
			CalWorldFunc = &CTransForm::CalWorldParent;
		}
		m_pParent = _pParent;
	}
	void SetScale(const Vec3& _Vec)
	{
		m_Scale = _Vec;
		m_bScale = true;
	}
	void SetScale(const Vec2& _Vec)
	{
		m_Scale = Vec3{ _Vec.x, _Vec.y, 1.0f };
		m_bScale = true;
	}
	void SetSize(const Vec3& _Vec)
	{
		m_Scale = _Vec;
		m_bScale = true;
	}
	void SetSize(const Vec2& _Vec)
	{
		m_Scale = Vec3{ _Vec.x,_Vec.y,1.0f };
		m_bScale = true;
	}
	void SetPos(const Vec3& _Vec)
	{
		m_Pos = _Vec;
		m_bTrans = true;
	}

	void SetPos(const Vec2& _Vec) 
	{
		m_Pos = { _Vec.x,_Vec.y,50.0f };
		m_bTrans = true;
	}

	void Move(const Vec3& _Vec)
	{
		m_Pos += _Vec;
		m_bTrans = true;
	}


	void SetRot(const Vec3& _Vec)
	{
		m_Rot = _Vec;
		m_bRot = true;
	}

	void SetRotX(float _RadX)
	{
		m_Rot.x = _RadX;
		m_bRot = true;
	}

	void SetRotY(float _RadY)
	{
		m_Rot.y = _RadY;
		m_bRot = true;
	}
	void SetRotZ(float _RadZ)
	{
		m_Rot.z = _RadZ;
		m_bRot = true;
	}
	void SetImageScale(const float& _Mag) 
	{
		m_fImageScale = _Mag;
	}
	const Vec3 AxisU() 
	{
		return Vec3{ m_AxisMat._11,m_AxisMat._12 ,m_AxisMat._13 };
	}
	const Vec3 AxisV()
	{
		return Vec3{ m_AxisMat._21,m_AxisMat._22 ,m_AxisMat._23 };
	}
	const Vec3 AxisN()
	{
		return Vec3{ m_AxisMat._31,m_AxisMat._32 ,m_AxisMat._33 };
	}

	void SetAxis(const Vec3& _Rot) 
	{
		Quat AxisQuat;
		Mat AxisRot;

		D3DXQuaternionRotationYawPitchRoll(&AxisQuat,_Rot.y, _Rot.x, _Rot.z);
		D3DXMatrixRotationQuaternion(&AxisRot, &AxisQuat);
		
		/*Mat X, Y, Z, AxisRot;

		D3DXMatrixRotationX(&X, _Rot.x);
		D3DXMatrixRotationX(&Y, _Rot.y);
		D3DXMatrixRotationX(&Z, _Rot.z);

		AxisRot = X*Y*Z;*/

		m_AxisMat = m_AxisMat * AxisRot;
		return;
	}

	void AxisRot(AXIS _Axis, const float _Rad) 
	{
		Vec3 Axis;

		switch (_Axis)
		{
		case TANGENT:	Axis = AxisU(); break;
		case BITANGENT:	Axis = AxisV();	break;
		case NORMAL:	Axis = AxisN(); break;
		default:
			return;
		}

		Mat AxisRot;
		D3DXMatrixRotationAxis(&AxisRot, &Axis, _Rad);
		m_AxisMat *= AxisRot;
		m_bRot = true;
	}

	void AxisRot(const Quat& _Quat) 
	{
		Mat QuatMat;
		Quat RotQuat = _Quat;

		D3DXMatrixRotationQuaternion(&QuatMat, &RotQuat);
		m_AxisMat *= QuatMat;
		
		m_bRot = true;
	}

	// Tangent : U , BiTangent : V , Normal : N
	void TangentRot(const float& _Rot) 
	{
		m_Rot = { _Rot,0.0f,0.0f };
		m_bRot = true;
	}
	void BiTangentRot(const float _Rot) 
	{
		m_Rot = { 0.0f,_Rot,0.0f };
		m_bRot = true;
	}
	void NormalRot(const float& _Rot) 
	{ 
		m_Rot = {0.0f,0.0f,_Rot};
		m_bRot = true;
	}

	const Vec3	GetScale() { return m_Scale; }
	const Vec3  GetSize() { return m_Scale; }
	const Vec3	GetPos();

public:
	const Mat& GetWorldMat() const
	{
		return matWorld;
	}

	void GetRenWorld(Mat* _OutMat,const Vec2& _SpritePivot, const Vec2& _SpriteSize);
	void GetRenWorld(Mat* _OutMat, const Vec2& _SpritePivot,const Vec2& RenPivot, const Vec2& _SpriteSize);
	

	void GetObjWorld(Mat* _OutMat);
	void GetObjWorld(Mat* _OutMat,const Vec2& _RenPivot);

	Mat GetAxisMat()
	{
		return m_AxisMat;
	}
public:
	virtual void PreUpdate();
	virtual void Update();
	virtual void PostUpdate();
	virtual void LastUpdate();


public:
	void CalWorld();
	void CalWorldParent();

public:
	CTransForm();
	CTransForm(CActObject* _ActObject);
	~CTransForm();
};

