#include "stdafx.h"
#include "EditCamState.h"


CEditCamState::CEditCamState()
{
}


CEditCamState::~CEditCamState()
{
}
void CEditCamState::Init(void* _pLogic)
{
	m_PairObject = (CEditCameraMen*)_pLogic;

}