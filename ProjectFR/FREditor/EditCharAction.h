#pragma once
#include "EditBattleState.h"

class CEditCharAction : public CEditBattleState
{
private:
	bool							m_bEnd;
	int								m_iActionIndex;


public:
	void StateReset();
	void Update();

	/*void ScriptUpdate();
	void HideUpdate();
	void MoveUpdate();*/

public:
	CEditCharAction();
	~CEditCharAction();
};

