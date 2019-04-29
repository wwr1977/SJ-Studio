#pragma once
#include <BaseState.h>

class CBattleUnit;
class CTurnMgr;
class CEffectMgr;
class CBasePhase : public CBaseState
{
protected:
	CTurnMgr*					m_pTurnMgr;
	CEffectMgr*					m_pEffectMgr;
	CGameScene*					m_pBattleScene;
	

	// ���� Ÿ������ ������ ���ֵ��� �����͸� ����
	vector<SPTR<CBattleUnit>>*	m_pCurTarget;


	// ��ų �׼� ��ũ��Ʈ�� ���õ� �������� TurnMgr���� ȹ��
	vector<SoundData>*			m_pSoundVector;
	vector<SKILLEFFECTDATA>*	m_pSkillEffect;

	queue<ANIMATIONSCRIPT>*		m_pAniQueue;
	queue<MOVEDATA>*			m_pMoveQueue;
	queue<tstring>*				m_pSoundQueue;
	queue<SKILLEFFECTDATA>*		m_pSkillEffectQueue;
	queue<SCENEEFFECTDATA>*		m_pSceneEffectQueue;

	int*						m_pAniScriptCount;
	int*						m_pMoveScriptCount;
	int*						m_pSceneEffectCount;

	float*						m_pSkillActionTime;
	
	/*Vec2*						m_pInitPos;
	int*						m_pInitPosMode;
	size_t*						m_pInitPosIndex;*/
	
	size_t*						m_pInitPosMode;
	Vec2*						m_pInitPos;
	Vec2*						m_pInitPosPivot;

	SKILLDATA*					m_pCurSkillData;
	LEVELDATA*					m_pCurSkillLevelData;
	list<HITDATA>*				m_pHitData;
	queue<HITDATA>*				m_pHitDataQueue;
	

private:
	void Init(void* _TurnMgr);
	virtual void InitPhase();

public:
	CTurnMgr* GetTurnMgr();

public:
	CBasePhase();
	~CBasePhase();
};

