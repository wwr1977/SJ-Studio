#pragma once
#include "BattleState.h"
class CCharAction : public CBattleState
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
	CCharAction();
	~CCharAction();
};

