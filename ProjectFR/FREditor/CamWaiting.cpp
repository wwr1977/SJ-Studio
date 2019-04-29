#include "stdafx.h"
#include "CamWaiting.h"
#include"EditCameraMen.h"


CCamWaiting::CCamWaiting()
{
}


CCamWaiting::~CCamWaiting()
{
}

void CCamWaiting::StateReset() 
{

}
void CCamWaiting::Update()
{
	if (TRUE == KEY("CameraMoveUp") ||
		TRUE == KEY("CameraMoveDown") ||
		TRUE == KEY("CameraMoveLeft") ||
		TRUE == KEY("CameraMoveRight") ||
		TRUE == KEY("CameraMoveUpLeft") ||
		TRUE == KEY("CameraMoveUpRight") ||
		TRUE == KEY("CameraMoveDownLeft") ||
		TRUE == KEY("CameraMoveDownRight")
		)
	{
		m_PairObject->ChangeState(1);
	}
}