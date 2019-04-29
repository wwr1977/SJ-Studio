#pragma once
#include <Logic.h>

class CBattleUnit;
class CCharSelectEffect : public CLogic
{
private:
	CBattleUnit*							m_pPairUnit;
	Vec3									m_EffectPivot;
	SPTR<CFixRenderer>						m_EffRen;
	SPTR<CAniRenderer>						m_NewEffectRen;
	float									m_fEffectScale;
	float									m_fAccTime;
	float									m_fAccSizeTime;
	float									m_fRotSpeed;
	bool									m_bReduceSize;


public:
	void Init();
	void PreUpdate();
	void Update();
	virtual void InitData(CBattleUnit* _PairUnit);

public:
	void TraceCamDir();

	void SetPos(const Vec3& _Pos);
	void On();
	void Off();
	void SetEffectPivot(const Vec3& _Pos);
	void SetCustomSize(const Vec3& _CustomSize);
	void SetImageScale(const float& _Scale);
	void ChangeSize();

public:
	CCharSelectEffect();
	~CCharSelectEffect();
};

