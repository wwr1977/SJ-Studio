#include "NameBase.h"



CNameBase::CNameBase()
{
}


CNameBase::~CNameBase()
{
}


void CNameBase::AddTag(const wchar_t* _Tag) 
{
	auto Iter = m_setTag.find(_Tag);

	if (Iter == m_setTag.end()) 
		m_setTag.insert(_Tag);
	
}
void CNameBase::DelTag(const wchar_t* _Tag) 
{
	auto Iter = m_setTag.find(_Tag);

	if (Iter != m_setTag.end())
		m_setTag.erase(_Tag);

}
const bool CNameBase::ExistTag(const wchar_t* _Tag) 
{
	auto Iter = m_setTag.find(_Tag);

	if (Iter != m_setTag.end())
		return true;
		
	return false;
}