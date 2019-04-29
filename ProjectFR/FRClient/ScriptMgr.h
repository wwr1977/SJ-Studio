#pragma once
#include <ObjBase.h>
class CScriptMgr : public CObjBase
{
	enum
	{
		SOUND_FADEIN,
		SOUND_FADEOUT,
		MAXSOUNDOPTION,
	}m_CurFade;


	class CGameMaster*								m_pGameMaster;
	CGameScene*										m_pMainScene;
	CSceneBuilder*									m_pMainSceneBuilder;
	
	map<tstring, vector<vector<tstring>>>			m_NPCBaseScript;
	map<tstring, vector<vector<tstring>>>			m_QuestScript;
	SPTR<class CScript>								m_ScriptWindow;

	SPTR<class CNPCBase>							m_CurEventNPC;

	// FinishScriptEvent가 발생한 순간 true가 되는 변수
	bool											m_bScriptEnd;
	SPTR<CSoundPlayer>								m_ScriptSpeaker;
	bool											m_bFadeUpdate;

	float											m_fAccTime;
	float											m_fMaxVol;
	float											m_fFadeInTime;
	float											m_fFadeOutTime;

private:
	void LoadNPCBaseScript();

public:
	void Init();
	void Update();
	void LastUpdate();

public:
	const bool IsScriptingEnd();
	const bool IsLastQuestScript(const tstring& _QuestKey,const UINT& _QuestCount);
	void FinishScriptEvent();

public:
	void ScriptWindowOn(const bool& _On);
	void SetBaseScript(const tstring& _NPCKey,const tstring& _NPCKName);
	void SetQuestScript(const tstring& _NPCName, const tstring& _QuestKey, const UINT& _QuestCount);
	void SetCurNPC(SPTR<CNPCBase> _NPC);
	void SetScriptPoster(SPTR<CGameSprite> _Poster);

public:
	void SetScriptSound(const tstring& _SoundKey);
	void SoundFadeInUpdate();
	void SoundFadeOutUpdate();
	void PlayScriptSoundEvent(const bool& _FadeIn);

public:
	CScriptMgr(CGameMaster* _Master,CGameScene* _Scene,CSceneBuilder* _Builder);
	~CScriptMgr();
};

