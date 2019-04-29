#include "Precom.h"
#include "TileCheckMgr.h"
#include"PlayerTileState.h"
#include"StepTile.h"
#include"StairTile.h"
#include"StepStairTile.h"
#include"Tile.h"


CTileCheckMgr::CTileCheckMgr(CPlayer* _Player)
	:m_pPlayer(_Player)
	, m_CurTileOption(MAXTILETOPTION)
{
	Init();
}


CTileCheckMgr::~CTileCheckMgr()
{
	m_vecTileState.clear();
}
void CTileCheckMgr::Init()
{
	m_vecTileState.clear();

	m_vecTileState.push_back(new CStepTile());
	m_vecTileState.push_back(new CStairTile());
	m_vecTileState.push_back(new CStepStairTile());

	m_CurTileState = m_vecTileState[STEPTILE];
}
void CTileCheckMgr::Update()
{

}

const TILEOPTION CTileCheckMgr::CurTileOption()
{
	return m_CurTileState->CurTileOption();
}
void CTileCheckMgr::ChangeTileState(const TILEOPTION& _Option)
{
	if (m_vecTileState.size() <= _Option)
		return;

	m_CurTileOption = _Option;
	m_CurTileState = m_vecTileState[_Option];
}
bool CTileCheckMgr::CheckChangeTile(SPTR<CTile>_CurTile, SPTR<CTile>_ChangeTile)
{
	if (nullptr == _CurTile || nullptr == _ChangeTile)
		return false;

	return m_CurTileState->CheckChangeTile(_CurTile,_ChangeTile);
}
const TILEOPTION CTileCheckMgr::CurTileState()
{
	return m_CurTileOption;
}