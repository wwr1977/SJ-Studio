#pragma once
#include <Logic.h>
class CUnitPortrait : public CLogic
{
private:
	SPTR<CFixRenderer>					m_UnitPortrait;
	

public:
	void Init();
	void Update();
	void FontRender();
	
public:
	void SetUnitPortrait(const tstring& _Portrait);


public:
	CUnitPortrait();
	~CUnitPortrait();
};

