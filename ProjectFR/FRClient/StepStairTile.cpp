#include "Precom.h"
#include "StepStairTile.h"
#include "Tile.h"

CStepStairTile::CStepStairTile()
	:CPlayerTileState(STEPSTAIRTILE)
{
}


CStepStairTile::~CStepStairTile()
{
}
const bool CStepStairTile::CheckChangeTile(SPTR<CTile>_CurTile, SPTR<CTile>_ChangeTile)
{
	if (nullptr == _ChangeTile || nullptr == _CurTile)
		return false;
	
	switch (_ChangeTile->GetTileOption())
	{
	case STEPTILE:
	{
		D3DXCOLOR CurColor = _CurTile->GetTileColor();
		D3DXCOLOR ChangeColor = _ChangeTile->GetTileColor();

		if (CurColor == ChangeColor)
			return true;

		return false;
	}
	case STAIRTILE:
		return true;
	case STEPSTAIRTILE:
	{
		D3DXCOLOR CurColor = _CurTile->GetTileColor();
		D3DXCOLOR ChangeColor = _ChangeTile->GetTileColor();

		if (CurColor == ChangeColor)
			return true;

		return false;
	}
	case MAPCHANGETILE:
		return true;
	case WORLDMAPTILE:
		return true;
	case NOENTERTILE:
		return false;
	}

	return false;
}