#include "stdafx.h"
#include "SoundPlayer.h"
#include"GameSound.h"


FMOD::System*	CSoundPlayer::m_FSys = nullptr;
int CSoundPlayer::m_iCurPlayingChannel = 0;

CSoundPlayer::CSoundPlayer()
	:m_bStop(false), m_fVolume(1.f), m_pChannel(nullptr)
	, m_CurSoundKey(_T(""))
	, m_bPause(false)
{
}


CSoundPlayer::~CSoundPlayer()
{

}
void CSoundPlayer::SystemInit(FMOD::System* _pSys) 
{
	m_FSys = _pSys;

	if (nullptr == m_FSys) 
	{
		assert(m_FSys);
		return;
	}
}


bool CSoundPlayer::PlaySound(const tstring& _SoundKey, FMOD_MODE  _Loop /*= FMOD_LOOP_OFF*/, unsigned int _LoopCount /*= -1*/) 
{
	if (nullptr == m_FSys) 
	{
		assert(m_FSys);
		return false;
	}

	SPTR<CGameSound> FindSound = CResourceMgr::FindSound(_SoundKey);

	if (nullptr == FindSound) 
	{
		return false;
		unordered_map<tstring, SPTR<CGameSound>>* SoundMap = CResourceMgr::GetSoundMap();
		//assert(FindSound);
	}

	if (32 <= m_iCurPlayingChannel) 
	{
		return false;
	}


	FMOD_RESULT hr = m_FSys->playSound(FindSound->GetSound(), nullptr, false, &m_pChannel);

	if (hr != FMOD_OK) 
	{
		assert(nullptr);
		return false;
	}

	m_pChannel->setMode(_Loop);
	m_pChannel->setVolume(m_fVolume);

	if (FMOD_LOOP_OFF != _Loop) 
	{
		m_pChannel->setLoopCount(_LoopCount);
	}

	m_pChannel->setCallback(PlayCallBack);
	m_pChannel->setUserData(this);

	CCriSectionObject<CGameSound>();
	++m_iCurPlayingChannel;

	m_CurSoundKey = _SoundKey;
	m_bPause = false;
	return true;
}

void CSoundPlayer::CurPlaySoundStop()
{
	if(nullptr != m_pChannel)
		m_pChannel->stop();

}	
void CSoundPlayer::CurPlaySoundPaused(const bool& _Pause)
{
	if (nullptr == m_pChannel)
		return;

	if (m_bPause == _Pause)
		return;

	m_pChannel->setPaused(_Pause);
	m_bPause = _Pause;
}
FMOD_RESULT F_CALLBACK CSoundPlayer::PlayCallBack(FMOD_CHANNELCONTROL *channelcontrol
	, FMOD_CHANNELCONTROL_TYPE controltype
	, FMOD_CHANNELCONTROL_CALLBACK_TYPE callbacktype
	, void *commanddata1
	, void *commanddata2) 
{
	if (FMOD_CHANNELCONTROL_CALLBACK_END == controltype)
	{
		CSoundPlayer * pSound = nullptr;
		FMOD_Channel_GetUserData((FMOD_CHANNEL*)channelcontrol, (void**)&pSound);
		
		assert(pSound);

		if (pSound->m_bStop == false)
		{
			pSound->ChannelRelease();
			CCriSectionObject<CGameSound>();
			--m_iCurPlayingChannel;
		}
	}


	return FMOD_OK;
}
void CSoundPlayer::Stop()
{
	if (nullptr != m_pChannel)
		m_pChannel->stop();
}
void CSoundPlayer::ChannelRelease()
{
	if (nullptr != m_pChannel) 
	{
		m_pChannel = nullptr;
		m_CurSoundKey = _T("");
	}
}
void CSoundPlayer::SetVolume(const float& _Volume)
{
	m_fVolume = _Volume;

	if (nullptr != m_pChannel)
		m_pChannel->setVolume(m_fVolume);
}
const bool CSoundPlayer::ExistChannel()
{
	if (nullptr == m_pChannel)
		return false;

	return true;
}
const tstring CSoundPlayer::GetCurSoundKey()
{
	return m_CurSoundKey;
}
const bool CSoundPlayer::IsPause()
{
	return m_bPause;
}