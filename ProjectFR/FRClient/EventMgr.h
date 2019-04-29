#pragma once

struct QUESTDATA 
{
	tstring					QuestKey;
	tstring					QuestTitle;				// (진짜 퀘스트 이름)퀘스트 종료후 공개
	tstring					QuestSubTitle;			// (퀘스트 진행중의 이름)퀘스트 수행중에 공개
	UINT					CurQuestCount;			// 현재 퀘스트 진행 카운트
	bool					bQuestEvent;			// 퀘스트를 진행중인지 여부
	bool					bQuestEnd;				// 퀘스트가 종료되었는지 여부
	QUESTTYPE				QuestType;

public:
	void operator=(const QUESTDATA& _Other) 
	{
		QuestKey = _Other.QuestKey;
		QuestTitle = _Other.QuestTitle;
		QuestSubTitle = _Other.QuestSubTitle;
		CurQuestCount = _Other.CurQuestCount;
		bQuestEvent = _Other.bQuestEvent;
		bQuestEnd = _Other.bQuestEnd;
		QuestType = _Other.QuestType;
	}

	QUESTDATA() {}
	QUESTDATA(const tstring& _QuestKey, const tstring& _QuestTitle, const tstring& _SubTitle)
		: QuestKey(_QuestKey), QuestTitle(_QuestTitle), QuestSubTitle(_SubTitle)
		, CurQuestCount(0), bQuestEvent(false), bQuestEnd(false)
		, QuestType(QUEST_ETC)
	{}
	QUESTDATA(const tstring& _QuestKey, const tstring& _QuestTitle, const tstring& _SubTitle
			,const QUESTTYPE& _Type)
		: QuestKey(_QuestKey), QuestTitle(_QuestTitle), QuestSubTitle(_SubTitle)
		, CurQuestCount(0), bQuestEvent(false), bQuestEnd(false)
		, QuestType(_Type)
	{}
	QUESTDATA(const QUESTDATA& _Other)
		: QuestKey(_Other.QuestKey), QuestTitle(_Other.QuestTitle), QuestSubTitle(_Other.QuestSubTitle)
		, CurQuestCount(_Other.CurQuestCount), bQuestEvent(_Other.bQuestEvent), bQuestEnd(_Other.bQuestEnd)
		, QuestType(_Other.QuestType)
	{}
	~QUESTDATA() {}
};

struct BATTLETRIGERKEY 
{
	tstring				QuestKey;
	UINT				QuestCount;
	UINT				BattleMonster;
	UINT				BattleBgmNo;
	UINT				BackGroundNo;

public:
	void operator=(const BATTLETRIGERKEY& _Other) 
	{
		QuestKey = _Other.QuestKey;
		QuestCount = _Other.QuestCount;
		BattleMonster = _Other.BattleMonster;
		BattleBgmNo = _Other.BattleBgmNo;
		BackGroundNo = _Other.BackGroundNo;
	}

public:
	BATTLETRIGERKEY()
		:QuestKey(_T("")), QuestCount(0), BattleMonster(-1)
		, BattleBgmNo(0), BackGroundNo(0)
	{}
	BATTLETRIGERKEY(const tstring& _QuestKey
		,const UINT& _QuestCount
		,const UINT& _BattleMonster)
		: QuestKey(_QuestKey), QuestCount(_QuestCount)
		, BattleMonster(_BattleMonster), BattleBgmNo(0)
		, BackGroundNo(0)
	{}
	BATTLETRIGERKEY(const tstring& _QuestKey
		, const UINT& _QuestCount
		, const UINT& _BattleMonster, const UINT& _BGMNo)
		: QuestKey(_QuestKey), QuestCount(_QuestCount)
		, BattleMonster(_BattleMonster), BattleBgmNo(_BGMNo)
		, BackGroundNo(0)
	{}
	BATTLETRIGERKEY(const tstring& _QuestKey
		, const UINT& _QuestCount
		, const UINT& _BattleMonster
		, const UINT& _BGMNo
		, const UINT& _BackGround)
		: QuestKey(_QuestKey), QuestCount(_QuestCount)
		, BattleMonster(_BattleMonster), BattleBgmNo(_BGMNo)
		,BackGroundNo(_BackGround)
	{}
	BATTLETRIGERKEY(const BATTLETRIGERKEY& _Other)
		:QuestKey(_Other.QuestKey), QuestCount(_Other.QuestCount)
		,BattleMonster(_Other.BattleMonster), BattleBgmNo(_Other.BattleBgmNo)
		,BackGroundNo(_Other.BackGroundNo)
	{}
	~BATTLETRIGERKEY() {}
};

struct QUESTMISSION 
{
	UINT						MissionIndex;
	tstring						MissionText;

	void operator=(const  QUESTMISSION& _Other) 
	{
		MissionIndex = _Other.MissionIndex;
		MissionText = _Other.MissionText;
	}



	QUESTMISSION() 
		:MissionIndex(-1), MissionText(_T("")){}
	QUESTMISSION(const UINT _Index, const tstring& _Text)
		:MissionIndex(_Index) , MissionText(_Text){}
	QUESTMISSION(const  QUESTMISSION& _Other)
		:MissionIndex(_Other.MissionIndex), MissionText(_Other.MissionText)
	{
	}
	~QUESTMISSION() {}
};
struct QUESTTEXT 
{
	// 퀘스트 내용
	tstring							QuestContents;
	// 퀘스트 보상
	tstring							QuestReward;
	// 퀘스트 임무 목록
	vector<QUESTMISSION>			QuestMission;
	UINT							CurMissionIndex;


	void operator=(const QUESTTEXT& _Other)
	{
		QuestContents = _Other.QuestContents;
		QuestReward = _Other.QuestReward;
		CurMissionIndex = _Other.CurMissionIndex;
		QuestMission.clear(); 
		QuestMission.assign(_Other.QuestMission.begin(), _Other.QuestMission.end());
	}

	const bool IsLastMission()
	{
		return CurMissionIndex >= QuestMission.size() - 1;
	}

	const UINT GetNextMissionIndex() 
	{
		if (CurMissionIndex	>= QuestMission.size() - 1)
			return -1;
		return QuestMission[CurMissionIndex + 1].MissionIndex;
	}
	const UINT GetCurMissionIndex() 
	{
		if (CurMissionIndex >= QuestMission.size())
			return -1;
		return QuestMission[CurMissionIndex].MissionIndex;
	}

	QUESTTEXT() 
		:QuestContents(_T("")), QuestReward(_T(""))
		, CurMissionIndex(0)
	{
		QuestMission.clear();
	}
	QUESTTEXT(const QUESTTEXT& _Other)
		:QuestContents(_Other.QuestContents), QuestReward(_Other.QuestReward)
		, CurMissionIndex(_Other.CurMissionIndex)
	{
		QuestMission.clear();
		QuestMission.assign(_Other.QuestMission.begin(), _Other.QuestMission.end());
	}
	~QUESTTEXT() { QuestMission.clear(); }
};

#include <ObjBase.h>
class CEventMgr : public CObjBase
{
	CGameScene*							m_pMainScene;
	CSceneBuilder*						m_pMainBuilder;
	
	class CGameMaster*					m_pGameMaster;
	class CScriptMgr*					m_pScriptMgr;

	map<tstring, QUESTDATA>				m_mapQuestData;
	map<tstring, QUESTTEXT>				m_mapQuestText;

	bool								m_bQuestScript;
	NPC									m_CurTrigerNPC;
	tstring								m_CurNPCName;
	tstring								m_CurQuestKey;
	UINT								m_CurQuestCount;


	list<BATTLETRIGERKEY>				m_listBattleTrigerKey;

	float								m_fEventAccTime;
	SPTR<CCamera>						m_pMainSceneCamera;
	Vec3								m_CamMoveDir;
	Vec3								m_PlayerMoveDir;
	// 플레이어를 자동으로 움직이게 했는지를 체크하는 변수
	bool								m_bPlayerAuto;

public:
	void ResetMgr();

public:
	void Init();
	void LoadQuestText();


public:
	// 퀘스트 이벤트가 발생시
	// 퀘스트 번호를 출력
	// -1은 퀘스트 수행없음을 알림
	const UINT CheckQuestEvent(const NPC& _NPCName);
	void SetNPCName(const tstring& _NPcName);
	void SettingQuestScript();
	const tstring GetQuestTitle(const tstring& _QuestKey);
	const tstring GetQuestSubTitle(const tstring& _QuestKey);
	const QUESTDATA GetCurQuestData(const tstring& _QuestKey);
	const QUESTTEXT GetQuestText(const tstring& _QuestKey);
	const size_t GetQuestCount();
	const bool QuestScriptEndEvent();
	const bool CheckBattleTriger();
	const BATTLETRIGERKEY GetCurBattleTrigerKey();

public:
	void SceneChangeEvent(const tstring& _PrevQuestKey, const UINT& _PrevQuestCount);
	void BattleWinEvent(const tstring& _PrevQuestKey,const UINT& _PrevQuestCount );


public:
	void QuestUpdateStandBy(const tstring& _NPC);
	function<void(void)> GetEventUpdateFunc(const tstring& _NPC);
	void Quest1BattleEvent();
	void Quest3BattleEvent();


public:
	const UINT GetQuestCount(const tstring& _QuestKey);
	const bool IsQuestStart(const tstring& _QuestKey);

public:
	CEventMgr();
	CEventMgr(CGameMaster* _Master
		,CGameScene* _Scene
		,CSceneBuilder* _Builder
		,CScriptMgr* _ScriptMgr
	);
	~CEventMgr();
};

