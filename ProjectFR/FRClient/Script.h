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
	void HideWaitUpdate();		// NPC�� �̺�Ʈ ���� ��� �Լ�
	void HideStandBy();

public:
	void SetScript(vector<tstring>* _Script);
	void SetScripterName(const tstring& _Name);
	bool CurScriptPageOn();
	// ���� ��ũ��Ʈ �ؽ�Ʈ�� ������� �ʾƾ� �Ұ�쿡 false �� ���
	bool SetEventScript(const tstring& _EventScript);
	void ChangeScriptPoster(SPTR<CGameSprite> _Poster);

public:
	void ScriptRenderEnable(const bool& _Enable);

public:
	CScript();
	~CScript();
};

