#pragma once
#include <Logic.h>

class CBattleUnit;
class CTargetUI : public CLogic
{
	static Vec2						FixSize;
	
private:
	CBattleUnit*					m_PairUnit;
	SPTR<CFixRenderer>				m_UpRenderer;
	SPTR<CFixRenderer>				m_DownRenderer;
	SPTR<CAniRenderer>				m_NewEffectRen;

	Vec2							m_UISize;
	Vec2							m_UpSize;
	Vec2							m_DownSize;
	// Up,Down Renderer의 y 좌표 배율의  x : min, y : max
	Vec2							m_ScaleRatio;

	float							m_fAccTime;
	// 확대 축소 주기
	float							m_fScaleTime;
	
	float							m_ScaleSign;

public:
	void Init();
	void Update();

public:
	void SetUnit(CBattleUnit* _Unit);
	void SetPos(const Vec2& _Pos);
	void SetPos(const Vec3& _Pos);
	void SetSize(const Vec2& _Size);
	void TargetUIOn();
	void TargetUIOff();

public:
	CBattleUnit* GetPairUnit();

public:
	CTargetUI();
	~CTargetUI();
};

