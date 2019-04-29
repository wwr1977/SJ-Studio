#include "Precom.h"
#include "Script.h"
#include "ScriptMgr.h"
#include "NPCBase.h"
#include "GameMaster.h"
#include <GameSprite.h>

CScript::CScript()
	: m_pScriptMgr(nullptr), m_ScriptBackRen(nullptr)
	, m_ScriptPoster(nullptr), m_ScriptRen(nullptr)
	, m_iCurScriptPage(0), m_CurUpdateState(UPDATE_TYPING)
{
	m_vecEventNPC.clear();
}


CScript::~CScript()
{
	m_vecEventNPC.clear();
}
void CScript::Init() 
{
	m_ScriptPoster = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_UIBACK, 0.95f, true));
	m_ScriptPoster->SetRenPivot(Vec2{ -640.f,0.f });

	m_ScriptBackRen = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_UIBACK, 0.7f,true));
	m_ScriptBackRen->SetMaterial(D3DXCOLOR(0.1f, 0.1f, 0.1f,1.f));
	m_ScriptBackRen->MaterialOn();
	m_ScriptBackRen->SetSprite(_T("ColorBase"));
	m_ScriptBackRen->SetCustomSize(Vec2{ 960.f,300.f });

	m_ScriptPosterName = ACTOR->CreateCom<CFontRenderer>(RENDATA(LAYER_UI, 0.95f, true));
	m_ScriptPosterName->SetLeftStartPivot(POINT{ -430,90 });
	m_ScriptPosterName->SetFontMag(2.5f);
	m_ScriptPosterName->SetFontStyle(_T("캐릭라인폰트"));

	m_ScriptRen = ACTOR->CreateCom<CScriptRenderer>(RENDATA(LAYER_UI, 0.95f, true));
	m_ScriptRen->SetLeftStartPivot(POINT{ -400,10 });
	m_ScriptRen->SetFontMag(2.5f);
	m_ScriptRen->SetScriptMode();
	m_ScriptRen->SetTypingTime(0.05f);
	m_ScriptRen->SetLineInterval(50);
	m_ScriptRen->SetFontStyle(_T("큰볼드굴림"));
	

	POINT WndSize = CClientGlobal::MainWindow->WndSize();
	float YPos = -0.5f * WndSize.y + 175;
	TRANS->SetPos(Vec2{ 150.f, YPos });
}
void CScript::InitData(void* _Arg) 
{
	if (nullptr == _Arg)
		return;

	m_pScriptMgr = (CScriptMgr*)_Arg;
}
void CScript::Update()
{
	switch (m_CurUpdateState)
	{
	case CScript::UPDATE_TYPING:
		TypingUpdate();
		break;
	case CScript::UPDATE_HIDEWAIT:
		HideWaitUpdate();
		break;
	case CScript::MAXSCRIPT_UPDATE:
		break;
	}
	

}
void CScript::TypingUpdate()
{
	if (nullptr == m_ScriptRen)
		return;

	if (true == m_ScriptRen->IsTyping())
	{
		if (TRUE == KEYDOWN("Enter"))
			m_ScriptRen->SkipTyping();
	}
	else
	{
		if (TRUE == KEYDOWN("Enter"))
			if (false == CurScriptPageOn())
			{	
				if (nullptr != m_pScriptMgr)
					m_pScriptMgr->FinishScriptEvent();

				ActorOff();
			}
	}
}
void CScript::HideWaitUpdate()
{
	if (m_vecEventNPC.empty()) 
	{
		CurScriptPageOn();
		ScriptRenderEnable(true);
		m_CurUpdateState = UPDATE_TYPING;
		return;
	}

	for (size_t i = 0; i < m_vecEventNPC.size(); i++)
	{
		if (nullptr == m_vecEventNPC[i])
			continue;

		if (!m_vecEventNPC[i]->IsHideEventEnd())
			return;
	}

	CurScriptPageOn();
	ScriptRenderEnable(true);
	m_CurUpdateState = UPDATE_TYPING;
	m_vecEventNPC.clear();
}
void CScript::HideStandBy()
{
	for (size_t i = 0; i < m_vecEventNPC.size(); i++)
	{
		if (nullptr == m_vecEventNPC[i])
			continue;

		m_vecEventNPC[i]->NPCHideEvent();
	}
}
void CScript::SetScript(vector<tstring>* _Script)
{
	if (nullptr == _Script)
		return;

	m_vecCurScript.clear();
	m_vecCurScript.assign(_Script->begin(), _Script->end());
	m_iCurScriptPage = 0;
}
void CScript::SetScripterName(const tstring& _Name)
{
	if (nullptr != m_ScriptPoster) 
		m_ScriptPosterName->SetString(_Name);
}
bool CScript::CurScriptPageOn()
{
	if (m_iCurScriptPage >= m_vecCurScript.size())
		return false;

	tstring Temp = m_vecCurScript[m_iCurScriptPage];

	size_t i = Temp.find(_T("["));
	size_t j = Temp.find(_T("]:")); 
	
	if (-1 != i && -1 != j) 
	{
		Temp.assign(m_vecCurScript[m_iCurScriptPage], i + 1, j - 1);
		m_vecCurScript[m_iCurScriptPage].replace(i, j - i + 2, _T(""));
		
		m_ScriptPosterName->SetString(CClientGlobal::GameMaster->GetNPCKName(Temp));
		

		ChangeScriptPoster(CClientGlobal::GameMaster->GetNPCPoster(Temp));
		//Temp+= _T("Poster");
		//m_ScriptPoster->SetSprite(Temp);
		//m_ScriptPoster->On();
	}

	i = Temp.find_first_of(_T("#"));
	j = Temp.find_last_of(_T("#"));
	size_t k = Temp.find(_T(":"));
	// 특수 효과 스크립트 입력시
	if ( ( -1 != i && -1 != j) &&  i != j )
	{
		Temp.assign(m_vecCurScript[m_iCurScriptPage], i + 1, j - 1);
		// 현재 스크립트 텍스트를 출력하지 않는다면
		// 스크립트를 끄고 다음 스크립트로 넘긴다.
		if (false == SetEventScript(Temp)) 
		{
			++m_iCurScriptPage;
			ScriptRenderEnable(false);
			return true;
		}
		else 
		{
			++m_iCurScriptPage;
			return CurScriptPageOn();
		}
	}

	m_ScriptRen->SetScriptString(m_vecCurScript[m_iCurScriptPage]);
	m_ScriptRen->StandbyTyping();
	++m_iCurScriptPage;
	return true;
}
bool CScript::SetEventScript(const tstring& _Event)
{
	tstring EventScript = _Event;
	size_t i = EventScript.find(_T(':'));
	
	if (-1 == i)
		return true;

	tstring EventType;
	EventType.assign(EventScript,0, i);
	EventScript.replace(0, i + 1, _T(""));

	if (_T("Music") == EventType) 
	{
		CClientGlobal::GameMaster->MapSoundPauseEvent();
		m_pScriptMgr-> SetScriptSound(EventScript);
		m_pScriptMgr-> PlayScriptSoundEvent(true);
	}
	else if (_T("Hide") == EventType) 
	{
		m_vecEventNPC.clear();

		while (true)
		{
			size_t j = EventScript.find(',');
			tstring NPCName;
			
			if (-1 == j) 
			{
				NPCName = EventScript;
			
				if (nullptr != CClientGlobal::GameMaster->GetNPC(NPCName)) 
					m_vecEventNPC.push_back(CClientGlobal::GameMaster->GetNPC(NPCName));
				
				break;
			}

			NPCName.assign(EventScript, 0, j);
			EventScript.replace(0,j+1,_T(""));

			if(nullptr != CClientGlobal::GameMaster->GetNPC(NPCName))
				m_vecEventNPC.push_back(CClientGlobal::GameMaster->GetNPC(NPCName));
		}

		HideStandBy();
		m_CurUpdateState = UPDATE_HIDEWAIT;
		return false;
	}
	
	return true;
}
void CScript::ChangeScriptPoster(SPTR<CGameSprite> _Poster)
{
	if (nullptr == _Poster)
		m_ScriptPoster->Off();
	else 
	{
		m_ScriptPoster->SetSprite(_Poster);
		m_ScriptPoster->On();
	}
}
void CScript::ScriptRenderEnable(const bool& _Enable)
{
	m_ScriptBackRen ->Active(_Enable);
	m_ScriptPoster->Active(_Enable);
	m_ScriptPosterName->Active(_Enable);
	m_ScriptRen->Active(_Enable);
}
