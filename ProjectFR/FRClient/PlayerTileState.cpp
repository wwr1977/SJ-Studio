#include "Precom.h"
#include "PlayerTileState.h"
#include "Tile.h"


CPlayerTileState::CPlayerTileState()	
	:m_TileOption(MAXTILETOPTION)
{
}
CPlayerTileState::CPlayerTileState(const TILEOPTION& _Option)
	: m_TileOption(_Option)
{
	if (m_TileOption > MAXTILETOPTION)
		m_TileOption = MAXTILETOPTION;
}

CPlayerTileState::~CPlayerTileState()
{
}

const TILEOPTION CPlayerTileState::CurTileOption()
{
	return m_TileOption;
}