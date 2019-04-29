#pragma once
#include "ObjBase.h"

class CGameSound :public CObjBase
{
private:
	FMOD::Sound*		m_pSound;

public:
	FMOD::Sound* GetSound() 
	{
		return m_pSound;
	}

public:
	static CGameSound* CreateGameSound(FMOD::System* _pSys, const tstring& _Path);

public:
	CGameSound(FMOD::Sound* _pSound);
	~CGameSound();
};

