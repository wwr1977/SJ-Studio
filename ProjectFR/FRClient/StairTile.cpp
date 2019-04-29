#include "Precom.h"
#include "StairTile.h"
#include "Tile.h"

CStairTile::CStairTile()
	:CPlayerTileState(STAIRTILE)
{
}


CStairTile::~CStairTile()
{
}

const bool CStairTile::CheckChangeTile(SPTR<CTile>_CurTile, SPTR<CTile>_ChangeTile)
{
	if (nullptr == _ChangeTile || nullptr == _CurTile)
		return false;

	switch (_ChangeTile->GetTileOption())
	{
	case STEPTILE:
		return false;
	case STAIRTILE:
		return true;
	case STEPSTAIRTILE:
		return true;
	case MAPCHANGETILE:
		return true;
	case WORLDMAPTILE:
		return true;
	case NOENTERTILE:
		return false;
	}

	return false;
}
