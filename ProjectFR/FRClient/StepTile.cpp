#include "Precom.h"
#include "StepTile.h"
#include "Tile.h"


CStepTile::CStepTile()
	:CPlayerTileState(STEPTILE)
{
}


CStepTile::~CStepTile()
{
}
// Change Tile로 이동이 가능한 경우 true를 출력
const bool CStepTile::CheckChangeTile(SPTR<CTile>_CurTile, SPTR<CTile>_ChangeTile)
{
	if (nullptr == _ChangeTile || nullptr == _CurTile)
		return false;

	TILEOPTION Op = _ChangeTile->GetTileOption();

	switch (Op)
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
		return false;
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
	default:
		return false;
	}

	if (_ChangeTile->GetTileOption() == NOENTERTILE)
		return false;



	return false;
}
