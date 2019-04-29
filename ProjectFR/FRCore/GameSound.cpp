#include "stdafx.h"
#include "GameSound.h"


CGameSound::CGameSound(FMOD::Sound* _pSound)
	:m_pSound(_pSound)
{
}


CGameSound::~CGameSound()
{
	if (nullptr != m_pSound)
		m_pSound->release();
}
CGameSound* CGameSound::CreateGameSound(FMOD::System* _pSys, const tstring& _Path)
{
	assert(_pSys);

	if (nullptr == _pSys) 
	{
		return nullptr;
	}

	FMOD::Sound* TempSound= nullptr;
	
	FMOD_RESULT hr = FMOD_OK;

	int Count = 0;

	

#ifdef _UNICODE
	string Path = CW2A(_Path.c_str(), CP_UTF8);
	hr =_pSys->createSound(Path.c_str(), FMOD_DEFAULT, nullptr, &TempSound);
#else
	hr = _pSys->createSound(_Path.c_str(), FMOD_DEFAULT, nullptr, &TempSound);
#endif

	
	if (FMOD_OK != hr)
	{
		assert(TempSound);
		//exit(1);
		return nullptr;
	}
	

	return new CGameSound(TempSound);
}