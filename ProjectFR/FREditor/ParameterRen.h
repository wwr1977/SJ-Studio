#pragma once
#include <Logic.h>

class CParameterRen : public CLogic
{
	enum 
	{
		MAXPARAMETER = 8
	};

private:
	Vec2						m_MiddlePos;
	vector<Vec2>				m_vecPoint;
	vector<tstring>				m_vecParameterName;
	float						m_fCurParameter[MAXPARAMETER];
	float						m_fRadius;

public:
	void Init();
	void Update();
	void FontRender();

public:
	void InitParameterValue(const STATPARAMETERDATA& _InitData = STATPARAMETERDATA());
	void SetParameterValue(const int& _Index, const float& _Value);
	float* GetParameterArr();

public:
	CParameterRen();
	~CParameterRen();
};

