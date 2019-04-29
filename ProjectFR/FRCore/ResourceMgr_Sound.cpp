#include "stdafx.h"
#include "ResourceMgr.h"
#include "GameSound.h"
#include "SoundPlayer.h"

FMOD::System* CResourceMgr::m_FSystem = nullptr;
unordered_map<tstring, SPTR<CGameSound>>				CResourceMgr::m_mapSound;
unordered_map<tstring, SPTR<CGameSound>>::iterator	    CResourceMgr::m_FindSoundIter;

FMOD::System* CResourceMgr::GetSoundSystem()
{
	return m_FSystem;
}
void CResourceMgr::SoundInit() 
{
	FMOD::System_Create(&m_FSystem);

	if (nullptr == m_FSystem) 
	{
		assert(m_FSystem);
		return;
	}
	m_FSystem->init(32, FMOD_DEFAULT, nullptr);
	CSoundPlayer::SystemInit(m_FSystem);
}
BOOL CResourceMgr::ExistGameSound(const tstring& _Key)
{
	return EXISTMAP_SPTR<CGameSound>(_Key, m_mapSound);
}
SPTR<CGameSound> CResourceMgr::FindSound(const tstring& _Key) 
{
	return FINDMAP_SPTR<CGameSound>(_Key, m_mapSound);
}
CGameSound* CResourceMgr::SoundLoad(const tstring& _SoundPath) 
{
	tstring SoundKey = CFilePathMgr::PathToFileName(_SoundPath);
	SPTR<CGameSound> pSound = FindSound(SoundKey);

	if (nullptr != pSound) 
	{
		return nullptr;
	}

	CGameSound* NewSound = CGameSound::CreateGameSound(m_FSystem, _SoundPath);

	if (nullptr == NewSound) 
	{
		return nullptr;
	}

	CCriSectionObject<CResourceMgr>();
	m_mapSound.insert(unordered_map<tstring, SPTR<CGameSound>>::value_type(SoundKey, NewSound));

	return NewSound;

}
CGameSound* CResourceMgr::SoundLoad(const tstring& _SoundPath, unordered_map<tstring, SPTR<CGameSound>>& _TempMap)
{
	tstring SoundKey = CFilePathMgr::PathToFileName(_SoundPath);
	SPTR<CGameSound> pSound = FindSound(SoundKey);

	if (nullptr != pSound)
	{
		return nullptr;
	}

	CGameSound* NewSound = CGameSound::CreateGameSound(m_FSystem, _SoundPath);

	if (nullptr == NewSound)
	{
		return nullptr;
	}

	_TempMap.insert(unordered_map<tstring, SPTR<CGameSound>>::value_type(SoundKey, NewSound));

	return NewSound;
}
void CResourceMgr::MergeSoundMap(unordered_map<tstring, SPTR<CGameSound>>& _TempMap)
{
	if (_TempMap.size() <= 0) 
	{
		assert(nullptr);
		return;
	}

	//CCriSectionObject<CResourceMgr>();
	m_mapSound.insert(_TempMap.begin(), _TempMap.end());
}