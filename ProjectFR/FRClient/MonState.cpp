#include "Precom.h"
#include "MonState.h"
#include"BattleMonster.h"

CMonState::CMonState()
{
}


CMonState::~CMonState()
{
}
void CMonState::Init(void* _pLogic) 
{
	m_pMonster = (CBattleMonster*)_pLogic;
}
CBattleMonster* CMonState::GetMonster()
{
	return m_pMonster;
}