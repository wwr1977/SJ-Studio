#pragma once
#include<ObjBase.h>

class CTile;
class CPlayerTileState : public CObjBase
{
protected:
	TILEOPTION				m_TileOption;

public:
	const TILEOPTION CurTileOption();
	virtual const bool CheckChangeTile(SPTR<CTile>_CurTile, SPTR<CTile>_ChangeTile) = 0;
public:

	CPlayerTileState();
	CPlayerTileState(const TILEOPTION& _Option);
	~CPlayerTileState();
};

