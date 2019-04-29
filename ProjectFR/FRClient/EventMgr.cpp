#include "Precom.h"
#include "EventMgr.h"
#include "ScriptMgr.h"
#include "GameMaster.h"
#include "NPCBase.h"
#include "Player.h"
#include "TurnMgr.h"
#include "InventoryMgr.h"
#include <GameSprite.h>
#include <Camera.h>

CEventMgr::CEventMgr()
	:m_bQuestScript(false), m_CurQuestKey(_T(""))
	,m_CurQuestCount(0), m_pGameMaster(nullptr)
	, m_pMainScene(nullptr),m_pMainBuilder(nullptr)
	, m_pScriptMgr(nullptr), m_CurNPCName(_T(""))
	, m_fEventAccTime(0.f)
	, m_pMainSceneCamera(nullptr)
	, m_CamMoveDir(Vec3{ 0.f,0.f,0.f })
	, m_PlayerMoveDir(Vec3{ 0.f,0.f,0.f })
	, m_bPlayerAuto(false)
{
	m_mapQuestData.clear();
	m_mapQuestText.clear();
	m_listBattleTrigerKey.clear();
}
CEventMgr::CEventMgr(CGameMaster* _Master
	, CGameScene* _Scene
	, CSceneBuilder* _Builder
	, CScriptMgr* _ScriptMgr)
	:m_bQuestScript(false), m_CurQuestKey(_T(""))
	, m_CurQuestCount(0), m_pGameMaster(_Master)
	, m_pMainScene(_Scene), m_pMainBuilder(_Builder)
	, m_pScriptMgr(_ScriptMgr), m_CurNPCName(_T(""))
	, m_fEventAccTime(0.f)
	, m_pMainSceneCamera(nullptr)
	, m_CamMoveDir(Vec3{0.f,0.f,0.f})
	, m_PlayerMoveDir(Vec3{ 0.f,0.f,0.f })
	, m_bPlayerAuto(false)
{
	m_mapQuestData.clear();
	m_mapQuestText.clear();
	m_listBattleTrigerKey.clear();

	if (nullptr != m_pMainScene)
		m_pMainSceneCamera = m_pMainScene->GetMainCamCom();
}
CEventMgr::~CEventMgr()
{
	m_mapQuestData.clear();
	m_mapQuestText.clear();
	m_listBattleTrigerKey.clear();
}
void CEventMgr::ResetMgr()
{

	map<tstring, QUESTDATA>::iterator Start = m_mapQuestData.begin();
	map<tstring, QUESTDATA>::iterator End = m_mapQuestData.end();

	for (;Start!=End; ++Start)
	{
		(*Start).second.bQuestEnd = false;
		(*Start).second.bQuestEvent = false;
		(*Start).second.CurQuestCount = 0;
	}

	map<tstring, QUESTTEXT>::iterator TStart = m_mapQuestText.begin();
	map<tstring, QUESTTEXT>::iterator TEnd = m_mapQuestText.end();

	for (; Start != End; ++Start)
		(*TStart).second.CurMissionIndex = 0;
	

	/*for_each(m_mapQuestData.begin(), m_mapQuestData.end(), [](QUESTDATA& _Data) 
	{
		_Data.bQuestEnd = false;
		_Data.bQuestEvent = false;
		_Data.CurQuestCount = 0;
	});*/

	/*for_each(m_mapQuestText.begin(), m_mapQuestText.end(), [](QUESTTEXT& _Text)
	{
		_Text.CurMissionIndex = 0;
	});*/
	
}
void CEventMgr::Init()
{
	QUESTDATA Temp = QUESTDATA(_T("Quest1")
		, _T("죽은 소녀를 위한 진혼곡")
		, _T("소녀의 소중한 물건을 찾아서...")
		, QUEST_STORY);

	Temp.bQuestEvent = false;
	//Temp.bQuestEvent = true;
	//Temp.CurQuestCount = 2;
	m_mapQuestData.insert(map<tstring,QUESTDATA>::value_type(Temp.QuestKey, Temp));

	Temp = QUESTDATA(_T("Quest2")
		, _T("마이스터를 꿈꾸는 소녀 ")
		, _T("모험을 떠난 견습 정비공")
		, QUEST_EVENT);

	Temp.bQuestEvent = false;
	//Temp.bQuestEvent = true;
	m_mapQuestData.insert(map<tstring, QUESTDATA>::value_type(Temp.QuestKey, Temp));

	Temp = QUESTDATA(_T("Quest3")
		, _T("최종 무곡")
		, _T("마지막을 향하여")
		, QUEST_STORY);

	Temp.bQuestEvent = false;
	//Temp.bQuestEvent = true;
	//Temp.CurQuestCount = 1;
	m_mapQuestData.insert(map<tstring, QUESTDATA>::value_type(Temp.QuestKey, Temp));


	m_listBattleTrigerKey.push_back(BATTLETRIGERKEY(_T("Quest1"), 2, CGameMaster::BATTLE_THIEF, BATTLE1_SOUND, 11));
	m_listBattleTrigerKey.push_back(BATTLETRIGERKEY(_T("Quest3"), 1, CGameMaster::BATTLE_DARKCOMMANDER, BATTLE2_SOUND, 7));
	m_listBattleTrigerKey.push_back(BATTLETRIGERKEY(_T("Quest3"), 2, CGameMaster::BATTLE_BOSS, BATTLE3_SOUND, 7));

	LoadQuestText();
}
void CEventMgr::LoadQuestText()
{
	CGameFile TextFile = CGameFile(_T("Data"), _T("QuestText.txt"));
	

	if (FALSE == TextFile.Open(_T("rt")))
	{
		assert(nullptr);
		return;
	}

	char Buf[MAXSTRING] = "";
	tstring StrBuf;
	int CheckNo;
	FILE* File = TextFile.GetFile();
	tstring QuestKey = _T("");
	QUESTTEXT TempQuestText;
	
	TempQuestText.QuestMission.clear();

	while (!feof(File))
	{
		fgets(Buf, MAXSTRING, File);

		StrBuf = CA2W(Buf);

		size_t Index = StrBuf.find(_T("[/"));

		if (-1 != Index)
		{
			Index = StrBuf.find_first_of(_T("]"));
			QuestKey.assign(StrBuf, 2, Index - 2);
			TempQuestText = QUESTTEXT();
			continue;
		}

		Index = StrBuf.find(_T("/]"));
		if (-1 != Index && -1 != StrBuf.find_first_of(_T("[")))
		{
			if ('\t' == StrBuf[StrBuf.size() - 1])
				StrBuf.pop_back();

			if (-1 != StrBuf.find(QuestKey)) 
			{
				sort(TempQuestText.QuestMission.begin(), TempQuestText.QuestMission.end(), [&](const QUESTMISSION& _Left, const QUESTMISSION& _Right)->bool {
					return _Left.MissionIndex < _Right.MissionIndex;
				});

				m_mapQuestText.insert(map<tstring, QUESTTEXT>::value_type(QuestKey, TempQuestText));
			}

			continue;
		}

		if (-1 != StrBuf.find(_T("</내용>")) ) 
		{
			if (-1 != StrBuf.find_first_of(_T("\n")))
				StrBuf.replace(StrBuf.find_first_of(_T("\n")), lstrlenW(_T("\n")), _T(""));

			StrBuf.replace(0, lstrlenW(_T("</내용>")), _T(""));
			if (-1 != StrBuf.find(_T("<내용/>")))
				StrBuf.replace(StrBuf.find(_T("<내용/>")), lstrlenW(_T("<내용/>")), _T(""));
			
			TempQuestText.QuestContents = StrBuf;
			continue;
		}
		else if (-1 != StrBuf.find(_T("</Contents>"))) 
		{
			if (-1 != StrBuf.find_first_of(_T("\n")))
				StrBuf.replace(StrBuf.find_first_of(_T("\n")), lstrlenW(_T("\n")), _T(""));
			StrBuf.replace(0, lstrlenW(_T("</Contents>")), _T(""));
			if (-1 != StrBuf.find(_T("<Contents/>")))
				StrBuf.replace(StrBuf.find(_T("<Contents/>")), lstrlenW(_T("<Contents/>")), _T(""));
			
			TempQuestText.QuestContents = StrBuf;
			continue;
		}

		if (-1 != StrBuf.find(_T("</보상>")))
		{
			if (-1 != StrBuf.find_first_of(_T("\n")))
				StrBuf.replace(StrBuf.find_first_of(_T("\n")), lstrlenW(_T("\n")), _T(""));
			StrBuf.replace(0, lstrlenW(_T("</보상>")), _T(""));
			if (-1 != StrBuf.find(_T("<보상/>")))
				StrBuf.replace(StrBuf.find(_T("<보상/>")), lstrlenW(_T("<보상/>")), _T(""));

			TempQuestText.QuestReward = StrBuf;
			continue;
		}
		else if (-1 != StrBuf.find(_T("</Reward>")))
		{
			if (-1 != StrBuf.find_first_of(_T("\n")))
				StrBuf.replace(StrBuf.find_first_of(_T("\n")), lstrlenW(_T("\n")), _T(""));
			StrBuf.replace(0, lstrlenW(_T("</Reward>")), _T(""));
			if (-1 != StrBuf.find(_T("<Reward/>")))
				StrBuf.replace(StrBuf.find(_T("<Reward/>")), lstrlenW(_T("<Reward/>")), _T(""));

			TempQuestText.QuestReward = StrBuf;
			continue;
		}


		if (-1 != StrBuf.find_first_of(_T("</"))) 
		{
			if (-1 != StrBuf.find_first_of(_T("\n")))
				StrBuf.replace(StrBuf.find_first_of(_T("\n")), lstrlenW(_T("\n")), _T(""));

			size_t Start = StrBuf.find_first_of(_T("</"));
			size_t End = StrBuf.find_first_of(_T(">"));
			tstring MissionKey;
			MissionKey.assign(StrBuf, Start, End - Start + 1);
			MissionKey.replace(Start, lstrlenW(_T("</")), _T(""));
			MissionKey.pop_back();
			
			StrBuf.replace(Start, End - Start + 1, _T(""));

			CheckNo = _wtoi(MissionKey.c_str());
			tstring TailTag = _T("<") + MissionKey + _T("/>");
		
			if (-1 != StrBuf.find_first_of(TailTag)) 
				StrBuf.replace(StrBuf.find_first_of(TailTag), lstrlenW(TailTag.c_str()), _T(""));

			TempQuestText.QuestMission.push_back(QUESTMISSION((UINT)CheckNo, StrBuf));
			continue;
		}

	}

}
const UINT CEventMgr::CheckQuestEvent(const NPC& _NPCNo)
{
	m_CurTrigerNPC = _NPCNo;
	switch (_NPCNo)
	{
	case NPC_GIRLS: 
	{
		QUESTDATA& Data = m_mapQuestData[_T("Quest1")];
		if (true == Data.bQuestEnd)
			break;

		if (false == Data.bQuestEvent)
		{
			Data.bQuestEvent = true;
			return -1;
		}
		else if (1 == Data.CurQuestCount)
		{
			m_bQuestScript = true;
			m_CurQuestKey = _T("Quest1");
			m_CurQuestCount = Data.CurQuestCount;
			return m_CurQuestCount;
		}
		else if (4 == Data.CurQuestCount) 
		{
			m_bQuestScript = true;
			m_CurQuestKey = _T("Quest1");
			m_CurQuestCount = Data.CurQuestCount;
			return m_CurQuestCount;
		}
	}
	break;
	case NPC_ELDER: 
	{
		if (false == m_mapQuestData[_T("Quest1")].bQuestEnd)
		{
			QUESTDATA& Data = m_mapQuestData[_T("Quest1")];

			if (false == Data.bQuestEvent)
				break;
		
			else if (0 == Data.CurQuestCount)
			{
				m_bQuestScript = true;
				m_CurQuestKey = _T("Quest1");
				m_CurQuestCount = Data.CurQuestCount;
				return m_CurQuestCount;
			}
			else if ( 5 == Data.CurQuestCount)
			{
				m_bQuestScript = true;
				m_CurQuestKey = _T("Quest1");
				m_CurQuestCount = Data.CurQuestCount;
				return m_CurQuestCount;
			}
		}
		else if (false == m_mapQuestData[_T("Quest3")].bQuestEvent) 
		{
			QUESTDATA& Data = m_mapQuestData[_T("Quest3")];

			Data.bQuestEvent = true;
			m_bQuestScript = true;
			m_CurQuestKey = _T("Quest3");
			m_CurQuestCount = Data.CurQuestCount;
			return m_CurQuestCount;

		}
	}
	break;
	case NPC_LID: 
	{
		QUESTDATA& Data = m_mapQuestData[_T("Quest2")];
		
		if (true == Data.bQuestEnd)
			break;

		
		if ( !Data.bQuestEvent  && 50000 <= CClientGlobal::InventoryMgr->GetGold()) 
		{
			Data.bQuestEvent = true;
			m_bQuestScript = true;
			m_CurQuestKey = _T("Quest2");
			m_CurQuestCount = Data.CurQuestCount;
			return m_CurQuestCount;

		}
		else if (1 == Data.CurQuestCount )
		{
			// 퀘스트 아이템이 존재할 경우 이벤트를 진행한다.
			if ( 0 >= CClientGlobal::InventoryMgr->RequestInventoryItemCount(24))
				break;

			CClientGlobal::InventoryMgr->DropItem(24);
			m_bQuestScript = true;
			m_CurQuestKey = _T("Quest2");
			m_CurQuestCount = Data.CurQuestCount;
			return m_CurQuestCount;
		}
		
	}
		break;
	case NPC_DESTROYER: 
	{
		if (false == m_mapQuestData[_T("Quest1")].bQuestEnd)
		{
			QUESTDATA& Data = m_mapQuestData[_T("Quest1")];

			if (false == Data.bQuestEvent)
				break;

			else if (2 == Data.CurQuestCount)
			{
				m_bQuestScript = true;
				m_CurQuestKey = _T("Quest1");
				m_CurQuestCount = Data.CurQuestCount;
				return m_CurQuestCount;
			}
		}
	}
	break;
	case NPC_GOLBEZ:
	{
		if (true == m_mapQuestData[_T("Quest3")].bQuestEvent)
		{
			QUESTDATA& Data = m_mapQuestData[_T("Quest3")];

			if (1 == Data.CurQuestCount || 2 == Data.CurQuestCount)
			{
				m_bQuestScript = true;
				m_CurQuestKey = _T("Quest3");
				m_CurQuestCount = Data.CurQuestCount;
				return m_CurQuestCount;
			}
		}
	}
	break;
	}

	m_bQuestScript = false;
	m_CurQuestKey = _T("");
	m_CurNPCName = _T("");
	m_CurQuestCount = -1;
	return -1;
}
void CEventMgr::SetNPCName(const tstring& _NPCName)
{
	m_CurNPCName = _NPCName;
}
void CEventMgr::SettingQuestScript()
{
	if (nullptr == m_pScriptMgr)
		return;

	m_pScriptMgr->SetQuestScript(m_CurNPCName, m_CurQuestKey, m_CurQuestCount);

}
const tstring CEventMgr::GetQuestTitle(const tstring& _QuestKey)
{
	map<tstring, QUESTDATA>::iterator Find = m_mapQuestData.find(_QuestKey);
	
	if (Find == m_mapQuestData.end())
		return _T("");

	return Find->second.QuestTitle;
}
const tstring CEventMgr::GetQuestSubTitle(const tstring& _QuestKey)
{
	map<tstring, QUESTDATA>::iterator Find = m_mapQuestData.find(_QuestKey);

	if (Find == m_mapQuestData.end())
		return _T("");

	return Find->second.QuestSubTitle;
}
const QUESTDATA CEventMgr::GetCurQuestData(const tstring& _QuestKey)
{
	map<tstring, QUESTDATA>::iterator Find = m_mapQuestData.find(_QuestKey);

	if (Find == m_mapQuestData.end())
		return QUESTDATA();

	return Find->second;
}
const QUESTTEXT CEventMgr::GetQuestText(const tstring& _QuestKey)
{
	map<tstring, QUESTTEXT>::iterator Find = m_mapQuestText.find(_QuestKey);

	if (Find == m_mapQuestText.end())
		return QUESTTEXT();

	return Find->second;
}
const size_t CEventMgr::GetQuestCount() 
{
	return m_mapQuestData.size();
}
const bool CEventMgr::QuestScriptEndEvent()
{
	if (false == m_bQuestScript)
		return false;

	if (true == CheckBattleTriger())
	{
		BATTLETRIGERKEY Key = GetCurBattleTrigerKey();
		m_pGameMaster->BattleSceneChangeEvent(Key.QuestKey, Key.QuestCount);
		m_pGameMaster->SettingBattleMonster((CGameMaster::BATTLECOUNT)Key.BattleMonster);
		CClientGlobal::TurnMgr->SetCurBattleBgmIndex(Key.BattleBgmNo);
		CClientGlobal::TurnMgr->ChangeBackGround(Key.BackGroundNo);
		CClientGlobal::StoryBattle = true;
	}

	if (nullptr != m_pScriptMgr)
	{
		if (true == m_pScriptMgr->IsLastQuestScript(m_CurQuestKey, m_CurQuestCount))
		{
			m_mapQuestData[m_CurQuestKey].bQuestEnd = true;
			m_pGameMaster->FinishQuestEvent(m_mapQuestData[m_CurQuestKey].QuestTitle);
			if (_T("Quest2") == m_CurQuestKey) 
			{
				m_pGameMaster->OpenCharacterLine(LID);
				m_pGameMaster->EnableNPC(NPC_LID,false);
			}
		}
		else 
		{
			UINT& CurCount = m_mapQuestData[m_CurQuestKey].CurQuestCount;
			++CurCount;

			map<tstring, QUESTTEXT>::iterator Find = m_mapQuestText.find(m_CurQuestKey);
			if (m_mapQuestText.end() != Find)
			{
				QUESTTEXT& Text = Find->second;   //.find(m_CurQuestKey);
				UINT& Index = Text.CurMissionIndex;
				if (!Text.IsLastMission() && CurCount >= Text.GetNextMissionIndex())
					Text.CurMissionIndex++;
			}
		}
	}


	
	m_bQuestScript = false;
	m_CurQuestKey = _T("");
	m_CurQuestCount = -1;
	return true;
}
const bool CEventMgr::CheckBattleTriger()
{
	list<BATTLETRIGERKEY>::iterator Start = m_listBattleTrigerKey.begin();
	list<BATTLETRIGERKEY>::iterator End = m_listBattleTrigerKey.end();

	for (;Start!= End; ++Start)
	{
		if (m_CurQuestKey == Start->QuestKey &&
			m_CurQuestCount == Start->QuestCount)
			return true;
	}

	return false;
}
const BATTLETRIGERKEY CEventMgr::GetCurBattleTrigerKey()
{
	list<BATTLETRIGERKEY>::iterator Start = m_listBattleTrigerKey.begin();
	list<BATTLETRIGERKEY>::iterator End = m_listBattleTrigerKey.end();

	for (; Start != End; ++Start)
	{
		if (m_CurQuestKey == Start->QuestKey &&
			m_CurQuestCount == Start->QuestCount)
			return *Start;
	}

	return BATTLETRIGERKEY();
}
// 승리 이후 씬이 전환되는 순간에 호출
void CEventMgr::SceneChangeEvent(const tstring& _PrevQuestKey, const UINT& _PrevQuestCount)
{

	if (_PrevQuestKey == _T("Quest1"))
	{
		if (nullptr != m_pMainSceneCamera)
			m_pMainSceneCamera->SetCamMode(CAM_MODE_TRACKING, CClientGlobal::MainPlayer->GetTransForm());

	}
	else if (_PrevQuestKey == _T("Quest3"))
	{
		if (nullptr != m_pMainSceneCamera) 
			m_pMainSceneCamera->SetCameraPos(Vec3{ 0.f,646.f,5.f });
					
		if (1 == _PrevQuestCount) 
		{
			for (UINT i = NPC_BERAMODE; i <= NPC_MAMON; i++)
				m_pGameMaster->ChangeNPCDyingAni(i);
		}
		else if (2 == _PrevQuestCount)
			m_pGameMaster->ChangeNPCDyingAni(NPC_GOLBEZ);

	}
}
// 씬 변경후 페이드 아웃효과가 끝나는 시점에 호출
void CEventMgr::BattleWinEvent(const tstring& _PrevQuestKey, const UINT& _PrevQuestCount)
{
	if (_PrevQuestKey == _T("Quest1"))
	{
		if (_PrevQuestCount == 2)
		{
			QUESTDATA& Data = m_mapQuestData[_T("Quest1")];

			m_bQuestScript = true;
			m_CurQuestCount = Data.CurQuestCount;
			m_CurQuestKey = Data.QuestKey;
			m_pScriptMgr->SetQuestScript(_T("Destroyer"), m_CurQuestKey, m_CurQuestCount);

			m_pScriptMgr->SetScriptPoster(nullptr);
			m_pScriptMgr->SetCurNPC(CClientGlobal::GameMaster->GetNPC(NPC_DESTROYER));
			m_pScriptMgr->ScriptWindowOn(true);
		}

	}
	else if (_PrevQuestKey == _T("Quest3"))
	{
		QUESTDATA& Data = m_mapQuestData[_T("Quest3")];

		m_bQuestScript = true;
		m_CurQuestCount = Data.CurQuestCount;
		m_CurQuestKey = Data.QuestKey;

		if (1 == _PrevQuestCount)
		{
			m_pScriptMgr->SetQuestScript(_T("Beramode"), m_CurQuestKey, m_CurQuestCount);
			m_pScriptMgr->SetScriptPoster(nullptr);
			m_pScriptMgr->SetCurNPC(CClientGlobal::GameMaster->GetNPC(NPC_GOLBEZ));
		}
		else if (2 == _PrevQuestCount)
		{
			m_pScriptMgr->SetQuestScript(_T("Golbez"), m_CurQuestKey, m_CurQuestCount);
			m_pScriptMgr->SetScriptPoster(CClientGlobal::GameMaster->GetNpcPoster(NPC_GOLBEZ));
			m_pScriptMgr->SetCurNPC(CClientGlobal::GameMaster->GetNPC(NPC_GOLBEZ));
			
			if (nullptr != m_pMainSceneCamera)
			m_pMainSceneCamera->SetCamMode(CAM_MODE_TRACKING, CClientGlobal::MainPlayer->GetTransForm());
		}

		m_pScriptMgr->ScriptWindowOn(true);

	}

}
void CEventMgr::QuestUpdateStandBy(const tstring& _NPC)
{
	m_fEventAccTime = 0.f;


	// 카메라 움직임
	if (_NPC == _T("Destroyer"))
	{
		m_CamMoveDir = Vec3{ 630.f,-384.f,0.f } -m_pMainSceneCamera->GetCameraPos();
		m_CamMoveDir.z = 0.f;
		m_bPlayerAuto = false;
		D3DXVec3Normalize(&m_CamMoveDir, &m_CamMoveDir);

		m_PlayerMoveDir = Vec3{ 1.f,0.f,0.f };
		m_pMainSceneCamera->SetAutoMoveData(Vec3{ 618.f,-384.f,5.f }, m_CamMoveDir, 200.f);
	}
	else if(_NPC == _T("Golbez"))
	{
		m_CamMoveDir = Vec3{ 0.f,646.f,0.f } -m_pMainSceneCamera->GetCameraPos();
		m_CamMoveDir.z = 0.f;
		m_bPlayerAuto = false;
		D3DXVec3Normalize(&m_CamMoveDir, &m_CamMoveDir);

		m_PlayerMoveDir = Vec3{ 0.f,1.f,0.f };
		m_pMainSceneCamera->SetAutoMoveData(Vec3{ 0.f,646.f,5.f }, m_CamMoveDir, 200.f);
	}

}
function<void(void)> CEventMgr::GetEventUpdateFunc(const tstring& _NPC)
{
	m_fEventAccTime = 0.f;

	if (_NPC == _T("Destroyer")) 
		return bind(&CEventMgr::Quest1BattleEvent, this);
	else if(_NPC == _T("Golbez"))
		return bind(&CEventMgr::Quest3BattleEvent, this);

	return nullptr;
}
void CEventMgr::Quest1BattleEvent() 
{
	if (m_fEventAccTime >= 5.f) 
	{
		m_fEventAccTime = 0.f;
		m_pGameMaster->FinishEventUpdate(_T("Destroyer"));
		return;
	}

	// 플레이어 움직임
	if (m_fEventAccTime >= 2.f && !m_bPlayerAuto)
	{
		m_bPlayerAuto = true;
		CClientGlobal::MainPlayer->SetActorPos(Vec3{ -100.f,-444.f,0.f });
		CClientGlobal::MainPlayer->SetAutoMoveData(CPlayer::STATE_RUN, Vec3{ 468.f ,-444.f,0.f }, Vec3{ 1.f,0.f,0.f });
	}
	m_fEventAccTime += DELTATIME;
}
void CEventMgr::Quest3BattleEvent() 
{
	if (m_fEventAccTime >= 5.f)
	{
		m_fEventAccTime = 0.f;
		m_pGameMaster->FinishEventUpdate(_T("Golbez"));
		return;
	}

	// 플레이어 움직임
	if (m_fEventAccTime >= 2.f && !m_bPlayerAuto)
	{
		m_bPlayerAuto = true;
		CClientGlobal::MainPlayer->SetActorPos(Vec3{ 0.f,156.f,5.f });
		CClientGlobal::MainPlayer->SetAutoMoveData(CPlayer::STATE_RUN, Vec3{ 0.f,446.f,5.f }, Vec3{ 0.f,1.f,0.f });
	}

	m_fEventAccTime += DELTATIME;
}
const UINT CEventMgr::GetQuestCount(const tstring& _QuestKey) 
{
	map<tstring, QUESTDATA>::iterator Find = m_mapQuestData.find(_QuestKey);

	if (Find == m_mapQuestData.end())
		return false;


	return Find->second.CurQuestCount;
}
const bool CEventMgr::IsQuestStart(const tstring& _QuestKey)
{
	map<tstring, QUESTDATA>::iterator Find = m_mapQuestData.find(_QuestKey);

	if (Find == m_mapQuestData.end())
		return false;

	if (Find->second.bQuestEnd)
		return false;

	return Find->second.bQuestEvent;
}