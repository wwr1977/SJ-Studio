#pragma once
#include <Logic.h>

class CFixRenderer;
class CMapBackground :public CLogic
{
private:
	vector<tstring>							m_MapSpriteName;
	SPTR<CFixRenderer>						m_BackRen;

public:
	void Init();


public:
	void ChangeMapBack(const MAINMAPNAME& _MapIndex);
	void ChangeMapBack(const UINT& _MapIndex);

public:
	CMapBackground();
	~CMapBackground();
};

