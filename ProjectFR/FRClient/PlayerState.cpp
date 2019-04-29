#include "Precom.h"
#include "PlayerState.h"
#include "PlayerLogic.h"


CPlayerState::CPlayerState()
{
}


CPlayerState::~CPlayerState()
{
}
void CPlayerState::Init(void* _pLogic)
{
	m_pPlayer = (CPlayerLogic*)_pLogic;
	
}


CPlayerLogic* CPlayerState::GetPlayer()
{
	return m_pPlayer;
}