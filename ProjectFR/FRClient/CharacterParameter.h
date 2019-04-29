#pragma once
#include <Logic.h>
class CCharacterParameter : public CLogic
{
	enum
	{
		MAXPARAMETER = 8
	};

	vector<SPTR<CFixRenderer>>					m_vecParameterBack;
	vector<tstring>								m_vecParameterName;
	vector<Vec2>								m_vecPoint;
	float										m_fCurParameter[MAXPARAMETER];
	float										m_fRadius;

public:
	void Init();
	void Update();
	void UIFontRender();

public:
	void SetChangeParameter(const CharacterIndex& _Index);
	void SetPos(const Vec3& _Pos);
	void ParameterOn();
	void ParameterOff();

public:
	CCharacterParameter();
	~CCharacterParameter();
};

