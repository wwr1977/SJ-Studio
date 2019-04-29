#pragma once
#include <Logic.h>
class CCharacterStatus : public CLogic
{
	enum 
	{
		GAUGE_XP,
		GAUGE_STR,
		GAUGE_DEX,
		GAUGE_INT,
		GAUGE_LUK,
		MAXGAUGE
	};

	enum 
	{
		STRING_LV,
		STRING_STR,
		STRING_DEX,
		STRING_INT,
		STRING_LUK,
		STRING_HP,
		STRING_MP,
		STRING_DEM,
		STRING_DEF,
		STRING_CRI,
		STRING_CRIDEM,
		STRING_AVD,
		MAXSTATUSSTRING,
	};

private:
	CharacterIndex							m_CurCharNo;
	SPTR<CAniRenderer>						m_CurCharacterRen;
	SPTR<CFontRenderer>						m_CurCharacterNameRen;

	Vec3									m_ModelPos;
	vector<UNITNAME>						m_CharacterName;
	SPTR<class CCharacterParameter>			m_Parameter;
	vector<SPTR<class CStdGauge>>			m_vecGauge;
	vector<tstring>							m_vecStatusString;

	
public:
	void Init();
	void Update();
	void UIFontRender();


public:
	void ChangeCharacterStatus(const CharacterIndex& _Index);
	void StatusOn();
	void StatusOn(const CharacterIndex& _Index);
	void StatusOff();


public:
	CCharacterStatus();
	~CCharacterStatus();
};

