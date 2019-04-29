#include "stdafx.h"
#include "CamMove.h"
#include"EditCameraMen.h"

CCamMove::CCamMove()
{
}


CCamMove::~CCamMove()
{
}
void CCamMove::StateReset()
{

}
void CCamMove::Update()
{
	
	if (TRUE == KEY("CameraMoveUpLeft")) 
	{
		m_PairObject->CamMove(GAMEVEC::LEFTUP);
	}
	if (TRUE == KEY("CameraMoveUpRight"))
	{
		m_PairObject->CamMove(GAMEVEC::RIGHTUP);
		//return;
	}
	if (TRUE == KEY("CameraMoveDownLeft"))
	{
		m_PairObject->CamMove(GAMEVEC::LEFTDOWN);
		//return;
	}
	if (TRUE == KEY("CameraMoveDownRight"))
	{
		m_PairObject->CamMove(GAMEVEC::RIGHTDOWN);
		//return;
	}
	if (TRUE == KEY("CameraMoveUp"))
	{
		m_PairObject->CamMove(GAMEVEC::UP);
		return;
	}
	if (TRUE == KEY("CameraMoveDown"))
	{
		m_PairObject->CamMove(GAMEVEC::DOWN);
		return;
	}
	if (TRUE == KEY("CameraMoveLeft"))
	{
		m_PairObject->CamMove(GAMEVEC::LEFT);
		return;
	}
	if (TRUE == KEY("CameraMoveRight"))
	{
		m_PairObject->CamMove(GAMEVEC::RIGHT);
		return;
	}
	


	m_PairObject->ChangeState(0);
}