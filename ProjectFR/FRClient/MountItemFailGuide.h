#pragma once
#include <Logic.h>
class CMountItemFailGuide : public CLogic
{
	SPTR<CFixRenderer>					m_GuideBackRen;
	SPTR<CFixRenderer>					m_FailRen;
	float								m_fAccTime;


public:
	void Init();
	void Update();

public:
	void StandBy();
public:
	CMountItemFailGuide();
	~CMountItemFailGuide();
};

