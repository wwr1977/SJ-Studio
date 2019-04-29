#pragma once
#include <Logic.h>
class CBattlePositionUI : public CLogic
{
	enum 
	{
		GAUGE_HP,
		GAUGE_MP,
		GAUGE_XP,
		MAXGAUGEINDEX
	};


private:
	CharacterIndex					m_CharacterNo;
	size_t							m_CharacterLine;
	size_t							m_LineIndex;
	tstring							m_CharacterKName;
	//SPTR<CFixRenderer>				m_IllustRen;
	SPTR<CAniRenderer>				m_AniIllustRen;
	SPTR<CAniRenderer>				m_SelectRen;
	SPTR<CFontRenderer>				m_CharNameRen;
	SPTR<CFontRenderer>				m_CharLevelRen;
	vector<SPTR<class CStdGauge>>	m_StatusGauge;

public:
	void Init();
	void InitData(const CharacterIndex& _CharNo);
	void DebugRender();


public:
	void UIOn();
	void UIOn(const Vec3& _OnPos);
	void UIOff();

public:
	void CharNameOn();
	void CharNameOff();
	void SelectUIOn();
	void SelectUIOff();

public:
	void ChangeRenderingVersion(const UIRENDERINGVERSION& _Ver);

public:
	CBattlePositionUI();
	~CBattlePositionUI();
};

