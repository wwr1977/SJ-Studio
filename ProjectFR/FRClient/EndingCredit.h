#pragma once
#include <Logic.h>
class CEndingCredit : public CLogic
{
	enum 
	{
		ENDING_UPDATE,
		ENDING_WAIT,

	}m_CurUpdate;

private:
	class CEndingSceneBuilder*			m_pEndingBuilder;
	SPTR<CFixRenderer>					m_EndingRen;
	SPTR<CFixRenderer>					m_CreditRen;
	SPTR<CGameMultiSprite>				m_EndingSprite;
	SPTR<CGameMultiSprite>				m_CreditSprite;
	SPTR<CSoundPlayer>					m_EndingSpeaker;
	float								m_fAccTime;
	float								m_fFrameTime;
	float								m_fMaxVol;
	size_t								m_CurIndex;
	bool								m_bEndingUpdate;
	

public:
	void Init();
	void InitData(void* _pEndingBuilder);
	void Update();

public:
	void EndingCreditUpdate();
	void EndingWaitUpdate();

public:
	void StandBy();

public:
	CEndingCredit();
	~CEndingCredit();
};

