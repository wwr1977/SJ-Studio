#include "Precom.h"
#include "MapBackground.h"


CMapBackground::CMapBackground()
	:m_BackRen(nullptr)
{
}


CMapBackground::~CMapBackground()
{
}
void CMapBackground::ChangeMapBack(const MAINMAPNAME& _MapIndex)
{
	if (_MapIndex >= MAXMAINMAPNAME)
		return;

	m_BackRen->SetSprite(m_MapSpriteName[_MapIndex]);
}
void CMapBackground::ChangeMapBack(const UINT& _MapIndex)
{
	return ChangeMapBack((MAINMAPNAME)_MapIndex);
}

void CMapBackground::Init()
{
	m_BackRen = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_BACKGROUND, 0.9f));

	m_MapSpriteName.push_back(_T("Town"));

	
	ChangeMapBack(TOWN);
}