#pragma once
#include "PlayerTileState.h"
class CStepStairTile : public CPlayerTileState
{

public:
	virtual const bool CheckChangeTile(SPTR<CTile>_CurTile, SPTR<CTile>_ChangeTile);

public:
	CStepStairTile();
	~CStepStairTile();
};

