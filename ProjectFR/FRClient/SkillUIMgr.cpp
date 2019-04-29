#include "Precom.h"
#include "SkillUIMgr.h"
#include "TurnMgr.h"
#include "SkillUI.h"
#include "SkillUITab.h"
#include "SkillUIBoard.h"


CSkillUIMgr::CSkillUIMgr()
	:m_PairScene(nullptr), m_SkillUIBoard(nullptr)
	, m_pTurnMgr(nullptr)
{
	
}


CSkillUIMgr::~CSkillUIMgr()
{
	m_vecSkillUI.clear();
}
void CSkillUIMgr::SettingScene(CGameScene* _Scene)
{
	if (nullptr == _Scene)
		return;

	m_PairScene = _Scene;
}
void CSkillUIMgr::SettingTurnMgr(CTurnMgr* _TurnMgr)
{
	if (nullptr == _TurnMgr)
		return;

	m_pTurnMgr = _TurnMgr;
}
SPTR<CSkillUI> CSkillUIMgr::operator[](const size_t& _SkillNo)
{
	if (m_vecSkillUI.size() <= _SkillNo) 
		return nullptr;
	
	return m_vecSkillUI[_SkillNo];
}
SPTR<CSkillUI> CSkillUIMgr::FindSkillUI(const size_t& _SkillNo)
{
	if (m_vecSkillUI.size() <= _SkillNo) 
		return nullptr;
	
	return m_vecSkillUI[_SkillNo];
}
bool CSkillUIMgr::ExistSkillUI(const tstring& _SkillName, const SKILLNAME& _Mode)
{
	for (size_t i = 0; i < m_vecSkillUI.size(); i++)
	{
		switch (_Mode)
		{
		case CSkillUIMgr::SKILL_NAME:
		{
			if (true == m_vecSkillUI[i]->EqualSkillName(_SkillName)) 
				return true;
		}
		break;
		case CSkillUIMgr::SKILL_KNAME:
		{
			if (true == m_vecSkillUI[i]->EqualSkillKName(_SkillName))
				return true;
		}
		break;
		}
	}

	return false;
}
CSkillUI* CSkillUIMgr::CreateSkillUI(const size_t& _SkillNo)
{
	if (CClientGlobal::vecSkill.size() <= _SkillNo) 
		return nullptr;
	
	if (nullptr == m_PairScene)
		return nullptr;

	SKILLUIDATA Data = CClientGlobal::vecSkill[_SkillNo].SkillUIData;

	CActObject* NewUIAct = CActObject::CreateActObject(Data.SkillName, m_PairScene);
	CSkillUI* NewUI = NewUIAct->CreateCom<CSkillUI>(Data);

	if(nullptr == NewUI)
	{
		assert(nullptr);
		return nullptr;
	}

	m_vecSkillUI[_SkillNo] = NewUI;
	NewUI->SkillUIOff();

	return NewUI;
}
void CSkillUIMgr::LoadSkillUIData()
{
	if (0 >= CClientGlobal::vecSkill.size())
		return;

	m_vecSkillUI.assign(CClientGlobal::vecSkill.size(), nullptr);

	for (size_t i = 0; i < CClientGlobal::vecSkill.size(); i++)
	{
		if (nullptr == CreateSkillUI(i)) 
		{
			assert(nullptr);
			continue;
		}
	}

	CActObject* BoardAct = CActObject::CreateActObject(_T("SkillBackBoard"), m_PairScene);
	
	if (nullptr == BoardAct) 
	{
		assert(nullptr);
		return;
	}

	m_SkillUIBoard = BoardAct->CreateCom<CSkillUIBoard>();

	if (nullptr == m_SkillUIBoard)
	{
		assert(nullptr);
		return;
	}
	m_SkillUIBoard->SetSkillUIMgr(this);
	m_SkillUIBoard->SetSkillIndexList(m_pTurnMgr->GetSkillIndexList());


	return;
}
SPTR<CSkillUIBoard> CSkillUIMgr::GetSkillBackBoard()
{
	return m_SkillUIBoard;
}
void CSkillUIMgr::SkillBoardOn()
{
	if (nullptr == m_SkillUIBoard)
		return;

	m_SkillUIBoard->BoardOn();
}
void CSkillUIMgr::SkillBoardOff() 
{
	if (nullptr == m_SkillUIBoard)
		return;

	m_SkillUIBoard->BoardOff();
}
