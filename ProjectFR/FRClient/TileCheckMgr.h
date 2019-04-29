#pragma once
#include<ObjBase.h>

class CTile;
class CPlayerTileState;
class CTileCheckMgr : public CObjBase
{
private:
	CPlayer*								m_pPlayer;
	SPTR<CPlayerTileState>					m_CurTileState;
	TILEOPTION								m_CurTileOption;
	vector<SPTR<CPlayerTileState>>			m_vecTileState;



public:
	void Init();
	void Update();
	const TILEOPTION CurTileOption();
	void ChangeTileState(const TILEOPTION& _Option);
	bool CheckChangeTile(SPTR<CTile>_CurTile , SPTR<CTile>_ChangeTile);
	const TILEOPTION CurTileState();

public:
	CTileCheckMgr(CPlayer* _Player);
	~CTileCheckMgr();
};

