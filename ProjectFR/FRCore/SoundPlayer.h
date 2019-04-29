#pragma once
#include "GameCom.h"

class CSoundPlayer :public CGameCom
{
public:
	friend CResourceMgr;


private:
	static FMOD::System*	m_FSys;
	static int m_iCurPlayingChannel;
	static void SystemInit(FMOD::System* _pSys);
	static FMOD_RESULT F_CALLBACK PlayCallBack(FMOD_CHANNELCONTROL *channelcontrol
		, FMOD_CHANNELCONTROL_TYPE controltype
		, FMOD_CHANNELCONTROL_CALLBACK_TYPE callbacktype
		, void *commanddata1
		, void *commanddata2);

public:
	FMOD::Channel*		m_pChannel;
	bool				m_bStop;
	bool				m_bPause;
	float				m_fVolume;
	tstring				m_CurSoundKey;
	
	bool PlaySound(const tstring& _SoundKey, FMOD_MODE _Loop = FMOD_LOOP_OFF, unsigned int _LoopCount = -1);
	void CurPlaySoundStop();
	void CurPlaySoundPaused(const bool& _Pause);
	void Stop();
	void ChannelRelease();
	void SetVolume(const float& _Volume);
	const bool ExistChannel();
	const tstring GetCurSoundKey();
	const bool IsPause();
public:
	CSoundPlayer();
	~CSoundPlayer();
};

