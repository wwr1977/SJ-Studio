#pragma once
#include "PlayerTileState.h"
class CStairTile :public CPlayerTileState
{
public:
	const bool CheckChangeTile(SPTR<CTile>_CurTile, SPTR<CTile>_ChangeTile);
public:
	CStairTile();
	~CStairTile();
};

