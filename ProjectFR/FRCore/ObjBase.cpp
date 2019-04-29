#include "stdafx.h"
#include "ObjBase.h"


CObjBase::CObjBase()
	:m_Name(_T("")),m_TypeInfo(nullptr),m_iRefCount(0)
	,m_bActive(true),m_bDeath(false)
{
}
CObjBase::CObjBase(tstring _Name)
	: m_Name(_Name), m_TypeInfo(nullptr), m_iRefCount(0)
	, m_bActive(true), m_bDeath(false)
{

}

CObjBase::~CObjBase()
{
}
