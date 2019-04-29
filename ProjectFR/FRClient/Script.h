#pragma once
#include <Logic.h>

class CScript : public CLogic
{
	enum SCRIPT_UPDATE
	{
		UPDATE_TYPING,
		UPDATE_HIDEWAIT,
		MAXSCRIPT_UPDATE
	}m_CurUpdateState;

private:
	class CScriptMgr*					m_pScriptMgr;
	SPTR<CFixRenderer>					m_ScriptBackRen;
	SPTR<CFixRenderer>					m_ScriptPoster;
	SPTR<CFontRenderer>					m_ScriptPosterName;
	SPTR<CScriptRenderer>				m_ScriptRen;
	vector<tstring>						m_vecCurScript;
	int									m_iCurScriptPage;


	bool								m_bFadeUpdate;
	float								m_fAccTime;
	float								m_fMaxVol;
	float								m_fFadeTime;


	vector<SPTR<class CNPCBase>>		m_vecEventNPC;
public:
	void Init();
	void InitData(void* _Arg);
	void Update();

public:
	void TypingUpdate();
	void HideWaitUpdate();		// NPC의 이벤트 종료 대기 함수
	void HideStandBy();

public:
	void SetScript(vector<tstring>* _Script);
	void SetScripterName(const tstring& _Name);
	bool CurScriptPageOn();
	// 현재 스크립트 텍스트를 출력하지 않아야 할경우에 false 를 출력
	bool SetEventScript(const tstring& _EventScript);
	void ChangeScriptPoster(SPTR<CGameSprite> _Poster);

public:
	void ScriptRenderEnable(const bool& _Enable);

public:
	CScript();
	~CScript();
};

