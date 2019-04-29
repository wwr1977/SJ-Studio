#include "Precom.h"
#include "TurnMgr.h"
#include "TurnOrderUI.h"
#include"BackGround.h"
#include"BattleUnit.h"
#include"UnitStatusBar.h"
#include"StandByPhase.h"
#include"SelectPhase.h"
#include"RotatePhase.h"
#include"ActionPhase.h"
#include"CheckPhase.h"
#include"RelocationPhase.h"
#include"BattleEndPhase.h"
#include"EffectMgr.h"
#include"SkillUIMgr.h"
#include"SkillUI.h"
#include"SkillUIBoard.h"
#include"SkillUITab.h"
#include"DemageFont.h"
#include"TargetUI.h"
#include"BattleSceneBuilder.h"
#include"Effect.h"
#include"BattleResultWindow.h"
#include"CharResultUI.h"
#include"ExpGauge.h"
#include"BattleEventObject.h"
//#include<SceneBuilder.h>

CTurnMgr::CTurnMgr()
	:m_FirstFrameSize(Vec2(80.0f, 80.0f)), m_OtherFrameSize(Vec2(55.0f, 55.0f))
	, m_BattleCamera(nullptr), m_pBattleScene(nullptr), m_BattleCamCom(nullptr)
	, m_StateMgr(nullptr), m_CurBattleUnit(nullptr), m_iRotateCount(-1)
	, m_fAccTime(0.0f), m_CharStatusSize({ 0.0f,0.0f }), m_MonStatusSize({ 0.0f,0.0f })
	, m_pEffectMgr(nullptr), m_bAllReturn(false), m_iCurOnDemageCount(0)
	, m_iCurSceneEffectCount(0), m_iAniScriptCount(0), m_iMoveScriptCount(0)
	, m_fSkillActionTime(0.0f), m_InitPosMode(0)
	, m_CurSkillData(SKILLDATA()), m_CurSkillLevelData(LEVELDATA())
	, m_pSkillUIMgr(nullptr), m_CurSelectSkill(-1), m_bSkillTrigger(false)
	, m_SelectTarget(MAXTARGET)
	, m_InitFixPos(Vec2{0.0f,0.0f}), m_InitPosPivot(Vec2{ 0.0f,0.0f })
	, m_bLastHitCheck(false), m_LastHitIndex(-1)
	, m_CurSkillLevel(-1), m_CurBattleRound(0)
	, m_fAccEndEventTime(0.0f), m_BattleWinTotalExp(0)
	, m_CurBattleBgmIndex(0), m_ResultWindow(nullptr)
	, m_bBGMMode(true), m_CurCharCount(0), m_CurMonCount(0)
{
	m_StateMgr = new CStateMgr();
	m_pEffectMgr = new CEffectMgr();
	m_pSkillUIMgr = new CSkillUIMgr();


	m_vecDemageQueue.assign(5,queue<UINT>());
}


CTurnMgr::~CTurnMgr()
{
	ReleaseTurnMgr();
}
void CTurnMgr::Init(CGameScene* _BattleScene, CBattleSceneBuilder* _Builder)
{
	m_pBattleSceneBuilder = _Builder;

	if (nullptr == _BattleScene) 
	{
		assert(nullptr);
		return;
	}

	m_pBattleScene = _BattleScene;
	m_pEffectMgr->SettingScene(_BattleScene);
	m_pSkillUIMgr->SettingScene(_BattleScene);
	m_pSkillUIMgr->SettingTurnMgr(this);

	CClientGlobal::TurnMgr = this;

	m_BattleCamera = m_pBattleScene->GetMainCamera();
	m_BattleCamCom = m_pBattleScene->GetMainCamCom();
	
	m_CharStatusSize = Vec2{ 138.0f,50.0f };
	m_MonStatusSize = Vec2{ 196.0f,56.0f };

	m_pEffectMgr->LoadEffectData();
	//CClientGlobal::CreateTestSkill();
	
	CBattleUnit::SetTurnMgr(this);
	CSkillUI::SetTurnMgr(this);
	
	CreateBattleObject();
	SettingObjectPosition();



	// 배틀시작시 유닛 배치(CStandByPhase)
	m_StateMgr->CreateState<CStandByPhase>(this);
	// 스킬 선택 단계(CSelectPhase)
	m_StateMgr->CreateState<CSelectPhase>(this);
	// 턴이 돌아가는 단계(CRotatePhase)
	m_StateMgr->CreateState<CRotatePhase>(this);
	// 지정된 행동을 실행하는 단계(CActionPhase)
	m_StateMgr->CreateState<CActionPhase>(this);
	// 현재 전투중인 유닛의 Dead상태를 확인하고 전투가 종료되었는지
	// 확인하는 단계(CCheckPhase)
	m_StateMgr->CreateState<CCheckPhase>(this);
	// 다음 배틀 라운드가 존재할 경우 몬스터와 UI를 재배치를 하고
	// 그에맞는 이벤트 함수를 호출하는 단계(CRelocationPhase)
	m_StateMgr->CreateState<CRelocationPhase>(this);
	// 전투가 종료되었을때 전투 승패를 결정하고
	// 전투씬을 마치는 단계(CActionPhase)
	m_StateMgr->CreateState<CBattleEndPhase>(this);


	m_StateMgr->ChangeState(PHASE_STANDBY);

	// 비 이벤트성 전투 배경음
	m_vecBattleBgmData.push_back(BGMDATA(_T("BattleBgm_000"),0.35f));
	// 도적 전투 배경음
	m_vecBattleBgmData.push_back(BGMDATA(_T("BattleBgm_001"), 0.35f));
	// 사천왕 전투 배경음
	m_vecBattleBgmData.push_back(BGMDATA(_T("BattleBgm_002"), 0.35f));
	// 보스 전투 배경음
	m_vecBattleBgmData.push_back(BGMDATA(_T("BattleBgm_003"), 0.2f));

	m_vecBattleBgmData.push_back(BGMDATA(_T("VictoryBgm"), 0.35f)); 
}
bool CTurnMgr::PushTurnUI(SPTR<CTurnOrderUI> _TurnUI)
{
	if (nullptr == _TurnUI) 
	{
		assert(nullptr);
		return false;
	}
	
	m_listCurTurnUI.push_back(_TurnUI);
	return true;
}
bool CTurnMgr::PushTurnUI(SPTR<CTurnOrderUI> _TurnUI, int _RotateCount)
{
	if (nullptr == _TurnUI) 
	{
		return false;
	}

	if (0 >= _RotateCount) 
	{
		PushTurnUI(_TurnUI);
		return true;
	}

	list<SPTR<CTurnOrderUI>>::iterator Prev = m_listCurTurnUI.begin();

	// _Rotate위치의 iterator를 찾는 로직
	// 만일 끝지점을 만날경우 가장 뒤에 넣는다
	for (int i = 0; i < _RotateCount; ++i) 
	{
		++Prev;
		if (Prev == m_listCurTurnUI.end()) 
		{
			return PushTurnUI(_TurnUI);
		}
	}
	
	m_listCurTurnUI.insert(Prev, _TurnUI);
	
	return true;
}
void CTurnMgr::InitList()
{
	for (size_t i = 0; i < m_listCurTurnUI.size(); i++)
	{
		m_listCurTurnUI.front()->Off();
		m_listCurTurnUI.pop_front();
	}
	
}
void CTurnMgr::Update()
{
	m_StateMgr->Update();

	if (true == KEYDOWN("ChangeDebugMode")) 
	{
		for (size_t i = 0; i < m_vecCharacter.size(); i++)
		{
			if (nullptr != m_vecCharacter[i])
				m_vecCharacter[i]->ReverseLineRen();
		}

		/*map<tstring, SPTR<CBattleUnit>>::iterator Start = m_mapMonster.begin();
		map<tstring, SPTR<CBattleUnit>>::iterator End = m_mapMonster.end();
		
		for (; Start!= End; ++Start)
		{
			if (nullptr != Start->second)
				Start->second->ReverseLineRen();
		}*/

		unordered_map<tstring, list<SPTR<CBattleUnit>>>::iterator Start = m_mapMonsterList.begin();
		unordered_map<tstring, list<SPTR<CBattleUnit>>>::iterator End = m_mapMonsterList.end();

		for (; Start != End; ++Start)
		{
			list<SPTR<CBattleUnit>>::iterator LStart = Start->second.begin();
			list<SPTR<CBattleUnit>>::iterator LEnd = Start->second.end();

			for (; LStart!=LEnd; ++LStart)
				if(nullptr != *LStart)
					(*LStart)->ReverseLineRen();
		}

		m_pEffectMgr->DebugButtonEvent();
	}


	if (true == KEYDOUBLE("MasterKey"))
		ChangeMasterMode();

}
void CTurnMgr::InitSkillUIMgr()
{
	for (size_t i = 0; i < SKILLDATA::MAXSKILLTYPE; i++)
		m_CurSkillIndexList.push_back(vector<size_t>());


	m_pSkillUIMgr->LoadSkillUIData();


	SPTR<CSkillUIBoard> Board = m_pSkillUIMgr->GetSkillBackBoard();

	if (nullptr != Board)
	{
		Board->SetSize(m_SkillBackBoardSize);
		Board->SetPos(m_SkillBackBoardPos);
	}

	for (size_t i = 0; i < Board->GetTabCount(); i++)
	{
		SPTR<CSkillUITab> Tab = Board->GetSkillUITab(i);
		if (nullptr == Tab)
			continue;

		Vec2 TabSize = Tab->GetSize();
		float X = m_SkillBackBoardSize.x * 0.5f + TabSize.x  * 0.5f - 15.0f;
		float Y = m_SkillBackBoardSize.y *0.5f - TabSize.y*(0.5f + 1.0f * i)+2.0f;
		Board->GetSkillUITab(i)->SetPos(Vec2(X, Y));
	}
}
CGameScene*	CTurnMgr::GetScene()
{
	return m_pBattleScene;
}
CBattleSceneBuilder* CTurnMgr::GetBattleSceneBuilder()
{
	return m_pBattleSceneBuilder;
}
SPTR<CTurnOrderUI> CTurnMgr::GetCurOrderUI()
{
	return m_listCurTurnUI.front();
}
CBattleUnit* CTurnMgr::GetCurUnit() 
{
	m_CurBattleUnit = m_listCurTurnUI.front()->GetBattleUnit();
	return  m_listCurTurnUI.front()->GetBattleUnit();
}
CActObject* CTurnMgr::CreateActObject(const tstring& _Name /*= _T("")*/)
{
	return CActObject::CreateActObject(_Name, m_pBattleScene);
}
void CTurnMgr::SettingObjectPosition()
{
	POINT WndSize = CClientGlobal::MainWindow->WndSize();
	
	Vec3 RightDown = { (float)WndSize.x* 0.5f,(float)WndSize.y*-0.5f,80.0f };
	Vec3 LeftDown = { (float)WndSize.x* -0.5f,(float)WndSize.y*-0.5f,80.0f };

	m_vecCharacterPos.push_back(CClientGlobal::MainWindow->WndPosToWorldPos(0.65f*(float)WndSize.x  , 0.45f*(float)WndSize.y));
	m_vecCharacterPos.push_back(CClientGlobal::MainWindow->WndPosToWorldPos(0.65f* (float)WndSize.x , 0.65f*(float)WndSize.y));
	m_vecCharacterPos.push_back(CClientGlobal::MainWindow->WndPosToWorldPos(0.775f* (float)WndSize.x, 0.35f*(float)WndSize.y));
	m_vecCharacterPos.push_back(CClientGlobal::MainWindow->WndPosToWorldPos(0.775f* (float)WndSize.x, 0.55f*(float)WndSize.y));
	m_vecCharacterPos.push_back(CClientGlobal::MainWindow->WndPosToWorldPos(0.775f* (float)WndSize.x, 0.75f*(float)WndSize.y));

	m_vecCharStatusPos.push_back(Vec3(RightDown.x - 1.9f*m_CharStatusSize.x, RightDown.y + 1.8f*m_CharStatusSize.y, 80.0f));
	m_vecCharStatusPos.push_back(Vec3(RightDown.x - 2.0f*m_CharStatusSize.x, RightDown.y + 0.6f*m_CharStatusSize.y, 80.0f));
	m_vecCharStatusPos.push_back(Vec3(RightDown.x - 0.6f*m_CharStatusSize.x, RightDown.y + 3.0f*m_CharStatusSize.y, 80.0f));
	m_vecCharStatusPos.push_back(Vec3(RightDown.x - 0.7f*m_CharStatusSize.x, RightDown.y + 1.8f*m_CharStatusSize.y, 80.0f));
	m_vecCharStatusPos.push_back(Vec3(RightDown.x - 0.8f*m_CharStatusSize.x, RightDown.y + 0.6f*m_CharStatusSize.y, 80.0f));


	m_vecMonStatusPos.push_back(Vec3(LeftDown.x + 0.6f*m_MonStatusSize.x, LeftDown.y + 2.8f*m_MonStatusSize.y, 80.0f));
	m_vecMonStatusPos.push_back(Vec3(LeftDown.x + 0.7f*m_MonStatusSize.x, LeftDown.y + 1.7f*m_MonStatusSize.y, 80.0f));
	m_vecMonStatusPos.push_back(Vec3(LeftDown.x + 0.8f*m_MonStatusSize.x, LeftDown.y + 0.6f*m_MonStatusSize.y, 80.0f));


	m_vecTurnUIPos.push_back(Vec3(WndSize.x * -0.5f + 10.0f + m_FirstFrameSize.x*0.5f, WndSize.y * 0.5f - 100.0f, 80.0f));
	m_vecTurnUIPos.push_back(Vec3(WndSize.x * -0.5f + 10.0f + m_OtherFrameSize.x*0.5f, m_vecTurnUIPos[0].y - m_FirstFrameSize.y*0.5f - m_OtherFrameSize.y*0.5f, 80.0f));
	m_vecTurnUIPos.push_back(m_vecTurnUIPos[1] - Vec3(0.0f, m_OtherFrameSize.y + 5.0f, 0.0f));
	m_vecTurnUIPos.push_back(m_vecTurnUIPos[2] - Vec3(0.0f, m_OtherFrameSize.y + 5.0f, 0.0f));
	m_vecTurnUIPos.push_back(m_vecTurnUIPos[3] - Vec3(0.0f, m_OtherFrameSize.y + 5.0f, 0.0f));
	m_vecTurnUIPos.push_back(m_vecTurnUIPos[4] - Vec3(0.0f, m_OtherFrameSize.y + 5.0f, 0.0f));
	m_vecTurnUIPos.push_back(m_vecTurnUIPos[5] - Vec3(0.0f, m_OtherFrameSize.y + 5.0f, 0.0f));
	m_vecTurnUIPos.push_back(m_vecTurnUIPos[6] - Vec3(0.0f, m_OtherFrameSize.y + 5.0f, 0.0f));

	m_vecSelectEffectPos.push_back(Vec3(930.0f, 22.0f, 0.0f));
	m_vecSelectEffectPos.push_back(Vec3(930.0f, -680.0f, 0.0f));
	m_vecSelectEffectPos.push_back(Vec3(1670.0f, 370.0f, 0.0f));
	m_vecSelectEffectPos.push_back(Vec3(1685.0f, -315.0f, 0.0f));
	m_vecSelectEffectPos.push_back(Vec3(1700.0f, -1015.0f, 0.0f));

	m_vecMonsterPos.push_back(Vec3(CClientGlobal::MainWindow->WndPosToWorldPos(0.2f* (float)WndSize.x, 0.6f*(float)WndSize.y)));
	m_vecMonsterPos.push_back(Vec3(CClientGlobal::MainWindow->WndPosToWorldPos(0.32f*(float)WndSize.x, 0.45f*(float)WndSize.y)));
	m_vecMonsterPos.push_back(Vec3(CClientGlobal::MainWindow->WndPosToWorldPos(0.38f* (float)WndSize.x, 0.73f*(float)WndSize.y)));

	m_SkillBackBoardSize = Vec2{ 425.0f,180.0f };
	m_SkillBackBoardPos = Vec3{ 0.0f,(float)-WndSize.y*0.5f + 5.0f + 0.5f * m_SkillBackBoardSize.y,10.0f };

	
}
void CTurnMgr::CreateBattleObject()
{
	SPTR<CActObject> BattleBack = CreateActObject(_T("Background"));
	m_BattleBack = BattleBack->CreateCom<CBackGround>();


	SPTR<CActObject> BattleResultWindow = CreateActObject(_T("BattleResultWindow"));
	m_ResultWindow = BattleResultWindow->CreateCom<CBattleResultWindow>();

	CreateBattleCharacter();
	CreateBattleMonster();
	
	for (size_t i = 0; i < MAXDEMAGEFONTCOUNT; ++i)
	{
		m_vecDemageFont.push_back(CreateActObject(_T("DemageFont"))->CreateCom<CDemageFont>(_T("Number_Black")));
		m_vecDemageFont[i]->SetTurnMgr(this);
		m_DemageReadyList.push_back(m_vecDemageFont[i]);
		m_vecDemageFont[i]->Off();
	}
	
}
void CTurnMgr::CreateBattleCharacter() 
{
	for (int i = DemonRain; i < MAXBATTLECHARACTER; ++i)
	{
		SPTR<CActObject> Unit = CreateActObject(_T("Charater"));
		CBattleUnit* UnitCom = Unit->CreateCom<CBattleUnit>((CharacterIndex)i);

		SPTR<CActObject> UnitStatus = CreateActObject(_T("CharUI"));
		CUnitStatusBar* StatusCom = UnitStatus->CreateCom<CUnitStatusBar>();

		SPTR<CActObject> TurnUI = CreateActObject(_T("CharTurnUI"));
		CTurnOrderUI* TurnUICom = TurnUI->CreateCom<CTurnOrderUI>();

		SPTR<CActObject> ResultUI = CreateActObject(_T("CharResultUI"));
		CCharResultUI* ResultUICom = ResultUI->CreateCom<CCharResultUI>();

		UnitCom->SetStatusBar(StatusCom);
		UnitCom->SetTurnUI(TurnUICom);
		UnitCom->SetResultUI(ResultUICom);
		StatusCom->SetUnit(UnitCom);
		TurnUICom->SetUnit(UnitCom);
		ResultUICom->SetPairUnit(UnitCom);


		m_vecCharacter.push_back(UnitCom);
		
		/*
		m_mapStatusBar.insert(unordered_map<tstring, SPTR<CUnitStatusBar>>::value_type(TurnUICom->GetUnitName(), StatusCom));
		m_mapTurnUI.insert(unordered_map<tstring, SPTR<CTurnOrderUI>>::value_type(TurnUICom->GetUnitName(), TurnUICom));
		*/

		list<SPTR<CUnitStatusBar>> SList = list<SPTR<CUnitStatusBar>>();
		SList.push_back(StatusCom);
		m_mapStatusBarList.insert(unordered_map<tstring, list<SPTR<CUnitStatusBar>>>::value_type(TurnUICom->GetUnitName(), SList));

		list<SPTR<CTurnOrderUI>> TList = list<SPTR<CTurnOrderUI>>();
		TList.push_back(TurnUICom);
		m_mapTurnUIList.insert(unordered_map<tstring, list<SPTR<CTurnOrderUI>>>::value_type(TurnUICom->GetUnitName(), TList));

		Unit->Off();
		UnitStatus->Off();
		TurnUI->Off();
	}

	m_EventObject = CreateActObject(_T("BattleEvnetObject"))->CreateCom<CBattleEventObject>();
	m_EventObject->LoadScreenImage();
}
void CTurnMgr::CreateBattleMonster() 
{
	set<tstring>::iterator MonStart = CClientGlobal::MonsterName.begin();
	set<tstring>::iterator MonEnd = CClientGlobal::MonsterName.end();

	for (; MonStart != MonEnd; ++MonStart)
	{
		SPTR<CActObject> Mon = CreateActObject(_T("Monster"));
		CBattleUnit* MonCom = Mon->CreateCom<CBattleUnit>((*MonStart));

		SPTR<CActObject> UnitStatus = CreateActObject(_T("MonUI"));
		CUnitStatusBar* StatusCom = UnitStatus->CreateCom<CUnitStatusBar>();

		SPTR<CActObject> TurnUI = CreateActObject(_T("MonTurnUI"));
		CTurnOrderUI* TurnUICom = TurnUI->CreateCom<CTurnOrderUI>();

		MonCom->SetStatusBar(StatusCom);
		MonCom->SetTurnUI(TurnUICom);
		StatusCom->SetUnit(MonCom);
		TurnUICom->SetUnit(MonCom);
		MonCom->SettingMonsterData();

	/*	m_mapMonster.insert(map<tstring, SPTR<CBattleUnit>>::value_type(*MonStart, MonCom));
		m_mapStatusBar.insert(unordered_map<tstring, SPTR<CUnitStatusBar>>::value_type(TurnUICom->GetUnitName(), StatusCom));
		m_mapTurnUI.insert(unordered_map<tstring, SPTR<CTurnOrderUI>>::value_type(TurnUICom->GetUnitName(), TurnUICom));*/


		list<SPTR<CBattleUnit>> MList = list<SPTR<CBattleUnit>>();
		MList.push_back(MonCom);
		m_mapMonsterList.insert(unordered_map<tstring, list<SPTR<CBattleUnit>>>::value_type(*MonStart, MList));


		list<SPTR<CUnitStatusBar>> SList = list<SPTR<CUnitStatusBar>>();
		SList.push_back(StatusCom);
		m_mapStatusBarList.insert(unordered_map<tstring, list<SPTR<CUnitStatusBar>>>::value_type(*MonStart, SList));

		list<SPTR<CTurnOrderUI>> TList = list<SPTR<CTurnOrderUI>>();
		TList.push_back(TurnUICom);
		m_mapTurnUIList.insert(unordered_map<tstring, list<SPTR<CTurnOrderUI>>>::value_type(*MonStart, TList));

		Mon->Off();
		UnitStatus->Off();
		TurnUI->Off();
	}

}
CBattleUnit* CTurnMgr::CreateBattleMonster(const tstring& _MonName)
{
	set<tstring>::iterator Find = CClientGlobal::MonsterName.find(_MonName);

	if (Find == CClientGlobal::MonsterName.end()) 
		return nullptr;
	
	tstring MonName = (*Find);

	unordered_map<tstring, list<SPTR<CBattleUnit>>>::iterator MFind = m_mapMonsterList.find(MonName);
	unordered_map<tstring, list<SPTR<CUnitStatusBar>>>::iterator SFind = m_mapStatusBarList.find(MonName);
	unordered_map<tstring, list<SPTR<CTurnOrderUI>>>::iterator TFind = m_mapTurnUIList.find(MonName);


	if (MFind == m_mapMonsterList.end() ||
		SFind == m_mapStatusBarList.end() ||
		TFind == m_mapTurnUIList.end())
		return nullptr;

	SPTR<CActObject> Mon = CreateActObject(_T("Monster"));
	CBattleUnit* MonCom = Mon->CreateCom<CBattleUnit>(MonName);

	SPTR<CActObject> UnitStatus = CreateActObject(_T("MonUI"));
	CUnitStatusBar* StatusCom = UnitStatus->CreateCom<CUnitStatusBar>();

	SPTR<CActObject> TurnUI = CreateActObject(_T("MonTurnUI"));
	CTurnOrderUI* TurnUICom = TurnUI->CreateCom<CTurnOrderUI>();

	MonCom->SetStatusBar(StatusCom);
	MonCom->SetTurnUI(TurnUICom);
	StatusCom->SetUnit(MonCom);
	TurnUICom->SetUnit(MonCom);
	MonCom->SettingMonsterData();

	MFind->second.push_back(MonCom);
	SFind->second.push_back(StatusCom);
	TFind->second.push_back(TurnUICom);

	Mon->Off();
	UnitStatus->Off();
	TurnUI->Off();
	
	
	return MonCom;
}
void CTurnMgr::ChangePhase(int _Index)
{
	m_StateMgr->ChangeState(_Index);
	//ChangeCurUnit();
}
void CTurnMgr::AllObjectOff()
{
	m_vecCurBattleChar.clear();
	m_vecCurBattleMon.clear();

	for (int i = DemonRain; i < MAXBATTLECHARACTER; i++)
	{
		m_vecCharacter[i]->Off();
		
	}

	// 모든 몬스터를 Off
	//map<tstring, SPTR<CBattleUnit>>::iterator StartMon = m_mapMonster.begin();
	//map<tstring, SPTR<CBattleUnit>>::iterator EndMon = m_mapMonster.end();
	//for (;StartMon != EndMon; ++StartMon)
	//{
	//	StartMon->second->Off();
	//}

	unordered_map<tstring, list<SPTR<CBattleUnit>>>::iterator Start = m_mapMonsterList.begin();
	unordered_map<tstring, list<SPTR<CBattleUnit>>>::iterator End = m_mapMonsterList.end();

	for (; Start != End; ++Start)
	{
		list<SPTR<CBattleUnit>>::iterator LStart = Start->second.begin();
		list<SPTR<CBattleUnit>>::iterator LEnd = Start->second.end();

		for (; LStart != LEnd; ++Start)
			if (nullptr != *LStart)
				(*LStart)->Off();
	}

	// 이전에 켜진 FrameUI를 다 꺼준다
	list<SPTR<CTurnOrderUI>>::iterator TurnStart= m_listCurTurnUI.begin();
	list<SPTR<CTurnOrderUI>>::iterator TurnEnd = m_listCurTurnUI.end();

	for (;TurnStart!=TurnEnd; )
	{
		(*TurnStart)->Off();
		TurnStart = m_listCurTurnUI.erase(TurnStart);
	}

	if (m_listCurTurnUI.size() > 0) 
	{
		assert(nullptr);
	}
	

}
void CTurnMgr::CurActiveObjectOff()
{
	for (size_t i = 0; i < m_vecCurBattleChar.size(); ++i)
	{
		if (nullptr != m_vecCurBattleChar[i]) 
			m_vecCurBattleChar[i]->Off();
		
	}

	for (size_t i = 0; i < m_vecCurBattleMon.size(); ++i)
	{
		if (nullptr != m_vecCurBattleMon[i])
			m_vecCurBattleMon[i]->Off();
	}

	list<SPTR<CTurnOrderUI>>::iterator TurnStart = m_listCurTurnUI.begin();
	list<SPTR<CTurnOrderUI>>::iterator TurnEnd = m_listCurTurnUI.end();

	for (; TurnStart != TurnEnd; )
	{
		(*TurnStart)->Off();
		TurnStart = m_listCurTurnUI.erase(TurnStart);
	}

}
void CTurnMgr::SettingBattleObject()
{
	for (size_t i = 0; i < m_vecCurBattleChar.size(); ++i)
		if (nullptr != m_vecCurBattleChar[i])
			m_vecCurBattleChar[i]->CurBattle(false);
	

	m_vecCurBattleChar.clear();
	m_vecCurBattleChar.assign(MAXTEAM,nullptr);

	UINT Count = MAXTEAM;

	for (int i = 0; i < MAXTEAM; i++)
	{
		if (CClientGlobal::AdvanceTeam[i] <= -1) 
		{
			--Count;
			continue;
		}
		
		m_vecCurBattleChar[i] = m_vecCharacter[CClientGlobal::AdvanceTeam[i]];
		m_vecCurBattleChar[i]->StandBy();
		m_vecCurBattleChar[i]->SetPos(m_vecCharacterPos[i]);
		m_vecCurBattleChar[i]->SetTeamNumber(i);
		m_vecCurBattleChar[i]->SetIdle();
		m_vecCurBattleChar[i]->UnitOn(m_vecSelectEffectPos[i]);
		m_vecCurBattleChar[i]->PairUIOff();
		m_vecCurBattleChar[i]->GetPairStatusBar()->StandBy();
		m_vecCurBattleChar[i]->GetPairStatusBar()->SetPos(m_vecCharStatusPos[i]);
		
		m_listCurTurnUI.push_back(m_vecCurBattleChar[i]->GetPairTurnUI());
		m_vecCurBattleChar[i]->GetPairTurnUI()->StandBy();
	}

	m_CurCharCount = Count;



	for (size_t i = 0; i < m_vecCurBattleMon.size(); ++i)
		if (nullptr != m_vecCurBattleMon[i])
			m_vecCurBattleMon[i]->CurBattle(false);
	
	m_vecCurBattleMon.clear();
	m_vecCurBattleMon.assign(MAXMONSTER, nullptr);

	

	Count = MAXMONSTER;

	for (size_t i = 0; i < MAXMONSTER; i++)
	{
		//m_vecCurBattleMon[i] = PopBattleMonster(CClientGlobal::MonsterTeam[i]);
		m_vecCurBattleMon[i]= PopBattleMonster(CClientGlobal::strMonsterTeam[m_CurBattleRound][i]);
		
		if (nullptr == m_vecCurBattleMon[i]) 
		{
			--Count;
			continue;
		}

		m_vecCurBattleMon[i]->StandBy();
		m_vecCurBattleMon[i]->SetPos(m_vecMonsterPos[i]);
		m_vecCurBattleMon[i]->SetTeamNumber((int)i);
		m_vecCurBattleMon[i]->UnitOn();

		m_vecCurBattleMon[i]->GetPairStatusBar()->StandBy();
		m_vecCurBattleMon[i]->GetPairStatusBar()->SetPos(m_vecMonStatusPos[i]);
		m_listCurTurnUI.push_back(m_vecCurBattleMon[i]->GetPairTurnUI());
		m_vecCurBattleMon[i]->GetPairTurnUI()->StandBy();
		// 나중에 체력 게이지(보스) UI를 배치
	}

	m_CurMonCount = Count;

	// 전투에 참여한 유닛의 초상화를 왼쪽UI가 모인곳으로 배치
	InitUILocation();
	InitTurnUIPos();

}

void CTurnMgr::UnitPairUIOn()
{
	for (int i = 0; i < MAXTEAM; i++) 
	{
		if(nullptr != m_vecCurBattleChar[i])
			m_vecCurBattleChar[i]->PairUIOn();
	}

	for (int i = 0; i < MAXMONSTER; i++) 
	{
		if (nullptr != m_vecCurBattleMon[i])
			m_vecCurBattleMon[i]->PairUIOn();
	}

}
bool CTurnMgr::PopCurTurnUI()
{
	if (0 >= m_listCurTurnUI.size()) 
	{
		return false;
	}

	m_listCurTurnUI.pop_front();
	return true;
}
bool CTurnMgr::UpdateTurnUIPos()
{
	if (m_iRotateCount <= 0) 
	{
		return DefaultRotateEvent();
	}

	return ChangeRotateEvent();
}
void CTurnMgr::SetRotateCount(const int& _RotCount) 
{
	m_iRotateCount = _RotCount;
}
int CTurnMgr::GetRotateCount()
{
	return m_iRotateCount;
}
void CTurnMgr::PopDeadUnitUI()
{
	list<SPTR<CTurnOrderUI>>::iterator Start = m_listCurTurnUI.begin();
	list<SPTR<CTurnOrderUI>>::iterator End = m_listCurTurnUI.end();

	for (; Start!= End; )
	{
		if ((*Start)->IsPopUI()) 
		{
			Start = m_listCurTurnUI.erase(Start);
			continue;
		}
	
		++Start;
	}
}
bool CTurnMgr::DefaultRotateEvent()
{
	SPTR<CTurnOrderUI>	FirstFrameUI =  m_listCurTurnUI.front();
	Vec3 StartPos;
	Vec3 EndPos;

	if (m_fAccTime <= 0.5f) 
	{
		list<SPTR<CTurnOrderUI>>::iterator Start = m_listCurTurnUI.begin();
		list<SPTR<CTurnOrderUI>>::iterator End = m_listCurTurnUI.end();

		for (int Index = 0;Start!= End ; ++Start, ++Index)
		{
			//최상단의 UI는 왼쪽 화면 바깥으로 밀어 버린다.
			if (Index == 0) 
			{
				StartPos = m_vecTurnUIPos[Index];
				EndPos = m_vecTurnUIPos[Index] - Vec3(100.0f, 0.0f, 0.0f);
				(*Start)->SetPos(CSJMath::Vec3Lerp(&StartPos, &EndPos, 2.5f* m_fAccTime));
				(*Start)->SetUIAlpha(1.0f - m_fAccTime);
			}
			// 다음 턴의 UI는 크기를 늘리면서 올라간다
			else if (Index == 1) 
			{
				StartPos = m_vecTurnUIPos[Index];
				EndPos = m_vecTurnUIPos[Index - 1];
				(*Start)->SetUISize(CSJMath::Vec2Lerp(&m_OtherFrameSize, &m_FirstFrameSize, 2.3f* m_fAccTime));
				(*Start)->SetPos(CSJMath::Vec3Lerp(&StartPos, &EndPos, 2.5f* m_fAccTime));
			}
			// 나머지 UI는 위로 올려주기만 한다.
			else 
			{
				StartPos = m_vecTurnUIPos[Index];
				EndPos = m_vecTurnUIPos[Index - 1];
				(*Start)->SetPos(CSJMath::Vec3Lerp(&StartPos, &EndPos, 2.5f* m_fAccTime));
			}
		}
	}
	// 화면 왼쪽으로 빠진 Frame을 아래에 넣는다
	else if (m_fAccTime > 1.0f && m_fAccTime < 1.5f) 
	{
		size_t LastIndex = m_listCurTurnUI.size() -1 ;
		StartPos = m_vecTurnUIPos[LastIndex] - Vec3(90.0f, 0.0f, 0.0f);
		EndPos = m_vecTurnUIPos[LastIndex];

		FirstFrameUI->SetUISize(m_OtherFrameSize);
		FirstFrameUI->SetPos(CSJMath::Vec3Lerp(&StartPos, &EndPos, (m_fAccTime - 1.0f)* 2.5f));
		FirstFrameUI->SetUIAlpha((m_fAccTime - 1.0f)* 1.85f);
	}
	else if (m_fAccTime > 1.5f) 
	{
		PopCurTurnUI();
		PushTurnUI(FirstFrameUI);
		SyncroUIPos();
		m_fAccTime = 0.0f;
		return true;
	}

	m_fAccTime += DELTATIME;
	return false;
}
// m_iRotateCount만큼 최상위 Ui를 아래로 내리는 무브먼트
bool CTurnMgr::ChangeRotateEvent()
{
	SPTR<CTurnOrderUI>	FirstFrameUI = m_listCurTurnUI.front();
	Vec3 StartPos;
	Vec3 EndPos;

	
	// 1단계 : 현재 최상위 UI를 오른쪽으로 빼고 
	// m_iRotateCount만큼 상위의 UI를 위로 올린다 
	if (0.5f > m_fAccTime) 
	{
		list<SPTR<CTurnOrderUI>>::iterator		Start = m_listCurTurnUI.begin();
		list<SPTR<CTurnOrderUI>>::iterator		End = m_listCurTurnUI.end();

		for (int Index = 0; Start != End && Index <= m_iRotateCount ; ++Start, ++Index)
		{
			//최상단의 UI는 오른쪽으로 밀어 버리면서 크기를 OtherFrameSize로 줄인다.
			if (Index == 0)
			{
				StartPos = m_vecTurnUIPos[Index];
				EndPos = m_vecTurnUIPos[Index] + Vec3(90.0f, 0.0f, 0.0f);
				(*Start)->SetPos(CSJMath::Vec3Lerp(&StartPos, &EndPos, 2.5f* m_fAccTime));
				(*Start)->SetUISize(CSJMath::Vec2Lerp(&m_FirstFrameSize, &m_OtherFrameSize, 2.3f*m_fAccTime));
			}
			// 다음 턴의 UI는 크기를 늘리면서 올라간다
			else if (Index == 1)
			{
				StartPos = m_vecTurnUIPos[Index];
				EndPos = m_vecTurnUIPos[Index - 1];
				(*Start)->SetUISize(CSJMath::Vec2Lerp(&m_OtherFrameSize, &m_FirstFrameSize, 2.3f* m_fAccTime));
				(*Start)->SetPos(CSJMath::Vec3Lerp(&StartPos, &EndPos, 2.5f* m_fAccTime));
			}
			// 나머지 UI는 위로 올려주기만 한다.
			else
			{
				StartPos = m_vecTurnUIPos[Index];
				EndPos = m_vecTurnUIPos[Index - 1];
				(*Start)->SetPos(CSJMath::Vec3Lerp(&StartPos, &EndPos, 2.5f* m_fAccTime));
			}
		}
	}
	// 2 단계 : 오른쪽으로 빠진 UI를 m_RotateCount만큼의 위치로 아래 이동 
	else if (m_fAccTime >= 0.5f && m_fAccTime < 0.9f) 
	{
		int InsertIndex = min(m_iRotateCount,(int)m_listCurTurnUI.size() -1);

		StartPos = m_vecTurnUIPos[0] + Vec3(90.0f, 0.0f, 0.0f);
		EndPos = m_vecTurnUIPos[InsertIndex] + Vec3(90.0f, 0.0f, 0.0f);
		FirstFrameUI->SetPos(CSJMath::Vec3Lerp(&StartPos, &EndPos, 2.8f* (m_fAccTime - 0.5f)));
	}
	// 3 단계 : 아래로간 UI를 TurnUI Line에 넣는다
	else if (m_fAccTime >= 0.9f && m_fAccTime < 1.3f) 
	{
		int InsertIndex = min(m_iRotateCount, (int)m_listCurTurnUI.size() - 1);

		StartPos = m_vecTurnUIPos[InsertIndex] + Vec3(90.0f, 0.0f, 0.0f);
		EndPos = m_vecTurnUIPos[InsertIndex];
		FirstFrameUI->SetPos(CSJMath::Vec3Lerp(&StartPos, &EndPos, 2.8f* (m_fAccTime - 0.9f)));
	}
	// 4 단계 : FirstFrameUI를 Pop한 뒤 m_iRotateCount의 위치로 보낸다
	else if(m_fAccTime >= 1.3f)
	{
		PopCurTurnUI();
		PushTurnUI(FirstFrameUI, m_iRotateCount);
		SyncroUIPos();
		m_fAccTime = 0.0f;
		return true;
	}

	m_fAccTime += DELTATIME;

	return false;
}
void CTurnMgr::SyncroUIPos()
{
	int Index = 0;

	list<SPTR<CTurnOrderUI>>::iterator Start = m_listCurTurnUI.begin();
	list<SPTR<CTurnOrderUI>>::iterator End = m_listCurTurnUI.end();

	for (;Start!= End; ++Start , ++Index)
	{
		if (Index == 0) 
		{
			(*Start)->SetPos(m_vecTurnUIPos[Index]);
			(*Start)->SetUISize(m_FirstFrameSize);
		}
		else 
		{
			(*Start)->SetPos(m_vecTurnUIPos[Index]);
			(*Start)->SetUISize(m_OtherFrameSize);
		}

		(*Start)->SetUIAlpha(0.9f);
	}
}
UINT CTurnMgr::RandomPushTurnUI(SPTR<CTurnOrderUI> _UI)
{
	if (nullptr == _UI)
		return -1;

	UINT Ran = (UINT)CSJRandom::RandomInt(2, (UINT)m_listCurTurnUI.size());

	_UI->SetUISize(m_OtherFrameSize);

	list<SPTR<CTurnOrderUI>>::iterator Start = m_listCurTurnUI.begin();
	list<SPTR<CTurnOrderUI>>::iterator End = m_listCurTurnUI.end();

	for (size_t i = 0; i < Ran; i++,++Start)
	{
		if (Start == End)
		{
			m_listCurTurnUI.push_back(_UI);
			return (UINT)(m_listCurTurnUI.size()-1);
		}
	}

	m_listCurTurnUI.insert(Start, _UI);
	return Ran;
}
void CTurnMgr::InitUILocation()
{
	m_listCurTurnUI.sort(greater<SPTR<CTurnOrderUI>>());


	vector<SPTR<CTurnOrderUI>> Temp;
	Temp.clear();

	while (m_listCurTurnUI.size() > 2) 
	{
		Temp.push_back(m_listCurTurnUI.back());
		m_listCurTurnUI.pop_back();
	}

	CSJRandom::ShuffleVector(Temp);


	while (Temp.size() > 0) 
	{
		m_listCurTurnUI.push_back(Temp.back());
		Temp.pop_back();
	}
	Temp.clear();

}
void CTurnMgr::ClearAllDemageQueue()
{
	for (size_t i = 0; i < m_vecDemageQueue.size(); i++)
	{
		queue<UINT> Empty;
		swap(Empty, m_vecDemageQueue[i]);
	}
}
SPTR<CBattleUnit> CTurnMgr::PopBattleMonster(const tstring& _MonsterName)
{
	unordered_map<tstring, list<SPTR<CBattleUnit>>>::iterator Find = m_mapMonsterList.find(_MonsterName);


	if (Find == m_mapMonsterList.end())
		return nullptr;

	list<SPTR<CBattleUnit>>::iterator LStart = Find->second.begin();
	list<SPTR<CBattleUnit>>::iterator LEnd = Find->second.end();

	for (; LStart != LEnd; ++LStart)
	{
		if (nullptr == (*LStart))
			continue;

		if (false == (*LStart)->IsCurBattle())
		{
			return (*LStart);
		}
	}

	return CreateBattleMonster(Find->first);

}
SPTR<CBattleUnit> CTurnMgr::PopBattleMonster(const int& _Index)
{
	if ((int)m_mapMonsterList.size() <= _Index)
	{
		return nullptr;
	}

	unordered_map<tstring, list<SPTR<CBattleUnit>>>::iterator Start = m_mapMonsterList.begin();
	unordered_map<tstring, list<SPTR<CBattleUnit>>>::iterator End = m_mapMonsterList.end();

	int Index = _Index;

	for (; Start != End, Index > 0; ++Start, --Index)
	{
	}

	if (Start == End)
		return nullptr;
	

	list<SPTR<CBattleUnit>>::iterator LStart = Start->second.begin();
	list<SPTR<CBattleUnit>>::iterator LEnd = Start->second.end();

	for (;LStart != LEnd; ++LStart)
	{
		if (nullptr == (*LStart))
			continue;

		if (false == (*LStart)->IsCurBattle()) 
		{
			return (*LStart);
		}
	}

	return CreateBattleMonster(Start->first);
}
CEffectMgr* CTurnMgr::GetEffectMgr()
{
	return m_pEffectMgr;
}
const Vec3 CTurnMgr::GetCharacterPos(const int& _TeamNumber)
{
	return GetCharacterPos((size_t)_TeamNumber);
}
const Vec3 CTurnMgr::GetCharacterPos(const size_t& _TeamNumber)
{
	if (_TeamNumber < 0 || _TeamNumber >= MAXTEAM)
	{
		return Vec3{ 0.0f,0.0f,10.0f };
	}

	return m_vecCharacterPos[_TeamNumber];
}
const Vec3 CTurnMgr::GetMonsterPos(const int& _TeamNumber) 
{
	return GetMonsterPos((size_t)_TeamNumber);
}
const Vec3 CTurnMgr::GetMonsterPos(const size_t& _TeamNumber) 
{
	if (_TeamNumber < 0 || _TeamNumber >= MAXMONSTER)
	{
		return Vec3{ 0.0f,0.0f,10.0f };
	}

	return m_vecMonsterPos[_TeamNumber];
}
SPTR<CBattleUnit> CTurnMgr::GetCharacter(const size_t& _CharNo)
{
	if (_CharNo >= m_vecCharacter.size())
		return nullptr;

	return m_vecCharacter[_CharNo];
}
SPTR<CBattleUnit> CTurnMgr::GetCurCharacter(const size_t& _TeamNo) 
{
	if (_TeamNo >= MAXTEAM) 
		return nullptr;
	
	return m_vecCurBattleChar[_TeamNo];
}
SPTR<CBattleUnit> CTurnMgr::GetCurMonster(const size_t& _TeamNo) 
{
	if (_TeamNo >= MAXMONSTER )
		return nullptr;
	
	return m_vecCurBattleMon[_TeamNo];
}
const UINT CTurnMgr::GetCurCharCount()
{
	return m_CurCharCount;
}
const UINT CTurnMgr::GetCurMonCount() 
{
	return m_CurMonCount;
}
void CTurnMgr::InitTurnUIPos()
{
	// 모든 유닛의 스탯테이블이 만들어진다면
	// 그떄 정렬함수를 먼저 배치할것

	int Index = 0;
	
	list<SPTR<CTurnOrderUI>>::iterator Start = m_listCurTurnUI.begin();
	list<SPTR<CTurnOrderUI>>::iterator End = m_listCurTurnUI.end();


	for (; Start != End ; ++Start,++Index)
	{
		(*Start)->SetPos(m_vecTurnUIPos[Index]);
		
		if (0 == Index) 
		{
			(*Start)->SetUISize(m_FirstFrameSize);
		}
		else 
		{
			(*Start)->SetUISize(m_OtherFrameSize);
		}
	}

}
void CTurnMgr::PopDemageFont(const int& _Number, const Vec3& _RenderPos, const int& _Count, const size_t& NumberSpriteIndex)
{
	return PopDemageFont(_Number, _RenderPos, _Count, CClientGlobal::GetDemageFont(NumberSpriteIndex));
}
SPTR<CDemageFont> CTurnMgr::GetDemageFont(const int& _Number, const Vec3& _RenderPos, const size_t& NumberSpriteIndex)
{
	if (true == m_DemageReadyList.empty())
	{
		return nullptr;
	}

	SPTR<CDemageFont> PopDemage = m_DemageReadyList.front();
	PopDemage->MiddleOn(_Number, Vec2{ _RenderPos.x ,_RenderPos.y }, NumberSpriteIndex);
	m_DemageReadyList.pop_front();
	++m_iCurOnDemageCount;
	PopDemage->Off();

	m_bAllReturn = false;

	return PopDemage;
}

void CTurnMgr::PopDemageFont(const int& _Number, const Vec3& _RenderPos, const int& _Count /*= 1*/)
{
	if (_Count <= 0)
		return;

	for (int i = 0; i < _Count; i++)
	{
		if (true == m_DemageReadyList.empty()) 
		{
			return;
		}

		SPTR<CDemageFont> PopDemage = m_DemageReadyList.front();
		PopDemage->On(_Number, Vec2{_RenderPos.x + 50.0f*i,_RenderPos.y + 25.0f*i });
		m_DemageReadyList.pop_front();
		PopDemage->SetRenderTimer(0.1f*i);
		++m_iCurOnDemageCount;
	}

	m_bAllReturn = false;
}

void CTurnMgr::PopDemageFont(const int& _Number, const Vec3& _RenderPos, const int& _Count, const tstring& _NumberSpriteName)
{
	if (_Count <= 0)
		return;

	for (int i = 0; i < _Count; i++)
	{
		if (true == m_DemageReadyList.empty())
		{
			return;
		}

		SPTR<CDemageFont> PopDemage = m_DemageReadyList.front();
		PopDemage->On(_Number, Vec2{ _RenderPos.x + 50.0f*i,_RenderPos.y + 25.0f*i }, _NumberSpriteName);
		PopDemage->SetRenderTimer(0.1f*i);
		m_DemageReadyList.pop_front();
		
		++m_iCurOnDemageCount;
	}

	m_bAllReturn = false;

}
void CTurnMgr::PopMiddleDemageFont(const int& _Number, const Vec3& _RenderPos, const UINT& _Count, const size_t& _NumberSpriteIndex /*= DEMAGE_BLACK*/)
{
	if (_Count <= 0)
		return;

	for (size_t i = 0; i < _Count; i++)
	{
		if (true == m_DemageReadyList.empty())
		{
			return;
		}

		SPTR<CDemageFont> PopDemage = m_DemageReadyList.front();
		PopDemage->MiddleOn(_Number, Vec2{ _RenderPos.x + 50.0f*i,_RenderPos.y + 25.0f*i }, _NumberSpriteIndex);
		m_DemageReadyList.pop_front();
		PopDemage->SetRenderTimer(0.1f*i);
		++m_iCurOnDemageCount;
	}

	m_bAllReturn = false;
}
void CTurnMgr::ReturnDemageFont(SPTR<CDemageFont> _Demage)
{
	if (nullptr == _Demage) 
	{
		assert(nullptr);
		return;
	}

	m_DemageReturnList.push_back(_Demage);
	
	if (m_DemageReturnList.size() + m_DemageReadyList.size() == MAXDEMAGEFONTCOUNT) 
	{
		m_bAllReturn = true;
	}

}
bool CTurnMgr::IsFontAllReturn()
{
	return m_bAllReturn;
}
void CTurnMgr::MergeDemageList()
{
	for (size_t i = 0; i < m_vecDemageFont.size(); i++)
	{
		m_vecDemageFont[i]->Off();
	}

	m_DemageReturnList.sort(CDemageFont::RenderOrder);
	
	m_DemageReadyList.merge(m_DemageReturnList, CDemageFont::RenderOrder);
	m_DemageReturnList.clear();

	if (MAXDEMAGEFONTCOUNT != m_DemageReadyList.size())
	{
		m_DemageReadyList.clear();

		for (size_t i = 0; i < MAXDEMAGEFONTCOUNT; i++)
		{
			m_DemageReadyList.push_back(m_vecDemageFont[i]);
		}
	}

	m_bAllReturn = true;
}
const size_t CTurnMgr::GetReadyListSize() 
{
	return m_DemageReadyList.size();
}
const size_t CTurnMgr::GetReturnListSize() 
{
	return m_DemageReturnList.size();
}
void CTurnMgr::ClearAllQueue() 
{
	queue<ANIMATIONSCRIPT> EmptyAni;
	queue<MOVEDATA> EmptyMove;
	queue<tstring>	EmptySound;
	queue<SKILLEFFECTDATA>	EmpSkillEffect;
	queue<SCENEEFFECTDATA>	EmpSceneEffect;
	queue<HITDATA>			EmpHitData;

	swap(m_qAniScript, EmptyAni);
	swap(m_qMoveScript, EmptyMove);
	swap(m_SoundQueue, EmptySound);
	swap(m_qSkillEffect, EmpSkillEffect);
	swap(m_qSceneEffect, EmpSceneEffect);
	swap(m_qHitData, EmpHitData);

}
void CTurnMgr::ClearQueue(const SKILLQUEUE_INDEX& _Index)
{
	if (_Index >= MAXSKILLQUEUEINDEX || _Index < ACTSCRIPT_ANI)
	{
		return;
	}

	switch (_Index)
	{
	case ACTSCRIPT_ANI:
	{
		queue<ANIMATIONSCRIPT> EmptyQueue;
		swap(m_qAniScript, EmptyQueue);
	}
	break;
	case ACTSCRIPT_MOVE:
	{
		queue<MOVEDATA> EmptyQueue;
		swap(m_qMoveScript, EmptyQueue);
	}
	break;
	case ACTSCRIPT_SOUND:
	{
		queue<tstring>	EmptyQueue;
		swap(m_SoundQueue, EmptyQueue);
	}
	break;
	case ACTSCRIPT_SKILLEFFECT:
	{
		queue<SKILLEFFECTDATA>	EmpSkillEffect;
		swap(m_qSkillEffect, EmpSkillEffect);
	}
	break;
	case ACTSCRIPT_SCENEEFFECT:
	{
		queue<SCENEEFFECTDATA>	EmpSceneEffect;
		swap(m_qSceneEffect, EmpSceneEffect);
	}
	break;
	case SKILLDATA_HITDATA:
	{
		queue<HITDATA>	EmpHitData;
		swap(m_qHitData, EmpHitData);
	}
	break;
	}
}

void CTurnMgr::ClearQueue(const int& _Index) 
{
	return ClearQueue((SKILLQUEUE_INDEX)_Index);
}
bool CTurnMgr::SetSkill(const size_t& _Index)
{
	// 테스트용 인덱스 범위
	if (_Index >= 1)
		return false;

	ClearAllQueue();

	SetActionScript(_Index);
	//SetSkillData(_Index);
	
	return true;
}
bool CTurnMgr::SetSkill(const size_t& _SkillNo, const size_t& _SkillLevel)
{
	if (_SkillNo >= CClientGlobal::vecSkill.size())
		return false;

	ClearAllQueue();
	
	m_bLastHitCheck = false;
	tstring SkillName = CClientGlobal::vecSkill[_SkillNo].SkillUIData.SkillName;
	
	if (0 == SkillName.compare(_T("Attack")) || 0 == SkillName.compare(_T("DoubleAttack")))
		m_bLastHitCheck = true;
	

	
	SetActionScript(_SkillNo);
	SetSkillData(_SkillNo, _SkillLevel);
	m_CurSkillLevel = (UINT)_SkillLevel;

	return true;
}
void CTurnMgr::SetActionScript(ACTIONSCRIPT* _ActionScript)
{
	if (nullptr == _ActionScript) 
	{
		return;
	}

	//ClearAllQueue();
	m_vecSkillEffect.clear();
	m_vecSoundScript.clear();

	m_iAniScriptCount = 0;
	m_iMoveScriptCount = 0;
	m_iCurSceneEffectCount = 0;

	/*m_InitPos = _ActionScript->InitPos;
	m_InitPosMode = _ActionScript->InitPosMode;
	m_InitPosIndex = _ActionScript->InitPosIndex;*/

	for (UINT i = 0; i < min(_ActionScript->AniCount, ACTIONSCRIPT::MAXANISCRIPTCOUNT); i++)
	{
		m_qAniScript.push(_ActionScript->AniSctipt[i]);
		++m_iAniScriptCount;
	}

	// MoveData를 큐에 넣을때 StartTime 빠른 순으로 넣어주기 위한 작업
	list<MOVEDATA> TempMoveData;
	TempMoveData.clear();

	for (UINT i = 0; i < min(_ActionScript->MoveCount, ACTIONSCRIPT::MAXMOVECOUNT); ++i)
	{
		TempMoveData.push_back(_ActionScript->CharMoveData[i]);
		++m_iMoveScriptCount;
	}

	TempMoveData.sort();
	
	for (int i = 0; i < m_iMoveScriptCount; i++)
	{
		m_qMoveScript.push(TempMoveData.front());
		TempMoveData.pop_front();
	}

	for (UINT i = 0; i < min(_ActionScript->CharSoundCount, ACTIONSCRIPT::MAXCHARSOUNDCOUNT); ++i)
	{
		m_vecSoundScript.push_back(SoundData(_ActionScript->CharSound[i]));
	}

	m_fSkillActionTime = _ActionScript->TotalSkillTime;

	for (UINT i = 0; i < min(_ActionScript->EffectCount,ACTIONSCRIPT::MAXEFFECTCOUNT); i++)
	{
		m_vecSkillEffect.push_back(_ActionScript->SkillEffect[i]);
	}

	for (UINT i = 0; i < min(_ActionScript->SceneEffectCount,ACTIONSCRIPT::MAXSCENEEFFECTCOUNT); i++)
	{
		m_qSceneEffect.push(_ActionScript->SceneEffectData[i]);
		++m_iCurSceneEffectCount;
	}

}
void CTurnMgr::SetActionScript(const size_t& _SkillNo)
{
	if (_SkillNo >= CClientGlobal::vecSkill.size())
	{
		return;
	}

	return SetActionScript(&CClientGlobal::vecSkill[_SkillNo].SkillActionScript);
}

vector<SKILLEFFECTDATA>* CTurnMgr::GetSkillEffectVector() 
{
	return &m_vecSkillEffect;

}
vector<SoundData>*  CTurnMgr::GetSoundVector()
{
	return &m_vecSoundScript;
}

queue<ANIMATIONSCRIPT>* CTurnMgr::GetAniQueue() 
{
	return &m_qAniScript;
}
queue<MOVEDATA>* CTurnMgr::GetMoveQueue()
{
	return &m_qMoveScript;
}
queue<tstring>*	CTurnMgr::GetSoundQueue()
{
	return &m_SoundQueue;
}
queue<SKILLEFFECTDATA>*	 CTurnMgr::GetSkillEffectQueue()
{
	return &m_qSkillEffect;
}
queue<SCENEEFFECTDATA>*	 CTurnMgr::GetSceneEffectQueue()
{
	return &m_qSceneEffect;
}
int* CTurnMgr::GetAnimationCount()
{
	return &m_iAniScriptCount;
}
int* CTurnMgr::GetMoveCount() 
{
	return &m_iMoveScriptCount;
}
int* CTurnMgr::GetSceneEffectCount()
{
	return &m_iCurSceneEffectCount;
}
float* CTurnMgr::GetSkillActionTime()
{
	return &m_fSkillActionTime;
}

size_t* CTurnMgr::GetInitPosMode()
{
	return &m_InitPosMode;
}
Vec2* CTurnMgr::GetInitPos() 
{
	return &m_InitFixPos;
}
Vec2* CTurnMgr::GetInitPosPivot() 
{
	return &m_InitPosPivot;
}
SKILLDATA* CTurnMgr::GetSkillData()
{
	return &m_CurSkillData;
}
LEVELDATA* CTurnMgr::GetSkillLevelData() 
{
	return &m_CurSkillLevelData;
}
list<HITDATA>* CTurnMgr::GetHitDataList()
{
	return &m_HitData;
}
queue<HITDATA>* CTurnMgr::GetHitDataQueue() 
{
	return &m_qHitData;
}

bool CTurnMgr::SetSkillData(const size_t& _SkillNo, const size_t& _SkillLevel)
{
	if (_SkillNo >= CClientGlobal::vecSkill.size() ||
		_SkillLevel >= CClientGlobal::vecSkillLevelData[_SkillNo].size())
	{
		return false;
	}

	// Queue는 SetSkill을 호출하면 시작하자마자 Queue부터 비우고 시작
	// SetSkill(_Index) -> SetActionScript(_Index)
	//					-> SetSkillData(_Index) 
	//					-> 순으로 호출

	m_HitData.clear();

	m_CurSkillData = CClientGlobal::vecSkill[_SkillNo].SkillData;

	// SkillLevelData[Skill번호][Unit의 현재 스킬레벨]
	m_CurSkillLevelData = CClientGlobal::vecSkillLevelData[_SkillNo][_SkillLevel];

	for (size_t i = 0; i < m_CurSkillLevelData.HitDataCount; i++)
	{
		m_HitData.push_back(m_CurSkillLevelData.HitData[i]);
		m_HitData.back().Apply = false;
	}
	// HitData의 StartTime 순으로 정렬
	m_HitData.sort();

	m_InitPosMode = (size_t)m_CurSkillData.InitPosMode;
	m_InitFixPos = m_CurSkillData.InitFixPos;
	m_InitPosPivot = m_CurSkillData.InitPosPivot;

	if (true == m_bLastHitCheck)
	{
		list<HITDATA>::iterator Start = m_HitData.begin();
		list<HITDATA>::iterator End = m_HitData.end();
		
		for (UINT i = 0; Start!= End; ++Start,++i)
		{
			if (0 != Start->HitDemageMag)
				m_LastHitIndex = i;
		}

	}

	return true;
}
void CTurnMgr::SetBattleUnit(CBattleUnit* _Unit)
{
	if (nullptr == _Unit)
		return;

	m_CurBattleUnit = _Unit;
}
void CTurnMgr::SetFirstBattleUnit()
{
	list<SPTR<CTurnOrderUI>>::iterator Start = m_listCurTurnUI.begin();

	if (Start == m_listCurTurnUI.end())
		return;

	SetBattleUnit((*Start)->GetBattleUnit());
}
vector<SPTR<CBattleUnit>>* CTurnMgr::GetCurTargetVector()
{
	return &m_vecCurTurnTarget;
}
vector<vector<size_t>>* CTurnMgr::GetSkillIndexList()
{
	return &m_CurSkillIndexList;
}
const UINT CTurnMgr::GetCurSkillNo()
{
	return m_CurSkillData.SkillNo;
}
const UINT CTurnMgr::GetCurSkillLv()
{
	return m_CurSkillLevel;
}
void CTurnMgr::ClearAllSkillList()
{
	for (size_t i = 0; i < m_CurSkillIndexList.size(); i++)
	{
		m_CurSkillIndexList[i].clear();
	}
}
void CTurnMgr::PushSkillIndex(const size_t& _SkillType, const size_t& _SkillIndex)
{
	if (_SkillType >= SKILLDATA::MAXSKILLTYPE || _SkillIndex >= CClientGlobal::vecSkill.size())
		return;

	m_CurSkillIndexList[_SkillType].push_back(_SkillIndex);
}
//스킬 선택& 타겟설정 관련된 함수
void CTurnMgr::SelectSkillUI(const size_t& _SkillIndex)
{
	if (_SkillIndex >= CClientGlobal::vecSkill.size())
		return;

	// Target 판단을 위해 스킬 정보만 가져온다.(SkillData)
	size_t SkillLevel = -1 ;

	if(nullptr != m_CurBattleUnit)
		 SkillLevel = m_CurBattleUnit->GetSkillLevel(_SkillIndex);
	

	if (-1 == SkillLevel) 
	{
		assert(nullptr);
		return;
	}
	
	m_CurSelectSkill = _SkillIndex;
	SetSkillData(_SkillIndex, SkillLevel);
}
void CTurnMgr::UnSelectSkill()
{
	m_CurSelectSkill = -1;
}
// 캐릭터 or 몬스터에 마우스가 충돌시 해당 충돌 유닛의 정보를 전송후
// 현재 스킬에 맞는 타겟을 설정하는 함수
void CTurnMgr::CheckSkillTarget(const bool& _bChar, const size_t& _TeamIndex)
{
	if (-1 == m_CurSelectSkill)
		return;

	if (m_CurSelectSkill >= CClientGlobal::vecSkill.size())
		return;

	// 마우스와 충돌한 유닛이 캐릭터일경우 
	// 선택된 스킬이 캐릭터 버프 스킬인 경우만 체크한다.

	if (true == _bChar)
	{
		if (m_CurSkillData.SkillBuffType == SKILLDATA::BUFFTYPE_TEAMBUFF)
		{
			TeamTargetUIOn(_TeamIndex);
		}
	}
	// 현재 마우스와 충돌한 유닛이 몬스터인 경우 
	// 공격스킬과 적에게 거는 버프 스킬일때만 타겟온한다.
	else 
	{
		// 공격 or 적 디버프 스킬인때만 적용
		if (m_CurSkillData.SkillBuffType < SKILLDATA::BUFFTYPE_TEAMBUFF)
		{
			MonsterTargetUIOn(_TeamIndex);
		}
	}

	CurTargetUIOn();
}

void CTurnMgr::TeamTargetUIOn(const size_t& _TeamIndex)
{
	if (_TeamIndex >= m_vecCurBattleChar.size())
		return;

	SPTR<CBattleUnit> TargetUnit = nullptr;

	switch (m_CurSkillData.TeamTargetType)
	{
	case SKILLDATA::TARGET_NONE:
		break;
	case SKILLDATA::TARGET_SINGLE:
	{
		TargetUnit = m_vecCurBattleChar[_TeamIndex];

		if (nullptr == TargetUnit || true == TargetUnit->isDead())
			break;

		m_CurActTargetUI.push_back(m_vecCurBattleChar[_TeamIndex]->GetTargetUI());
		m_SelectTarget = TARGET_SINGLE;
	}
	break;
	case SKILLDATA::TARGET_COLUMN:
	{
		// 앞줄 캐릭터중 하나가 선택시 앞줄 전체 캐릭터의 타겟UI 온
		if (_TeamIndex < 2)
		{
			for (size_t i = 0; i < 2; i++)
			{
				TargetUnit = m_vecCurBattleChar[i];

				if (nullptr == TargetUnit || true == TargetUnit->isDead())
					continue;

				m_CurActTargetUI.push_back(m_vecCurBattleChar[i]->GetTargetUI());
			}
			m_SelectTarget = TARGET_FORWARDCOLUMN;
		}
		// 뒷줄 캐릭터중 하나가 선택시 뒷줄 전체 캐릭터의 타겟UI 온
		else
		{
			for (size_t i = 2; i < 5; i++)
			{
				TargetUnit = m_vecCurBattleChar[i];

				if (nullptr == TargetUnit || true == TargetUnit->isDead())
					continue;

				m_CurActTargetUI.push_back(m_vecCurBattleChar[i]->GetTargetUI());
			}
			m_SelectTarget = TARGET_REARCOLUMN;

		}
	}
	break;
	case SKILLDATA::TARGET_ALL:
	{
		for (size_t i = 0; i < 5; i++)
		{
			TargetUnit = m_vecCurBattleChar[i];

			if (nullptr == TargetUnit || true == TargetUnit->isDead())
				continue;

			m_CurActTargetUI.push_back(m_vecCurBattleChar[i]->GetTargetUI());
		}
		m_SelectTarget = TARGET_ALL;

	}
	break;
	}

}
void CTurnMgr::MonsterTargetUIOn(const size_t& _MonTeamIndex)
{
	if (_MonTeamIndex >= m_vecCurBattleMon.size())
		return;

	SPTR<CBattleUnit> TargetUnit = nullptr;

	switch (m_CurSkillData.EnemyTargetType)
	{
	case SKILLDATA::TARGET_NONE:
		break;
	case SKILLDATA::TARGET_SINGLE:
	{
		TargetUnit = m_vecCurBattleMon[_MonTeamIndex];

		if (nullptr == TargetUnit || true == TargetUnit->isDead())
			break;

		m_CurActTargetUI.push_back(m_vecCurBattleMon[_MonTeamIndex]->GetTargetUI());
		m_SelectTarget = TARGET_SINGLE;
	}
	break;
	case SKILLDATA::TARGET_COLUMN:
	{
		// 앞줄 몬스터중 하나가 선택시 앞줄 전체 캐릭터의 타겟UI 온
		if (_MonTeamIndex == 0 )
		{
			for (size_t i = 0; i < 1; i++)
			{
				TargetUnit = m_vecCurBattleMon[i];

				if (nullptr == TargetUnit || true == TargetUnit->isDead())
					continue;

				m_CurActTargetUI.push_back(m_vecCurBattleMon[i]->GetTargetUI());
			}
			m_SelectTarget = TARGET_REARCOLUMN;
			
		}
		// 뒷줄 캐릭터중 하나가 선택시 뒷줄 전체 캐릭터의 타겟UI 온
		else
		{
			for (size_t i = 1; i < 3; i++)
			{
				TargetUnit = m_vecCurBattleMon[i];

				if (nullptr == TargetUnit || true == TargetUnit->isDead())
					continue;

				m_CurActTargetUI.push_back(m_vecCurBattleMon[i]->GetTargetUI());

			}
			m_SelectTarget = TARGET_FORWARDCOLUMN;
		}
	}
	break;
	case SKILLDATA::TARGET_ALL:
	{
		for (size_t i = 0; i < 3; i++)
		{
			TargetUnit = m_vecCurBattleMon[i];

			if (nullptr == TargetUnit || true == TargetUnit->isDead())
				continue;

			m_CurActTargetUI.push_back(m_vecCurBattleMon[i]->GetTargetUI());
		}
		m_SelectTarget = TARGET_ALL;
	}
	break;
	}
}
void CTurnMgr::CurTargetUIOn() 
{
	list<SPTR<CTargetUI>>::iterator Start = m_CurActTargetUI.begin();
	list<SPTR<CTargetUI>>::iterator End = m_CurActTargetUI.end();

	for (; Start != End; ++Start)
	{
		if (nullptr != (*Start))
			(*Start)->TargetUIOn();
	}
}
void CTurnMgr::CurTargetUIOff()
{
	list<SPTR<CTargetUI>>::iterator Start = m_CurActTargetUI.begin();
	list<SPTR<CTargetUI>>::iterator End = m_CurActTargetUI.end();

	for (;Start!=End; )
	{
		if(nullptr != (*Start))
			(*Start)->TargetUIOff();

		Start = m_CurActTargetUI.erase(Start);
	}
}
bool CTurnMgr::IsSelectSkill()
{
	if (m_CurSelectSkill >= CClientGlobal::vecSkill.size())
		return false;

	return true;
}
void CTurnMgr::TriggerSkill()
{
	if (m_CurSelectSkill >= CClientGlobal::vecSkill.size())
		return;

	
	m_CurBattleUnit->SelectSkill(m_CurSelectSkill);
	SettingUnitTarget();
	m_CurSelectSkill = -1;
	m_SelectTarget = MAXTARGET;
	m_bSkillTrigger = true;
	SkillBoardOff();
}
bool CTurnMgr::isSkillTrigger()
{
	return m_bSkillTrigger;
}
void CTurnMgr::SetSkillTrigger(const bool& _Trigger)
{
	m_bSkillTrigger = _Trigger;
}
void CTurnMgr::SettingUnitTarget()
{
	list<SPTR<CTargetUI>>::iterator Start = m_CurActTargetUI.begin();
	list<SPTR<CTargetUI>>::iterator End = m_CurActTargetUI.end();

	for ( ; Start != End ; ++Start)
	{
		m_CurBattleUnit->SetCurTarget((*Start)->GetPairUnit());
	}

	m_CurBattleUnit->SetSelectTarget(m_SelectTarget);
}
const size_t  CTurnMgr::SettingRandomTarget() 
{
	if (nullptr == m_CurBattleUnit)
		return 0;

	m_CurBattleUnit->ClearCurTarget();

	TARGET Target =  m_CurBattleUnit->GetSelectTarget();

	switch (Target)
	{
	case TARGET_SINGLE:
		return SingleRandomTarget();
	case TARGET_FORWARDCOLUMN:
		// 전방 타겟들이 모두 무효이므로 후방 타겟으로 임의 지정
		return RearTarget();
		// 후방 타겟들이 모두 무효이므로 전방 타겟으로 임의 지정
	case TARGET_REARCOLUMN:
		return ForwardTarget();
	case TARGET_ALL:
		return AllTarget();
	}

	return 0;
}
const size_t CTurnMgr::SingleRandomTarget()
{
	vector<size_t> Temp;

	for (size_t i = 0; i < m_vecCurBattleMon.size(); i++)
	{
		if (nullptr == m_vecCurBattleMon[i])
			continue;

		if ( false == m_vecCurBattleMon[i]->isDead())
			Temp.push_back(i);
	}

	int RandomTarget = (int)CSJRandom::RandomInt(0, (int)Temp.size() -1);

	if (Temp.size() > m_vecCurBattleMon.size())
		return 0;

	if (RandomTarget >= Temp.size())
		return 0;

	m_CurBattleUnit->SetCurTarget(m_vecCurBattleMon[Temp[RandomTarget]]);
	return 1;
}
const size_t CTurnMgr::ForwardTarget()
{
	size_t Count = 0;

	for (size_t i = 1; i < 3; i++)
	{
		if (nullptr == m_vecCurBattleMon[i])
			continue;

		if ( false == m_vecCurBattleMon[i]->isDead())
		{
			m_CurBattleUnit->SetCurTarget(m_vecCurBattleMon[i]);
			++Count;
		}
	}

	return Count;
}
const size_t CTurnMgr::RearTarget()
{
	size_t Count = 0;

	for (size_t i = 0; i < 1; i++)
	{
		if (nullptr == m_vecCurBattleMon[i])
			continue;
		
		if ( false == m_vecCurBattleMon[i]->isDead())
		{
			m_CurBattleUnit->SetCurTarget(m_vecCurBattleMon[i]);
			++Count;
		}
	}
	return Count;
}
const size_t CTurnMgr::AllTarget()
{
	size_t Count = 0;

	for (size_t i = 0; i < MAXMONSTER; i++)
	{
		if (nullptr == m_vecCurBattleMon[i])
			continue;

		if (false == m_vecCurBattleMon[i]->isDead())
		{
			m_CurBattleUnit->SetCurTarget(m_vecCurBattleMon[i]);
			++Count;
		}
	}
	return Count;

}
const size_t CTurnMgr::SingleRandomTarget(SPTR<CBattleUnit> _Unit)
{
	if (nullptr == _Unit)
		return -1;

	vector<size_t> Temp;

	for (size_t i = 0; i < m_vecCurBattleMon.size(); i++)
	{
		if (nullptr == m_vecCurBattleMon[i])
			continue;

		if ( false == m_vecCurBattleMon[i]->isDead())
			Temp.push_back(i);
	}

	int RandomTarget = (int)CSJRandom::RandomInt(0, (int)Temp.size() - 1);

	if (Temp.size() > m_vecCurBattleMon.size())
		return 0;

	if (RandomTarget >= Temp.size())
		return 0;

	_Unit->SetCurTarget(m_vecCurBattleMon[Temp[RandomTarget]]);
	return 1;
}
const size_t CTurnMgr::ForwardTarget(SPTR<CBattleUnit> _Unit) 
{
	if (nullptr == _Unit)
		return -1;

	size_t Count = 0;

	for (size_t i = 1; i < 3; i++)
	{
		if (nullptr == m_vecCurBattleMon[i])
			continue;

		if (false == m_vecCurBattleMon[i]->isDead())
		{
			_Unit->SetCurTarget(m_vecCurBattleMon[i]);
			++Count;
		}
	}

	return Count;
}
const size_t CTurnMgr::RearTarget(SPTR<CBattleUnit> _Unit) 
{
	if (nullptr == _Unit)
		return -1;

	size_t Count = 0;

	for (size_t i = 0; i < 1; i++)
	{
		if ( nullptr == m_vecCurBattleMon[i])
			continue;

		if ( false == m_vecCurBattleMon[i]->isDead())
		{
			_Unit->SetCurTarget(m_vecCurBattleMon[i]);
			++Count;
		}
	}
	return Count;
}
const size_t CTurnMgr::AllTarget(SPTR<CBattleUnit> _Unit) 
{
	if (nullptr == _Unit)
		return -1;

	size_t Count = 0;

	for (size_t i = 0; i < MAXMONSTER; i++)
	{
		if (nullptr == m_vecCurBattleMon[i])
			continue;

		if (false == m_vecCurBattleMon[i]->isDead())
		{
			_Unit->SetCurTarget(m_vecCurBattleMon[i]);
			++Count;
		}
	}
	return Count;
}
void CTurnMgr::SkillBoardOn() 
{
	if (nullptr == m_pSkillUIMgr)
		return;

	m_pSkillUIMgr->SkillBoardOn();
}
void CTurnMgr::SkillBoardOff() 
{
	if (nullptr == m_pSkillUIMgr)
		return;

	m_pSkillUIMgr->SkillBoardOff();
}
SPTR<CSkillUI> CTurnMgr::GetSkillUI(const size_t& _SkillNo)
{
	return (*m_pSkillUIMgr)[_SkillNo];
}
void CTurnMgr::SetSkillUILevel(const size_t& _SkillNo, const size_t& _SkillLevel)
{
	SPTR<CSkillUI> SkillUI = GetSkillUI(_SkillNo);

	
	if (nullptr != SkillUI && _SkillLevel < 5)
		SkillUI->SetCurSkillLevel(_SkillLevel);
}
void CTurnMgr::CurUnitCollisionOn()
{
	for (size_t i = 0; i < m_vecCurBattleChar.size(); i++)
	{
		if (nullptr != m_vecCurBattleChar[i] && false == m_vecCurBattleChar[i]->isDead())
			m_vecCurBattleChar[i]->CollisionComOn();
	}

	for (size_t i = 0; i < m_vecCurBattleMon.size(); i++)
	{
		if (nullptr != m_vecCurBattleMon[i] && false == m_vecCurBattleMon[i]->isDead())
			m_vecCurBattleMon[i]->CollisionComOn();
	}
}
void CTurnMgr::CurUnitCollisionOff() 
{
	for (size_t i = 0; i < m_vecCurBattleChar.size(); i++)
	{
		if (nullptr != m_vecCurBattleChar[i] && false == m_vecCurBattleChar[i]->isDead())
			m_vecCurBattleChar[i]->CollisionComOff();
	}

	for (size_t i = 0; i < m_vecCurBattleMon.size(); i++)
	{
		if (nullptr != m_vecCurBattleMon[i] && false == m_vecCurBattleMon[i]->isDead())
			m_vecCurBattleMon[i]->CollisionComOff();
	}
}
void CTurnMgr::ReleaseTurnMgr()
{
	m_vecCurTurnTarget.clear();
	m_vecCharacter.clear();
	//m_mapStatusBar.clear();
	//m_mapMonster.clear();
	//m_mapTurnUI.clear();
	m_vecCurBattleChar.clear();
	m_vecCurBattleMon.clear();
	m_listCurTurnUI.clear();
	m_vecDemageFont.clear();
	m_DemageReadyList.clear();
	m_DemageReturnList.clear();
	m_vecCurTurnTarget.clear();
	m_vecCurTurnTarget.clear();


	ALLCLEAR_UMAP(m_mapMonsterList);
	ALLCLEAR_UMAP(m_mapStatusBarList);
	ALLCLEAR_UMAP(m_mapTurnUIList);

	SAFE_DELETE(m_pEffectMgr);
	SAFE_DELETE(m_pSkillUIMgr);
}
const bool CTurnMgr::CheckSkillCost()
{
	if (nullptr == m_CurBattleUnit)
		return false;

	return m_CurBattleUnit->CheckSkillAvailable(m_CurSkillLevelData);
}
void CTurnMgr::CalSkillCost()
{
	m_CurBattleUnit->CalSkillCost(m_CurSkillLevelData);
}
void CTurnMgr::ChangeCurUnit()
{
	if (true == m_listCurTurnUI.empty())
		return;

	m_CurBattleUnit = m_listCurTurnUI.front()->GetBattleUnit();
}
const bool CTurnMgr::IsLastHitCheckAction()
{
	return m_bLastHitCheck;
}
const bool CTurnMgr::DetectionLastHit(const UINT& _CurHitDataIndex)
{
	if (false == m_bLastHitCheck)
		return false;

	if (m_LastHitIndex == _CurHitDataIndex)
		return true;
	
	return false;
}
void CTurnMgr::PushDemage(const UINT& _QueueIndex, const UINT& _Demage)
{
	if (m_vecDemageQueue.size() <= _QueueIndex)
		return;

	m_vecDemageQueue[_QueueIndex].push(_Demage);
}
const UINT CTurnMgr::PopDemage(const UINT& _Index)
{
	if (m_vecDemageQueue.size() <= _Index)
		return -1;
	
	if(true == m_vecDemageQueue[_Index].empty())
		return -1;

	UINT Demage = m_vecDemageQueue[_Index].front();
	m_vecDemageQueue[_Index].pop();
	return Demage;
}
void CTurnMgr::BattleUnitDeadEvent(UINT& _AccExp)
{
	for (size_t i = 0; i < m_vecCurBattleChar.size(); i++)
	{
		if (nullptr == m_vecCurBattleChar[i])
			continue;

		if (true == m_vecCurBattleChar[i]->isDead())
			m_vecCurBattleChar[i]->SetDying();
	}

	for (size_t i = 0; i < m_vecCurBattleMon.size(); i++)
	{
		if (nullptr == m_vecCurBattleMon[i])
			continue;

		if (true == m_vecCurBattleMon[i]->isDead()) 
		{
			UINT Exp = m_vecCurBattleMon[i]->GetExp();

			if (0 != Exp) 
			{
				m_BattleWinTotalExp += Exp;
				_AccExp += Exp;
			}
			m_vecCurBattleMon[i]->ChangeState(BATTLE_MONDEAD);
		}
	}
}
const BATTLEPHASE CTurnMgr::GetNextPhaseToCheckPhase()
{
	UINT Count = MAXMONSTER;

	for (size_t i = 0; i < m_vecCurBattleMon.size(); i++)
	{
		if (nullptr == m_vecCurBattleMon[i] || true == m_vecCurBattleMon[i]->isDead())
		{
			--Count;
		}
	}

	if (Count == 0) 
	{
		if (m_CurBattleRound >= CClientGlobal::CurBattleEndRound)
			return PHASE_BATTLEEND;
		else 
			return PHASE_RELOCATION;
	}

	Count = MAXTEAM;

	for (size_t i = 0; i < m_vecCurBattleChar.size(); i++)
	{
		if (nullptr == m_vecCurBattleChar[i]) 
		{
			--Count;
			continue;
		}

		if (BATTLE_DYING == m_vecCurBattleChar[i]->GetCurUnitState())
			--Count;

		/*if (true  == m_vecCurBattleChar[i]->isDead())
			--Count;*/

	}

	if (0 == Count)
		return PHASE_BATTLEEND;

	return PHASE_ROTATE;
}
bool CTurnMgr::IsCharacterBattleWin()
{
	UINT Count = MAXMONSTER;

	for (size_t i = 0; i < m_vecCurBattleMon.size(); i++)
	{
		if (nullptr == m_vecCurBattleMon[i] || true == m_vecCurBattleMon[i]->isDead())
		{
			--Count;
		}
	}

	if (0 == Count)
		return true;

	Count = MAXTEAM;

	for (size_t i = 0; i < m_vecCurBattleChar.size(); i++)
	{
		if (nullptr == m_vecCurBattleChar[i])
		{
			--Count;
			continue;
		}

		if (BATTLE_DYING == m_vecCurBattleChar[i]->GetCurUnitState())
			--Count;
	}

	if (Count == 0)
		return false;

	return true;
}
void CTurnMgr::CharacterWinEvent()
{

	for (size_t i = 0; i < m_vecCurBattleChar.size(); i++)
	{
		if (nullptr == m_vecCurBattleChar[i]
			|| BATTLE_DYING == m_vecCurBattleChar[i]->GetCurUnitState())
			continue;

		m_vecCurBattleChar[i]->ChangeState(BATTLE_WIN);
	}

	/*TrigerResultWindow();
	CurUIOff();*/
}
void CTurnMgr::CharacterLoseEvent()
{
	for (size_t i = 0; i < m_vecCurBattleChar.size(); i++)
	{
		if (nullptr == m_vecCurBattleChar[i])
			continue;

		m_vecCurBattleChar[i]->ChangeState(BATTLE_DEAD);
	}

}
void CTurnMgr::TrigerResultWindow()
{
	if (nullptr == m_ResultWindow)
		return;

	for (size_t i = 0; i < m_vecCurBattleChar.size(); i++)
	{
		if (nullptr == m_vecCurBattleChar[i])
			continue;

		m_ResultWindow->PushResultUI((UINT)i, m_vecCurBattleChar[i]->GetPairResultUI());
	}

	m_ResultWindow->ResultWindowOn(m_BattleWinTotalExp,m_CurCharCount);
}
void CTurnMgr::ResultWindowOff()
{
	if (nullptr == m_ResultWindow)
		return;

	CExpGauge::InitGaugeUpdate();
	m_ResultWindow->ResultWindowOff();

}
const bool CTurnMgr::CheckChangeScene()
{
	return m_ResultWindow->CheckChangeScene();
}
void CTurnMgr::TrigerSceneChangeEvent()
{
	m_fAccEndEventTime = 0.0f;

	m_pBattleSceneBuilder->SceneEffect(SCENE_EFFECT_FADE_OUT | FADE_COLOR_BLACK, 2.0f);
	m_pBattleSceneBuilder->SetFadeData(0.8f);
}
const bool CTurnMgr::IsSceneChangeEventEnd()
{
	if (1.5f < m_fAccEndEventTime) 
	{
		m_pBattleSceneBuilder->EndFadeEffect();
		return true;
	}

	float Vol = 1.3f - m_fAccEndEventTime;
	m_BattleBack->SetVolume(Vol);
	
	m_fAccEndEventTime += DELTATIME;
	return false;
}
void CTurnMgr::InitBattleRound()
{
	m_CurBattleRound = 0;
	m_BattleWinTotalExp = 0;
}

void CTurnMgr::RelocationNextMonster()
{
	++m_CurBattleRound;

	for (size_t i = 0; i < m_vecCurBattleMon.size(); ++i)
		if (nullptr != m_vecCurBattleMon[i]) 
		{
			m_vecCurBattleMon[i]->Off();
			m_vecCurBattleMon[i]->SetDead(false);
			m_vecCurBattleMon[i]->CurBattle(false);
		}

	m_vecCurBattleMon.clear();
	m_vecCurBattleMon.assign(MAXMONSTER, nullptr);
	
	if (m_CurBattleRound >= MAXROUND)
		assert(nullptr);


	for (size_t i = 0; i < MAXMONSTER; i++)
	{
		m_vecCurBattleMon[i]= PopBattleMonster(CClientGlobal::strMonsterTeam[m_CurBattleRound][i]);
		
		if (nullptr == m_vecCurBattleMon[i])
			continue;

		m_vecCurBattleMon[i]->On();
		m_vecCurBattleMon[i]->SetAppear();
		m_vecCurBattleMon[i]->SetPos(m_vecMonsterPos[i]);
		m_vecCurBattleMon[i]->StandBy();
		m_vecCurBattleMon[i]->GetPairStatusBar()->SetPos(m_vecMonStatusPos[i]);
		m_vecCurBattleMon[i]->GetPairStatusBar()->StandBy();
		m_vecCurBattleMon[i]->GetPairStatusBar()->RelocationEvent();
		RandomPushTurnUI(m_vecCurBattleMon[i]->GetPairTurnUI());
		m_vecCurBattleMon[i]->GetPairTurnUI()->EntryUI(true);
		m_vecCurBattleMon[i]->GetPairTurnUI()->StandBy();
		m_vecCurBattleMon[i]->SetTeamNumber((int)i);
		m_vecCurBattleMon[i]->On();
		// 나중에 체력 게이지(보스) UI를 배치
	}

	list<SPTR<CTurnOrderUI>>::iterator Start = m_listCurTurnUI.begin();
	list<SPTR<CTurnOrderUI>>::iterator End = m_listCurTurnUI.end();

	for (int i = 0; Start != End; ++Start, ++i)
		(*Start)->RelocationEvent(m_vecTurnUIPos[i]);
	
}
void CTurnMgr::RelocationEnd()
{
	list<SPTR<CTurnOrderUI>>::iterator Start = m_listCurTurnUI.begin();
	list<SPTR<CTurnOrderUI>>::iterator End = m_listCurTurnUI.end();

	for (int i = 0; Start != End; ++Start, ++i)
		(*Start)->EntryUI(false);
}
const bool CTurnMgr::FinishRelocation()
{
	list<SPTR<CTurnOrderUI>>::iterator Start = m_listCurTurnUI.begin();
	list<SPTR<CTurnOrderUI>>::iterator End = m_listCurTurnUI.end();

	for (int i = 0; Start != End; ++Start, ++i)
	{
		if (CTurnOrderUI::EVENT_NO != (*Start)->GetCurEvent())
			return false;
	}

	return true;
}
const UINT CTurnMgr::GetLiveUnitCount(const UNITCOLUMN& _Op)
{
	UINT Count = 0;

	switch (_Op)
	{
	case CHARACTER_FRONT: 
	{
		Count = 2;

		for (size_t i = 0; i < 2; i++)
		{
			if (nullptr == m_vecCurBattleChar[i]) 
			{
				--Count;
				continue;
			}

			if(true == m_vecCurBattleChar[i]->isDead())
				--Count;
		}
		return Count;
	}
	case CHARACTER_REAR:
	{
		Count = 3;

		for (size_t i = 2; i < MAXTEAM; i++)
		{
			if (nullptr == m_vecCurBattleChar[i])
			{
				--Count;
				continue;
			}
			if (true == m_vecCurBattleChar[i]->isDead())
				--Count;
		}
		return Count;
	}
	case MONSTER_FRONT:
	{
		Count = 2;

		for (size_t i = 1; i < MAXMONSTER; i++)
		{
			if (nullptr == m_vecCurBattleMon[i])
			{
				--Count;
				continue;
			}
			if (true == m_vecCurBattleMon[i]->isDead())
				--Count;
		}
		return Count;
	}
	case MONSTER_REAR:
	{
		Count = 1;

		for (size_t i = 0; i < 1; i++)
		{
			if (nullptr == m_vecCurBattleMon[i])
			{
				--Count;
				continue;
			}
			if (true == m_vecCurBattleMon[i]->isDead())
				--Count;
		}
		return Count;
	}
	}

	return Count;
}
void CTurnMgr::CurCharTargetOverride()
{
	for (size_t i = 0; i < m_vecCurBattleChar.size(); i++)
	{
		if (nullptr == m_vecCurBattleChar[i])
			continue;

		if (true == m_vecCurBattleChar[i]->IsSelectAction())
			m_vecCurBattleChar[i]->TargetOverride(true);
	}
}

vector<SPTR<CBattleUnit>>* CTurnMgr::GetCurBattleCharVec() 
{
	return &m_vecCurBattleChar;
}
vector<SPTR<CBattleUnit>>* CTurnMgr::GetCurBattleMonVec()  
{
	return &m_vecCurBattleMon;
}
void CTurnMgr::ChangeStateDeadCharacter()
{
	for (size_t i = 0; i < m_vecCharacter.size(); i++)
	{
		if (nullptr == m_vecCharacter[i])
			continue;

		if (true == m_vecCharacter[i]->isDead())
			m_vecCharacter[i]->SetDying();
	}
}
void CTurnMgr::CharacterLevelUpEvent()
{
	map<tstring,size_t>::iterator Find  = CClientGlobal::EffectIndexMap.find(_T("Blessing"));

	if (Find == CClientGlobal::EffectIndexMap.end())
		return;


	size_t LevelUpNo = Find->second;

	SPTR<CEffect> LevelUpEffect = m_pEffectMgr->EffectOn(LevelUpNo);

	if (nullptr != LevelUpEffect)
		LevelUpEffect->SetPos(m_CurBattleUnit->GetUnitMiddlePos());
	

}

void CTurnMgr::SetCurBattleBgmIndex(const UINT& _BgmIndex)
{
	if (m_vecBattleBgmData.size() <= _BgmIndex)
		return;

	m_CurBattleBgmIndex = _BgmIndex;
}
void CTurnMgr::PlayBgm() 
{
	if (m_CurBattleBgmIndex >= m_vecBattleBgmData.size())
		return;

	m_BattleBack->SetVolume(m_vecBattleBgmData[m_CurBattleBgmIndex].Volume);
	m_BattleBack->PlayBgm(m_vecBattleBgmData[m_CurBattleBgmIndex].BgmName);
}
void CTurnMgr::BgmOff() 
{
	m_BattleBack->BgmOff();
}
void CTurnMgr::BgmPause(const bool& _Pause)
{
	m_BattleBack->BgmPause(_Pause);
}
void CTurnMgr::ReverseBgmPlay()
{
	m_bBGMMode = !m_bBGMMode;
	BgmPause(m_bBGMMode);
}
void CTurnMgr::CurUIOff()
{
	for (size_t i = 0; i < m_vecCurBattleChar.size(); i++)
	{
		if (nullptr == m_vecCurBattleChar[i])
			continue;

		m_vecCurBattleChar[i]->PairUIOff();
	}

	for (size_t i = 0; i < m_vecCurBattleMon.size(); i++)
	{
		if (nullptr == m_vecCurBattleMon[i])
			continue;

		m_vecCurBattleMon[i]->PairUIOff();
	}
}
void CTurnMgr::GlobalCharacterDataUpdate(const bool& _bWin)
{

		for (size_t i = 0; i < m_vecCurBattleChar.size(); i++)
		{
			if (nullptr == m_vecCurBattleChar[i])
				continue;

			m_vecCurBattleChar[i]->CharacterDataUpdate(_bWin);
		}
	
	
}
void CTurnMgr::BattleStartEvent()
{
	if (nullptr != m_EventObject)
		m_EventObject->EventTriger();
}
void CTurnMgr::ChangeBackGround(const UINT& _BackGround)
{
	if (nullptr != m_BattleBack)
		m_BattleBack->ChangeBackGround(_BackGround);
}