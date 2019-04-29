#include "Precom.h"
#include "ScriptMgr.h"
#include "Script.h"
#include "GameMaster.h"
#include "NPCBase.h"
#include "Player.h"
#include <SceneBuilder.h>
#include <GameSprite.h>

CScriptMgr::CScriptMgr(CGameMaster* _Master, CGameScene* _Scene, CSceneBuilder* _Builder)
	:m_pMainScene(_Scene), m_pMainSceneBuilder(_Builder)
	, m_ScriptWindow(nullptr), m_pGameMaster(_Master)
	, m_bScriptEnd(false), m_CurEventNPC(nullptr)
	, m_ScriptSpeaker(nullptr)
	, m_bFadeUpdate(false)
	, m_fAccTime(0.f)
	, m_fMaxVol(0.3f)
	, m_fFadeInTime(4.f)
	, m_fFadeOutTime(2.f)
	, m_CurFade(MAXSOUNDOPTION)
{
	m_NPCBaseScript.clear();
	m_QuestScript.clear();
}


CScriptMgr::~CScriptMgr()
{
	m_NPCBaseScript.clear();
	m_QuestScript.clear();
	m_ScriptSpeaker = nullptr;
}
void CScriptMgr::LoadNPCBaseScript()
{
	CGameFile ScriptFile = CGameFile(_T("Data"), _T("NPCScript.txt"));
	CGameFile QuestFile = CGameFile(_T("Data"), _T("QuestScript.txt"));
	
	if (FALSE == ScriptFile.Open(_T("rt")))
	{
		assert(nullptr);
		return;
	}

	char Buf[MAXSTRING] = "";
	tstring StrBuf;
	int CheckNo;
	FILE* File = ScriptFile.GetFile();
	tstring NPCName = _T("");
	vector<vector<tstring>> TempScriptVec;
	vector<tstring> TempScript;

	TempScriptVec.clear();
	TempScript.clear();

	while (!feof(File))
	{
		fgets(Buf, MAXSTRING, File);

		StrBuf = CA2W(Buf);

		size_t Index = StrBuf.find(_T("[/"));

		if (-1 != Index)
		{
			Index = StrBuf.find_first_of(_T("]"));
			NPCName.assign(StrBuf, 2, Index - 2);
			TempScriptVec.clear();
			continue;
		}

		Index = StrBuf.find(_T("/]"));
		if (-1 != Index && -1 != StrBuf.find_first_of(_T("[")))
		{
			if ('\t' == StrBuf[StrBuf.size()-1])
				StrBuf.pop_back();

			TCHAR Temp[MAXKEYLENGTH];
			_stprintf_s(Temp, MAXKEYLENGTH, _T("[%s/]"), NPCName.c_str());

			if (-1 != StrBuf.find(Temp))
				m_NPCBaseScript.insert(map<tstring, vector<vector<tstring>>>::value_type(NPCName, TempScriptVec));

			continue;
		}

		Index = StrBuf.find(_T("</"));
		if (-1 != Index)
		{
			StrBuf.replace(0, 2, _T(""));
			size_t Index2 = StrBuf.find_first_of(_T(">"));
			StrBuf.replace(Index2, 1, _T(""));

			CheckNo = _wtoi(StrBuf.c_str());
			TempScript.clear();

			continue;
		}
		Index = StrBuf.find(_T("<"));
		if (-1 != Index&& -1 != StrBuf.find_first_of(_T("/>")))
		{
			StrBuf.replace(0, 1, _T(""));
			size_t Index2 = StrBuf.find_first_of(_T("/>"));
			StrBuf.replace(Index2, 2, _T(""));

			if (CheckNo == _wtoi(StrBuf.c_str()))
				TempScriptVec.push_back(TempScript);

			TempScript.clear();

			continue;
		}

		TempScript.push_back(StrBuf);
	}


	if (FALSE == QuestFile.Open(_T("rt")))
	{
		assert(nullptr);
		return;
	}

	TempScriptVec.clear();
	TempScript.clear();

	File = QuestFile.GetFile();

	while (!feof(File))
	{
		fgets(Buf, MAXSTRING, File);

		StrBuf = CA2W(Buf);

		size_t Index = StrBuf.find(_T("[/"));

		if (-1 != Index)
		{
			Index = StrBuf.find_first_of(_T("]"));
			NPCName.assign(StrBuf, 2, Index - 2);
			TempScriptVec.clear();
			continue;
		}

		Index = StrBuf.find(_T("/]"));
		if (-1 != Index && -1 != StrBuf.find_first_of(_T("[")))
		{
			if ('\t' == StrBuf[StrBuf.size() - 1])
				StrBuf.pop_back();

			TCHAR Temp[MAXKEYLENGTH];
			_stprintf_s(Temp, MAXKEYLENGTH, _T("[%s/]"), NPCName.c_str());

			if (-1 != StrBuf.find(Temp))
				m_QuestScript.insert(map<tstring, vector<vector<tstring>>>::value_type(NPCName, TempScriptVec));

			continue;
		}

		Index = StrBuf.find(_T("</"));
		if (-1 != Index)
		{
			StrBuf.replace(0, 2, _T(""));
			size_t Index2 = StrBuf.find_first_of(_T(">"));
			StrBuf.replace(Index2, 1, _T(""));

			CheckNo = _wtoi(StrBuf.c_str());
			TempScript.clear();

			continue;
		}
		Index = StrBuf.find(_T("<"));
		if (-1 != Index&& -1 != StrBuf.find_first_of(_T("/>")))
		{
			StrBuf.replace(0, 1, _T(""));
			size_t Index2 = StrBuf.find_first_of(_T("/>"));
			StrBuf.replace(Index2, 2, _T(""));

			if (CheckNo == _wtoi(StrBuf.c_str()))
				TempScriptVec.push_back(TempScript);

			TempScript.clear();

			continue;
		}

		TempScript.push_back(StrBuf);
	}
}

void CScriptMgr::Init()
{
	m_ScriptSpeaker = m_pGameMaster->CreateActObject(_T("ScriptSpeaker"))->CreateCom<CSoundPlayer>();
	m_ScriptSpeaker->SetVolume(0.f);

	LoadNPCBaseScript();

	m_ScriptWindow = CActObject::CreateActObject(_T("ScriptWindow"), m_pMainScene)->CreateCom<CScript>(this);
	m_ScriptWindow->ActorOff();
}
void CScriptMgr::Update()
{
	if (!m_bFadeUpdate)
		return;

	switch (m_CurFade)
	{
	case CScriptMgr::SOUND_FADEIN:
		SoundFadeInUpdate();
		break;
	case CScriptMgr::SOUND_FADEOUT:
		SoundFadeOutUpdate();
		break;
	}

	m_fAccTime += DELTATIME;
}
void CScriptMgr::LastUpdate()
{
	if (true == m_bScriptEnd)
	{
		m_bScriptEnd = false;
		CNPCBase::bScriptEvent = true;
		CClientGlobal::MainPlayer->PlayerPause(false);
	}

}
const bool CScriptMgr::IsScriptingEnd()
{
	return m_bScriptEnd;
}
const bool CScriptMgr::IsLastQuestScript(const tstring& _QuestKey, const UINT& _QuestCount)
{
	map<tstring, vector<vector<tstring>>>::iterator  Find = m_QuestScript.find(_QuestKey);

	if (Find == m_QuestScript.end()) 
	{
		assert(nullptr);
		return false;
	}

	if (_QuestCount >= Find->second.size() - 1)
		return true;

	return false;
}
void CScriptMgr::FinishScriptEvent()
{
	if (_T("") != m_ScriptSpeaker->GetCurSoundKey())
		PlayScriptSoundEvent(false);

	CClientGlobal::GameMaster->FinishScriptEvent();

	if (nullptr != m_CurEventNPC)
		m_CurEventNPC->InitDefaultDir();

	m_bScriptEnd = true;
}
void CScriptMgr::ScriptWindowOn(const bool& _On)
{
	if (nullptr == m_ScriptWindow)
		return;

	if (true == _On) 
	{
		m_ScriptWindow->ActorOn();
		m_ScriptWindow->CurScriptPageOn();
	}
	else 
		m_ScriptWindow->ActorOff();
}
void CScriptMgr::SetBaseScript(const tstring& _NPCKey, const tstring& _NPCKName)
{
	map<tstring, vector<vector<tstring>>>::iterator Find = m_NPCBaseScript.find(_NPCKey);

	if (Find == m_NPCBaseScript.end())
		return;

	int Ran = 0;
	
	if(1 < Find->second.size())
		Ran = CSJRandom::RandomInt(0, (int)Find->second.size() - 1);

	if (nullptr != m_ScriptWindow) 
	{
		m_ScriptWindow->SetScript(&(Find->second[Ran]));
		m_ScriptWindow->SetScripterName(_NPCKName);
	}
}
void CScriptMgr::SetQuestScript(const tstring& _NPCName, const tstring& _QuestKey, const UINT& _QuestCount)
{
	map<tstring, vector<vector<tstring>>>::iterator Find = m_QuestScript.find(_QuestKey);

	if (Find == m_QuestScript.end() || _QuestCount >= Find->second.size())
		return;


	if (nullptr != m_ScriptWindow)
	{
		m_ScriptWindow->SetScript(&(Find->second[_QuestCount]));
		m_ScriptWindow->SetScripterName(CClientGlobal::GameMaster->GetNPCKName(_NPCName));
	}
}
void CScriptMgr::SetCurNPC(SPTR<CNPCBase> _NPC)
{
	if (nullptr != _NPC)
		m_CurEventNPC = _NPC;
}
void CScriptMgr::SetScriptPoster(SPTR<CGameSprite> _Poster)
{
	if (nullptr == m_ScriptWindow )
		return;

	m_ScriptWindow->ChangeScriptPoster(_Poster);
}

void CScriptMgr::SetScriptSound(const tstring& _SoundKey)
{
	if (nullptr == m_ScriptSpeaker)
		return;

	if (m_ScriptSpeaker->GetCurSoundKey() == _SoundKey)
		return;

	m_ScriptSpeaker->PlaySound(_SoundKey, FMOD_LOOP_NORMAL);
}
void CScriptMgr::SoundFadeInUpdate()
{
	if (m_fAccTime >= m_fFadeInTime)
	{
		m_bFadeUpdate = false;
		m_ScriptSpeaker->SetVolume(m_fMaxVol);
		return;
	}

	float t = powf(m_fAccTime / m_fFadeInTime, 3.f);
	m_ScriptSpeaker->SetVolume(t*m_fMaxVol);
}
void CScriptMgr::SoundFadeOutUpdate()
{
	if (m_fAccTime >= m_fFadeOutTime)
	{
		m_bFadeUpdate = false;
		m_ScriptSpeaker->SetVolume(0.f);
		m_ScriptSpeaker->CurPlaySoundPaused(true);
		return;
	}

	float t = 1.f - powf(m_fAccTime / m_fFadeOutTime, 2.f);
	m_ScriptSpeaker->SetVolume(max(0.f, t*m_fMaxVol));
}
void CScriptMgr::PlayScriptSoundEvent(const bool& _FadeIn)
{
	m_bFadeUpdate = true;
	m_fAccTime = 0.f;

	if (_FadeIn)
	{
		m_CurFade = SOUND_FADEIN;
		m_ScriptSpeaker->SetVolume(0.f);
		m_ScriptSpeaker->CurPlaySoundPaused(false);
	}
	else
	{
		m_CurFade = SOUND_FADEOUT;
		m_ScriptSpeaker->SetVolume(m_fMaxVol);
	}
}