#pragma once
#include <Logic.h>
class CCharStatusTab : public CLogic
{
private:
	CharacterIndex						m_CharacterNo;
	SPTR<CFixRenderer>					m_CharacterIconRen;
	SPTR<CFixRenderer>					m_SelectRen;
	bool								m_bSelect;

public:
	void Init();
	void InitData(const CharacterIndex& _Index);

public:
	void SetPos(const Vec3& _Pos);
	void SelectTab(const bool& _Select);
	void TabOn();
	void TabOff();

public:
	const CharacterIndex GetCharacterNo();

public:
	CCharStatusTab();
	~CCharStatusTab();
};

