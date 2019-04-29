#pragma once
#include "PlayerTileState.h"
class CStepTile : public CPlayerTileState
{

public:
	virtual const bool CheckChangeTile(SPTR<CTile>_CurTile, SPTR<CTile>_ChangeTile);
public:
	CStepTile();
	~CStepTile();
};

