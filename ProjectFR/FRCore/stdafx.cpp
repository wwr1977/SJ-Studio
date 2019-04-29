#include "stdafx.h"


bool RendereYSort(SPTR<CRenderer> _Left, SPTR<CRenderer> _Right) 
{
	return _Left->GetPosY() > _Right->GetPosY();
}